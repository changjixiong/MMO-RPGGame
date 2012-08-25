#ifndef MYBITMAP_HEAD
#define MYBITMAP_HEAD

#include <windows.h>

class MyBitMap
{
public:
	MyBitMap(HDC hdcScreen, const char * pzPic);
	
	~MyBitMap();

	void Show(HDC hdcDest);

	

protected:
	HBITMAP bitmap;
	HBITMAP bitOld;
	HDC hdcBitMap;
	
	int Width, Height;
private:
};

#endif