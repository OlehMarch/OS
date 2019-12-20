
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
void Protocol::VirificationOfMutex(HANDLE hMutex, CString Reader_Writer, bool create_open) {
	if (Reader_Writer == _T("Reader")) {
		if (hMutex == NULL) {
			if (create_open == true) {
				logOfMutexRead = _T("Reader Mutex creation - FAILED");
			}
			else {
				logOfMutexRead = _T("Reader Mutex open - FAILED");
			}
		}
		else {
			if (create_open == true) {
				logOfMutexRead = _T("Reader Mutex - CREATED");
			}
			else {
				logOfMutexRead = _T("Reader Mutex - OPENED");
			}
		}
	}
	else {
		if (hMutex == NULL) {
			if (create_open == true) {
				logOfMutexWrite = _T("Writer Mutex creation - FAILED");
			}
			else {
				logOfMutexWrite = _T("Writer Mutex open - FAILED");
			}
		}
		else {
			if (create_open == true) {
				logOfMutexWrite = _T("Writer Mutex - CREATED");
			}
			else {
				logOfMutexWrite = _T("Writer Mutex - OPENED");
			}
		}
	}
}


void Protocol::mutexOccupation(HANDLE hMutex, CString reader_writer) {
	if (WaitForSingleObject(hMutex, 100) == false) {
		if (reader_writer == _T("Writer")) {
			logOfMutexWriteOccupation = reader_writer + _T(" Mutex - OCCUPIED");
		}
		else {
			logOfMutexReadOccupation = reader_writer + _T(" Mutex - OCCUPIED");
		}
	}
	else {
		if (reader_writer == _T("Writer")) {
			logOfMutexWriteOccupation = reader_writer + _T(" Mutex occupation - FAILED");
		}
		else {
			logOfMutexReadOccupation = reader_writer + _T(" Mutex occupation - FAILED");
		}
	}
}


void Protocol::mutexRelease(HANDLE hMutex, CString reader_writer) {
	if (ReleaseMutex(hMutex) == true) {
		if (reader_writer == _T("Writer")) {
			logOfMutexWriteRelease = reader_writer + _T(" Mutex - RELEASED");
		}
		else {
			logOfMutexReadRelease = reader_writer + _T(" Mutex - RELEASED");
		}
	}
	else {
		if (reader_writer == _T("Writer")) {
			logOfMutexWriteRelease = _T("Release of ") + reader_writer + _T(" Mutex - FAILED");
		}
		else {
			logOfMutexReadRelease = _T("Release of ") + reader_writer + _T(" Mutex - FAILED");
		}
	}
}

//CString handleType = ( _T("Reader") || _T("Writer") || _T("FileMapping") || _T("Buffer") )
void Protocol::handleClose(bool state, CString handleType) {
	if (handleType == _T("Reader")) {
		if (state == true) {
			logOfHandleClose_mutexRead = handleType + _T(" Mutex - CLOSED");
		}
		else {
			logOfHandleClose_mutexRead = handleType + _T(" Mutex closing - FAILED");
		}
	}
	else if (handleType == _T("Writer")) {
		if (state == true) {
			logOfHandleClose_mutexWrite = handleType + _T(" Mutex - CLOSED");
		}
		else {
			logOfHandleClose_mutexWrite = handleType + _T(" Mutex closing - FAILED");
		}
	}
	else if (handleType == _T("FileMapping")) {
		if (state == true) {
			logOfHandleClose_fileMapping = handleType + _T(" Mutex - CLOSED");
		}
		else {
			logOfHandleClose_fileMapping = handleType + _T(" Mutex closing - FAILED");
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
	if (FileExists(_T("ClientLogFile.log"))) {
		//CString name = _T("ClientLogFile") + rand() % 1000;
		logFile.Open(_T("ClientLogFile.log"), CStdioFile::modeNoTruncate | CStdioFile::typeText);
	}
	else {
		logFile.Open(_T("ClientLogFile.log"), CFile::modeCreate | CFile::modeWrite | CStdioFile::typeText);
	}
}


void Protocol::getLogOfFileMap() {
	logFile.WriteString(prefix + logOfFileMap + _T("\n"));
}


void Protocol::getlogOfMutexRead() {
	logFile.WriteString(prefix + logOfMutexRead + _T("\n"));
}


void Protocol::getLogOfMutexWrite() {
	logFile.WriteString(prefix + logOfMutexWrite + _T("\n"));
}


void Protocol::getLogOfMapViewOfFile() {
	logFile.WriteString(prefix + logOfMapViewOfFile + _T("\n"));
}


void Protocol::getLogOfMutexWriteOccupation() {
	logFile.WriteString(prefix + logOfMutexWriteOccupation + _T("\n"));
}


void Protocol::getLogOfMutexWriteRelease() {
	logFile.WriteString(prefix + logOfMutexWriteRelease + _T("\n"));
}


void Protocol::getLogOfMutexReadOccupation() {
	logFile.WriteString(prefix + logOfMutexReadOccupation + _T("\n"));
}


void Protocol::getLogOfMutexReadRelease() {
	logFile.WriteString(prefix + logOfMutexReadRelease + _T("\n"));
}


void Protocol::setSeparator() {
	logFile.WriteString(_T("-------------------------------------\n\n"));
}


void Protocol::getLogOfHandleClose_mutexRead() {
	logFile.WriteString(prefix + logOfHandleClose_mutexRead + _T("\n"));
}


void Protocol::getLogOfHandleClose_mutexWrite() {
	logFile.WriteString(prefix + logOfHandleClose_mutexWrite + _T("\n"));
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
	getlogOfMutexRead();
	getLogOfMutexWrite();
	getLogOfMapViewOfFile();
	getLogOfMutexWriteOccupation();
	getLogOfMutexWriteRelease();
	setSeparator();
}


void Protocol::getServerLogOfDataClode() {
	getLogOfHandleClose_mutexRead();
	getLogOfHandleClose_mutexWrite();
	getLogOfUnmapViewOfFile();
	getLogOfHandleClose_fileMapping();
	setSeparator();
}


void Protocol::overallProtocol() {
	clientLogFileCreation();


}


Protocol::~Protocol() {
	delete(this);
}