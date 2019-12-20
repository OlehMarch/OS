
#pragma once

#include "stdafx.h"
#include "targetver.h"
#include <windows.h>
#include <algorithm>
#include <vector>

#pragma comment(lib, "user32.lib")

class CHeapControl {
private:
	static HANDLE s_hHeap;
	static UINT s_uNumAllocsInHeap;

public:
	int *Array = NULL;
	int *Buff = NULL;

	void* operator new (size_t size);
	void operator delete (void* p);

	static void* allocInHeap(size_t size);
	static void* deleteFromHeap(void *p, size_t size);

	void ArrayCreation(int arrayDimension);
	int* ArrayInsertion(CString arrayInString);
	CString ArrayOutput(int arrayDimension);

	CString MemStatus();
	CString HardwareInfo();
	CString AddressStatus();

	CString getAllocProtectString(unsigned long type);
	CString getAllocTypeString(DWORD type);
	CString getAllocStateString(DWORD state);

	void mergeSort(int* elems, int* tmp_elems, int size);
};