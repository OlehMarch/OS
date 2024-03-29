﻿#pragma once
//////////////////////// Windows Version Build Option /////////////////////////
#define _WIN32_WINNT 0x0500
//#define WINVER       0x0500
//////////////////////////// Unicode Build Option /////////////////////////////
#ifndef _M_IX86
#define UNICODE
#endif
//#define UNICODE
#ifdef UNICODE
#define _UNICODE
#endif
///////////////////////////// Windows Definitions /////////////////////////////
#pragma warning(push, 3)
#include <Windows.h>
#pragma warning(pop)
#pragma warning(push, 4)

#ifndef WT_EXECUTEINPERSISTENTIOTHREAD
#pragma message("You are not using the latest Platform SDK header/library ")
#pragma message("files. This may prevent the project from building correctly.")
#endif


#pragma warning(disable:4001)
#pragma warning(disable:4100)
#pragma warning(disable:4699)
#pragma warning(disable:4710)
#pragma warning(disable:4514)
#pragma warning(disable:4512)
//////////////////// Pragma Message ////////////////////

#define chSTR2(x)      #x
#define chSTR(x)   chSTR2(x)
#define chMSG(desc) message(__FILE__ "(" chSTR(__LINE__) "):" #desc)
////////////////////////////// chINRANGE ///////////////////////////////
#define chINRANGE(low, Num, High) (((low) <= (Num)) && ((Num) <= (High)))
//////////////////////////////// chDIMOF ///////////////////////////////
#define chDIMOF(Array) (sizeof(Array) / sizeof(Array[0]))
//////////////////////////// chBEGINTHREADEX ///////////////////////////
typedef unsigned(__stdcall *PTHREAD_START) (void *);
#define chBEGINTHREADEX(psa, cbStack, pfnStartAddr,\
pvParam, fdwCreate, pdwThreadId)\
((HANDLE)_beginthreadex(\
(void *)(psa),\
(unsigned)(cbStack),\
(PTHREAD_START)(pfnStartAddr),\
(void *)(pvParam),\
(unsigned)(fdwCreate),\
(unsigned *)(pdwThreadId)))

#ifdef _X86_
#define DebugBreak()    _asm { int 3 }
#endif

#define MAKESOFTWAREEXCEPTION(Severity, Facility, Exception) \
((DWORD) ( \
  (Severity       ) |\
  (1         << 29) |\
  (0         << 28) |\
  (Facility  << 16) |\
  (Exception <<  0)))
/////////////////////////// Quick MessageBox ///////////////////////////
inline void chMB(PCSTR s) {
	char szTMP[128];
	GetModuleFileNameA(NULL, szTMP, chDIMOF(szTMP));
	MessageBoxA(GetActiveWindow(), s, szTMP, MB_OK);
}
///////////////////////// chASSERT, chVERIFY /////////////////////////
inline void chFAIL(PSTR szMsg) {
	chMB(szMsg);
	DebugBreak();
}
inline void chASSERTFAIL(LPCSTR file, int line, PCSTR expr) {
	char sz[128];
	wsprintfA(sz, "File %s, line %d : %s", file, line, expr);
	chFAIL(sz);
}
#ifdef _DEBUG
#define chASSERT(x) if (!(x)) chASSERTFAIL(__FILE__, __LINE__, #x)
#else
#define chASSERT(x)
#endif
#ifdef _DEBUG
#define chVERIFY(x) chASSERT(x)
#else
#define chVERIFY(x) (x)
#endif
/////////////////////////// chHANDLE_DLGMSG ////////////////////////////
#define chHANDLE_DLGMSG(hwnd, message, fn)\
case (message): return (SetDlgMsgResult(hwnd, uMsg,\
HANDLE_##message((hwnd), (wParam), (lParam), (fn))))
//////////////////////////// chSETDLGICONS /////////////////////////////
inline void chSETDLGICONS(HWND hwnd, int idi) {
	SendMessage(hwnd, WM_SETICON, TRUE, (LPARAM)
		LoadIcon((HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
		MAKEINTRESOURCE(idi)));
	SendMessage(hwnd, WM_SETICON, FALSE, (LPARAM)
		LoadIcon((HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
		MAKEINTRESOURCE(idi)));
}

inline void chWindows9xNotAllowed() {
	OSVERSIONINFO vi = { sizeof(vi) };
	GetVersionEx(&vi);
	if (vi.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS) {
		chMB("This application requires features not present in Windows 9x.");
		ExitProcess(0);
	}
}
inline void chWindows2000Required() {
	OSVERSIONINFO vi = { sizeof(vi) };
	GetVersionEx(&vi);
	if ((vi.dwPlatformId != VER_PLATFORM_WIN32_NT) && (vi.dwMajorVersion < 5)) {
		chMB("This application requires features present in Windows 2000.");
		ExitProcess(0);
	}
}

#ifdef UNICODE
class CUnicodeSupported {
public:
	CUnicodeSupported() {
		if (GetWindowsDirectoryW(NULL, 0) <= 0) {
			chMB("This application requires an OS that supports Unicode.");
			ExitProcess(0);
		}
	}
};
static CUnicodeSupported g_UnicodeSupported;
#endif
#pragma comment(linker, "/subsystem:Windows")