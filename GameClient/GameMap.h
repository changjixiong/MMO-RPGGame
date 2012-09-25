#ifndef GAMEMAP_HEAD
#define GAMEMAP_HEAD

#include <vector>

#include "GameWorld.h"
#include "MyBitMap.h"

using namespace std;

class Sprite;

class GameMap
{
public:
	GameMap();
	~GameMap();

	int Init(HDC hdcScreen);
	int Draw(HDC hdcDest);
	void MoveViewport(int roleX, int roleY);
	void DrawMini(HDC hdcDest, const vector<Sprite *> & vecPplayer);
protected:
	MyBitMap * pMap;
	HDC		hdcMiniMap;
	HBITMAP bitMiniMap;
	HBITMAP bitOldMiniMap;


private:
};

#endif