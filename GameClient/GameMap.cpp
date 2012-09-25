#include "GameMap.h"

const int miniMap_Width		= 160;
const int miniMap_Height	= 120;


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

	SelectObject(hdcMiniMap, bitOldMiniMap);
	DeleteObject(bitMiniMap);
	DeleteDC(hdcMiniMap);
}

int GameMap::Init(HDC hdcScreen)
{
	pMap	= new MyBitMap("./pic/map/ground.BMP", false);

	hdcMiniMap	= CreateCompatibleDC(hdcScreen);

	bitMiniMap	= CreateCompatibleBitmap(hdcScreen, miniMap_Width, miniMap_Height);
	bitOldMiniMap= (HBITMAP)SelectObject(hdcMiniMap, bitMiniMap);

	
	SetStretchBltMode( hdcMiniMap,  HALFTONE );
	for (int i=0;i<4;i++)
	{
		for (int j=0;j<4;j++)
		{
			pMap->StretchDraw(hdcMiniMap, 
				i*GAME_WIDTH/16, j*GAME_HEIGHT/16,
				GAME_WIDTH/16, GAME_HEIGHT/16);
		}		
	}

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


void GameMap::DrawMini(HDC hdcDest, const vector<Sprite *> & vecPplayer)
{
	::MoveToEx(hdcDest, GAME_WIDTH - miniMap_Width -1, 0, NULL);
	::LineTo(hdcDest, GAME_WIDTH - miniMap_Width -1, miniMap_Height+1);
	::LineTo(hdcDest, GAME_WIDTH -1, miniMap_Height+1);

	BitBlt(hdcDest, 
			GAME_WIDTH - miniMap_Width, 0, 
			miniMap_Width, miniMap_Height, 
			hdcMiniMap, 
			0, 0, 
			SRCCOPY);

	vector<Sprite *>::const_iterator itr;
	for (itr=vecPplayer.begin();itr!=vecPplayer.end();itr++)
	{
		int x = GAME_WIDTH - miniMap_Width + (*itr)->GetX()/16;
		int y = (*itr)->GetY()/16;
		::Rectangle(hdcDest, x-2, y -2, x+2, y+2);
	}
}