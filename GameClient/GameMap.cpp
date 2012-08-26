#include "GameMap.h"

GameMap::GameMap()
{

}

GameMap::~GameMap()
{
	if (pMap)
	{
		delete pMap;
		pMap = NULL;
	}
}

int GameMap::Init()
{
	pMap	= new MyBitMap("./pic/map/ground.BMP", false);
	return 0;
}

int GameMap::Draw(HDC hdcDest, int x, int y)
{
	pMap->Show(hdcDest, 0, 0);
	return 0;
}