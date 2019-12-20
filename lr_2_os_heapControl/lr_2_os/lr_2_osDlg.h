
// lr_2_osDlg.h : файл заголовка
//

#pragma once


// диалоговое окно Clr_2_osDlg
class Clr_2_osDlg : public CDialogEx
{
// Создание
public:
	Clr_2_osDlg(CWnd* pParent = NULL);	// стандартный конструктор

// Данные диалогового окна
	enum { IDD = IDD_LR_2_OS_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// поддержка DDX/DDV


// Реализация
protected:
	HICON m_hIcon;

	// Созданные функции схемы сообщений
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CString edit_memoryMap_v;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	CString edit_systemInfo_v;
	int edit_arrayDimension_v;
	CString edit_arrayInsertion_v;
	CString edit_arrayOutput_v;
	afx_msg void OnBnClickedButtonExit();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
};
