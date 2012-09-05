#ifndef SPRITE_HEAD
#define SPRITE_HEAD
#include "GameWorld.h"
#include "MyBitMap.h"

#define MAX_SPRITE_FRAMES 400		//in this game a man has 400 frames for actions
#define MAX_SPRITE_ANIMATIONS 48		//a man has six actions with eight directions at present

class Sprite
{
public:
	Sprite();
	~Sprite();
	int Init();
	void Animate();
	void Draw(HDC hdcDest);	
	void Move(int x, int y);
	int	GetX(){return pos_x;}
	int GetY(){return pos_y;}
	enum Action GetAction() const;
	enum DIR	GetDir() const;

protected:
	int Load_Frame(int nFrom, int nCount);
	int Load_Animation(int anim_index, int num_frames, int *sequence);
	void ChangeDir(int x, int y);
	void ChangeAction(enum Action act);
	void MovePos();
private:

	int *animations[MAX_SPRITE_ANIMATIONS];
	MyBitMap *pBitMap[MAX_SPRITE_FRAMES];

	int BitMapFrame;
	int animIndex;
	int pos_x, pos_y;
	enum Action action;
	enum DIR Dir;

};

#endif