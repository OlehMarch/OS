// Client.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "iostream"
#include <windows.h>

using namespace std;


int factorial(int value) {

	if (value < 0) {
		return 0;
	}
	if (value == 0) {
		return 1;
	}
	else {
		return value * factorial(value - 1);
	}

}


int _tmain(int argc, _TCHAR* argv[])
{
	int ReadCount = 0;

	HANDLE FileMapping = OpenFileMapping(FILE_MAP_READ, false, _T("FileMapping"));

	if (FileMapping == NULL) {
		return 1;
	}

	void *Buffer = MapViewOfFile(FileMapping, FILE_MAP_READ, 0, 0, 1024);
	HANDLE hMutexRead = OpenMutex(SYNCHRONIZE, false, _T("MutexRead"));
	
	if (hMutexRead == NULL)	{
		return 1;
	}

	HANDLE hMutexWrite = OpenMutex(SYNCHRONIZE, false, _T("MutexWrite"));

	if (hMutexWrite == NULL) {
		return 1;
	}

	WaitForSingleObject(hMutexRead, INFINITE);
	ReadCount++;

	if (ReadCount == 1) {
		WaitForSingleObject(hMutexWrite, INFINITE);
	}

	ReleaseMutex(hMutexRead);
	int value = atoi((char*)Buffer);
	value = factorial(value);
	cout << value << '\n';
	ReadCount--;

	if (ReadCount == 0)	{
		ReleaseMutex(hMutexWrite);
	}
	
	system("PAUSE");
	return 0;
}

