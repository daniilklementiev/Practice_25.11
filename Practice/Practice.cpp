// Practice.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "Practice.h"

#define MAX_LOADSTRING          100
#define CMD_BUTTON_START        1001
#define THREADCOUNT             3
// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
HWND total;
HWND active;
HANDLE mutex = NULL;
HANDLE Semaphore;


// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
void                StartThread();
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
    LoadStringW(hInstance, IDC_PRACTICE, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PRACTICE));

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

    return (int) msg.wParam;
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

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PRACTICE));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = CreateSolidBrush(RGB(10,100,80));
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_PRACTICE);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

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
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
    {
        CreateWindowW(L"Button", L"Start", WS_CHILD | WS_VISIBLE, 10, 10, 75, 23, hWnd, (HMENU)CMD_BUTTON_START, hInst, 0);
        total = CreateWindowW(L"Listbox", L"", WS_CHILD | WS_VISIBLE, 10, 90, 250, 300, hWnd, 0, hInst, 0);
        active = CreateWindowW(L"Listbox", L"", WS_CHILD | WS_VISIBLE, 320, 90, 250, 300, hWnd, 0, hInst, 0);
        mutex = CreateMutex(NULL, FALSE, NULL);
        /*if (mutex == NULL) {
            MessageBoxW(hWnd, L"Mutex error", L"Error", MB_OK);
            return -1;
        }
        else {
            SendMessage(active, LB_ADDSTRING, 100, (LPARAM)L"Mutex OK");
        }*/

        Semaphore = CreateSemaphore(NULL, THREADCOUNT, THREADCOUNT, NULL);

        if (Semaphore == NULL)
        {
            MessageBoxW(0, L"Semaphore error!", L"Error", MB_ICONERROR);
        }
        else {
            SendMessage(active, LB_ADDSTRING, 100, (LPARAM)L"Semaphore OK");
        }
    }
        break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case CMD_BUTTON_START:
                StartThread();
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
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
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

/**********************/
HANDLE SemaphoreHandle[THREADCOUNT];

struct ThreadData {
    int month;
    float percent;
    ThreadData(int month, float percent) : month{ month }, percent{ percent } {}
};

int activeThreadsCount;

DWORD WINAPI ThreadProc(LPVOID params) {

    WCHAR txt[100];
    ThreadData* data = (ThreadData*)params;

    DWORD waitResult = WaitForSingleObject(Semaphore, 10);
    switch (waitResult)
    {
    case WAIT_OBJECT_0: // NORMAL
        activeThreadsCount++;

        _snwprintf_s(txt, 100, L"thread - %d, month - %d", activeThreadsCount, data->month);
        SendMessageW(active, LB_ADDSTRING, 100, (LPARAM)txt);
        _snwprintf_s(txt, 100, L"%d %f", data->month, data->percent);
        SendMessageW(total, LB_ADDSTRING, 100, (LPARAM)txt);
        
        activeThreadsCount--;
        ReleaseSemaphore(Semaphore, 1, NULL);
    
        break;

    case WAIT_TIMEOUT:  // TIMEOUT
        _snwprintf_s(txt, 100, L"%d timeout ", data->month);
        SendMessageW(total, LB_ADDSTRING, 100, (LPARAM)txt);
        break;

    default:
     {
        _snwprintf_s(txt, 100, L"%d error %d", data->month, GetLastError());
        SendMessageW(total, LB_ADDSTRING, 100, (LPARAM)txt);
     }
    }


    return 0;
}

void StartThread() {
    WCHAR txt[100];
    SendMessageW(total, LB_RESETCONTENT, 0, 0);
    SendMessageW(active, LB_RESETCONTENT, 0, 0);
    
    for (int i = 1; i <= 12; i++)
    {
        CreateThread(NULL, 0, ThreadProc, new ThreadData(i, 10), 0, NULL);
    }
}