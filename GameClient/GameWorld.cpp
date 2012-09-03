#include "GameWorld.h"


HDC		GameWorld::hdcScreen	= NULL;
HWND	GameWorld::hwnd_window	= NULL;

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

	spMan		= new Sprite();
	spMan->Init();

	pGameMap	= new GameMap();
	pGameMap->Init();
	
	::SetTimer(hwnd_window, 1, 150, NULL);
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

	if (serverMessage.length()>0)
	{
		int x,y;
			
		sscanf(serverMessage.c_str(),"[%d,%d]", &x, &y);
		char szmessage[64]={0};
		sprintf(szmessage, "[%d,%d]", x, y);
		spMan->Move(x,y);
		serverMessage = "";
	}

	pGameMap->Draw(hdcCanvas, 0, 0);
	
	spMan->Animate();
	spMan->Draw(hdcCanvas);

	Refresh();
	
	return 0;
}

int GameWorld::Refresh()
{
	BitBlt(hdcScreen, 0, 0, GAME_WIDTH, GAME_HEIGHT, hdcCanvas, 0, 0, SRCCOPY);
	return 0;
}

void GameWorld::SetMessage(const string &strMessage)
{
	serverMessage = strMessage;
}