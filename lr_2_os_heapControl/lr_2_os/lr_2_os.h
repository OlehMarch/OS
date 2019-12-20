
// lr_2_os.h : главный файл заголовка для приложения PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "включить stdafx.h до включения этого файла в PCH"
#endif

#include "resource.h"		// основные символы


// Clr_2_osApp:
// О реализации данного класса см. lr_2_os.cpp
//

class Clr_2_osApp : public CWinApp
{
public:
	Clr_2_osApp();

// Переопределение
public:
	virtual BOOL InitInstance();

// Реализация

	DECLARE_MESSAGE_MAP()
};

extern Clr_2_osApp theApp;