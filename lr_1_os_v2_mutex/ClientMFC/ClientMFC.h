
// ClientMFC.h : ������� ���� ��������� ��� ���������� PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�������� stdafx.h �� ��������� ����� ����� � PCH"
#endif

#include "resource.h"		// �������� �������


// CClientMFCApp:
// � ���������� ������� ������ ��. ClientMFC.cpp
//

class CClientMFCApp : public CWinApp
{
public:
	CClientMFCApp();

// ���������������
public:
	virtual BOOL InitInstance();

// ����������

	DECLARE_MESSAGE_MAP()
};

extern CClientMFCApp theApp;