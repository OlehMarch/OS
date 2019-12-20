// ServerConsole.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "ServerConsole.h"

#include "protocol.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Единственный объект приложения

CWinApp theApp;

using namespace std;

HANDLE FileMapping;
HANDLE hSemRead;
HANDLE hSemWrite;
void *Buffer;
Protocol *protocol = new Protocol(_T("Server"));

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

	//Файловая проекция размером 1024 и именем MyFileMapping
	FileMapping = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, 1024, _T("FileMapping"));
	protocol->VirificationOfFileMapping(FileMapping);
	if (FileMapping == NULL)
	{
		cout << "Error\n";
	}
	//Отображаем проекцию файла в адресное пространство и запоминаем полученный адрес
	Buffer = MapViewOfFile(FileMapping, FILE_MAP_WRITE, 0, 0, 1024);
	protocol->VirificationOfMapViewOfFile(Buffer);
	hSemRead = CreateSemaphore(NULL, 1, 1, _T("SemRead"));
	protocol->VirificationOfSemaphore(hSemRead, _T("Reader"));
	hSemWrite = CreateSemaphore(NULL, 1, 1, _T("SemWrite"));
	protocol->VirificationOfSemaphore(hSemWrite, _T("Writer"));

	char *str;
	char buffer[1024];
	int fact = 1;
	int valueToFactCalc = 0;
	protocol->clientLogFileCreation();

	cout << "Insert value to calculate factorial: ";
	cin >> valueToFactCalc;

	for (int i = 1; i <= valueToFactCalc; i++) {
		if ((valueToFactCalc < 0) || (valueToFactCalc == 0)) {
			return 0;
		}
		else {
			fact *= i;

			str = _itoa(fact, buffer, 10);

			WaitForSingleObject(hSemWrite, 1000);
			protocol->semaphoreOccupation(hSemWrite);
			Sleep(200);
			memcpy(Buffer, str, 1024);
			ReleaseSemaphore(hSemWrite, 1, NULL);
			protocol->semaphoreRelease(hSemWrite);

			protocol->getServerLogData();
		}
	}
	protocol->logFileClose();

	system("pause");

	bool state;
	protocol->clientLogFileCreation();

	state = CloseHandle(hSemRead);
	protocol->handleClose(state);
	state = CloseHandle(hSemWrite);
	protocol->handleClose(state, _T("Writer"));
	state = UnmapViewOfFile(Buffer);
	protocol->handleClose(state, _T("Buffer"));
	state = CloseHandle(FileMapping);
	protocol->handleClose(state, _T("FileMapping"));

	protocol->getServerLogOfDataClode();
	protocol->logFileClose();

	return nRetCode;
}
