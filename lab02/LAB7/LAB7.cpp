#include "framework.h"
#include "LAB7.h"

#define MAX_STRING   100
#define EDIT           1
#define BUTTON_ADD     2
#define BUTTON_CLEAR   3
#define BUTTON_TORIGHT 4 
#define BUTTON_DELETE  5
#define LISTBOX_1      6
#define LISTBOX_2      7


HINSTANCE hInst;                            // текущий экземпляр
WCHAR szTitle[MAX_STRING];                  // текст строки заголовка
WCHAR szWindowClass[MAX_STRING];            // имя класса главного окна


HWND hEdit, hButtonAdd, hButtonClear, hButtonToRight, 
       hButtonDelete, hListBox1, hListBox2;
HWND hWnd;

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

bool ContainsString(HWND hListBox, TCHAR* s) {
    int count = SendMessage(hListBox, LB_GETCOUNT, 0, 0);
    for (int i = 0; i < count; i++) {
        TCHAR buffer[256];
        SendMessage(hListBox, LB_GETTEXT, i, (LPARAM)buffer);

        if (!_tcscmp(s, buffer))
            return true;
    }

    return false;
}

void AddStringInListBox(HWND hListBox, TCHAR* s) {
    if (!ContainsString(hListBox, s)) {
        SendMessage(hListBox, LB_ADDSTRING, 0, (LPARAM)s);
    }
}

bool DeleteSelectedItem(HWND hListBox) {
    int selCount = SendMessage(hListBox, LB_GETSELCOUNT, 0, 0);
    int countBuffer[100];
    SendMessage(hListBox, LB_GETSELITEMS, 100, (LPARAM)countBuffer);

    for (int i = selCount - 1; i >= 0; --i) {
        int itemId = countBuffer[i];
        SendMessage(hListBox, LB_DELETESTRING, itemId, 0);
    }
    return true;
}

void ToRight() {
    int selCount = SendMessage(hListBox1, LB_GETSELCOUNT, 0, 0);
    int countBuffer[100];
    SendMessage(hListBox1, LB_GETSELITEMS, 100, (LPARAM)countBuffer);

    if (selCount == 0) 
        MessageBox(hWnd, L"Select item in first ListBox", L"Error", MB_OK);
    else {
        for (int i = selCount - 1; i >= 0; --i) {
            int itemId = countBuffer[i];
            TCHAR buffer[256];
            SendMessage(hListBox1, LB_GETTEXT, itemId, (LPARAM)buffer);
            AddStringInListBox(hListBox2, buffer);
        }
        DeleteSelectedItem(hListBox1);
    }
}


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                      _In_opt_ HINSTANCE hPrevInstance,
                      _In_ LPWSTR lpCmdLine,
                      _In_ int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_STRING);
    LoadStringW(hInstance, IDC_LAB7, szWindowClass, MAX_STRING);
    MyRegisterClass(hInstance);

    if (!InitInstance (hInstance, nCmdShow))
        return FALSE;

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB7));

    MSG msg;

    while (GetMessage(&msg, nullptr, 0, 0)) {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    return (int) msg.wParam;
}



//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  ЦЕЛЬ: Регистрирует класс окна.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAB7));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(CreateSolidBrush(RGB(55, 255, 200)));
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_LAB7);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   ЦЕЛЬ: Сохраняет маркер экземпляра и создает главное окно
//
//   КОММЕНТАРИИ:
//
//        В этой функции маркер экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // Store instance handle in our global variable

    hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, 550, 330, NULL, NULL, hInstance, NULL);

    hEdit = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", L"", WS_TABSTOP | WS_CHILD | WS_VISIBLE | ES_AUTOVSCROLL | ES_AUTOHSCROLL,
        40, 220, 330, 35, hWnd, (HMENU)EDIT, hInst, NULL);

    hButtonAdd = CreateWindow(L"BUTTON", L"Добавить", WS_TABSTOP | WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
        390, 215, 100, 40, hWnd, (HMENU)BUTTON_ADD, hInst, NULL);

    hButtonClear = CreateWindow(L"BUTTON", L"Очистить", WS_TABSTOP | WS_CHILD | WS_VISIBLE,
        225, 40, 80, 30, hWnd, (HMENU)BUTTON_CLEAR, hInst, NULL);

    hButtonToRight = CreateWindow(L"BUTTON", L"Перекинуть", WS_TABSTOP | WS_CHILD | WS_VISIBLE,
        225, 100, 80, 30, hWnd, (HMENU)BUTTON_TORIGHT, hInst, NULL);

    hButtonDelete = CreateWindow(L"BUTTON", L"Удалить", WS_TABSTOP | WS_CHILD | WS_VISIBLE,
        225, 155, 80, 30, hWnd, (HMENU)BUTTON_DELETE, hInst, NULL);

    hListBox1 = CreateWindowEx(WS_EX_CLIENTEDGE, L"LISTBOX", L"", WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL | ES_AUTOVSCROLL | ES_AUTOHSCROLL | LBS_MULTIPLESEL,
        40, 40, 160, 160, hWnd, (HMENU)LISTBOX_1, hInst, NULL);

    hListBox2 = CreateWindowEx(WS_EX_CLIENTEDGE, L"LISTBOX", L"", WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_AUTOVSCROLL | WS_HSCROLL | ES_AUTOVSCROLL | LBS_MULTIPLESEL,
        332, 40, 160, 160, hWnd, (HMENU)LISTBOX_2, hInst, NULL);


    if (!hWnd)
    {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}


//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ЦЕЛЬ: Обрабатывает сообщения в главном окне.
//
//  WM_COMMAND  - обработать меню приложения
//  WM_PAINT    - Отрисовка главного окна
//  WM_DESTROY  - отправить сообщение о выходе и вернуться
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{  

    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            int wmEvent = HIWORD(wParam);

            switch (wmEvent) {
            case BN_CLICKED:
                switch (wmId) {
                case BUTTON_ADD:
                    int len;
                    len = SendMessage(hEdit, WM_GETTEXTLENGTH, 0, 0);
                    if (len == 0)
                        MessageBox(hWnd, L"Fill the text field", L"Error", MB_OK);
                    else {
                        TCHAR* buffer = new TCHAR[len];
                        SendMessage(hEdit, WM_GETTEXT, len + 1, (LPARAM)buffer);

                        AddStringInListBox(hListBox1, buffer);
                        SetWindowText(GetDlgItem(hWnd, EDIT), L"");//очищаем edit
                    }
                    break;
                case BUTTON_CLEAR:
                    SendMessage(hListBox1, LB_RESETCONTENT, 0, 0);
                    SendMessage(hListBox2, LB_RESETCONTENT, 0, 0);
                    break;
                case BUTTON_TORIGHT:
                    ToRight();
                    break;
                case BUTTON_DELETE:
                    DeleteSelectedItem(hListBox1);
                    DeleteSelectedItem(hListBox2);
                    break;
                default:
                    break;
                }
            default:
                switch (wmId)
                {
                case IDM_ABOUT:
                    DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                    break;
                case IDM_EXIT:
                    DestroyWindow(hWnd);
                    break;
                default:
                    return DefWindowProc(hWnd, message, wParam, lParam);
                }
            }
            break;
        }
        break;
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
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

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message) {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
