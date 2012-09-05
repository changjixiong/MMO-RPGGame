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

int GameMap::Draw(HDC hdcDest)
{
	int windowPos_x_offset = ViewportPos_x % GAME_WIDTH;
	int windowPos_y_offset = ViewportPos_y % GAME_HEIGHT;

	if (windowPos_x_offset<0)
	{
		windowPos_x_offset =0;
		ViewportPos_x =0;
	}

	if (windowPos_y_offset<0)
	{
		windowPos_y_offset =0;
		ViewportPos_y =0;
	}


	pMap->Show(hdcDest, -windowPos_x_offset, -windowPos_y_offset);

	if (windowPos_x_offset>0)
	{
		pMap->Show(hdcDest, GAME_WIDTH - windowPos_x_offset, -windowPos_y_offset);
	}

	if (windowPos_y_offset>0)
	{
		pMap->Show(hdcDest,  -windowPos_x_offset, GAME_HEIGHT- windowPos_y_offset);
	}

	if (windowPos_x_offset>0 && windowPos_y_offset>0)
	{
		pMap->Show(hdcDest,  GAME_WIDTH - windowPos_x_offset, GAME_HEIGHT- windowPos_y_offset);
	}
	

	
	return 0;
}

void GameMap::MoveViewport(int roleX, int roleY)
{

	ViewportPos_x = roleX - GAME_WIDTH/2;
	ViewportPos_y = roleY - GAME_HEIGHT/2;
	

	if (ViewportPos_x < 0)
	{
		ViewportPos_x = 0;
	}

	if (ViewportPos_x + GAME_WIDTH > SceneWidth)
	{
		ViewportPos_x = (ViewportPos_x/GAME_WIDTH) * GAME_WIDTH;
	}

	if (ViewportPos_y < 0)
	{
		ViewportPos_y = 0;
	}

	if (ViewportPos_y + GAME_HEIGHT> SceneHeight)
	{
		ViewportPos_y = (ViewportPos_y/GAME_HEIGHT)*GAME_HEIGHT;
	}

};