// ServerConsole.cpp: ���������� ����� ����� ��� ����������� ����������.
//

#include "stdafx.h"
#include "ServerConsole.h"

#include "protocol.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ������������ ������ ����������

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
		// ���������������� MFC, � ����� ������ � ��������� �� ������� ��� ����
		if (!AfxWinInit(hModule, NULL, ::GetCommandLine(), 0))
		{
			// TODO: �������� ��� ������ �������������� ����� ������������
			_tprintf(_T("����������� ������: ���� ��� ������������� MFC\n"));
			nRetCode = 1;
		}
		else
		{
			// TODO: �������� ���� ��� ��� ����������.
		}
	}
	else
	{
		// TODO: �������� ��� ������ �������������� ����� ������������
		_tprintf(_T("����������� ������: ��������� ���������� GetModuleHandle\n"));
		nRetCode = 1;
	}
//----------------------------------------------------------------------------------------------------------//

	//�������� �������� �������� 1024 � ������ MyFileMapping
	FileMapping = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, 1024, _T("FileMapping"));
	protocol->VirificationOfFileMapping(FileMapping);
	if (FileMapping == NULL)
	{
		cout << "Error\n";
	}
	//���������� �������� ����� � �������� ������������ � ���������� ���������� �����
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
