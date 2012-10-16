#ifndef MESSAGEOUT
#define MESSAGEOUT
#include <vector>
#include "GameWorld.h"

using namespace std;

class MessageOut
{
public:
	MessageOut(int width, int height, int fontHeight, int fnWeight);
	~MessageOut();
	void Draw(HDC hdcDest, vector<string> &vecContent, int x, int y);
	void Draw(HDC hdcDest, const string & strMsg, int x, int y);
	void SetTextColor(COLORREF color);
protected:
private:
	HDC hdcContent;
	HDC hdcMask;
	
	HBITMAP hbitContent;
	HBITMAP hbitMask;
	HBITMAP hbitOldContent;
	HBITMAP hbitOldMask;

	HBRUSH hbrBlack;
	HBRUSH hbrOldContent;
	HBRUSH hbrOldMask;

	HFONT hfontInfo;
	HFONT hfontContentOld;
	HFONT hfontMaskOld;

	int width, height;
};

#endif