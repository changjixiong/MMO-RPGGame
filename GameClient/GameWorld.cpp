#include "GameWorld.h"
#include "Socket/SocketLibErrors.h"
#include "Socket/SocketLibSystem.h"

HDC		GameWorld::hdcScreen	= NULL;
HWND	GameWorld::hwnd_window	= NULL;

const int stepLen_x				= 32;
const int stepLen_y				= 24;
const int walkFrames			= 10;
const int DebugMsgNum			= 10;

const int DebugMsg_Index_viewPort	= 0;
const int DebugMsg_Index_SerMsg		= 1;

const int MsgLen				= 64;
const int DebugMsgLen			= 64;

const int DebugOut_X			= 10;
const int DebugOut_Y			= 10;
const int DebugOut_H			= 18;

const int TimeInterval			= 16;


int SceneWidth = GAME_WIDTH*3;
int SceneHeight = GAME_HEIGHT*3;
int ViewportPos_x=0;
int ViewportPos_y=0;


GameWorld::GameWorld()
{
	severMessageBuffer="";
}

GameWorld::~GameWorld()
{

}

int GameWorld::HandleMsg()
{
	char szMsg[MsgLen]={0};
	try
	{
		datasock.Receive(szMsg, MsgLen);
		severMessageBuffer+=szMsg;
	}
	catch (...)
	{
	}		

	string msg = severMessageBuffer.substr(0, severMessageBuffer.find("]")+1);
	severMessageBuffer=severMessageBuffer.substr(msg.length());

	int i;

	while (msg.length()>0)
	{
		int action, id, x, y;
		sscanf(msg.c_str(),"[%d,%d,%d,%d]",  &id, &action,&x, &y);
		vecDebugMessage[DebugMsg_Index_SerMsg]=msg;
		vector<Sprite *>::iterator iter;
		
		switch (action)
		{
		case SPRITEINITROLE:
			spMan		= new Sprite();		
			spMan->Init(msg.c_str());
			vecPplayer.push_back(spMan);
			break;
		case SPRITEINITPLAYER:
			{
				Sprite *spPlayer	= new Sprite();	
				spPlayer->Init(msg.c_str());
				vecPplayer.push_back(spPlayer);
			}
			break;

		case SPRITEDESTORYPLAYER:
			i=vecPplayer.size();
			for (iter=vecPplayer.begin(); iter!=vecPplayer.end();iter++)
			{
				if ((*iter)->GetID() == id)
				{
					Sprite *spPlayer = *iter;
					vecPplayer.erase(iter);
					iter--;
					delete spPlayer;
				}
			}
			
		case WALK:			
			for (iter=vecPplayer.begin(); iter!=vecPplayer.end();iter++)
			{
				if ((*iter)->GetID() == id)
				{
					(*iter)->Move(x,y);
				}
			}			
			break;
			
		default:
			break;
		}

		msg = severMessageBuffer.substr(0, severMessageBuffer.find("]")+1);
		severMessageBuffer=severMessageBuffer.substr(msg.length());
	}

	return 0;
}

int GameWorld::SendMsg(const string & strMsg)
{
	if (strMsg.length()==0)
	{
		return 0;
	}

	string msg = "["+strMsg+"]";

	try
	{
		datasock.Send(msg.c_str(), msg.length());
	}
	catch (SocketLib::Exception &e)
	{
		MessageBox(hwnd_window, e.PrintError().c_str(),"", MB_OK);
		return -1;
		
	}
	catch (...)
	{
		MessageBox(hwnd_window, "unknow error","", MB_OK);
		return -1;
	}	
	

	return 0;
}

int GameWorld::Init(HWND hwnd)
{
	for (int i=0;i<DebugMsgNum;i++)
	{
		vecDebugMessage.push_back("");
	}

	hwnd_window = hwnd;
	hdcScreen	= GetDC(hwnd);

	hdcCanvas	= CreateCompatibleDC(hdcScreen);

	bitCanvas	= CreateCompatibleBitmap(hdcScreen, GAME_WIDTH, GAME_HEIGHT);
	bitOldCanvas= (HBITMAP)SelectObject(hdcCanvas, bitCanvas);

	char szIP[16]={0};
	char szPort[16]={0};
	CHAR szDir[64]={0};
	
	::GetCurrentDirectory(64, szDir);
	::GetPrivateProfileString("server","IP","",szIP,16,(szDir+string("/config.ini")).c_str());
	::GetPrivateProfileString("server","port","",szPort,16,(szDir+string("/config.ini")).c_str());

	try
	{
		datasock.Connect(SocketLib::GetIPAddress(szIP),atoi(szPort));
	}
	catch (SocketLib::Exception &e)
	{
		MessageBox(hwnd_window, e.PrintError().c_str(),"", MB_OK);
		return -1;
		
	}
	catch (...)
	{
		MessageBox(hwnd_window, "unknow error","", MB_OK);
		return -1;
	}	
	
	if (Login() == -1)
	{
		MessageBox(hwnd_window, "Login failed","", MB_OK);
		return -1;
	}
	
	::SetTimer(hwnd_window, 1, TimeInterval, NULL);
	SetBkMode(hdcCanvas,TRANSPARENT);
	
	return 0;
}

int GameWorld::Login()
{
	char szUser[32]={0};
	CHAR szDir[64]={0};
	
	::GetCurrentDirectory(64, szDir);
	::GetPrivateProfileString("config","user","",szUser,32,(szDir+string("/config.ini")).c_str());

	char szData[MsgLen]={0};
	
	datasock.Receive(szData, MsgLen);
	
	if (strcmp(szData, "[username,password]"))
	{
		MessageBox(hwnd_window, "expect server msg error","",MB_OK);
		return -1;
	}	
	
	SendMsg(szUser);
	
	pGameMap	= new GameMap();
	pGameMap->Init();
	
	memset(szData, 0 ,MsgLen);
	datasock.Receive(szData, MsgLen);
	
	if (strcmp(szData,"[user is online]")==0)
	{
		MessageBox(hwnd_window, szData, "", MB_OK);
		return -1;
	}
	datasock.SetBlocking(false);
	
	severMessageBuffer+=szData;
	
	HandleMsg();	

	return 0;
}

int GameWorld::Shutdown()
{
	SelectObject(hdcCanvas, bitOldCanvas);
	DeleteObject(bitCanvas);
	DeleteDC(hdcCanvas);

	delete spMan;
	spMan = NULL;

	delete pGameMap;
	pGameMap = NULL;
	return 0;
}

int GameWorld::Main()
{		
	HandleMsg();	

	if (spMan == NULL)
	{
		return 0;
	}
	
	vector<Sprite *>::iterator iter;
	for (iter = vecPplayer.begin(); iter!=vecPplayer.end();iter++)
	{
		(*iter)->Animate();
	}
	
	pGameMap->MoveViewport(spMan->GetX(), spMan->GetY());

	char szDebugMessage[DebugMsgLen]={0};
	sprintf(szDebugMessage, "Viewport X:%d, Y:%d",ViewportPos_x,ViewportPos_y);
	vecDebugMessage[DebugMsg_Index_viewPort]=szDebugMessage;
	
	pGameMap->Draw(hdcCanvas);
	
	for (iter = vecPplayer.begin(); iter!=vecPplayer.end();iter++)
	{
		(*iter)->Draw(hdcCanvas);
	}	

	Refresh();
	
	return 0;
}

int GameWorld::Refresh()
{
	DebugOut();
	
	BitBlt(hdcScreen, 0, 0, GAME_WIDTH, GAME_HEIGHT, hdcCanvas, 0, 0, SRCCOPY);
	return 0;
}

void GameWorld::FixToGrid(Sprite * spMan, int & x, int & y)
{

	x = spMan->GetX() + (abs(x - spMan->GetX()) > stepLen_x/2 ? 1 :0) * ( x > spMan->GetX() ? 1 : -1) * stepLen_x;
	y = spMan->GetY() + (abs(y - spMan->GetY()) > stepLen_y/2 ? 1 :0) * ( y > spMan->GetY() ? 1 : -1) * stepLen_y;
}

void GameWorld::SetMessageFromInput(UINT msg, int x, int y)
{
	char szMessage[MsgLen]={0};
	char szDebugMessage[DebugMsgLen]={0};

	sprintf((char *)szDebugMessage,"click X:%d Y:%d",x,y);
	SetWindowText(hwnd_window,szDebugMessage);

	x += ViewportPos_x;
	y += ViewportPos_y;	

	switch (msg)
	{
		case WM_LBUTTONDOWN:
			if (spMan->AnimationBegin() || spMan->GetAction() == STAND)
			{
				FixToGrid(spMan, x, y);
				sprintf(szMessage, "%d,%d,%d,%d", spMan->GetID(), WALK, x, y);
			}
			

			break;
		default:
			break;
	}
	SendMsg(szMessage);	
}


int GameWorld::DebugOut()
{
	for (int i=0;i<vecDebugMessage.size();i++)
	{
		TextOut(hdcCanvas, DebugOut_X,DebugOut_Y+i*DebugOut_H,vecDebugMessage[i].c_str(),vecDebugMessage[i].length());
	}

	return 0;
}