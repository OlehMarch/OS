
#pragma once

#include "stdafx.h"
#include "iostream"
#include <windows.h>

class Protocol {

protected:
	CString prefix;
	CString logOfFileMap;
	CString logOfSemaphoreRead;
	CString logOfSemaphoreWrite;
	CString logOfMapViewOfFile;
	CString logOfSemaphoreReadOccupation;
	CString logOfSemaphoreReadRelease;
	CString logOfSemaphoreWriteOccupation;
	CString logOfSemaphoreWriteRelease;
	CString logOfHandleClose_SemaphoreRead;
	CString logOfHandleClose_SemaphoreWrite;
	CString logOfHandleClose_fileMapping;
	CString logOfUnmapViewOfFile;
	CStdioFile logFile;

public:

	Protocol(CString Prefix);

	void VirificationOfFileMapping(HANDLE FileMapping);
	void VirificationOfMapViewOfFile(void *Buffer);
		//bool create_open (true - create; false - open)
	void VirificationOfSemaphore(HANDLE hSemaphore, CString Reader_Writer, bool create_open = true);
	void semaphoreOccupation(HANDLE hSemaphore, CString reader_writer = _T("Writer"));
	void semaphoreRelease(HANDLE hSemaphore, CString reader_writer = _T("Writer"));
		//CString handleType = ( _T("Reader") || _T("Writer") || _T("FileMapping") || _T("Buffer") )
	void handleClose(bool state, CString handleType = _T("Reader"));

	bool FileExists(LPCTSTR fname);
	void serverLogFileCreation();
	void clientLogFileCreation();
	void getLogOfFileMap();
	void getlogOfSemaphoreRead();
	void getLogOfSemaphoreWrite();
	void getLogOfMapViewOfFile();
	void getLogOfSemaphoreWriteOccupation();
	void getLogOfSemaphoreWriteRelease();
	void getLogOfSemaphoreReadOccupation();
	void getLogOfSemaphoreReadRelease();
	void setSeparator();
	void getLogOfHandleClose_SemaphoreRead();
	void getLogOfHandleClose_SemaphoreWrite();
	void getLogOfHandleClose_fileMapping();
	void getLogOfUnmapViewOfFile();
	void logFileClose();
	void getServerLogData();
	void getServerLogOfDataClode();
	void overallProtocol();

	~Protocol();

};