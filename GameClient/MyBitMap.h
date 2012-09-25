#ifndef MYBITMAP_HEAD
#define MYBITMAP_HEAD
#define  WIN32_LEAN_AND_MEAN
#include <windows.h>

class MyBitMap
{
public:
	MyBitMap(const char * pzPic, bool bNeedTransparent);
	
	~MyBitMap();

	void Show(HDC hdcDest, int x, int y, bool invert=false);
	void Draw(HDC hdcDest, int x, int y, bool invert=false);	
	void SetOffSet(int x, int y);
	void StretchDraw(HDC hdcDest, int x, int y, int width, int height);

	

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
	int offset_x, offset_y;
};

#endif