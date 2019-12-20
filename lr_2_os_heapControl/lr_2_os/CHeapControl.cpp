
#include "stdafx.h"
#include "CHeapControl.h"
#include "inttypes.h"

#define DIV 1024	// Use to convert bytes to KB


HANDLE CHeapControl::s_hHeap = NULL;
UINT CHeapControl::s_uNumAllocsInHeap = 0;


void* CHeapControl::operator new (size_t size){
	if (s_hHeap == NULL) {
		// куча не существует - создаем ее
		s_hHeap = HeapCreate(HEAP_NO_SERIALIZE, 0, 0);
		if (s_hHeap == NULL) {
			return(NULL);
		}
	}
	// куча для CHeapControl существует
	void* p = HeapAlloc(s_hHeap, 0, size);
	if (p != NULL) {
		// память выделена успешно; увеличиваем счетчик обьектов CHeapControl в куче
		s_uNumAllocsInHeap++;
	}
	// возвращаем адрес созданного объекта CHeapControl
	return(p);
}


void CHeapControl::operator delete (void* p) {
	if (HeapFree(s_hHeap, 0, p)) {
		// объект успешно удален
		s_uNumAllocsInHeap--;
	}
	if (s_uNumAllocsInHeap == 0) {
		// если в куче больше нет объектов, уничтожаем ее
		if (HeapDestroy(s_hHeap)) {
			// описатель кучи приравниваем NULL, чтобы оператор new
			// мог создать новую кучу при создании нового объекта CHeapControl
			s_hHeap = NULL;
		}
	}
}


void* CHeapControl::allocInHeap(size_t size) {
	if (s_hHeap == NULL) {
		s_hHeap = HeapCreate(HEAP_NO_SERIALIZE, 0, 0);
	}
	void *p = HeapAlloc(s_hHeap, NULL, size);
	if (p == NULL) {
		return NULL;
	}
	return p;
}


void* CHeapControl::deleteFromHeap(void *p, size_t size){
	bool result = HeapFree(s_hHeap, NULL, p);
	if (!result) {
		return NULL;
	}
}


void CHeapControl::ArrayCreation(int arrayDimension) {
	Array = new int[arrayDimension];
	Buff = new int[arrayDimension];
}


int* CHeapControl::ArrayInsertion(CString arrayInString) {
	CString temp = _T("");
	int j = 0;
	for (int i = 0; i < arrayInString.GetLength(); i++) {
		if ((arrayInString[i] != '\r') && (arrayInString[i + 1] != '\n')) {
			temp += arrayInString[i];
		}
		else if ((arrayInString[i] == '\r') && (arrayInString[i+1] == '\n')) {
			i++;
			Array[j] = _wtoi(temp);
			temp = _T("");
			j++;
		}
	}
	Array[j] = _wtoi(temp);
	return Array;
}

CString CHeapControl::ArrayOutput(int arrayDimension) {
	CString outputString = _T("");
	for (int i = 0; i < arrayDimension; i++) {
		CString temp = _T("");
		temp.Format(_T("%d"), Array[i]);
		outputString += temp + _T(" ");
	}
	return outputString;
}


CString CHeapControl::MemStatus() {

#define WIDTH 8

	MEMORYSTATUS stat; 
	MEMORYSTATUSEX statex;
	
	CString memStatus;
	CString memStatusEx;
	CString overallMemStatus;

	stat.dwLength = sizeof(stat);
	statex.dwLength = sizeof(statex);

	GlobalMemoryStatus(&stat);
	GlobalMemoryStatusEx(&statex);

	memStatus.Format(
		_T("------------------- GlobalMemoryStatus -------------------\r\n\
There is  %*u percent of memory in use.\r\n\
There are %*u total KB of physical memory.\r\n\
There are %*u free  KB of physical memory.\r\n\
There are %*u total KB of paging file.\r\n\
There are %*u free  KB of paging file.\r\n\
There are %*u total KB of virtual memory.\r\n\
There are %*u free  KB of virtual memory.\r\n\
---------------------------------------------------------------------\r\n")
			, WIDTH, stat.dwMemoryLoad
			, WIDTH, stat.dwTotalPhys / DIV
			, WIDTH, stat.dwAvailPhys / DIV
			, WIDTH, stat.dwTotalPageFile / DIV
			, WIDTH, stat.dwAvailPageFile / DIV
			, WIDTH, stat.dwTotalVirtual / DIV
			, WIDTH, stat.dwTotalVirtual / DIV
			);

	memStatusEx.Format(
		_T("----------------- GlobalMemoryStatusEx -----------------\r\n\
There is  %*ld percent of memory in use.\r\n\
There are %*I64d total KB of physical memory.\r\n\
There are %*I64d free  KB of physical memory.\r\n\
There are %*I64d total KB of paging file.\r\n\
There are %*I64d free  KB of paging file.\r\n\
There are %*I64d total KB of virtual memory.\r\n\
There are %*I64d free  KB of virtual memory.\r\n\
There are %*I64d free  KB of extended memory.\r\n\
---------------------------------------------------------------------\r\n")
		   , WIDTH, statex.dwMemoryLoad
		   , WIDTH, statex.ullTotalPhys / DIV
		   , WIDTH, statex.ullAvailPhys / DIV
		   , WIDTH, statex.ullTotalPageFile / DIV
		   , WIDTH, statex.ullAvailPageFile / DIV
		   , WIDTH, statex.ullTotalVirtual / DIV
		   , WIDTH, statex.ullTotalVirtual / DIV
		   , WIDTH, statex.ullAvailExtendedVirtual / DIV
		   );

	overallMemStatus = memStatus + memStatusEx;

	return overallMemStatus;
}


CString CHeapControl::HardwareInfo() {
	
	SYSTEM_INFO sysInfo;
	CString hwInfo;
	GetSystemInfo(&sysInfo);	// Copy the hardware information to the SYSTEM_INFO structure. 

	// Display the contents of the SYSTEM_INFO structure. 

	hwInfo.Format(
		_T("------------------- Hardware information ------------------\r\n\
  OEM ID: %u\r\n\
  Number of processors: %u\r\n\
  Page size: %u KB\r\n\
  Allocation granularity: %u KB\r\n\
  Processor type: %u\r\n\
  Minimum application address: %lx\r\n\
  Maximum application address: %lx\r\n\
  Active processor mask: %u\r\n\
---------------------------------------------------------------------\r\n")
		   , sysInfo.dwOemId
		   , sysInfo.dwNumberOfProcessors
		   , sysInfo.dwPageSize / DIV
		   , sysInfo.dwAllocationGranularity / DIV
		   , sysInfo.dwProcessorType
		   , sysInfo.lpMinimumApplicationAddress
		   , sysInfo.lpMaximumApplicationAddress
		   , sysInfo.dwActiveProcessorMask
		   );

	return hwInfo;
}


CString CHeapControl::AddressStatus() {
	void *virtualMemory = NULL;
	MEMORY_BASIC_INFORMATION mbi;
	SYSTEM_INFO si;
	GetSystemInfo(&si);
	CString addrStat;

	virtualMemory = VirtualAlloc(NULL, si.dwPageSize, MEM_RESERVE, PAGE_READWRITE);

	while (VirtualQuery(virtualMemory, &mbi, sizeof(mbi)) != NULL) {
		CString str;
		str.Format(_T("RegionSize:\t%d\r\n\
State:\t\t%s\r\n\
Protect:\t\t%s\r\n\
AllocProtect:\t%s\r\n\
Type:\t\t%s\r\n\
Address:\t\t%d\r\n\
---------------------------------------------------------------------\r\n")
			  , (int)mbi.RegionSize
			  , getAllocStateString(mbi.State)
			  , getAllocProtectString(mbi.Protect)
			  , getAllocProtectString(mbi.AllocationProtect)
			  , getAllocTypeString(mbi.Type)
			  , (int)virtualMemory
			  );

		virtualMemory = (char*)virtualMemory + mbi.RegionSize;

		addrStat += str;
	}

	VirtualFree(virtualMemory, 0, MEM_RELEASE);

	return addrStat;
}


CString CHeapControl::getAllocProtectString(unsigned long type) {
	switch (type) {
	case PAGE_EXECUTE:
		return _T("PAGE_EXECUTE");
	case PAGE_EXECUTE_READ:
		return _T("PAGE_EXECUTE_READ");
	case PAGE_EXECUTE_READWRITE:
		return _T("PAGE_EXECUTE_READWRITE");
	case PAGE_EXECUTE_WRITECOPY:
		return _T("PAGE_EXECUTE_WRITECOPY");
	case PAGE_NOACCESS:
		return _T("PAGE_NOACCESS");
	case PAGE_READONLY:
		return _T("PAGE_READONLY");
	case PAGE_READWRITE:
		return _T("PAGE_READWRITE");
	case PAGE_WRITECOPY:
		return _T("PAGE_WRITECOPY");
	case 0x40000000:
		return _T("PAGE_TARGETS_NO_UPDATE | PAGE_TARGETS_INVALID");
	case 0:
		return _T("DO NOT HAVE ACCESS!");
	default:
		CString str;
		str.Format(_T("UNDEF '%d'!"), type);
		return str;
	}
}


CString CHeapControl::getAllocTypeString(DWORD type) {
	switch (type) {
	case MEM_IMAGE:
		return _T("MEM_IMAGE");
	case MEM_MAPPED:
		return _T("MEM_MAPPED");
	case MEM_PRIVATE:
		return _T("MEM_PRIVATE");
	case 0:
		return _T("DO NOT HAVE ACCESS!");
	default:
		CString str;
		str.Format(_T("UNDEF '%d'!"), type);
		return str;
	}
}


CString CHeapControl::getAllocStateString(DWORD state) {
	switch (state) {
	case MEM_COMMIT:
		return _T("MEM_COMMIT");
	case MEM_FREE:
		return _T("MEM_FREE");
	case MEM_RESERVE:
		return _T("MEM_RESERVE");
	case 0:
		return _T("DO NOT HAVE ACCESS!");
	default:
		CString str;
		str.Format(_T("UNDEF '%d'!"), state);
		return str;
	}
}


using namespace std;
void CHeapControl::mergeSort(int* elems, //original array
				int* tmp_elems, //temp array to hold intermediate results, should be same size as array "elems"
				int size)
{
	if (size <= 1) return; //nothing to sort

	const int left_size = size / 2;
	const int right_size = size - left_size;

	//memStatusUnderSorting += heap.MemStatus();

	mergeSort(elems, tmp_elems, left_size);
	mergeSort(elems + left_size, tmp_elems + left_size, right_size);

	int* leftIt = elems; // pointer to walk through left part
	int* const pivot = elems + left_size; //end of left part, start of right  part
	int* rightIt = pivot; // pointer to walk through right part
	int* const end = elems + size;
	int* outputIt = tmp_elems; //pointer to where to write when merging left and right subparts

	while (true)
	{
		if (*leftIt < *rightIt)
		{
			*outputIt++ = *leftIt++;
			if (leftIt == pivot)
			{
				// copy the rest of right part that has not been copied yet
				copy(rightIt, end, outputIt);
				break;
			}
		}
		else
		{
			*outputIt++ = *rightIt++;
			if (rightIt == end)
			{
				// copy the rest of left part that has not been copied yet
				copy(leftIt, pivot, outputIt);
				break;
			}
		}
	}

	copy(tmp_elems, tmp_elems + size, elems);
}