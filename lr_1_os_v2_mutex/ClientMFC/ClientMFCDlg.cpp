
// ClientMFCDlg.cpp : файл реализации
//

#include "stdafx.h"
#include "ClientMFC.h"
#include "ClientMFCDlg.h"
#include "afxdialogex.h"

#include "protocol.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Диалоговое окно CAboutDlg используется для описания сведений о приложении

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Данные диалогового окна
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

// Реализация
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// диалоговое окно CClientMFCDlg



CClientMFCDlg::CClientMFCDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CClientMFCDlg::IDD, pParent)
	, static_factValue(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CClientMFCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_FACT, static_factValue);
}

BEGIN_MESSAGE_MAP(CClientMFCDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_ACTIVATE()
END_MESSAGE_MAP()


// обработчики сообщений CClientMFCDlg

BOOL CClientMFCDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Добавление пункта "О программе..." в системное меню.

	// IDM_ABOUTBOX должен быть в пределах системной команды.
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

	// Задает значок для этого диалогового окна.  Среда делает это автоматически,
	//  если главное окно приложения не является диалоговым
	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок

	// TODO: добавьте дополнительную инициализацию

	return TRUE;  // возврат значения TRUE, если фокус не передан элементу управления
}

void CClientMFCDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// При добавлении кнопки свертывания в диалоговое окно нужно воспользоваться приведенным ниже кодом,
//  чтобы нарисовать значок.  Для приложений MFC, использующих модель документов или представлений,
//  это автоматически выполняется рабочей областью.

void CClientMFCDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // контекст устройства для рисования

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Выравнивание значка по центру клиентского прямоугольника
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Нарисуйте значок
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// Система вызывает эту функцию для получения отображения курсора при перемещении
//  свернутого окна.
HCURSOR CClientMFCDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



Protocol *protocol = new Protocol(_T("Client"));

int factorial(int value) {

	if (value < 0) {
		return 0;
	}
	if (value == 0) {
		return 1;
	}
	else {
		return value * factorial(value - 1);
	}

}


void CClientMFCDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialogEx::OnActivate(nState, pWndOther, bMinimized);
	
	int ReadCount = 0;

	HANDLE FileMapping = OpenFileMapping(FILE_MAP_READ, false, _T("FileMapping"));
	protocol->VirificationOfFileMapping(FileMapping);

	if (FileMapping == NULL) {
		return;
	}

	void *Buffer = MapViewOfFile(FileMapping, FILE_MAP_READ, 0, 0, 1024);
	protocol->VirificationOfMapViewOfFile(Buffer);
	HANDLE hMutexRead = OpenMutex(SYNCHRONIZE, false, _T("MutexRead"));
	protocol->VirificationOfMutex(hMutexRead, _T("Reader"), false);

	if (hMutexRead == NULL)	{
		return;
	}

	HANDLE hMutexWrite = OpenMutex(SYNCHRONIZE, false, _T("MutexWrite"));
	protocol->VirificationOfMutex(hMutexWrite, _T("Writer"), false);

	if (hMutexWrite == NULL) {
		return;
	}

	WaitForSingleObject(hMutexRead, INFINITE);
	protocol->mutexOccupation(hMutexRead, _T("Reader"));
	ReadCount++;

	if (ReadCount == 1) {
		WaitForSingleObject(hMutexWrite, INFINITE);
		protocol->mutexOccupation(hMutexWrite);
	}

	ReleaseMutex(hMutexRead);
	protocol->mutexRelease(hMutexRead, _T("Reader"));
			
	int value = atoi((char*)Buffer);
	//value = factorial(value);
	static_factValue = value;
	UpdateData(0);
	
	ReadCount--;

	if (ReadCount == 0)	{
		ReleaseMutex(hMutexWrite);
		protocol->mutexRelease(hMutexWrite);
	}
	
	protocol->overallProtocol();
}
