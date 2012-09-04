#include "MyBitMap.h"
#include "GameWorld.h"


MyBitMap::MyBitMap(const char * pzPic, bool bNeedTransparent)
{
	bitOriginal		= NULL;
	bitOldOriginal	= NULL;
	hdcOriginal		= NULL;
	
	bitMask			= NULL;
	bitOldMask		= NULL;
	hdcMask			= NULL;
	
	bitBlackBack	= NULL;
	bitOldBlackBack	= NULL;
	hdcBlackBack	= NULL;
	
	Width	= 0;
	Height	= 0;
	
	hdcOriginal=CreateCompatibleDC(GameWorld::hdcScreen);
	bitOriginal=(HBITMAP)LoadImage(NULL, pzPic, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE|LR_CREATEDIBSECTION);
	bitOldOriginal=(HBITMAP)SelectObject(hdcOriginal, bitOriginal);
	
	DIBSECTION ds;
	GetObject(bitOriginal, sizeof(ds), &ds);
	
	Width	= ds.dsBmih.biWidth;
	Height	= ds.dsBmih.biHeight;
	
	if (bNeedTransparent)
	{
		
		bitBlackBack=CreateCompatibleBitmap(GameWorld::hdcScreen, Width, Height);
		bitMask=CreateBitmap(Width, Height, 1, 1, NULL);
		
		hdcBlackBack=CreateCompatibleDC(GameWorld::hdcScreen);
		hdcMask=CreateCompatibleDC(GameWorld::hdcScreen);
		
		bitOldBlackBack=(HBITMAP)SelectObject(hdcBlackBack,bitBlackBack);
		bitOldMask=(HBITMAP)SelectObject(hdcMask,bitMask);
		
		BitBlt(hdcBlackBack, 0, 0, Width, Height, hdcOriginal, 0, 0, SRCCOPY);
		
		SetBkColor(hdcBlackBack, RGB(255,255,255));	
		BitBlt(hdcMask, 0, 0, Width, Height, hdcBlackBack, 0, 0, SRCCOPY);
		
		
		SetBkColor(hdcBlackBack, RGB(0,0,0));
		SetTextColor(hdcBlackBack, RGB(255,255,255));
		BitBlt(hdcBlackBack, 0, 0, Width, Height, hdcMask, 0, 0, SRCAND);
		
	}

	offset_x	= 0;
	offset_y	= 0;
}
MyBitMap::~MyBitMap()
{
	SelectObject(hdcOriginal, bitOldOriginal);
	DeleteObject(bitOriginal);
	DeleteDC(hdcOriginal);
	
	if (hdcBlackBack)
	{
		SelectObject(hdcBlackBack, bitOldBlackBack);
		DeleteObject(bitOldBlackBack);
		DeleteDC(hdcBlackBack);		
	}
	
	if (hdcMask)
	{
		SelectObject(hdcMask, bitOldMask);
		DeleteObject(bitMask);
		DeleteDC(hdcMask);
	}
}

void MyBitMap::Show(HDC hdcDest, int x, int y, bool invert)
{
	y -= offset_y;
	if (invert)
	{
		x += offset_x;	
		StretchBlt(hdcDest,
			x, y ,
			-Width,Height,hdcOriginal,
			0,0,Width,Height,
			SRCCOPY);
	}
	else
	{
		x -= offset_x;	
		BitBlt(hdcDest,
			x, y ,
			Width,Height,hdcOriginal,
			0,0,
			SRCCOPY);
	}

	
	
	
}

void MyBitMap::Draw(HDC hdcDest, int x, int y, bool invert)
{
	y -= offset_y;

	if (invert)
	{
		x += offset_x;			
		StretchBlt(hdcDest, x , y , -Width, Height, hdcMask, 0, 0, Width, Height, SRCAND);
		StretchBlt(hdcDest, x , y , -Width, Height, hdcBlackBack, 0, 0, Width, Height, SRCPAINT);
	}
	else
	{
		x -= offset_x;	
		BitBlt(hdcDest, x , y , Width, Height, hdcMask, 0, 0, SRCAND);
		BitBlt(hdcDest, x , y , Width, Height, hdcBlackBack, 0, 0, SRCPAINT);
	}	
}

void MyBitMap::SetOffSet(int x, int y)
{
	offset_x	= x;
	offset_y	= y;
}