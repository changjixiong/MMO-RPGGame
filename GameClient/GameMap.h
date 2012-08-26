#ifndef GAMEMAP_HEAD
#define GAMEMAP_HEAD

#include "MyBitMap.h"

class GameMap
{
public:
	GameMap();
	~GameMap();

	int Init();
	int Draw(HDC hdcDest, int x, int y);
protected:
	MyBitMap * pMap;
private:
};

#endif