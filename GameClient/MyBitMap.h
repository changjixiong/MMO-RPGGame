#ifndef MYBITMAP_HEAD
#define MYBITMAP_HEAD
#include <windows.h>

class MyBitMap
{
public:
	MyBitMap(const char * pzPic, bool bNeedTransparent);
	
	~MyBitMap();

	void Show(HDC hdcDest, int x, int y);
	void Draw(HDC hdcDest, int x, int y);	

	

protected:
	HBITMAP bitOriginal;
	HBITMAP bitOldOriginal;
	HDC hdcOriginal;

	HBITMAP bitMask;
	HBITMAP bitOldMask;
	HDC hdcMask;
	
	HBITMAP bitBlackBack;
	HBITMAP bitOldBlackBack;
	HDC hdcBlackBack;
	
	int Width, Height;
private:
};

#endif