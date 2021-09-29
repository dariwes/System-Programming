#include "framework.h"
#include <windows.h>
#include <tchar.h>
#include <wingdi.h>
#include "win1.h"

#define CLICK_START 4
#define CLICK_STOP 5
#define WINDOW_WIDTH 850
#define WINDOW_HEIGHT 450
#define IDM_FILE_QUIT 3
#define ICON "photo.ico"

bool FLAG = false;
int X = WINDOW_WIDTH / 2 - 250;
int Y = WINDOW_HEIGHT / 2 - 40;


void AddMenus(HWND hwnd)
{
	HMENU hMenubar;
	HMENU hMenu;

	hMenubar = CreateMenu();
	hMenu = CreateMenu();

	AppendMenuW(hMenu, MF_STRING, CLICK_START, L"&Начать двигаться");
	AppendMenuW(hMenu, MF_STRING, CLICK_STOP, L"&Закончить двигаться");
	AppendMenuW(hMenu, MF_SEPARATOR, NULL, NULL);
	AppendMenuW(hMenu, MF_STRING, NULL, L"&Шагнуть");

	AppendMenuW(hMenubar, MF_POPUP, (UINT_PTR)hMenu, L"&Что я могу");
	SetMenu(hwnd, hMenubar);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	const int TIMER_ID = 1;

	switch (message)
	{
	case WM_CREATE:
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDM_FILE_QUIT:
			SendMessage(hWnd, WM_CLOSE, 0, 0);
			break;
		case CLICK_START:
		{
			UINT_PTR ret = SetTimer(hWnd, TIMER_ID, 50, NULL);
			if (ret == 0)
				MessageBox(hWnd, _T("Timer not set"), _T("Error"), MB_OK | MB_ICONEXCLAMATION);
		}
		break;
		case CLICK_STOP:
			KillTimer(hWnd, TIMER_ID);
		break;
		}

	case WM_TIMER:
	{
		if (X >= WINDOW_WIDTH - 200) FLAG = true;
		if (X <= 150) FLAG = false;
		if (!FLAG) X += 2;
		else X -= 2;
		
		InvalidateRect(hWnd, NULL, TRUE);
	}
	break;

	case WM_PAINT:
	{
		const char text[] = "Hi..";
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		TextOutA(hdc, X, Y, text, strlen(text));
		EndPaint(hWnd, &ps);
	}
	break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

ATOM RegMyWindowClass(HINSTANCE hInst, LPCTSTR lpzClassName)
{
	WNDCLASS wcWindowClass = { 0 };
	wcWindowClass.lpfnWndProc = (WNDPROC)WndProc;
	wcWindowClass.style = CS_HREDRAW | CS_VREDRAW;
	wcWindowClass.hInstance = hInst;
	wcWindowClass.lpszClassName = lpzClassName;
	wcWindowClass.hCursor = (HICON)LoadImage(NULL, _T("./poin.ico"), IMAGE_CURSOR, 20, 20, LR_LOADFROMFILE);
	wcWindowClass.hIcon = (HICON)LoadImage(NULL, _T("./photo.ico"), IMAGE_ICON, 0, 0, LR_LOADFROMFILE);
	wcWindowClass.hbrBackground = (HBRUSH)(CreateSolidBrush(RGB(255, 255, 255)));
	return RegisterClass(&wcWindowClass);
}

int APIENTRY WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	LPCTSTR lpzClass = _T("My Window Class!");
	if (!RegMyWindowClass(hInstance, lpzClass))
		return 1;

	RECT screen_rect;
	GetWindowRect(GetDesktopWindow(), &screen_rect);
	int x = screen_rect.right / 2 - 150;
	int y = screen_rect.bottom / 2 - 75;

	HWND hWnd = CreateWindow(lpzClass, _T("Lab6 SP"),
		WS_OVERLAPPEDWINDOW | WS_VISIBLE, x, y, WINDOW_WIDTH, WINDOW_HEIGHT, NULL, NULL,
		hInstance, NULL);

	if (!hWnd)
		return 2;

	AddMenus(hWnd);

	MSG msg = { 0 };
	int iGetOk = 0;
	while ((iGetOk = GetMessage(&msg, NULL, 0, 0)) != 0)
	{
		if (iGetOk == -1)
			return 3;
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}