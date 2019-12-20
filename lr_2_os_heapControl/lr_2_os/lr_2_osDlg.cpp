
// lr_2_osDlg.cpp : файл реализации
//

#include "stdafx.h"
#include "lr_2_os.h"
#include "lr_2_osDlg.h"
#include "afxdialogex.h"

#include "CHeapControl.h"
#include <windows.h>
#include <algorithm>
#include <vector>


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


// диалоговое окно Clr_2_osDlg



Clr_2_osDlg::Clr_2_osDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(Clr_2_osDlg::IDD, pParent)
	, edit_memoryMap_v(_T(""))
	, edit_systemInfo_v(_T(""))
	, edit_arrayDimension_v(0)
	, edit_arrayInsertion_v(_T(""))
	, edit_arrayOutput_v(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void Clr_2_osDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, edit_memoryMap_v);
	DDX_Text(pDX, IDC_EDIT2, edit_systemInfo_v);
	DDX_Text(pDX, IDC_EDIT4, edit_arrayDimension_v);
	DDV_MinMaxInt(pDX, edit_arrayDimension_v, 2, 10000);
	DDX_Text(pDX, IDC_EDIT3, edit_arrayInsertion_v);
	DDX_Text(pDX, IDC_EDIT5, edit_arrayOutput_v);
}

BEGIN_MESSAGE_MAP(Clr_2_osDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &Clr_2_osDlg::OnBnClickedOk)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_BUTTON_EXIT, &Clr_2_osDlg::OnBnClickedButtonExit)
	ON_BN_CLICKED(IDC_BUTTON1, &Clr_2_osDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &Clr_2_osDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &Clr_2_osDlg::OnBnClickedButton3)
END_MESSAGE_MAP()


// обработчики сообщений Clr_2_osDlg

BOOL Clr_2_osDlg::OnInitDialog()
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

void Clr_2_osDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void Clr_2_osDlg::OnPaint()
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
HCURSOR Clr_2_osDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



//To avoid error C4996 (std) write to preprocessor "_SCL_SECURE_NO_WARNINGS"
CHeapControl heap;
CHeapControl *heap2;
int *Array;


using namespace std;
template <typename T>
void mergeSort(T* elems, //original array
	T* tmp_elems, //temp array to hold intermediate results, should be same size as array "elems"
	size_t size)
{
	if (size <= 1) return; //nothing to sort

	const size_t left_size = size / 2;
	const size_t right_size = size - left_size;

	//memStatusUnderSorting += heap.MemStatus();

	mergeSort(elems, tmp_elems, left_size);
	mergeSort(elems + left_size, tmp_elems + left_size, right_size);

	T* leftIt = elems; // pointer to walk through left part
	T* const pivot = elems + left_size; //end of left part, start of right  part
	T* rightIt = pivot; // pointer to walk through right part
	T* const end = elems + size;
	T* outputIt = tmp_elems; //pointer to where to write when merging left and right subparts

	while (true)
	{
		if (*leftIt < *rightIt)
		{
			*outputIt++ = *leftIt++;
			if (leftIt == pivot)
			{
				// copy the rest of right part that has not been copied yet
				copy(rightIt, end, outputIt);
				break;
			}
		}
		else
		{
			*outputIt++ = *rightIt++;
			if (rightIt == end)
			{
				// copy the rest of left part that has not been copied yet
				copy(leftIt, pivot, outputIt);
				break;
			}
		}
	}

	copy(tmp_elems, tmp_elems + size, elems);
}


CString heapCreationLog(bool state) {
	CString heapStatus;
	if (state) {
		heapStatus = _T("-------------- Куча создана --------------\r\n");
	}
	else {
		heapStatus = _T("-------------- Ошибка создания объекта! --------------\r\n");
	}
	return heapStatus;
}


CString heapDeleteLog(bool state) {
	CString heapStatus;
	if (state) {
		heapStatus = _T("-------------- Куча удалена --------------\r\n");
	}
	else {
		heapStatus = _T("-------------- Ошибка удаления объекта! --------------\r\n");
	}
	return heapStatus;
}


void Clr_2_osDlg::OnBnClickedOk()
{
	UpdateData(1);

	void *p = heap.allocInHeap(sizeof(CHeapControl*) * edit_arrayDimension_v);
	if (p != NULL) {
		edit_arrayOutput_v = heapCreationLog(true);
	}
	else {
		edit_arrayOutput_v += heapCreationLog(false);
	}
	heap.ArrayCreation(edit_arrayDimension_v);
	Array = heap.ArrayInsertion(edit_arrayInsertion_v);
	edit_arrayOutput_v += _T("Сортировка начинается...\r\n");
	heap.mergeSort(heap.Array, heap.Buff, edit_arrayDimension_v);
	edit_arrayOutput_v += heap.ArrayOutput(edit_arrayDimension_v);
	edit_arrayOutput_v += _T("\r\nСортировка завершена...\r\n");
	if (heap.deleteFromHeap(p, sizeof(CHeapControl*) * edit_arrayDimension_v)) {
		edit_arrayOutput_v += heapDeleteLog(true);
	}
	else {
		edit_arrayOutput_v += heapDeleteLog(true);
	}

	UpdateData(0);
}


int Clr_2_osDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	edit_memoryMap_v = _T("");
	edit_systemInfo_v = heap.HardwareInfo();
	edit_systemInfo_v += heap.MemStatus();

	return 0;
}


void Clr_2_osDlg::OnBnClickedButtonExit()
{
	exit(0);
}


void Clr_2_osDlg::OnBnClickedButton1()
{
	UpdateData(1);

	edit_memoryMap_v = _T("--------------- Определение состояния ---------------\r\n\
--------------- адресного пространства ---------------\r\n");

	edit_memoryMap_v += heap.AddressStatus();

	UpdateData(0);
}


void Clr_2_osDlg::OnBnClickedButton2()
{
	UpdateData(1);
	edit_systemInfo_v += heap.HardwareInfo();
	edit_systemInfo_v += heap.MemStatus();
	UpdateData(0);
}


void Clr_2_osDlg::OnBnClickedButton3()
{
	UpdateData(1);
	edit_arrayDimension_v = NULL;
	edit_arrayInsertion_v = _T("");
	edit_arrayOutput_v = _T("");
	edit_memoryMap_v = _T("");
	edit_systemInfo_v = _T("");
	UpdateData(0);
}
