// Space War.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Space War.h"
#include "Utils.h"

using namespace std;

// Global Variables:
HINSTANCE hInst;                                // current instance
TCHAR szTitle[MAX_LOADSTRING];                  // The name of player
TCHAR szplayer[MAX_LOADSTRING] = L"Player";     // The name of ememy
TCHAR szenemy[MAX_LOADSTRING] = L"Enemy";       // The title bar text
TCHAR statusString[MAX_LOADSTRING] = _T("Start a new game or shuffle!");
TCHAR scoreString[MAX_LOADSTRING] = _T("Score: ");
TCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
HWND hMainWnd;									// handle to main window
int WINDOW_WIDTH = 1000;
int WINDOW_HEIGHT = 700;
HWND hShuffleButton;
HANDLE    hBitBuffer, hBitBack;
static DWORD StartTime;
// cursor position of the cursor
POINT pCursor;
POINT square;
BOOL Move = FALSE;


// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK       NetworkProc(HWND, UINT, WPARAM, LPARAM);

BaseGame*	game = NULL;
int	myTotalCells = 20;
int enemyTotalCells = 20;
vector<vector<int>> map;		//map of my battleships
vector<Battleship>  ships;		//where my ships are located
vector<vector<int>> enemy_map;	//map of enemy's battleships
volatile static unsigned int connectionEstablished = 0;
volatile static unsigned int gameIsFinished = 0;
volatile static unsigned int myTurn = 0;
DWORD threadID = 0;
DWORD recThreadID = 0;
DWORD WINAPI CreatePvPGame(LPVOID );
DWORD WINAPI ConnectToPvPGame(LPVOID );
DWORD WINAPI ReceiveLoop(LPVOID );
DWORD ipAddress = 0;
char ipAddr[20];

void DrawMatrix(HDC hdc, HWND hWnd);

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

    wcex.style            = CS_HREDRAW | CS_VREDRAW | CS_DROPSHADOW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra        = 0;
    wcex.cbWndExtra        = 0;
    wcex.hInstance        = hInstance;
    wcex.hIcon            = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SPACEWAR));
    wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground    = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName    = MAKEINTRESOURCE(IDC_SPACEWAR);
    wcex.lpszClassName    = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassEx(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;  
	RECT rect;
	DWORD style;
	//HRGN region;
	HBITMAP hwater;

	rect.left = 140;                                //x
	rect.top = 10;                                //y
	rect.right = rect.left + WINDOW_WIDTH;        //width
	rect.bottom = rect.top + WINDOW_HEIGHT;        //height


	hInst = hInstance; // Store instance handle in our global variable

	style = WS_POPUP | WS_CLIPSIBLINGS | WS_OVERLAPPED | WS_SYSMENU | 
			WS_MINIMIZEBOX | WS_CLIPCHILDREN;
  

	hWnd = CreateWindow(
			szWindowClass,          // (opt) classname
			szTitle,                // (opt) The window name
			WS_OVERLAPPED|WS_BORDER|WS_SYSMENU | WS_MINIMIZEBOX,    // The style of the window being created.
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

	if(!hWnd)
	{
		return FALSE; 
	}
 
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	hMainWnd = hWnd;

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
    HWND hStatic;
HANDLE hImage;
  
       int i = 1;

    int ret;
    POINT square = {0};
	int result = 0;
    
    switch (message)
    {
    case WM_TIMER:
        if (wParam == 1)
        {
            InvalidateRect(hWnd, NULL, FALSE);
        }
        break;
    case WM_CREATE:
        hShuffleButton=CreateWindowEx(NULL,
										L"BUTTON",
										L"Shuffle",          
										WS_CHILD | WS_VISIBLE | ES_CENTER,
										335,              
										102,              
										80,               
										23,               
										hWnd,              
										(HMENU)IDC_MAIN_BUTTON,
										GetModuleHandle(NULL),
										NULL);              

        shuffleMap(map, ships, enemy_map);

		sprintf(ipAddr, "127.0.0.1");	//localhost ip-address by default


        //loadAndDrawBitmap(hWnd, hInst, hdc, ships);
    

        break;
    case WM_COMMAND:
        wmId    = LOWORD(wParam);
        wmEvent = HIWORD(wParam);

        if((HWND)lParam == hShuffleButton)
        {
            shuffleMap(map, ships, enemy_map);
			InvalidateRect(hWnd, NULL, FALSE);
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
			if( game )
			{
				game->close();
			}
			SAFE_DELETE(game);

			game = new PvCGame();
			game->init();

			myTurn = 1;
			connectionEstablished = 1;
			CreateThread(NULL, 0, ReceiveLoop, NULL, 0, &threadID);
			EnableWindow(hShuffleButton, FALSE);

			_stprintf(statusString, _T("Game started! You attack!"));
			InvalidateRect(hMainWnd, NULL, FALSE);
            break;
        case ID_VS_CREATEGAME:
			if( game )
			{
				game->close();
			}
			SAFE_DELETE(game);

			game = new PvPGame();
			game->init();

			CreateThread(NULL, 0, CreatePvPGame, NULL, 0, &threadID);
            break;
        case ID_VS_CONNECTTOGAME:
			if( game )
			{
				game->close();
			}
			SAFE_DELETE(game);

			game = new PvPGame();
			game->init();

			CreateThread(NULL, 0, ConnectToPvPGame, NULL, 0, &threadID);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
        break;

    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        static HBITMAP hbmp, hback, hwater;
        BITMAP  bitmap;
        static int cxSource, cySource;
        HDC hdc, hdcMem;
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
			//loadAndDrawBitmap(hWnd, hInst, hdcMem, ships);
            
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
            rect2.left = 420;
            rect2.right = 700;
            rect2.bottom = 680;
            hFont = CreateFont(35,0,0,0,FW_SEMIBOLD,FALSE,FALSE,FALSE,DEFAULT_CHARSET,OUT_OUTLINE_PRECIS,
                CLIP_DEFAULT_PRECIS,CLEARTYPE_QUALITY, VARIABLE_PITCH,TEXT("Times New Roman"));
            SelectObject(hdc, hFont);
			_stprintf(scoreString, _T("Score: %d - %d"), 20 - enemyTotalCells, 20 - myTotalCells);
            DrawText (hdc, scoreString, -1, &rect2, DT_SINGLELINE) ;

            hFont = CreateFont(25,0,0,0,FW_SEMIBOLD,FALSE,FALSE,FALSE,DEFAULT_CHARSET,OUT_OUTLINE_PRECIS,
                CLIP_DEFAULT_PRECIS,CLEARTYPE_QUALITY, VARIABLE_PITCH,TEXT("Times New Roman"));
            SelectObject(hdc, hFont);
            rect2.top = 40;
            rect2.left = 380;
            rect2.right = 730;
            rect2.bottom = 70;
            DrawText (hdc, statusString, -1, &rect2, DT_SINGLELINE) ;

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

    case WM_LBUTTONDOWN:
        // get cursor position
        pCursor.x = LOWORD(lParam);
        pCursor.y = HIWORD(lParam);
        // check if the cursor is in the zone

        if((pCursor.x<RightZone2) && (pCursor.x>LeftZone2) && (pCursor.y>TopZone2) && (pCursor.y< BottomZone2))
        {
            //see for the cell
            float x = ((pCursor.x-LeftZone2)/40.0f);
            square.x = floor(x);
            float y = ((pCursor.y-TopZone2)/40.0f);
            square.y = floor(y);
        
			if( connectionEstablished )
			{
				if( game )
				{
					if( myTurn==1 )
					{
						game->sendState(square.x, square.y, &result);
						switch( result )
						{
						case -1:			//you missed!
							myTurn = 0;
							_stprintf(statusString, _T("You missed! Enemy attacks..."));
							break;
						case -2:			//you hit him!
							myTurn = 1;
							enemyTotalCells--;
							_stprintf(statusString, _T("You hit your enemy! You attack!"));
							break;
						case -3:			//you destroyed his ship!
							myTurn = 1;
							enemyTotalCells--;
							_stprintf(statusString, _T("You destroyed enemy's ship! You attack!"));
							break;
						default:
							break;
						}
						if( enemyTotalCells<=0 )
						{
							gameIsFinished = 1;
							EnableWindow(hShuffleButton, TRUE);
							_stprintf(statusString, _T("You won! Start a new game or shuffle!"));
							InvalidateRect(hMainWnd, NULL, FALSE);
						}
						enemy_map.at(square.y).at(square.x) = -2;
						InvalidateRect(hMainWnd, NULL, FALSE);
					}
				}
			}
		}
        break;
    case WM_LBUTTONUP:
        Move = FALSE;
        break;

    case WM_DESTROY:
		gameIsFinished = 1;
		if( game )
		{
			game->close();
		}
		SAFE_DELETE(game);
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
    static HWND          PortInput, IPInput;
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

void DrawMatrix(HDC hdc, HWND hWnd)
{
    //int i, j;
    HPEN hPen;
    HBRUSH hBrush;
    POINT Pt[4];
    int RGB = RGB(82,90,90);

    // 1st Zone1
    // prepare the color
    hPen = CreatePen (PS_SOLID, 6,RGB);
    hBrush = CreateSolidBrush(RGB);
    SelectObject (hdc, hPen);
    SelectObject (hdc, hBrush);

    Rectangle(hdc,40, 100, 461, 542);
    Rectangle(hdc,40, 80, 160, 120);
    
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

    for (int i=1; i<10; i++)
    {
        MoveToEx(hdc, LeftZone1, TopZone1+40*i, NULL);
        LineTo(hdc, RightZone1, TopZone1+40*i);
    }
    
	//vertical lines
    for (int i=1; i<10; i++)
    {
        MoveToEx(hdc, LeftZone1+40*i, TopZone1, NULL);
        LineTo(hdc, LeftZone1+40*i, BottomZone1);
    }

	int left, top, right, bottom;

    //load ships bitmap
	loadAndDrawBitmap(hWnd, hInst, hdc, ships);

	//Draw ships(with green)
/*	hBrush = CreateSolidBrush(RGB(0, 255, 0));
	SelectObject(hdc, hBrush);
	for(size_t i=0; i<ships.size(); i++)
	{
		left = LeftZone1+40*ships.at(i).y1;
		top = TopZone1+40*ships.at(i).x1;
		right = LeftZone1+40*(ships.at(i).y2+1);
		bottom = TopZone1+40*(ships.at(i).x2+1);
		Rectangle(hdc, left, top, right, bottom);
	}
    */
	//Draw bombed zones(with red)
	hBrush = CreateSolidBrush(RGB(255, 0, 0));
	SelectObject(hdc, hBrush);
	for(size_t i=0; i<map.size(); i++)
	{
		for(size_t j=0; j<map.at(i).size(); j++)
		{
			if( map.at(i).at(j)==-2 )
			{
				left = LeftZone1+40*j;
				top = TopZone1+40*i;
				right = LeftZone1+40*(j+1);
				bottom = TopZone1+40*(i+1);
				Rectangle(hdc, left, top, right, bottom);
			}
		}
	}

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
        MoveToEx(hdc, LeftZone2, TopZone2+40*i, NULL);
        LineTo(hdc, RightZone2, TopZone2+40*i);
    }
    
    for (int i=1; i<10; i++)
    {
        MoveToEx(hdc, LeftZone2+40*i, TopZone2, NULL);
        LineTo(hdc, LeftZone2+40*i, BottomZone2);
    }

	//Draw bombed zones(with red)
	hBrush = CreateSolidBrush(RGB(255, 0, 0));
	SelectObject(hdc, hBrush);
	for(size_t i=0; i<enemy_map.size(); i++)
	{
		for(size_t j=0; j<enemy_map.at(i).size(); j++)
		{
			if( enemy_map.at(i).at(j)==-2 )
			{
				left = LeftZone2+40*j;
				top = TopZone2+40*i;
				right = LeftZone2+40*(j+1);
				bottom = TopZone2+40*(i+1);
				Rectangle(hdc, left, top, right, bottom);
			}
		}
	}
}

DWORD WINAPI CreatePvPGame(LPVOID )
{
	int result = 0;
	myTurn = 1;
	myTotalCells = 20;
	enemyTotalCells = 20;
	//Update status
	_stprintf(statusString, _T("Waiting for connection..."));
	InvalidateRect(hMainWnd, NULL, FALSE);
	EnableWindow(hShuffleButton, FALSE);
	if( game )
	{
		result = ((PvPGame*)game)->createGame();
	}
	if( !result )
	{
		InterlockedIncrement(&connectionEstablished);
	}
	//Update status
	_stprintf(statusString, _T("Connected! Drop a bomb on your enemy's map!"));
	InvalidateRect(hMainWnd, NULL, FALSE);
	//Start receiving loop in a separate thread
	CreateThread(NULL, 0, ReceiveLoop, NULL, 0, &recThreadID);
	return 0;
}

DWORD WINAPI ConnectToPvPGame(LPVOID )
{
	int result = 0;
	myTurn = 0;
	myTotalCells = 20;
	enemyTotalCells = 20;
	//Update status
	_stprintf(statusString, _T("Connecting to server..."));
	InvalidateRect(hMainWnd, NULL, FALSE);
	EnableWindow(hShuffleButton, FALSE);
	if( game )
	{
		result = ((PvPGame*)game)->connectToGame(ipAddr);
	}
	if( !result )
	{
		InterlockedIncrement(&connectionEstablished);
	}
	//Update status
	_stprintf(statusString, _T("Connected! Your opponents attacks! Hold On"));
	InvalidateRect(hMainWnd, NULL, FALSE);
	//Start receiving loop in a separate thread
	CreateThread(NULL, 0, ReceiveLoop, NULL, 0, &recThreadID);
	return 0;
}

DWORD WINAPI ReceiveLoop(LPVOID )
{
	int x=-1, y=-1;
	while( gameIsFinished!=1 )
	{
		if( game )
		{
			if( !myTurn )
			{
				game->receiveState(&x, &y);
				if( x>=0 && x<10 && y>=0 && y<10 )
				{
					switch(map.at(y).at(x))
					{
					case -1:
						game->sendResult(-1);
						map.at(y).at(x) = -2;	//make this location already bombed
						myTurn = 1;
						_stprintf(statusString, _T("Enemy missed! You attack!"));
						InvalidateRect(hMainWnd, NULL, FALSE);
						break;
					case -2:
						game->sendResult(-1);
						_stprintf(statusString, _T("Enemy missed! You attack!"));
						InvalidateRect(hMainWnd, NULL, FALSE);
						myTurn = 1;
						break;
					default:
						myTurn = 0;							//enemy hit us, he attacks again :(
						ships.at( map.at(y).at(x) ).health--;	//decrement ships health
						
						if( ships.at( map.at(y).at(x) ).health<=0 )
						{
							game->sendResult(-3);				//he destroyed my ship
						}
						else
						{
							game->sendResult(-2);				//he hit my ship
						}
						map.at(y).at(x) = -2;					//now this cell is bombed

						myTotalCells--;			
						if( myTotalCells<=0 )
						{
							gameIsFinished = 1;
							EnableWindow(hShuffleButton, TRUE);
							_stprintf(statusString, _T("You loose! Start a new game or shuffle!"));
							InvalidateRect(hMainWnd, NULL, FALSE);
						}
						_stprintf(statusString, _T("Enemy hit you! He continues attacking..."));
						InvalidateRect(hMainWnd, NULL, FALSE);
						break;
					}
				}
			}
		}
	}
	return 0;
}