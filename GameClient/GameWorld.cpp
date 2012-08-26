#include "GameWorld.h"


HDC		GameWorld::hdcScreen	= NULL;
HWND	GameWorld::hwnd_window	= NULL;

GameWorld::GameWorld()
{
}

GameWorld::~GameWorld()
{
}

int GameWorld::Game_Init(HWND hwnd)
{
	hwnd_window = hwnd;
	hdcScreen	= GetDC(hwnd);

	hdcCanvas	= CreateCompatibleDC(hdcScreen);

	bitCanvas	= CreateCompatibleBitmap(hdcScreen, GAME_WIDTH, GAME_HEIGHT);
	bitOldCanvas= (HBITMAP)SelectObject(hdcCanvas, bitCanvas);

	//pMap	= new MyBitMap("./pic/map/ground.BMP", false);
	//pMan	= new MyBitMap("./pic/man/C00000.BMP", true);

	spMan		= new Sprite();
	spMan->Init();

	pGameMap	= new GameMap();
	pGameMap->Init();
	
	::SetTimer(hwnd_window, 1, 150, NULL);
	return 0;
}

int GameWorld::Game_Shutdown()
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

int GameWorld::Game_Main()
{
	//pMap->Show(hdcCanvas, 0, 0);
	//pMan->Draw(hdcCanvas, 100, 100);

	pGameMap->Draw(hdcCanvas, 0, 0);

	spMan->Animate();
	spMan->Draw(hdcCanvas, 100, 100);

	Game_Refresh();
	
	return 0;
}

int GameWorld::Game_Refresh()
{
	BitBlt(hdcScreen, 0, 0, GAME_WIDTH, GAME_HEIGHT, hdcCanvas, 0, 0, SRCCOPY);
	return 0;
}