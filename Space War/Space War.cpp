// Space War.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Space War.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
int WINDOW_WIDTH = 1000;
int WINDOW_HEIGHT = 700;
HANDLE	hBitBuffer, hBitBack;


// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);



void DrawMatrix(HDC hdc){
	int i, j;
	HPEN hPen;
    HBRUSH hBrush;

    // prepare the color
    hPen = CreatePen (PS_SOLID, 6,RGB(180,180,180));
    hBrush = CreateSolidBrush(RGB(42,85,255));
    SelectObject (hdc, hPen);
    SelectObject (hdc, hBrush);

	// 1st zone
    Rectangle(hdc,27, 127,434, 534);
    //horizontal lines
	hPen = CreatePen (PS_SOLID, 1,RGB(0,0,0));
	SelectObject (hdc, hPen);
    for (int i=1; i<10; i++){
        MoveToEx(hdc, 30, 130+40*i, NULL);
        LineTo(hdc, 430, 130+40*i);
    }
	
	for (int i=1; i<10; i++){
        MoveToEx(hdc, 30+40*i, 130, NULL);
		LineTo(hdc, 30+40*i, 530);
    }

}
int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_SPACEWAR, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	//loading the background
	hBitBack = LoadImage(hInstance, MAKEINTRESOURCE(IDB_BACKGROUND), IMAGE_BITMAP, 
		WINDOW_WIDTH, WINDOW_HEIGHT, 0);
	hInst = hInstance;
	if (!hBitBack)
		return FALSE;

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SPACEWAR));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
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
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SPACEWAR));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_SPACEWAR);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
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
   HWND hWnd;
   RECT rect;
   DWORD style;
   HRGN region;

   rect.left = 100;										//x
   rect.top = 100;										//y
   rect.right = rect.left + WINDOW_WIDTH;			//width
   rect.bottom = rect.top + WINDOW_HEIGHT;		//height


   hInst = hInstance; // Store instance handle in our global variable

   style = WS_POPUP | WS_CLIPSIBLINGS | WS_OVERLAPPED | WS_SYSMENU | 
		WS_MINIMIZEBOX | WS_CLIPCHILDREN;

   hWnd = CreateWindow(
        szWindowClass,          // (opt) classname
        szTitle,                // (opt) The window name
        WS_OVERLAPPEDWINDOW & (~WS_THICKFRAME),
        rect.left, 
        rect.top,
        rect.right-rect.left,           // width of the window
        rect.bottom-rect.top,          // hight vertical position of the window.
        NULL,                   // (opt) handle to the parent or owner window of the window being created
        NULL,                   // (opt) handle to a menu
        hInstance,              // (opt) handle to the instance of the module to be associated with the window.
        NULL                    // (opt) pter to a value to be passed to the window through the CREATESTRUCT
    );
   
   
	/*region = CreateRoundRectRgn(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 200, 200);
	if (!region)
		return FALSE;

	if (!SetWindowRgn(hWnd, region, FALSE))
		return FALSE;*/

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
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	static HDC hdc, hdcMem;
    // A bitmap handle for the double buffring
    static HBITMAP hbmMem;
    // A handle of old memory context
    static HANDLE hOld;

	HDC memhdc;
	HBITMAP hbitmp;
	RECT rect;

	switch (message)
	{

	case WM_TIMER:
			if (wParam == 1){
				InvalidateRect(hWnd, NULL, FALSE);
			}

	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
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
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		HDC		hdc_buf, hdc_temp;
		HGDIOBJ	temp;
		static HBITMAP hbmp;
		BITMAP  bitmap;
		static int cxSource, cySource;
		HDC hdc, hdcMem;
		HPEN Pen;
		PAINTSTRUCT ps;

			hdc = BeginPaint(hWnd, &ps);
			hbmp = LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BACKGROUND));
			GetObject( hbmp, sizeof(BITMAP), &bitmap);

			hdc = GetDC(hWnd);
			hdcMem = CreateCompatibleDC (hdc) ;
			SelectObject (hdcMem, hbmp) ;
			DrawMatrix(hdcMem);
			BitBlt (hdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, hdcMem, 0, 0, SRCCOPY) ;
				 
		//delete hdcMem, hbmp	
		EndPaint(hWnd, &ps);
		break;

	case WM_CREATE:
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
