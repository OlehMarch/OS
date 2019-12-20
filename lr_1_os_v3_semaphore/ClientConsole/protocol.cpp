
#include "stdafx.h"
#include "protocol.h"


Protocol::Protocol(CString Prefix) {
	prefix = Prefix + _T(": ");
}


void Protocol::VirificationOfFileMapping(HANDLE FileMapping) {
	if (FileMapping == NULL) {
		logOfFileMap = _T("File Mapping creation - FAILED");
	}
	else {
		logOfFileMap = _T("File Mapping - CREATED");
	}
}


void Protocol::VirificationOfMapViewOfFile(void *Buffer) {
	if (Buffer == NULL) {
		logOfMapViewOfFile = _T("File Map View creation - FAILED");
	}
	else {
		logOfMapViewOfFile = _T("File Map View  - CREATED");
	}
}


//bool create_open (true - create; false - open)
void Protocol::VirificationOfSemaphore(HANDLE hSemaphore, CString Reader_Writer, bool create_open) {
	if (Reader_Writer == _T("Reader")) {
		if (hSemaphore == NULL) {
			if (create_open == true) {
				logOfSemaphoreRead = _T("Reader Semaphore creation - FAILED");
			}
			else {
				logOfSemaphoreRead = _T("Reader Semaphore open - FAILED");
			}
		}
		else {
			if (create_open == true) {
				logOfSemaphoreRead = _T("Reader Semaphore - CREATED");
			}
			else {
				logOfSemaphoreRead = _T("Reader Semaphore - OPENED");
			}
		}
	}
	else {
		if (hSemaphore == NULL) {
			if (create_open == true) {
				logOfSemaphoreWrite = _T("Writer Semaphore creation - FAILED");
			}
			else {
				logOfSemaphoreWrite = _T("Writer Semaphore open - FAILED");
			}
		}
		else {
			if (create_open == true) {
				logOfSemaphoreWrite = _T("Writer Semaphore - CREATED");
			}
			else {
				logOfSemaphoreWrite = _T("Writer Semaphore - OPENED");
			}
		}
	}
}


void Protocol::semaphoreOccupation(HANDLE hSemaphore, CString reader_writer) {
	if (WaitForSingleObject(hSemaphore, 100) == false) {
		if (reader_writer == _T("Writer")) {
			logOfSemaphoreWriteOccupation = reader_writer + _T(" Semaphore occupation - FAILED");
		}
		else {
			logOfSemaphoreReadOccupation = reader_writer + _T(" Semaphore occupation - FAILED");
		}
	}
	else {
		if (reader_writer == _T("Writer")) {
			logOfSemaphoreWriteOccupation = reader_writer + _T(" Semaphore - OCCUPIED");
		}
		else {
			logOfSemaphoreReadOccupation = reader_writer + _T(" Semaphore - OCCUPIED");
		}
	}
}


void Protocol::semaphoreRelease(HANDLE hSemaphore, CString reader_writer) {
	if (ReleaseSemaphore(hSemaphore, 1, NULL) == false) {
		if (reader_writer == _T("Writer")) {
			logOfSemaphoreWriteRelease = reader_writer + _T(" Semaphore - RELEASED");
		}
		else {
			logOfSemaphoreReadRelease = reader_writer + _T(" Semaphore - RELEASED");
		}
	}
	else {
		if (reader_writer == _T("Writer")) {
			logOfSemaphoreWriteRelease = _T("Release of ") + reader_writer + _T(" Semaphore - FAILED");
		}
		else {
			logOfSemaphoreReadRelease = _T("Release of ") + reader_writer + _T(" Semaphore - FAILED");
		}
	}
}

//CString handleType = ( _T("Reader") || _T("Writer") || _T("FileMapping") || _T("Buffer") )
void Protocol::handleClose(bool state, CString handleType) {
	if (handleType == _T("Reader")) {
		if (state == true) {
			logOfHandleClose_SemaphoreRead = handleType + _T(" Semaphore - CLOSED");
		}
		else {
			logOfHandleClose_SemaphoreRead = handleType + _T(" Semaphore closing - FAILED");
		}
	}
	else if (handleType == _T("Writer")) {
		if (state == true) {
			logOfHandleClose_SemaphoreWrite = handleType + _T(" Semaphore - CLOSED");
		}
		else {
			logOfHandleClose_SemaphoreWrite = handleType + _T(" Semaphore closing - FAILED");
		}
	}
	else if (handleType == _T("FileMapping")) {
		if (state == true) {
			logOfHandleClose_fileMapping = handleType + _T(" Semaphore - CLOSED");
		}
		else {
			logOfHandleClose_fileMapping = handleType + _T(" Semaphore closing - FAILED");
		}
	}
	else if (handleType == _T("Buffer")) {
		if (state == true) {
			logOfUnmapViewOfFile = _T("Unmapping View Of File - SUCCESS");
		}
		else {
			logOfUnmapViewOfFile = _T("Unmapping View Of File - FAILED");
		}
	}
}


bool Protocol::FileExists(LPCTSTR fname) {
	return CFileFind().FindFile(fname) == TRUE;
}


void Protocol::serverLogFileCreation() {
	logFile.Open(_T("ServerLogFile.log"), CFile::modeCreate | CFile::modeWrite);
}


void Protocol::clientLogFileCreation() {
	logFile.Open(_T("LogFile.log"), CStdioFile::modeCreate | CStdioFile::modeNoTruncate | CStdioFile::modeReadWrite | CStdioFile::typeText);
	logFile.SeekToEnd();
}


void Protocol::getLogOfFileMap() {
	logFile.WriteString(prefix + logOfFileMap + _T("\n"));
}


void Protocol::getlogOfSemaphoreRead() {
	logFile.WriteString(prefix + logOfSemaphoreRead + _T("\n"));
}


void Protocol::getLogOfSemaphoreWrite() {
	logFile.WriteString(prefix + logOfSemaphoreWrite + _T("\n"));
}


void Protocol::getLogOfMapViewOfFile() {
	logFile.WriteString(prefix + logOfMapViewOfFile + _T("\n"));
}


void Protocol::getLogOfSemaphoreWriteOccupation() {
	logFile.WriteString(prefix + logOfSemaphoreWriteOccupation + _T("\n"));
}


void Protocol::getLogOfSemaphoreWriteRelease() {
	logFile.WriteString(prefix + logOfSemaphoreWriteRelease + _T("\n"));
}


void Protocol::getLogOfSemaphoreReadOccupation() {
	logFile.WriteString(prefix + logOfSemaphoreReadOccupation + _T("\n"));
}


void Protocol::getLogOfSemaphoreReadRelease() {
	logFile.WriteString(prefix + logOfSemaphoreReadRelease + _T("\n"));
}


void Protocol::setSeparator() {
	logFile.WriteString(_T("-------------------------------------\n\n"));
}


void Protocol::getLogOfHandleClose_SemaphoreRead() {
	logFile.WriteString(prefix + logOfHandleClose_SemaphoreRead + _T("\n"));
}


void Protocol::getLogOfHandleClose_SemaphoreWrite() {
	logFile.WriteString(prefix + logOfHandleClose_SemaphoreWrite + _T("\n"));
}


void Protocol::getLogOfHandleClose_fileMapping() {
	logFile.WriteString(prefix + logOfHandleClose_fileMapping + _T("\n"));
}


void Protocol::getLogOfUnmapViewOfFile() {
	logFile.WriteString(prefix + logOfUnmapViewOfFile + _T("\n"));
}


void Protocol::logFileClose() {
	logFile.Close();
}


void Protocol::getServerLogData() {
	getLogOfFileMap();
	getlogOfSemaphoreRead();
	getLogOfSemaphoreWrite();
	getLogOfMapViewOfFile();
	getLogOfSemaphoreWriteOccupation();
	getLogOfSemaphoreWriteRelease();
	setSeparator();
}


void Protocol::getServerLogOfDataClode() {
	getLogOfHandleClose_SemaphoreRead();
	getLogOfHandleClose_SemaphoreWrite();
	getLogOfUnmapViewOfFile();
	getLogOfHandleClose_fileMapping();
	setSeparator();
}


void Protocol::overallProtocol() {
	HANDLE hMutex = CreateMutex(NULL, false, _T("mutexProtocol"));
	WaitForSingleObject(hMutex, INFINITE);
	clientLogFileCreation();
	getLogOfFileMap();
	getlogOfSemaphoreRead();
	getLogOfSemaphoreWrite();
	getLogOfMapViewOfFile();
	getLogOfSemaphoreReadOccupation();
	getLogOfSemaphoreWriteOccupation();
	getLogOfSemaphoreReadRelease();
	getLogOfSemaphoreWriteRelease();
	setSeparator();
	logFileClose();
	ReleaseMutex(hMutex);
}


Protocol::~Protocol() {
	delete(this);
}