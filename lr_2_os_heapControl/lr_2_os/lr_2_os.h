
// lr_2_os.h : ������� ���� ��������� ��� ���������� PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�������� stdafx.h �� ��������� ����� ����� � PCH"
#endif

#include "resource.h"		// �������� �������


// Clr_2_osApp:
// � ���������� ������� ������ ��. lr_2_os.cpp
//

class Clr_2_osApp : public CWinApp
{
public:
	Clr_2_osApp();

// ���������������
public:
	virtual BOOL InitInstance();

// ����������

	DECLARE_MESSAGE_MAP()
};

extern Clr_2_osApp theApp;