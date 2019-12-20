
#pragma once

#include "stdafx.h"
#include "iostream"
#include <windows.h>

class Protocol {

protected:
	CString prefix;
	CString logOfFileMap;
	CString logOfMutexRead;
	CString logOfMutexWrite;
	CString logOfMapViewOfFile;
	CString logOfMutexReadOccupation;
	CString logOfMutexReadRelease;
	CString logOfMutexWriteOccupation;
	CString logOfMutexWriteRelease;
	CString logOfHandleClose_mutexRead;
	CString logOfHandleClose_mutexWrite;
	CString logOfHandleClose_fileMapping;
	CString logOfUnmapViewOfFile;
	CStdioFile logFile;

public:

	Protocol(CString Prefix);

	void VirificationOfFileMapping(HANDLE FileMapping);
	void VirificationOfMapViewOfFile(void *Buffer);
		//bool create_open (true - create; false - open)
	void VirificationOfMutex(HANDLE hMutex, CString Reader_Writer, bool create_open = true);
	void mutexOccupation(HANDLE hMutex, CString reader_writer = _T("Writer"));
	void mutexRelease(HANDLE hMutex, CString reader_writer = _T("Writer"));
		//CString handleType = ( _T("Reader") || _T("Writer") || _T("FileMapping") || _T("Buffer") )
	void handleClose(bool state, CString handleType = _T("Reader"));

	bool FileExists(LPCTSTR fname);
	void serverLogFileCreation();
	void clientLogFileCreation();
	void getLogOfFileMap();
	void getlogOfMutexRead();
	void getLogOfMutexWrite();
	void getLogOfMapViewOfFile();
	void getLogOfMutexWriteOccupation();
	void getLogOfMutexWriteRelease();
	void getLogOfMutexReadOccupation();
	void getLogOfMutexReadRelease();
	void setSeparator();
	void getLogOfHandleClose_mutexRead();
	void getLogOfHandleClose_mutexWrite();
	void getLogOfHandleClose_fileMapping();
	void getLogOfUnmapViewOfFile();
	void logFileClose();
	void getServerLogData();
	void getServerLogOfDataClode();
	void overallProtocol();

	~Protocol();

};