// Space War.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Space War.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The name of player
TCHAR szplayer[MAX_LOADSTRING] = L"Player";					// The name of ememy
TCHAR szenemy[MAX_LOADSTRING] = L"Enemy";				// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
int WINDOW_WIDTH = 1000;
int WINDOW_HEIGHT = 700;
HANDLE	hBitBuffer, hBitBack;
static DWORD StartTime;

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);



void InitializeTime()
{
    StartTime = GetTickCount();
}

float GetTime()
{
    return (GetTickCount() - StartTime) / 1000.0f;
}

void DrawMatrix(HDC hdc, HWND hWnd){
	int i, j;
	HPEN hPen;
    HBRUSH hBrush;
	POINT Pt[4];
	int RGB = RGB(82,90,90);


	
	// 1st zone
	////////////////////////////////////////////////////
    // prepare the color
    hPen = CreatePen (PS_SOLID, 6,RGB);
    hBrush = CreateSolidBrush(RGB);
    SelectObject (hdc, hPen);
    SelectObject (hdc, hBrush);

	Rectangle(hdc,40, 100,461, 542);
	Rectangle(hdc,40, 80,160, 120);
	

	// Triangle
	int lpPts[] = { 3 };
	Pt[0].x = 160;
	Pt[0].y = 80;
	Pt[1].x = 140;
	Pt[1].y = 120;
	Pt[2].x = 260;
	Pt[2].y = 120;
	PolyPolygon(hdc, Pt, lpPts, 1);

	//water
	hBrush = CreateSolidBrush(RGB(85,127,255));
    SelectObject (hdc, hBrush);
    Rectangle(hdc,47, 127,454, 535);

    //horizontal lines
	hPen = CreatePen (PS_SOLID, 1,RGB(127,170,255));
	SelectObject (hdc, hPen);
    for (int i=1; i<10; i++){
        MoveToEx(hdc, 50, 130+40*i, NULL);
        LineTo(hdc, 450, 130+40*i);
    }
	
	for (int i=1; i<10; i++){
        MoveToEx(hdc, 50+40*i, 130, NULL);
		LineTo(hdc, 50+40*i, 530);
    }
	////////////////////////////////////////////////////


	// 2nd zone
	////////////////////////////////////////////////////
    // prepare the color
    hPen = CreatePen (PS_SOLID, 6,RGB);
    hBrush = CreateSolidBrush(RGB);
    SelectObject (hdc, hPen);
    SelectObject (hdc, hBrush);

	Rectangle(hdc,539, 100,960, 542);
	Rectangle(hdc,840, 80,960, 120);
	

	// Triangle
	int lpPts2[] = { 3 };
	Pt[0].x = 838;
	Pt[0].y = 80; 
	Pt[1].x = 840;
	Pt[1].y = 120;
	Pt[2].x = 720;
	Pt[2].y = 120;
	PolyPolygon(hdc, Pt, lpPts, 1);

	//water
	hBrush = CreateSolidBrush(RGB(85,127,255));
    SelectObject (hdc, hBrush);
	//Rectangle(hdc,40, 100,461, 542);
    //Rectangle(hdc,47, 127,454, 535);


	Rectangle(hdc,546, 127,953, 535);

    //horizontal lines
	hPen = CreatePen (PS_SOLID, 1,RGB(127,170,255));
	SelectObject (hdc, hPen);
    for (int i=1; i<10; i++){
        MoveToEx(hdc, 549, 130+40*i, NULL);
        LineTo(hdc, 949, 130+40*i);
    }
	
	for (int i=1; i<10; i++){
        MoveToEx(hdc, 549+40*i, 130, NULL);
		LineTo(hdc, 549+40*i, 530);
    }
	////////////////////////////////////////////////////


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

	wcex.style			= CS_HREDRAW | CS_VREDRAW | CS_DROPSHADOW;
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
   HBITMAP hwater;

   rect.left = 140;								//x
   rect.top = 10;								//y
   rect.right = rect.left + WINDOW_WIDTH;		//width
   rect.bottom = rect.top + WINDOW_HEIGHT;		//height


   hInst = hInstance; // Store instance handle in our global variable

   style = WS_POPUP | WS_CLIPSIBLINGS | WS_OVERLAPPED | WS_SYSMENU | 
		WS_MINIMIZEBOX | WS_CLIPCHILDREN;
  

   hWnd = CreateWindow(
        szWindowClass,          // (opt) classname
        szTitle,                // (opt) The window name
        WS_OVERLAPPED|WS_BORDER|WS_SYSMENU | WS_MINIMIZEBOX,	// The style of the window being created.
        rect.left, 
        rect.top,
        rect.right-rect.left,   // width of the window
        rect.bottom-rect.top,   // hight vertical position of the window.
        NULL,                   // (opt) handle to the parent or owner window of the window being created
        NULL,                   // (opt) handle to a menu
        hInstance,              // (opt) handle to the instance of the module to be associated with the window.
        NULL                    // (opt) pter to a value to be passed to the window through the CREATESTRUCT
    );
   
   hwater = LoadBitmap(hInst,MAKEINTRESOURCE(IDB_WATER));
   
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
		static HBITMAP hbmp, hback, hwater;
		BITMAP  bitmap;
		static int cxSource, cySource;
		HDC hdc, hdcMem, hdcMem2;
		HPEN Pen;
		PAINTSTRUCT ps;
		HFONT hFont;
		RECT rect, rect1, rect2;
		HBRUSH hBrush;

			hdc = BeginPaint(hWnd, &ps);
			hbmp = LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BACKGROUND));
			GetObject( hbmp, sizeof(BITMAP), &bitmap);

			hdc = GetDC(hWnd);
			hdcMem = CreateCompatibleDC (hdc) ;
			SelectObject (hdcMem, hbmp) ;
			DrawMatrix(hdcMem, hWnd);
			BitBlt (hdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, hdcMem, 0, 0, SRCCOPY) ;
			DeleteDC(hdcMem);
	        DeleteObject(hbmp);
		
			rect.top = 550;
			rect.left = 100;
			rect.right = 650;
			rect.bottom = 650;
			hFont = CreateFont(25,0,0,0,FW_SEMIBOLD,FALSE,FALSE,FALSE,DEFAULT_CHARSET,OUT_OUTLINE_PRECIS,
                CLIP_DEFAULT_PRECIS,CLEARTYPE_QUALITY, VARIABLE_PITCH,TEXT("Times New Roman"));
			SelectObject(hdc, hFont);
			SetBkMode (hdc, TRANSPARENT) ;
			SetTextColor(hdc, RGB(255,255,255));
			DrawText (hdc, L"Ships", -1, &rect, DT_SINGLELINE) ;

			rect1.top = 550;
			rect1.left = 850;
			rect1.right = 920;
			rect1.bottom = 650;
			DrawText (hdc, L"Ships", -1, &rect1, DT_SINGLELINE) ;

			rect2.top = 600;
			rect2.left = 310;
			rect2.right = 400;
			rect2.bottom = 680;
			DrawText (hdc, L"SCORE", -1, &rect2, DT_SINGLELINE) ;

			rect2.top = 600;
			rect2.left = 610;
			rect2.right = 700;
			rect2.bottom = 680;
			DrawText (hdc, L"TIME", -1, &rect2, DT_SINGLELINE) ;

			rect2.top = 40;
			rect2.left = 380;
			rect2.right = 530;
			rect2.bottom = 70;
			DrawText (hdc, L"STATUS", -1, &rect2, DT_SINGLELINE) ;

			//name of the players
			SetTextColor(hdc, RGB(0,0,0));
			hBrush = CreateSolidBrush(RGB(247,214,0));
			SelectObject (hdc, hBrush);
			RoundRect(hdc, 55, 90, 185, 120, 20, 20 );
			RoundRect(hdc, 820, 90, 945, 120, 20, 20 );

			hFont = CreateFont(25,0,0,0,FW_LIGHT,FALSE,FALSE,FALSE,DEFAULT_CHARSET,OUT_OUTLINE_PRECIS,
                CLIP_DEFAULT_PRECIS,CLEARTYPE_QUALITY, VARIABLE_PITCH,TEXT("Times New Roman"));
			SelectObject(hdc, hFont);
			SetBkMode (hdc, TRANSPARENT) ;

			rect2.top = 90;
			rect2.left = 70;
			rect2.right = 170;
			rect2.bottom = 150;
			DrawText (hdc, szplayer, -1, &rect2, DT_SINGLELINE) ;
			
			rect2.top = 90;
			rect2.left = 830;
			rect2.right = 930;
			rect2.bottom = 150;
			DrawText (hdc, szenemy, -1, &rect2, DT_SINGLELINE) ;

					
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
