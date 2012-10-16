#include "MessageOut.h"

const int DebugOut_H			= 18;

MessageOut::MessageOut(int width, int height, int fontHeight, int fnWeight)
{
	hdcContent	= CreateCompatibleDC(GameWorld::hdcScreen);
	hdcMask		= CreateCompatibleDC(GameWorld::hdcScreen);
	
	hbitContent	= CreateCompatibleBitmap(GameWorld::hdcScreen, width, height);
	hbitMask	= CreateCompatibleBitmap(GameWorld::hdcScreen, width, height);
	
	hbitOldMask				= (HBITMAP)SelectObject(hdcMask, hbitContent);
	hbitOldContent			= (HBITMAP)SelectObject(hdcContent, hbitMask);

	

	SetBkMode(hdcContent, TRANSPARENT);
	SetTextColor(RGB(0x00, 0x00, 0xff));
	hbrBlack = CreateSolidBrush(RGB(0x00, 0x00, 0x00));
	hbrOldContent = (HBRUSH)SelectObject(hdcContent, hbrBlack);
	
	hbrOldMask = (HBRUSH)SelectObject(hdcMask, WHITE_BRUSH);
	
	this->width		= width;
	this->height	= height;

	hfontInfo = CreateFont(fontHeight,0,
								0,0,fnWeight,
								false,false,false,
								CHINESEBIG5_CHARSET,OUT_CHARACTER_PRECIS,
								CLIP_CHARACTER_PRECIS,DEFAULT_QUALITY,
								FF_MODERN,"ËÎÌו"); 	

	hfontContentOld	= (HFONT)SelectObject(hdcContent, hfontInfo);
	hfontMaskOld	= (HFONT)SelectObject(hdcMask, hfontInfo);


}

void MessageOut::SetTextColor(COLORREF color)
{
	::SetTextColor(hdcContent, color);
}

MessageOut::~MessageOut()
{
	SelectObject(hdcContent, hbitOldContent);
	SelectObject(hdcContent, hbrOldContent);
	SelectObject(hdcContent,hfontContentOld);

	SelectObject(hdcMask, hbitOldMask);	
	SelectObject(hdcMask, hbrOldMask);
	SelectObject(hdcMask, hfontMaskOld);

	
	
	DeleteObject(hbitContent);
	DeleteObject(hbitMask);

	DeleteObject(hbrBlack);

	DeleteObject(hfontInfo);
	
	DeleteDC(hdcContent);
	DeleteDC(hdcMask);
}

void MessageOut::Draw(HDC hdcDest, vector<string> &vecContent, int x, int y)
{
	Rectangle(hdcContent, -1, -1, width+1, height+1);
	Rectangle(hdcMask, -1, -1, width+1, height+1);
	for (int i=0;i<vecContent.size();i++)
	{
		TextOut(hdcContent, 0, i*DebugOut_H, vecContent[i].c_str(), vecContent[i].length());
		TextOut(hdcMask, 0, i*DebugOut_H, vecContent[i].c_str(), vecContent[i].length());
	}

	BitBlt(hdcDest,
			x,y,
			width, height,
			hdcMask,
			0,0,
			SRCAND);

	BitBlt(hdcDest,
			x,y,
			width, height,
			hdcContent,
			0,0,
			SRCPAINT);

}

void MessageOut::Draw(HDC hdcDest, const string & strMsg, int x, int y)
{
	Rectangle(hdcContent, -1, -1, width+1, height+1);
	Rectangle(hdcMask, -1, -1, width+1, height+1);

	TextOut(hdcContent, 0, 0, strMsg.c_str(), strMsg.length());
	TextOut(hdcMask, 0, 0, strMsg.c_str(), strMsg.length());

	
	BitBlt(hdcDest,
		x,y,
		width, height,
		hdcMask,
		0,0,
		SRCAND);
	
	BitBlt(hdcDest,
		x,y,
		width, height,
		hdcContent,
		0,0,
		SRCPAINT);
}