#ifndef SPRITE_HEAD
#define SPRITE_HEAD
#include "GameWorld.h"
#include "MyBitMap.h"

#define MAX_SPRITE_FRAMES 400		//in this game a man has 400 frames for actions
#define MAX_SPRITE_ANIMATIONS 48		//a man has six actions with eight directions at present

enum Action;
enum DIR;

class Sprite
{
public:
	Sprite();
	Sprite(const Sprite & sprite);
	~Sprite();
	int Init(const char * szData);
	void Animate();
	void Draw(HDC hdcDest);	
	void Move(int x, int y);
	int	GetX(){return pos_x;}
	int GetY(){return pos_y;}
	Action GetAction() const;
	DIR	GetDir() const;
	int GetFrameNum() const {return BitMapFrame;}
	int AnimationBegin() const {return BitMapFrame==0;}
	int GetID(){return id;}

protected:
	int Load_Frame(int nFrom, int nCount);
	int Load_Animation(int anim_index, int num_frames, int *sequence);
	void ChangeDir(int x, int y);
	void ChangeAction(Action act);
	void MovePos();
private:

	int *animations[MAX_SPRITE_ANIMATIONS];
	MyBitMap *pBitMap[MAX_SPRITE_FRAMES];

	
	int animIndex;
	int pos_x, pos_y;
	Action action;
	DIR Dir;

	int anim_counter;
	int anim_count_max;
	int BitMapFrame;
	int id;

};

#endif