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
#include "Socket/SocketLibSocket.h"

#define GAME_WIDTH 640
#define GAME_HEIGHT 480

class GameMap;
class Sprite;

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

	SPRITEINITROLE =1,
	SPRITEINITPLAYER =2,
	SPRITEDESTORYPLAYER=3,
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
	Sprite	*spMan;

	HDC		hdcCanvas;
	HBITMAP	bitCanvas;
	HBITMAP	bitOldCanvas;

private:
	int Refresh();
	void FixToGrid(Sprite * spMan, int & x, int & y);
	int HandleMsg();	
	int DebugOut();
	int SendMsg(const string & strMsg);
	int Login();

	vector<string> vecDebugMessage;
	vector<Sprite *> vecPplayer;
	SocketLib::DataSocket datasock;
	string severMessageBuffer;
};



#endif