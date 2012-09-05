#ifndef GAMEMAP_HEAD
#define GAMEMAP_HEAD

#include "GameWorld.h"
#include "MyBitMap.h"

class GameMap
{
public:
	GameMap();
	~GameMap();

	int Init();
	int Draw(HDC hdcDest);
	void MoveViewport(int roleX, int roleY);
protected:
	MyBitMap * pMap;

private:
};

#endif