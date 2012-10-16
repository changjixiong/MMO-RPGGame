#ifndef SPRITE_HEAD
#define SPRITE_HEAD
#include "GameWorld.h"
#include "MyBitMap.h"
#include "MessageOut.h"

#define MAX_SPRITE_FRAMES 400		//in this game a man has 400 frames for actions
#define MAX_SPRITE_ANIMATIONS 48		//a man has six actions with eight directions at present

enum Action;
enum DIR;
enum MiniPosColor;
enum RoleInfoColor;

class MessageOut;

class SpriteResource
{
public:
	SpriteResource();
	~SpriteResource();
	int init();	
	int *animations[MAX_SPRITE_ANIMATIONS];
	MyBitMap *pBitMap[MAX_SPRITE_FRAMES];


private:
	int Load_Frame(int nFrom, int nCount);
	int Load_Animation(int anim_index, int num_frames, int *sequence);

};

class Sprite
{
public:
	Sprite();
	~Sprite();
	int GetResource(const SpriteResource & spriteResource);
	int Init(const char * szData, const SpriteResource & spriteResource);
	void Animate();
	void Draw(HDC hdcDest);	
	void DrawInfo(HDC hdcDest);
	void Move(int x, int y);
	void Attack(int x, int y);
	void Die(int x, int y);
	int	GetX(){return pos_x;}
	int GetY(){return pos_y;}
	Action GetAction() const;
	DIR	GetDir() const;
	int GetFrameNum() const {return BitMapFrame;}
	int GetAnimIndex() const {return animIndex;}
	int AnimationBegin() const {return BitMapFrame==0;}
	int GetID(){return id;}
	bool NeedRevive();
	void ChangePos(int x, int y){pos_x = x; pos_y = y;}
	void ChangeAction(Action act);
	COLORREF GetMiniPosColor(){return MiniPos_color;}
	void SetMiniPosColor(MiniPosColor color);

protected:

	void ChangeDir(int x, int y);	
	void MovePos();
private:

	int *animations[MAX_SPRITE_ANIMATIONS];
	MyBitMap *pBitMap[MAX_SPRITE_FRAMES];

	MessageOut * pMessageOut;

	
	int animIndex;
	int pos_x, pos_y;
	Action action;
	DIR Dir;

	int anim_counter;
	int anim_count_max;
	int BitMapFrame;
	int id;

	MiniPosColor MiniPos_color;
	RoleInfoColor roleInfoColor;
};

#endif