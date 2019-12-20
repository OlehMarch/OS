// ClientConsole.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "ClientConsole.h"

#include "protocol.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Единственный объект приложения

CWinApp theApp;

using namespace std;

Protocol *protocol = new Protocol(_T("Client"));

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

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	HMODULE hModule = ::GetModuleHandle(NULL);

	if (hModule != NULL)
	{
		// инициализировать MFC, а также печать и сообщения об ошибках про сбое
		if (!AfxWinInit(hModule, NULL, ::GetCommandLine(), 0))
		{
			// TODO: измените код ошибки соответственно своим потребностям
			_tprintf(_T("Критическая ошибка: сбой при инициализации MFC\n"));
			nRetCode = 1;
		}
		else
		{
			// TODO: Вставьте сюда код для приложения.
		}
	}
	else
	{
		// TODO: Измените код ошибки соответственно своим потребностям
		_tprintf(_T("Критическая ошибка: неудачное завершение GetModuleHandle\n"));
		nRetCode = 1;
	}
//----------------------------------------------------------------------------------------------------------//

	int ReadCount = 0;

	HANDLE FileMapping = OpenFileMapping(FILE_MAP_READ, false, _T("FileMapping"));
	protocol->VirificationOfFileMapping(FileMapping);

	if (FileMapping == NULL) {
		return 0;
	}

	void *Buffer = MapViewOfFile(FileMapping, FILE_MAP_READ, 0, 0, 1024);
	protocol->VirificationOfMapViewOfFile(Buffer);
	HANDLE hSemRead = OpenSemaphore(SYNCHRONIZE, false, _T("SemRead"));
	protocol->VirificationOfSemaphore(hSemRead, _T("Reader"), false);

	if (hSemRead == NULL)	{
		return 0;
	}

	HANDLE hSemWrite = OpenSemaphore(SYNCHRONIZE, false, _T("SemWrite"));
	protocol->VirificationOfSemaphore(hSemWrite, _T("Writer"), false);

	if (hSemWrite == NULL) {
		return 0;
	}

	WaitForSingleObject(hSemRead, INFINITE);
	protocol->semaphoreOccupation(hSemRead, _T("Reader"));
	ReadCount++;

	if (ReadCount == 1) {
		WaitForSingleObject(hSemWrite, INFINITE);
		protocol->semaphoreOccupation(hSemWrite);
	}

	ReleaseSemaphore(hSemRead, 1, NULL);
	protocol->semaphoreRelease(hSemRead, _T("Reader"));

	int value = atoi((char*)Buffer);
	//value = factorial(value);
	cout << value << endl;

	ReadCount--;

	if (ReadCount == 0)	{
		ReleaseSemaphore(hSemWrite, 1, NULL);
		protocol->semaphoreRelease(hSemWrite);
	}

	protocol->overallProtocol();

	system("pause");

	return nRetCode;
}
