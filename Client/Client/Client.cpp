// Client.cpp
//--------------------------------------------------------------------------------------------------------------------------
#include "stdafx.h"
#include "Client.h"
//--------------------------------------------------------------------------------------------------------------------------
#define MAX_LOADSTRING 100
//--------------------------------------------------------------------------------------------------------------------------
// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
//--------------------------------------------------------------------------------------------------------------------------
// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
//--------------------------------------------------------------------------------------------------------------------------
int APIENTRY _tWinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPTSTR lpCmdLine,int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;
	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_CLIENT, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);
	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow)) {
		return FALSE;
		}
	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CLIENT));
//---------------------------------------------------------------
//	// Main message loop:
//	while (GetMessage(&msg, NULL, 0, 0)) {
//		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {
//			TranslateMessage(&msg);
//			DispatchMessage(&msg);
//		}
//	}
//---------------------------------------------------------------
// Изменить основной цикл обработки сообщений, чтобы добавить
// обработку интераций основного класса приложения
	BOOL Run = true;
	while(Run) {
		if(PeekMessage(&msg,NULL,0,0,PM_NOREMOVE)) {
			if(Run = GetMessage(&msg,NULL,0,0)) {
				if(!TranslateAccelerator(msg.hwnd,hAccelTable,&msg)) {
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			}
		}
		else if(Vnes) Vnes->Interation();
	}
//---------------------------------------------------------------
	return (int) msg.wParam;
}
//--------------------------------------------------------------------------------------------------------------------------
ATOM MyRegisterClass(HINSTANCE hInstance) {
//  FUNCTION: MyRegisterClass()
//  PURPOSE: Registers the window class.
//  COMMENTS:
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CLIENT));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_CLIENT);
//	wcex.lpszMenuName	= 0;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
	return RegisterClassEx(&wcex);
}
//--------------------------------------------------------------------------------------------------------------------------
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow) {
//   FUNCTION: InitInstance(HINSTANCE, int)
//   PURPOSE: Saves instance handle and creates main window
//   COMMENTS:
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
	HWND hWnd;
   hInst = hInstance; // Store instance handle in our global variable
   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);
   if (!hWnd) {
	return FALSE;
	}
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);
   return TRUE;
}
//--------------------------------------------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//  PURPOSE:  Processes messages for the main window.
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	switch (message) {
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId) {
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
		// TODO: Add any drawing code here...
		EndPaint(hWnd, &ps);
		break;
//------------------------------------------------------------------------------------------------------------------------	
// При закрытии программы
	case WM_DESTROY:
		Vnes->Destroy();	// Удалить класс основного приложения
		delete Vnes;
		Opt->Save();		// Сохранить и удалить настройки
		delete Opt;
		delete Log;			// Удалить Log-файл

		PostQuitMessage(0);
		break;
//------------------------------------------------------------------------------------------------------------------------	
// При перемещении окна
	case WM_MOVE:
		Opt->SetByName("FormLeft",Common::IntToStr(LOWORD(lParam)));
		Opt->SetByName("FormTop",Common::IntToStr(HIWORD(lParam)));
		break;
//------------------------------------------------------------------------------------------------------------------------	
// При изменении размеров окна
	case WM_SIZE:
		// Переинициализировать основной класс приложения
		switch(wParam) {
			case SIZE_RESTORED:		// Восстановить
				if(Vnes) Vnes->Resize(LOWORD(lParam),HIWORD(lParam));
				Opt->SetByName("FormStyle",Common::IntToStr((int)wParam));
				Opt->SetByName("FormWidth",Common::IntToStr(LOWORD(lParam)));
				Opt->SetByName("FormHeight",Common::IntToStr(HIWORD(lParam)));
				break;
			case SIZE_MINIMIZED:	// Свернуть
				Opt->SetByName("FormStyle",Common::IntToStr((int)wParam));
				break;
			case SIZE_MAXIMIZED:	// Развернуть
				if(Vnes) Vnes->Resize(LOWORD(lParam),HIWORD(lParam));
				Opt->SetByName("FormStyle",Common::IntToStr((int)wParam));
				break;
			default:	// Сообщения от др. окон
				Opt->SetByName("FormWidth",Common::IntToStr(LOWORD(lParam)));
				Opt->SetByName("FormHeight",Common::IntToStr(HIWORD(lParam)));
				break;
		}
		break;
//------------------------------------------------------------------------------------------------------------------------	
// При запуске программы
	case WM_CREATE: {
		// Загрузить сохрененные настройки
		Opt = new Options();
		GetCurrentDirectory(100,CurrentPath);
		Opt->File = std::string(CurrentPath)+"\\Vn.cfg";
		Opt->Load();
//		MessageBox(NULL,"","",MB_OK);
		// Установить начальное положение и размеры окна (с корректировкой автоматического сдвига)
		if(Common::StrToInt(Opt->FindByName("FormStyle"))==1) SendMessage(hWnd,WM_SYSCOMMAND,SC_MINIMIZE,0);
		if(Common::StrToInt(Opt->FindByName("FormStyle"))==2) SendMessage(hWnd,WM_SYSCOMMAND,SC_MAXIMIZE,0);
		if(Common::StrToInt(Opt->FindByName("FormStyle"))==0) MoveWindow(hWnd,Common::StrToInt(Opt->FindByName("FormLeft"))-4,Common::StrToInt(Opt->FindByName("FormTop"))-42,Common::StrToInt(Opt->FindByName("FormWidth"))+8,Common::StrToInt(Opt->FindByName("FormHeight"))+46,true);
		if(Common::StrToInt(Opt->FindByName("FullScreen"))==1) {
			// Полноэкранный режим
			// Удалить главное меню
			HMENU MainMenu = GetMenu(hWnd);
			SetMenu(hWnd,NULL);
			DestroyMenu(MainMenu);
			// Изменить стиль окна т.к. иначе после Alt+Tab реагирует не изменения размеров окна мышкой
			long WindowStyle = GetWindowLong(hWnd,GWL_STYLE);
			WindowStyle &= ~WS_OVERLAPPEDWINDOW;
			WindowStyle |= WS_BORDER;
			SetWindowLong(hWnd,GWL_STYLE,WindowStyle);
		}
		// Создать Log-файл
		Log = new LogFile();
		Log->File = std::string(CurrentPath)+"\\VN_Log.log";
		Log->Add("--------------------------------------------------");
		// Запустить основной класс приложения
		Vnes = new Vn(hWnd);
		// Установить параметры и запустить основное приложение
		Vnes->Log = Log;
		if(Common::StrToInt(Opt->FindByName("FullScreen"))==0) {
			// Оконный режим
			Vnes->Windowed = true;
			RECT Size;
			GetClientRect(hWnd,&Size);
			Vnes->Width = Size.right;
			Vnes->Height = Size.bottom;
		}
		else {
			// Полноэкранный режим
			Vnes->Windowed = false;
			Vnes->Width = Common::StrToInt(Opt->FindByName("FullScreenW"));
			Vnes->Height = Common::StrToInt(Opt->FindByName("FullScreenH"));
		}
		Vnes->MultisamplingLevel = Common::StrToInt(Opt->FindByName("MultisamplingLevel"));
		// Инициализация 3D
		bool Rez = Vnes->Init();
		if(Rez==false) DestroyWindow(hWnd);
		if(Common::StrToInt(Opt->FindByName("ShowFps"))==1) Vnes->ShowFPS();
		}
		break;
//------------------------------------------------------------------------------------------------------------------------	
// При нажатии текстовых клавиш
	case WM_CHAR:
		if(Vnes) Vnes->KeyPress((char)wParam,(char)lParam);
		break;
//------------------------------------------------------------------------------------------------------------------------	
// При нажатии системных клавиш (стрелок и т.п.)
	case WM_KEYDOWN:
		if(Vnes) Vnes->SystemKeyPress((char)wParam,(char)lParam);
		break;
//------------------------------------------------------------------------------------------------------------------------
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
//--------------------------------------------------------------------------------------------------------------------------
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
// Message handler for about box.
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
//--------------------------------------------------------------------------------------------------------------------------