#include "GameWorld.h"
#include "Socket/SocketLibErrors.h"
#include "Socket/SocketLibSystem.h"

HDC		GameWorld::hdcScreen	= NULL;
HWND	GameWorld::hwnd_window	= NULL;

const int stepLen_x = 32;
const int stepLen_y = 24;
const int walkFrames= 10;
int SceneWidth = GAME_WIDTH*3;
int SceneHeight = GAME_HEIGHT*3;

int ViewportPos_x=0;
int ViewportPos_y=0;

GameWorld::GameWorld()
{
}

GameWorld::~GameWorld()
{
}

int GameWorld::Init(HWND hwnd)
{
	hwnd_window = hwnd;
	hdcScreen	= GetDC(hwnd);

	hdcCanvas	= CreateCompatibleDC(hdcScreen);

	bitCanvas	= CreateCompatibleBitmap(hdcScreen, GAME_WIDTH, GAME_HEIGHT);
	bitOldCanvas= (HBITMAP)SelectObject(hdcCanvas, bitCanvas);

	try
	{
		datasock.Connect(SocketLib::GetIPAddress("127.0.0.1"),5099);
		datasock.SetBlocking(false);

	}
	catch (SocketLib::Exception &e)
	{
		MessageBox(hwnd_window, e.PrintError().c_str(),"", MB_OK);
		return -1;
		
	}
	catch (...)
	{
		MessageBox(hwnd_window, "unknow error","", MB_OK);
		return -1;
	}

	spMan		= new Sprite();
	spMan->Init();
	
	pGameMap	= new GameMap();
	pGameMap->Init();
	
	::SetTimer(hwnd_window, 1, 16, NULL);
	SetBkMode(hdcCanvas,TRANSPARENT);
	
	return 0;
}

int GameWorld::Shutdown()
{
	SelectObject(hdcCanvas, bitOldCanvas);
	DeleteObject(bitCanvas);
	DeleteDC(hdcCanvas);

	delete spMan;
	spMan = NULL;

	delete pGameMap;
	pGameMap = NULL;
	return 0;
}

int GameWorld::Main()
{
	char szDebugMessage[64]={0};
	sprintf(szDebugMessage, "man X:%d, Y:%d",spMan->GetX(),spMan->GetY());
	PushDebugMessage(szDebugMessage);

	try
	{
		datasock.Receive(szDebugMessage, 64);
	}
	catch (SocketLib::Exception &e)
	{
		
	}
	
	
	if ((spMan->AnimationBegin() || spMan->GetAction() == STAND) 
		&& strlen(szDebugMessage)>0)
	{
		int x,y;
			
		sscanf(szDebugMessage,"[move,%d,%d]", &x, &y);		
		FixToGrid(x, y);
		char szmessage[64]={0};
		sprintf(szmessage, "[%d,%d]", x, y);
		spMan->Move(x,y);

		sprintf(szDebugMessage,"click X:%d Y:%d",x,y);
		SetWindowText(hwnd_window,szDebugMessage);
	}
	
	spMan->Animate();	
	pGameMap->MoveViewport(spMan->GetX(), spMan->GetY());

	sprintf(szDebugMessage, "man X:%d, Y:%d",spMan->GetX(),spMan->GetY());
	PushDebugMessage(szDebugMessage);

	sprintf(szDebugMessage, "Viewport X:%d, Y:%d",ViewportPos_x,ViewportPos_y);
	PushDebugMessage(szDebugMessage);
	
	pGameMap->Draw(hdcCanvas);
	spMan->Draw(hdcCanvas);

	Refresh();
	
	return 0;
}

int GameWorld::Refresh()
{
	for (int i=0;i<vecDebugMessage.size();i++)
	{
		TextOut(hdcCanvas, 10,10+i*18,vecDebugMessage[i].c_str(),vecDebugMessage[i].length());
	}
	
	vecDebugMessage.clear();

	
	BitBlt(hdcScreen, 0, 0, GAME_WIDTH, GAME_HEIGHT, hdcCanvas, 0, 0, SRCCOPY);
	return 0;
}

void GameWorld::FixToGrid(int & x, int & y)
{
	x = ((x + stepLen_x/2)/stepLen_x)*stepLen_x;
	y = ((y + stepLen_y/2)/stepLen_y)*stepLen_y;
}

void GameWorld::PushDebugMessage(char * pzMessage)
{
	vecDebugMessage.push_back(pzMessage);
	memset(pzMessage, 0, strlen(pzMessage));
}

void GameWorld::SetMessageFromInput(UINT msg, int x, int y)
{
	char szMessage[64]={0};
	x += ViewportPos_x;
	y += ViewportPos_y;	

	switch (msg)
	{
		case WM_LBUTTONDOWN:
			sprintf(szMessage, "[move,%d,%d]", x, y);

			break;
		default:
			break;
	}

	datasock.Send(szMessage, strlen(szMessage));
}


