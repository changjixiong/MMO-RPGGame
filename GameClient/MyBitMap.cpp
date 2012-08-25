#include "MyBitMap.h"

MyBitMap::MyBitMap(HDC hdcScreen, const char * pzPic)
{
	hdcBitMap=CreateCompatibleDC(hdcScreen);
	bitmap=(HBITMAP)LoadImage(NULL,pzPic,IMAGE_BITMAP,0,0,LR_LOADFROMFILE|LR_CREATEDIBSECTION);
	bitOld=(HBITMAP)SelectObject(hdcBitMap,bitmap);
	
	DIBSECTION ds;
	GetObject(bitmap,sizeof(ds),&ds);
	
	Width	= ds.dsBmih.biWidth;
	Height	= ds.dsBmih.biHeight;
}
MyBitMap::~MyBitMap()
{
	SelectObject(hdcBitMap,bitOld);
	DeleteObject(bitOld);
}

void MyBitMap::Show(HDC hdcDest)
{
	BitBlt(hdcDest,
			0,0,
			Width,Height,hdcBitMap,
			0,0,
			SRCCOPY);
}