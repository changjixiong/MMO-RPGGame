#ifndef SPRITE_HEAD
#define SPRITE_HEAD

#include "MyBitMap.h"

#define MAX_SPRITE_FRAMES 400		//in this game a man has 400 frames for actions
#define MAX_SPRITE_ANIMATIONS 6		//a man has only six actions at present

class Sprite
{
public:
	Sprite();
	~Sprite();
	int Init();
	void Animate();
	void Draw(HDC hdcDest, int x, int y);
protected:
	int Load_Frame(int nFrom, int nCount);
	int Load_Animation(int anim_index, int num_frames, int *sequence);
private:

	int *animations[MAX_SPRITE_ANIMATIONS];
	MyBitMap *pBitMap[MAX_SPRITE_FRAMES];

	int BitMapFrame;
	int animIndex;

};

#endif