#ifndef GAMEWORLD_HEAD
#define GAMEWORLD_HEAD
#define WIN32_LEAN_AND_MEAN  // just say no to MFC

#include <windows.h>
#include <string>
#include <vector>
#include <map>
#include "MyBitMap.h"
#include "Sprite.h"
#include "GameMap.h"
#include "MessageOut.h"
#include "Socket/SocketLibSocket.h"

#define GAME_WIDTH 640
#define GAME_HEIGHT 480

class GameMap;
class Sprite;
class SpriteResource;
class MessageOut;
class Player;
class wolf;

using namespace std;

extern const int walkFrames;
extern const int stepLen_x;
extern const int stepLen_y;
extern int ViewportPos_x;
extern int ViewportPos_y;

extern int SceneWidth;
extern int SceneHeight;

enum DIR
{
	SOUTH,
		SOUTHWEST,
		WEST,
		NORTHWEST,
		NORTH,
		NORTHEAST,
		EAST,
		SOUTHEAST,
};

enum Action
{
	STAND=0,	
	WALK=8,
	ATTACK=16,
	DIE = 24,
	INITMAINROLE =1,
	INITSPRITE =2,
	SPRITEDESTORYPLAYER=3,
	SPRITEREVIVE=4,

};

enum MiniPosColor
{
	MiniPosColor_Role	= RGB(0xff, 0xff, 0xff),	
	MiniPosColor_Player	= RGB(0, 0, 0xff),
	MiniPosColor_animal	= RGB(0xff, 0, 0)
};

enum RoleInfoColor
{
	RoleNormal = RGB(0xff, 0xff,0xff)
};

enum OBJ_TYPE
{
	OBJ_Player =1,
	OBJ_wolf = 2,
};

enum OBJ_STATU
{
	STATU_dormant = 0,
    STATU_normal = 1,
};

class GameWorld
{
public:
	GameWorld();
	~GameWorld();
	int Init(HWND hwnd);
	int Shutdown();
	int Main();	
	void SetMessageFromInput(UINT msg, int x, int y);	

	static HDC	hdcScreen;
	static HWND	hwnd_window;
protected:

private:

	GameMap *pGameMap;
	Sprite	*spMainRole;

	
	HDC		hdcCanvas;
	HBITMAP	bitCanvas;
	HBITMAP	bitOldCanvas;

private:
	int Refresh();
	void FixToGrid(Sprite * spMan, int & x, int & y);
	Sprite* haveSprite(int x, int y);
	int HandleMsg();	
	int DebugOut();
	int SendMsg(const string & strMsg);
	int Login();
	int GenerateMsg();
	Sprite * GameWorld::CreateSprite(int TypeID);

	vector<string> vecDebugMessage;
	vector<Sprite *> vecpSprite;
	SocketLib::DataSocket datasock;
	string severMessageBuffer;

	MessageOut *pMessageOut;
	
};



#endif