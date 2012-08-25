// INCLUDES ///////////////////////////////////////////////
#define WIN32_LEAN_AND_MEAN  // just say no to MFC

#include <windows.h>   // include all the windows headers
#include <windowsx.h>  // include useful macros
#include <stdio.h>     
#include <math.h>

#include "GameWorld.h"

// DEFINES ////////////////////////////////////////////////

// defines for windows 
#define WINDOW_CLASS_NAME "WINCLASS1"

// GLOBALS ////////////////////////////////////////////////
int AdjustWindow(HWND hwnd)
{
	RECT window_rect;
	GetWindowRect(hwnd, &window_rect);
	AdjustWindowRectEx(&window_rect,
		GetWindowStyle(hwnd),
		GetMenu(hwnd)!=NULL,
		GetWindowExStyle(hwnd));
	
	SetWindowPos(hwnd,
		HWND_TOP,
		(GetSystemMetrics(SM_CXSCREEN) - (window_rect.right - window_rect.left))/2,
		(GetSystemMetrics(SM_CYSCREEN) - (window_rect.bottom - window_rect.top))/2,
		window_rect.right - window_rect.left,
		window_rect.bottom - window_rect.top,
		SWP_SHOWWINDOW);
	
	return 0;
}

// FUNCTIONS //////////////////////////////////////////////
LRESULT CALLBACK WindowProc(HWND hwnd, 
						    UINT msg, 
                            WPARAM wparam, 
                            LPARAM lparam)
{
	// this is the main message handler of the system
	PAINTSTRUCT		ps;		// used in WM_PAINT
	HDC				hdc;	// handle to a device context

	// what is the message 
	switch(msg)
		{	
			case WM_CREATE: 
				{
				// do initialization stuff here

				// return success
				return(0);
				} break;

			case WM_PAINT: 
				{
				// simply validate the window
				hdc = BeginPaint(hwnd,&ps);	 
				// you would do all your painting here
				EndPaint(hwnd,&ps);

				// return success
				return(0);
   				} break;

			case WM_DESTROY: 
				{
				// kill the application, this sends a WM_QUIT message 
				PostQuitMessage(0);

				// return success
				return(0);
				} break;

			default:break;
		} // end switch

	// process any messages that we didn't take care of 
	return (DefWindowProc(hwnd, msg, wparam, lparam));

} // end WinProc

// WINMAIN ////////////////////////////////////////////////
int WINAPI WinMain(	HINSTANCE hinstance,
					HINSTANCE hprevinstance,
					LPSTR lpcmdline,
					int ncmdshow)
{

	WNDCLASSEX winclass; // this will hold the class we create
	HWND	   hwnd;	 // generic window handle
	MSG		   msg;		 // generic message

	// first fill in the window class stucture
	winclass.cbSize         = sizeof(WNDCLASSEX);
	winclass.style			= CS_DBLCLKS | CS_OWNDC | 
							  CS_HREDRAW | CS_VREDRAW;
	winclass.lpfnWndProc	= WindowProc;
	winclass.cbClsExtra		= 0;
	winclass.cbWndExtra		= 0;
	winclass.hInstance		= hinstance;
	winclass.hIcon			= LoadIcon(NULL, IDI_APPLICATION);
	winclass.hCursor		= LoadCursor(NULL, IDC_ARROW);
	winclass.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH);
	winclass.lpszMenuName	= NULL;
	winclass.lpszClassName	= WINDOW_CLASS_NAME;
	winclass.hIconSm        = LoadIcon(NULL, IDI_APPLICATION);

	// register the window class
	if (!RegisterClassEx(&winclass))
		return(0);

	// create the window
	if (!(hwnd = CreateWindowEx(NULL,                  // extended style
								WINDOW_CLASS_NAME,     // class
								"Your Basic Window++", // title
								WS_OVERLAPPEDWINDOW | WS_VISIBLE,
								(GetSystemMetrics(SM_CXSCREEN) - GAME_WIDTH)/2,
								(GetSystemMetrics(SM_CYSCREEN) - GAME_HEIGHT)/2, 
								GAME_WIDTH,GAME_HEIGHT,  // initial width, height
								NULL,	    // handle to parent 
								NULL,	    // handle to menu
								hinstance,// instance of this application
								NULL)))	// extra creation parms
	return(0);

	AdjustWindow(hwnd);

// 	if (Game_Init()!=0)
// 	{
// 		MessageBox(hWnd,"Game_Init error","",MB_OK);
// 		return 0;
// 	}

	HDC			hdcScreen = GetDC(hwnd);
	MyBitMap	bitGround(hdcScreen, "./pic/map/ground.BMP");
	MyBitMap	bitMan(hdcScreen, "./pic/man/c00000.bmp");
		
// 	HDC hdcScreen	= GetDC(hwnd);
// 	HDC hdcGround	= CreateCompatibleDC(hdcScreen);
// 
// 	HBITMAP bitmap	= (HBITMAP)LoadImage(NULL,"./pic/map/ground.BMP",IMAGE_BITMAP,0,0,LR_LOADFROMFILE|LR_CREATEDIBSECTION);
// 	HBITMAP bitOld	= (HBITMAP)SelectObject(hdcGround,bitmap);



	// enter main event loop, but this time we use PeekMessage()
	// instead of GetMessage() to retrieve messages
	while(TRUE)
		{
		// test if there is a message in queue, if so get it
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		   { 
		   // test if this is a quit
		   if (msg.message == WM_QUIT)
			   break;
		
		   // translate any accelerator keys
		   TranslateMessage(&msg);

		   // send the message to the window proc
		   DispatchMessage(&msg);
		   } // end if
    
		// main game processing goes here
		// Game_Main(); // or whatever your loop is called		
		//BitBlt(hdcScreen, 0, 0, 640, 480, hdcGround, 0, 0, SRCCOPY);
		bitGround.Show(hdcScreen);
		bitMan.Show(hdcScreen);
		
		//
		} // end while

	//Game_Shutdown();	
	//SelectObject(hdcGround, bitOld);
	//DeleteObject(bitmap);
	//DeleteDC(hdcGround);

	// return to Windows like this
	return(msg.wParam);

} // end WinMain

///////////////////////////////////////////////////////////

