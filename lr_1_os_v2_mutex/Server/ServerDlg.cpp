
// ServerDlg.cpp : ���� ����������
//

#include "stdafx.h"
#include "Server.h"
#include "ServerDlg.h"
#include "afxdialogex.h"

#include "protocol.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ���������� ���� CServerDlg



CServerDlg::CServerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CServerDlg::IDD, pParent)
	, edit_valueToFactCalc(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, edit_valueToFactCalc);
}

BEGIN_MESSAGE_MAP(CServerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CServerDlg::OnBnClickedOk)
	ON_WM_CREATE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// ����������� ��������� CServerDlg

BOOL CServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���������� ������ "� ���������..." � ��������� ����.

	// IDM_ABOUTBOX ������ ���� � �������� ��������� �������.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ������ ������ ��� ����� ����������� ����.  ����� ������ ��� �������������,
	//  ���� ������� ���� ���������� �� �������� ����������
	SetIcon(m_hIcon, TRUE);			// ������� ������
	SetIcon(m_hIcon, FALSE);		// ������ ������

	// TODO: �������� �������������� �������������

	return TRUE;  // ������� �������� TRUE, ���� ����� �� ������� �������� ����������
}

void CServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// ��� ���������� ������ ����������� � ���������� ���� ����� ��������������� ����������� ���� �����,
//  ����� ���������� ������.  ��� ���������� MFC, ������������ ������ ���������� ��� �������������,
//  ��� ������������� ����������� ������� ��������.

void CServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �������� ���������� ��� ���������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ������������ ������ �� ������ ����������� ��������������
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ��������� ������
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ������� �������� ��� ������� ��� ��������� ����������� ������� ��� �����������
//  ���������� ����.
HCURSOR CServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


HANDLE FileMapping;
HANDLE hMutexRead;
HANDLE hMutexWrite;
void *Buffer;
Protocol *protocol = new Protocol(_T("Server"));


void CServerDlg::OnBnClickedOk()
{
	UpdateData(1);

	char *str;
	char buffer[1024];
	int fact = 1;
	protocol->serverLogFileCreation();

	for (int i = 1; i <= edit_valueToFactCalc; i++) {
		if ((edit_valueToFactCalc < 0) || (edit_valueToFactCalc == 0)) {
			return;
		}
		else {
			fact *= i;

			str = _itoa(fact, buffer, 10);

			WaitForSingleObject(hMutexWrite, INFINITE);
			protocol->mutexOccupation(hMutexWrite);
			Sleep(200);
			memcpy(Buffer, str, 1024);
			ReleaseMutex(hMutexWrite);
			protocol->mutexRelease(hMutexWrite);

			protocol->getServerLogData();
		}
	}
}


int CServerDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

		//�������� �������� �������� 1024 � ������ MyFileMapping
	FileMapping = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, 1024, _T("FileMapping"));
	protocol->VirificationOfFileMapping(FileMapping);
	if (FileMapping == NULL)
	{
		MessageBox(_T("Error!"), _T("Error!"), NULL);
	}
		//���������� �������� ����� � �������� ������������ � ���������� ���������� �����
	Buffer = MapViewOfFile(FileMapping, FILE_MAP_WRITE, 0, 0, 1024);
	protocol->VirificationOfMapViewOfFile(Buffer);
	hMutexRead = CreateMutex(NULL, false, _T("MutexRead"));
	protocol->VirificationOfMutex(hMutexRead, _T("Reader"));
	hMutexWrite = CreateMutex(NULL, false, _T("MutexWrite"));
	protocol->VirificationOfMutex(hMutexWrite, _T("Writer"));

	return 0;
}


void CServerDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	bool state;

	state = CloseHandle(hMutexRead);
	protocol->handleClose(state);
	state = CloseHandle(hMutexWrite);
	protocol->handleClose(state, _T("Writer"));
	state = UnmapViewOfFile(Buffer);
	protocol->handleClose(state, _T("Buffer"));
	state = CloseHandle(FileMapping);	
	protocol->handleClose(state, _T("FileMapping"));

	protocol->getServerLogOfDataClode();
	protocol->logFileClose();
}
