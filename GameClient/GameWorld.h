#ifndef GAMEWORLD_HEAD
#define GAMEWORLD_HEAD
#include <windows.h>
#include <string>
#include <vector>
#include "MyBitMap.h"
#include "Sprite.h"
#include "GameMap.h"

#define GAME_WIDTH 640
#define GAME_HEIGHT 480

using namespace std;

class GameWorld
{
public:
	GameWorld();
	~GameWorld();
	int Init(HWND hwnd);
	int Shutdown();
	int Main();	

	void SetMessage(const string &strMessage);

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
	int Refresh();
	void FixToGrid(int & x, int & y);
	string serverMessage;

};



#endif