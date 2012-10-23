#ifndef SPRITE_HEAD
#define SPRITE_HEAD
#include <string>
#include "GameWorld.h"
#include "MyBitMap.h"
#include "MessageOut.h"

using namespace std;

#define MAX_SPRITE_FRAMES 400		//in this game a man has 400 frames for actions
#define MAX_SPRITE_ANIMATIONS 48		//a man has six actions with eight directions at present

enum Action;
enum DIR;
enum MiniPosColor;
enum RoleInfoColor;
enum OBJ_STATU;

class MessageOut;

class Sprite
{
public:
	Sprite();
	virtual ~Sprite() = 0;
	
	int Init(const char * szData);
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
	int GetTypeID(){return TypeID;}
	int GetStatu(){return obj_statu;}
//	bool NeedRevive();
	void ChangePos(int x, int y){pos_x = x; pos_y = y;}
	void ChangeAction(Action act);
	void RandDir();
	COLORREF GetMiniPosColor(){return MiniPos_color;}
	void SetMiniPosColor(MiniPosColor color);	
	static int Load_Resource();
	static int Load_Frame(int indexNo, int nFromPicNo, int nCount, const char * pzFolderPath);
	static int Load_Animation(int anim_index, int num_frames, int *sequence);

	virtual int ** getpAnimations()		= 0;
	virtual MyBitMap ** getpBitMap()	= 0;

	virtual void AttachAnimations()		= 0;
	virtual	void AttachBitMaps()		= 0;

protected:

	void ChangeDir(int x, int y);	
	void MovePos();	
	MessageOut * pMessageOut;
	
	
	int animIndex;
	int pos_x, pos_y;
	Action action;
	DIR Dir;
	
	int anim_counter;
	int anim_count_max;
	int BitMapFrame;
	int id;
	int TypeID;
	
	MiniPosColor MiniPos_color;
	RoleInfoColor roleInfoColor;
	
	string bitmapFolderPath;
	int bitmapIndexOffset;
	int animationIndexoffset;
	int sprite_atrr;

	int **spAnimations;
	MyBitMap **spBitMap;	
	
	static int **spAnimationsResource;
	static MyBitMap **spBitMapResource;
	static bool resouceLoaded;	
	OBJ_STATU obj_statu;

};

class Player : public Sprite
{
public:
	Player();
	virtual ~Player();
	virtual int ** getpAnimations(){return pAnimations;}
	virtual MyBitMap ** getpBitMap(){return pBitMap;}

	virtual void AttachAnimations();
	virtual	void AttachBitMaps();
protected:
	
	static int **pAnimations;
	static MyBitMap **pBitMap;
	static bool resourceAttached;

};

class wolf: public Sprite
{
public:
	wolf();
	virtual ~wolf();
	virtual int ** getpAnimations(){return pAnimations;}
	virtual MyBitMap ** getpBitMap(){return pBitMap;}

	virtual void AttachAnimations();
	virtual	void AttachBitMaps();
protected:

	static int **pAnimations;
	static MyBitMap **pBitMap;
	static bool resourceAttached;

private:
};

#endif