#include "stdafx.h"

static TCHAR szWindowClass[] = _T("winApiApplication");
static TCHAR szTitle[] = _T("Client");
HINSTANCE hInst;
#define ID_BUTTON1 1
#define ID_BUTTON2 3
#define ID_EDIT1 2
#define ID_EDIT2 4
#define ID_STATIC1 10
#define SEND_MESSAGE 100

PCOPYDATASTRUCT pMyCDS;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)				//Функция обработки сообщений
{
	PAINTSTRUCT ps;
	HDC hdc;
	switch (message)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;
	case WM_COMMAND:
		if (wParam == ID_BUTTON1)
		{
			TCHAR buf1[100];
			GetDlgItemText(hWnd, ID_EDIT1, buf1, sizeof(buf1));
			LPCTSTR message = (LPCTSTR)buf1;
			MessageBox(NULL, message, _T("Input Text"), MB_OK);
		}
		if (wParam == ID_BUTTON2)
		{
			TCHAR buf1[100];
			GetDlgItemText(hWnd, ID_EDIT1, buf1, sizeof(buf1));
			LPCTSTR message = (LPCTSTR)buf1;
			COPYDATASTRUCT cds;
			cds.dwData = SEND_MESSAGE;
			cds.cbData = sizeof(TCHAR) * (_tcslen(message) + 1);
			cds.lpData = (void*)message;
			HWND hwDispatch = FindWindow(_T("winApiApplication"), _T("Server"));
			if (hwDispatch != NULL) {
				SendMessage(hwDispatch, WM_COPYDATA, (WPARAM)hWnd, (LPARAM)(LPVOID)&cds);
			}
			else {
				MessageBox(hWnd, _T("Can't send WM_COPYDATA, thus, send to ID_EDIT2"), _T("MyApp"), MB_OK);
				SetDlgItemText(hWnd, ID_EDIT2, message);
			}
		}
		break;
	case WM_COPYDATA:
		pMyCDS = (PCOPYDATASTRUCT)lParam;
		switch (pMyCDS->dwData)
		{
		case SEND_MESSAGE:
			LPCTSTR message = (LPCTSTR)pMyCDS->lpData;
			SetDlgItemText(hWnd, ID_EDIT2, message);
		}
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}
	return 0;
}

//Главная функция входа
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wcex;			//Создаем класс окна

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

	if (!RegisterClassEx(&wcex))	//Регистрируем класс
	{
		MessageBox(NULL,
			_T("Call to RegisterClassEx failed!"),
			_T("Win32 Guided Tour"),
			NULL);

		return 1;
	}
	hInst = hInstance; // Store instance handle in our global variable
	HWND hWnd = CreateWindow(			//Создаем родительское окно
		szWindowClass,
		szTitle,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		500, 130,
		NULL,
		NULL,
		hInstance,
		NULL
		);

	/****************************************************** Создаем элементы окна ************************************************************/
	//Создаем кнопку
	CreateWindow(TEXT("button"), TEXT("Show Message"), WS_CHILD | WS_VISIBLE, 270, 10, 200, 30, hWnd, (HMENU)ID_BUTTON1, hInstance, 0);
	CreateWindow(TEXT("button"), TEXT("Send Message"), WS_CHILD | WS_VISIBLE, 270, 50, 200, 30, hWnd, (HMENU)ID_BUTTON2, hInstance, 0);
	//Создаем line edit
	CreateWindow(_T("EDIT"), _T(""), WS_CHILD | WS_BORDER | WS_VISIBLE | ES_MULTILINE, 10, 10, 250, 30, hWnd, (HMENU)ID_EDIT1, hInstance, 0);
	CreateWindow(_T("EDIT"), _T(""), WS_CHILD | WS_BORDER | WS_VISIBLE | ES_MULTILINE, 10, 50, 250, 30, hWnd, (HMENU)ID_EDIT2, hInstance, 0);
	CreateWindow(_T("STATIC"), _T("text"), WS_CHILD | WS_VISIBLE | SS_CENTER, 10, 100, 50, 20, hWnd, (HMENU)ID_STATIC1, hInstance, 0);

	/****************************************************** Элементы окна созданы ************************************************************/

	if (!hWnd)
	{
		MessageBox(NULL,
			_T("Call to CreateWindow failed!"),
			_T("Win32 Guided Tour"),
			NULL);

		return 1;
	}
	// hWnd: возвращаемое значение после создания окна
	// nCmdShow: четвертый парметр от WinMain
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))		//Цикл обработки сообщений
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}