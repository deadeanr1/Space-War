// Space War.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Space War.h"

#define MAX_LOADSTRING 100

using namespace std;

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The name of player
TCHAR szplayer[MAX_LOADSTRING] = L"Player";		// The name of ememy
TCHAR szenemy[MAX_LOADSTRING] = L"Enemy";		// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
int WINDOW_WIDTH = 1000;
int WINDOW_HEIGHT = 700;
HANDLE	hBitBuffer, hBitBack;
static DWORD StartTime;
// cursor position of the cursor
POINT pCursor;
POINT square;
BOOL Move = FALSE;

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK 		NetworkProc(HWND, UINT, WPARAM, LPARAM);

struct Battleship
{
	int totalHealth;
	int health;
	int textureType;
    int x1, y1, x2, y2;
};

DWORD ipAddress = 0;
char ipAddr[20];
vector<vector<int>> map;	//map of my battleships
vector<Battleship>  ships;	//where my ships are located
vector<vector<int>> enemy_map; //map of enemy's battleships

void shuffleMap(vector<vector<int> > &map, vector<Battleship> &ships);
int randships(int map[10][10], Battleship B[10]);

void InitializeTime()
{
    StartTime = GetTickCount();
}

float GetTime()
{
    return (GetTickCount() - StartTime) / 1000.0f;
}

// get the cell of the cursor
POINT GetCell()
{
  POINT square = {0};

  // get cursor position
  GetCursorPos(&pCursor);

  // check if the cursor is in the zone
  if ((pCursor.x<550) && (pCursor.x>50) && (pCursor.y>130) && (pCursor.y< 530))
  {
    //see for the cell
    float x = ((pCursor.x-50)/40);
    square.x = floor(x)+1;
    float y = ((pCursor.y-130)/40)+1;
    square.y = floor(y)+1;
  }
    //printf("\n%c",square.x);
  return square;
}

void DrawMatrix(HDC hdc, HWND hWnd)
{
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

	Rectangle(hdc,546, 127,953, 535);

    //horizontal lines
	hPen = CreatePen (PS_SOLID, 1,RGB(127,170,255));
	SelectObject (hdc, hPen);
    for (int i=1; i<10; i++)
	{
        MoveToEx(hdc, 549, 130+40*i, NULL);
        LineTo(hdc, 949, 130+40*i);
    }
	
	for (int i=1; i<10; i++)
	{
        MoveToEx(hdc, 549+40*i, 130, NULL);
		LineTo(hdc, 549+40*i, 530);
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

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	static HDC hdc, hdcMem;
    // A bitmap handle for the double buffring
    static HBITMAP hbmMem;
    // A handle of old memory context
    static HANDLE hOld;
	static HWND ShuffleButton;

	HDC memhdc;
	HBITMAP hbitmp;
	RECT rect;
	int ret;
	
	switch (message)
	{
	case WM_TIMER:
		if (wParam == 1)
		{
			InvalidateRect(hWnd, NULL, FALSE);
		}
		break;
	case WM_CREATE:
		ShuffleButton=CreateWindowEx(NULL,
				L"BUTTON",
				L"Shuffle",          
				WS_CHILD | WS_VISIBLE | ES_CENTER,
				350,              
		        105,              
                80,               
                20,               
				hWnd,	          
				(HMENU)IDC_MAIN_BUTTON,
				GetModuleHandle(NULL),
				NULL);		      

		shuffleMap(map, ships);
		break;
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);

		if((HWND)lParam == ShuffleButton)
		{
			shuffleMap(map, ships);
		}

		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
			
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;

		case ID_SETTINGS_NETWORKSETTINGS: 
			ret = DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_NETWORK), hWnd, NetworkProc);
               
			InvalidateRect(hWnd, NULL, FALSE);
			
			if(ret == -1)
			{
				MessageBox(hWnd, L"Dialog failed!", L"Error", MB_OK | MB_ICONINFORMATION);   
			}	   
			break;
		case ID_NEWGAME_VERSUSCOMPUTER:
			MessageBox(0, _T("vs. Comp"), 0, 0);
			break;
		case ID_VS_CREATEGAME:
			MessageBox(0, _T("waiting for connection"), 0, 0);
			break;
		case ID_VS_CONNECTTOGAME:
			MessageBox(0, _T("Connecting to server"), 0, 0);
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

	case WM_KEYDOWN:
    pCursor.x = LOWORD(lParam);
    pCursor.y = HIWORD(lParam);
    GetCursorPos(&pCursor);

    case WM_LBUTTONDOWN:
         Move = TRUE;
	     square = GetCell();
		 break;

    case WM_MOUSEMOVE:
		// get coordonate of the square
		square = GetCell();
        break;

    case WM_LBUTTONUP:
		Move = FALSE;
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

BOOL CALLBACK NetworkProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	static HWND		  PortInput, IPInput;
	int ip1, ip2, ip3, ip4;
    switch(Message)
    {
	case WM_INITDIALOG:
		IPInput = GetDlgItem(hwnd, IDC_IPADDRESS1);
		SendMessage(IPInput, IPM_SETADDRESS, 0, (LPARAM)ipAddress);
		break;
	case WM_CREATE:
		break;
    case WM_COMMAND:
		switch(LOWORD(wParam))  
		{         
		case IDOK:
			IPInput = GetDlgItem(hwnd, IDC_IPADDRESS1);
			SendMessage(IPInput, IPM_GETADDRESS, 0, (LPARAM)&ipAddress);

			ip1 = FIRST_IPADDRESS(ipAddress);
			ip2 = SECOND_IPADDRESS(ipAddress);
			ip3 = THIRD_IPADDRESS(ipAddress);
			ip4 = FOURTH_IPADDRESS(ipAddress);

			sprintf(ipAddr, "%d.%d.%d.%d", ip1, ip2, ip3, ip4);

			EndDialog(hwnd, IDCANCEL);
			break;    
		case IDCANCEL:
			EndDialog(hwnd, IDCANCEL);    
			break;
		}  
		break;     
	default:
		return FALSE;
    }
    return TRUE;
}

void shuffleMap(vector<vector<int> > &map, vector<Battleship> &ships)
{
	int m[10][10];
    int i,j;
    Battleship B[10];
    vector<int> temp;
    randships(m,B);

	for(int i=0; i<map.size(); i++)
	{
		map.at(i).clear();
	}
	map.clear();

	ships.clear();

	map.resize(10);
	ships.resize(10);
    for (i=0; i<10; i++)
    {
		map.at(i).resize(10);
        for (j=0; j<10; j++)
		{
			map.at(i).at(j) = m[i][j];
            temp.push_back(m[i][j]);
		}
		ships.at(i) = B[i];
	}

	enemy_map.clear();
	enemy_map.resize(10);
	for(size_t i=0; i<10; i++)
	{
		enemy_map[i].resize(10);
		for(size_t j=0; j<10; j++)
		{
			enemy_map.at(i).at(j) = -1;
		}
	}
}

int randships(int map[10][10],Battleship B[10])
{
	int i,j,f,k,r,h=9;
    int randNum1, randNum2, ts,s; //pozitionare+directie

    srand(time(NULL));      //huevoznaetce

    for (i=0; i<10; i++)
        for (j=0; j<10; j++)
            map [i][j]=-1;

    do
    {

        f=0;                                 //4-sized ship
        randNum1 =  rand() %10;
        randNum2=  rand() %10;

        ts = 1+rand()%4;
        //verify if ship does not exceed the limit of map
        if (ts== 1&&0<=randNum1 &&randNum1<3)f=1;
        if (ts== 2 && 7 <= randNum1 && randNum1 < 10)f=1;

        if (ts== 3 && 0<=randNum2 &&randNum2<3)f=1;
        if (ts== 4 && 7 <= randNum2 && randNum2 < 10)f=1;

        //    printf("%d %d %d %d\n",randNum1,randNum2,ts,i);
    }
    while (f==1);
    switch (ts)
    {
    case 1:
        map[randNum1][randNum2]=h;
        map[randNum1-1][randNum2]=h;
        map[randNum1-2][randNum2]=h;
        map[randNum1-3][randNum2]=h;

        B[h].x1=randNum1-3;
        B[h].x2=randNum1;
        B[h].y1=B[h].y2=randNum2;

        break;
    case 2:
        map[randNum1][randNum2]=h;
        map[randNum1+1][randNum2]=h;
        map[randNum1+2][randNum2]=h;
        map[randNum1+3][randNum2]=h;

        B[h].x1=randNum1;
        B[h].x2=randNum1+3;
        B[h].y1=B[h].y2=randNum2;

        break;
    case 3:
        map[randNum1][randNum2]=h;
        map[randNum1][randNum2-1]=h;
        map[randNum1][randNum2-2]=h;
        map[randNum1][randNum2-3]=h;

        B[h].x1=B[h].x2=randNum1;
        B[h].y1=randNum2-3;
        B[h].y2=randNum2;

        break;
    case 4:
        map[randNum1][randNum2]=h;
        map[randNum1][randNum2+1]=h;
        map[randNum1][randNum2+2]=h;
        map[randNum1][randNum2+3]=h;

        B[h].x1=B[h].x2=randNum1;
        B[h].y1=randNum2;
        B[h].y2=randNum2+3;

        break;
    }
    B[h].totalHealth=4;
    B[h].textureType=3;
     B[h].health=4;
     h--;

    for (k=0; k<2; k++)                                     //?3sized ship
    {
        do
        {
            f=0;
            randNum1 =  rand() %10;
            randNum2=  rand() %10;

            ts = 1+rand()%4;

            if (ts==1)
            {
                if(randNum1>1 )//up damper
                {
                    for (i=randNum1-3; i<randNum1+2; i++)
                        for (j=randNum2-1; j<=randNum2+1; j++)
                        {
                            if (map[i][j]!=-1)
                            {
                                f=1;
                            }
                        }
                }
                else f=1;
            }
            if (ts==2)
            {
                if (randNum1<8)//down damper
                {
                    for (i=randNum1-1; i<=randNum1+3; i++)
                        for (j=randNum2-1; j<=randNum2+1; j++)//
                        {
                            if (map[i][j]!=-1)
                            {
                                f=1;
                            }
                        }
                }
                else f=1;
            }
            if (ts==3)
            {
                if (randNum2>1)//left damper
                {
                    for (i=randNum1-1; i<=randNum1+1; i++)
                        for (j=randNum2-3; j<=randNum2+1; j++)
                        {
                            if (map[i][j]!=-1)
                            {
                                f=1;
                            }
                        }
                }
                else f=1;
            }
            if (ts==4)
            {
                if (randNum2<8) //right damper
                {
                    for (i=randNum1-1; i<=randNum1+1; i++)
                        for (j=randNum2-1; j<=randNum2+3; j++)
                        {
                            if (map[i][j]!=-1)
                            {
                                f=1;
                            }
                        }
                }
                else f=1;
            }
        }
        while (f==1);
        switch (ts)
        {
        case 1:
            map[randNum1][randNum2]=h;
            map[randNum1-1][randNum2]=h;
            map[randNum1-2][randNum2]=h;

            B[h].x1=randNum1-2;
            B[h].x2=randNum1;
            B[h].y1=B[h].y2=randNum2;

            break;
        case 2:
            map[randNum1][randNum2]=h;
            map[randNum1+1][randNum2]=h;
            map[randNum1+2][randNum2]=h;

            B[h].x1=randNum1;
            B[h].x2=randNum1+2;
            B[h].y1=B[h].y2=randNum2;

            break;
        case 3:
            map[randNum1][randNum2]=h;
            map[randNum1][randNum2-1]=h;
            map[randNum1][randNum2-2]=h;

            B[h].x1=B[h].x2=randNum1;
            B[h].y1=randNum2-2;
            B[h].y2=randNum2;

            break;
        case 4:
            map[randNum1][randNum2]=h;
            map[randNum1][randNum2+1]=h;
            map[randNum1][randNum2+2]=h;

            B[h].x1=B[h].x2=randNum1;
            B[h].y1=randNum2;
            B[h].y2=randNum2+2;

            break;
        }

        B[h].totalHealth=3;
        B[h].health=3;
        B[h].textureType=2;
        h--;
    }

    for (k=0; k<3; k++)                                     //2 sized ships
    {
        do
        {
            f=0;
            randNum1 =  rand() %10;
            randNum2=  rand() %10;

            ts = 1+rand()%4;

            if (ts==1)
            {
                if(randNum1 > 0)
                {
                    for (i=randNum1-2; i<randNum1+2; i++)
                        for (j=randNum2-1; j<=randNum2+1; j++)
                        {
                            if (map[i][j]!=-1)
                            {
                                f=1;
                            }
                        }
                }
                else f=1;
            }
            if (ts==2)
            {
                if (randNum1< 9)
                {
                    for (i=randNum1-1; i<=randNum1+2; i++)
                        for (j=randNum2-1; j<=randNum2+1; j++)
                        {
                            if (map[i][j]!=-1)
                            {
                                f=1;
                            }
                        }
                }
                else f=1;
            }
            if (ts==3)
            {
                if (randNum2> 0)
                {
                    for (i=randNum1-1; i<=randNum1+1; i++)
                        for (j=randNum2-2; j<=randNum2+1; j++)
                        {
                            if (map[i][j]!=-1)
                            {
                                f=1;
                            }
                        }
                }
                else f=1;
            }
            if (ts==4)
            {
                if (randNum2 < 9)
                {
                    for (i=randNum1-1; i<=randNum1+1; i++)
                        for (j=randNum2-1; j<=randNum2+2; j++)
                        {
                            if (map[i][j]!=-1)
                            {
                                f=1;
                            }
                        }
                }
                else f=1;
            }
        }
        while (f==1);
        switch (ts)
        {
        case 1:
            map[randNum1][randNum2]=h;
            map[randNum1-1][randNum2]=h;

             B[h].x1=randNum1-1;
            B[h].x2=randNum1;
            B[h].y1=B[h].y2=randNum2;

            break;
        case 2:
            map[randNum1][randNum2]=h;
            map[randNum1+1][randNum2]=h;

              B[h].x1=randNum1;
            B[h].x2=randNum1+1;
            B[h].y1=B[h].y2=randNum2;
            break;
        case 3:
            map[randNum1][randNum2]=h;
            map[randNum1][randNum2-1]=h;

             B[h].x1=B[h].x2=randNum1;
            B[h].y1=randNum2-1;
            B[h].y2=randNum2;

            break;
        case 4:
            map[randNum1][randNum2]=h;
            map[randNum1][randNum2+1]=h;

             B[h].x1=B[h].x2=randNum1;
            B[h].y1=randNum2;
            B[h].y2=randNum2+1;

            break;
        }
         B[h].totalHealth=2;
        B[h].health=2;
        B[h].textureType=1;
        h--;
    }

    for (i=0; i<4; i++ )                         //1 size
    {
        do
        {
            randNum1 =  rand() %10;
            randNum2=  rand() %10;
        }
        while (map[randNum1][randNum2]!=-1||map[randNum1+1][randNum2]!=-1||map[randNum1-1][randNum2]!=-1||map[randNum1][randNum2+1]!=-1||map[randNum1][randNum2-1]!=-1||map[randNum1+1][randNum2+1]!=-1||map[randNum1-1][randNum2-1]!=-1||map[randNum1+1][randNum2-1]!=-1||map[randNum1-1][randNum2+1]!=-1);

        map[randNum1][randNum2]=h;
        B[h].totalHealth=1;
          B[h].health=1;
          B[h].textureType=0;
          B[h].x1=B[h].x2=randNum1;
          B[h].y1=B[h].y2=randNum2;
           h--;
    }

    return 1;
}