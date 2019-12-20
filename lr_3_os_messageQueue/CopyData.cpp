//#include "CmnHdr.h"
//#include <windowsx.h>
//#include <tchar.h>
//#include <malloc.h>
//#include "resource.h"
/////////////////////////////////////////////////////////////////////////////////
//BOOL Dlg_OnCopyData(HWND hwnd, HWND hwndFrom, PCOPYDATASTRUCT cds) {
//	Edit_SetText(GetDlgItem(hwnd, cds->dwData ? IDC_DATA2 : IDC_DATA1),
//		(PTSTR)cds->lpData);
//	return(TRUE);
//}
/////////////////////////////////////////////////////////////////////////////////
//BOOL Dlg_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam) {
//	chSETDLGICONS(hwnd, IDI_COPYDATA);
//	Edit_SetText(GetDlgItem(hwnd, IDC_DATA1), TEXT("Some test data"));
//	Edit_SetText(GetDlgItem(hwnd, IDC_DATA2), TEXT("Some more test data"));
//	return(TRUE);
//}
/////////////////////////////////////////////////////////////////////////////////
//void Dlg_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify) {
//	switch (id) {
//	case IDCANCEL:
//		EndDialog(hwnd, id);
//		break;
//	case IDC_COPYDATA1:
//	case IDC_COPYDATA2:
//		if (codeNotify != BN_CLICKED)
//			break;
//		HWND hwndEdit = GetDlgItem(hwnd,
//			(id == IDC_COPYDATA1) ? IDC_DATA1 : IDC_DATA2);
//		COPYDATASTRUCT cds;
//		cds.dwData = (DWORD)((id == IDC_COPYDATA1) ? 0 : 1);
//		cds.cbData = (Edit_GetTextLength(hwndEdit) + 1) * sizeof(TCHAR);
//		cds.lpData = _alloca(cds.cbData);
//		Edit_GetText(hwndEdit, (PTSTR)cds.lpData, cds.cbData);
//		TCHAR szCaption[100];
//		GetWindowText(hwnd, szCaption, chDIMOF(szCaption));
//		HWND hwndT = NULL;
//		do {
//			hwndT = FindWindowEx(NULL, hwndT, NULL, szCaption);
//			if (hwndT != NULL) {
//				FORWARD_WM_COPYDATA(hwndT, hwnd, &cds, SendMessage);
//			}
//		} while (hwndT != NULL);
//		break;
//	}
//}
/////////////////////////////////////////////////////////////////////////////////
//INT_PTR WINAPI Dlg_Proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
//	switch (uMsg) {
//		chHANDLE_DLGMSG(hwnd, WM_INITDIALOG, Dlg_OnInitDialog);
//		chHANDLE_DLGMSG(hwnd, WM_COMMAND, Dlg_OnCommand);
//		chHANDLE_DLGMSG(hwnd, WM_COPYDATA, Dlg_OnCopyData);
//	}
//	return(FALSE);
//}
/////////////////////////////////////////////////////////////////////////////////
//int WINAPI _tWinMain(HINSTANCE hinstExe, HINSTANCE, PTSTR pszCmdLine, int) {
//	DialogBox(hinstExe, MAKEINTRESOURCE(IDD_COPYDATA), NULL, Dlg_Proc);
//	return(0);
//}
/////////////////////////////////////////////////////////////////////////////////