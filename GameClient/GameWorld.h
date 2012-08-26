#ifndef GAMEWORLD_HEAD
#define GAMEWORLD_HEAD
#include <windows.h>
#include "MyBitMap.h"
#include "Sprite.h"
#include "GameMap.h"

#define GAME_WIDTH 640
#define GAME_HEIGHT 480


class GameWorld
{
public:
	GameWorld();
	~GameWorld();
	int Game_Init(HWND hwnd);
	int Game_Shutdown();
	int Game_Main();	

	static HDC	hdcScreen;
	static HWND	hwnd_window;
protected:

private:
	//MyBitMap * pMap;
	//MyBitMap * pMan;
	GameMap *pGameMap;
	Sprite	*spMan;

	HDC		hdcCanvas;
	HBITMAP	bitCanvas;
	HBITMAP	bitOldCanvas;

private:
	int Game_Refresh();

};



#endif