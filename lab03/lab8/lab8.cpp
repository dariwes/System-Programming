#include "framework.h"
#include "lab8.h"
#include "math.h"
#define MAX_LOADSTRING 100
#define BUTTON_DRAW_ID 1
#define BUTTON_CLEAR_ID 2

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

bool pictureVisible = false;
HWND buttonAdd, buttonClear;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.

	// Initialize global strings
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_LAB8, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB8));

	MSG msg;

	// Main message loop:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAB8));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(CreateSolidBrush(RGB(220, 220, 255)));
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_LAB8);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Store instance handle in our global variable

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	buttonAdd = CreateWindow(L"BUTTON", NULL, WS_VISIBLE | WS_CHILD | BS_OWNERDRAW | BS_PUSHBUTTON,
		20, 320, 60, 60, hWnd, (HMENU)BUTTON_DRAW_ID, NULL, NULL);
	buttonClear = CreateWindow(L"BUTTON", NULL, WS_VISIBLE | WS_CHILD | BS_OWNERDRAW | BS_PUSHBUTTON,
		90, 320, 60, 60, hWnd, (HMENU)BUTTON_CLEAR_ID, NULL, NULL);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//

void Figure(int R1, int R2, int count, int XC, int YC, HDC hdc)
{
	double step = 0.2 * 3.14, angle = 1.6 * 3.14;

	POINT P[10];

	while (++count < 10)
	{
		P[count].x = XC + R1 * sin(angle);
		P[count].y = YC - R1 * cos(angle);
		angle += step;
		P[++count].x = XC + R2 * sin(angle);
		P[count].y = YC - R2 * cos(angle);
		angle += step;
	}

	Polygon(hdc, P, 10);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;

	switch (message)
	{
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case BUTTON_DRAW_ID:
			pictureVisible = true;
			InvalidateRect(hWnd, NULL, true);
			break;
		case BUTTON_CLEAR_ID:
			pictureVisible = false;
			InvalidateRect(hWnd, NULL, true);
			break;
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_DRAWITEM:
	{
		DRAWITEMSTRUCT* draw = (DRAWITEMSTRUCT*)lParam;
		HBRUSH brush;
		HPEN pen;

		switch (draw->CtlID)
		{
		case BUTTON_DRAW_ID:
		{
			HDC hdc = draw->hDC;

			if (draw->itemAction == ODA_SELECT) {
				brush = CreateSolidBrush(RGB(0, 0, 255));
			}
			else {
				brush = CreateSolidBrush(RGB(0, 255, 0));
			}


			pen = CreatePen(BS_SOLID, 2, RGB(0, 0, 0));

			SelectObject(hdc, brush);
			SelectObject(hdc, pen);

			Rectangle(hdc, 0, 0, 60, 60);
			MoveToEx(hdc, 10, 30, NULL);
			LineTo(hdc, 50, 30);
			MoveToEx(hdc, 30, 10, NULL);
			LineTo(hdc, 30, 50);
		}
		break;
		case BUTTON_CLEAR_ID:
		{
			HDC hdc = draw->hDC;

			if (draw->itemAction == ODA_SELECT) {
				brush = CreateSolidBrush(RGB(0, 0, 255));
			}
			else {
				brush = CreateSolidBrush(RGB(255, 0, 0));
			}
			pen = CreatePen(BS_SOLID, 2, RGB(0, 0, 0));

			SelectObject(hdc, brush);
			SelectObject(hdc, pen);

			Rectangle(hdc, 0, 0, 60, 60);
			MoveToEx(hdc, 20, 20, NULL);
			LineTo(hdc, 40, 40);
			MoveToEx(hdc, 40, 20, NULL);
			LineTo(hdc, 20, 40);
		}
		break;
		default:
			break;
		}

	}
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...

		HBRUSH brush;
		HPEN pen;
		if (pictureVisible)
		{
			brush = CreateSolidBrush(RGB(149, 184, 242));
			SelectObject(hdc, brush);
			Figure(25, 10, -1, 80, 80, hdc);
			Figure(25, 10, -1, 340, 100, hdc);
			Figure(25, 10, -1, 500, 70, hdc);
			Figure(25, 10, -1, 600, 220, hdc);
			Figure(25, 10, -1, 60,290, hdc);

			brush = CreateSolidBrush(RGB(198, 73, 99));
			SelectObject(hdc, brush);
			Ellipse(hdc, 120, 150, 270, 300);

			static int dx = 6;
			static int dy = 7;

			pen = CreatePen(BS_SOLID, 6, RGB(90, 57, 110));
			SelectObject(hdc, pen);

			MoveToEx(ps.hdc, 400, 200, 0);
			LineTo(ps.hdc, 400 + 3 * dx, 200 - 5 * dy);
			LineTo(ps.hdc, 400 + 6 * dx, 200 + 0 * dy);
			LineTo(ps.hdc, 400 + 6 * dx, 200 + 18 * dy);
			LineTo(ps.hdc, 400 + 0 * dx, 200 + 18 * dy);
			LineTo(ps.hdc, 400, 200);

			LineTo(ps.hdc, 400 + 0 * dx, 200 + 6 * dy);
			LineTo(ps.hdc, 400 - 5 * dx, 200 + 10 * dy);
			LineTo(ps.hdc, 400 - 5 * dx, 200 + 20 * dy);
			LineTo(ps.hdc, 400 - 2 * dx, 200 + 20 * dy);
			LineTo(ps.hdc, 400 + 0 * dx, 200 + 18 * dy);

			MoveToEx(ps.hdc, 400 + 6 * dx, 200 + 0 * dy, 0);
			LineTo(ps.hdc, 400 + 6 * dx, 200 + 6 * dy);
			LineTo(ps.hdc, 400 + 11 * dx, 200 + 10 * dy);
			LineTo(ps.hdc, 400 + 11 * dx, 200 + 20 * dy);
			LineTo(ps.hdc, 400 + 8 * dx, 200 + 20 * dy);
			LineTo(ps.hdc, 400 + 6 * dx, 200 + 18 * dy);

			pen = CreatePen(BS_SOLID, 3, RGB(90, 57, 110));
			SelectObject(hdc, pen);

			brush = CreateSolidBrush(RGB(198, 73, 99));
			SelectObject(hdc, brush);

			Rectangle(hdc, 410, 240, 425, 255);
		}
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}


// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}