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
const int DebugMsg_Index_GetMsg		= 1;
const int DebugMsg_Index_SendMsg	= 2;
const int DebugMsg_Index_Animat		= 3;

const int MsgLen				= 64;
const int DebugMsgLen			= 64;

const int DebugOut_Width		= 200;
const int DebugOut_Height		= 300;

const int TimeInterval			= 16;

const int DebugOut_X			= 10;
const int DebugOut_Y			= 10;


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
		int len = datasock.Receive(szMsg, MsgLen - 1);
		severMessageBuffer.append(szMsg, len);
	}
	catch (...)
	{
	}		

	string msg = severMessageBuffer.substr(0, severMessageBuffer.find("]")+1);
	severMessageBuffer=severMessageBuffer.substr(msg.length());

	while (msg.length()>0)
	{
		//typeid, id, statu, action, targetTypeID, targetid, x, y
		int typeID,id, statu, action, targetTypeID, TargetID, x, y;
		sscanf(msg.c_str(),"[%d,%d,%d,%d,%d,%d,%d,%d]", &typeID, &id, &statu, &action, &targetTypeID, &TargetID, &x, &y);
		vecDebugMessage[DebugMsg_Index_GetMsg] = "get:" +msg;
		vector<Sprite *>::iterator iter;
		
		switch (action)
		{
		case INITMAINROLE:
			spMainRole	= CreateSprite(typeID);
			spMainRole->Init(msg.c_str());
			spMainRole->SetMiniPosColor(MiniPosColor_Role);
			vecpSprite.push_back(spMainRole);			
			break;
		case INITSPRITE:		
			{
				Sprite * sp = CreateSprite(typeID);
				sp->Init(msg.c_str());
				vecpSprite.push_back(sp);
			}
			break;

		case SPRITEDESTORYPLAYER:
			for (iter=vecpSprite.begin(); iter!=vecpSprite.end();iter++)
			{
				if ((*iter)->GetTypeID() == typeID && (*iter)->GetID() == id)
				{
					Sprite *spPlayer = *iter;
					vecpSprite.erase(iter);
					iter--;
					delete spPlayer;
				}
			}
		case SPRITEREVIVE:
			for (iter=vecpSprite.begin(); iter!=vecpSprite.end();iter++)
			{
				if ((*iter)->GetTypeID() == typeID && (*iter)->GetID() == id)
				{
					(*iter)->ChangeAction(STAND);
					(*iter)->ChangePos(x, y);
					(*iter)->RandDir();
				}
			}
			break;
			
		case WALK:			
			for (iter=vecpSprite.begin(); iter!=vecpSprite.end();iter++)
			{
				if ((*iter)->GetTypeID() == typeID && (*iter)->GetID() == id)
				{
					(*iter)->Move(x,y);
				}
			}			
			break;
		case ATTACK:
			for (iter=vecpSprite.begin(); iter!=vecpSprite.end();iter++)
			{
				if ((*iter)->GetTypeID() == typeID && (*iter)->GetID() == id)
				{
					(*iter)->Attack(x,y);
				}
			}	
			break;

		case DIE:
			for (iter=vecpSprite.begin(); iter!=vecpSprite.end();iter++)
			{
				if ((*iter)->GetTypeID() == typeID && (*iter)->GetID() == id)
				{
					(*iter)->Die(x,y);
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
	if (strMsg.length()==0 || datasock.IsConnected() == false)
	{
		return 0;
	}

	string msg = "["+strMsg+"]";

	vecDebugMessage[DebugMsg_Index_SendMsg] = "send:" +msg;
	try
	{
		datasock.Send(msg.c_str(), msg.length());
	}
	catch (SocketLib::Exception &e)
	{
		MessageBox(hwnd_window, e.PrintError().c_str(),"", MB_OK);
		datasock.Close();
		return -1;
		
	}
	catch (...)
	{
		datasock.Close();
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
	Sprite::Load_Resource();

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

	pMessageOut = new MessageOut(DebugOut_Width, DebugOut_Height, 12, FW_THIN);

	return 0;
}

int GameWorld::Login()
{
	char szUser[32]={0};
	CHAR szDir[64]={0};
	
	::GetCurrentDirectory(64, szDir);
	::GetPrivateProfileString("config","user","",szUser,32,(szDir+string("/config.ini")).c_str());

	char szData[MsgLen]={0};
	
	memset(szData, 0, MsgLen);
	datasock.Receive(szData, MsgLen);
	
	if (strcmp(szData, "[username,password]"))
	{
		MessageBox(hwnd_window, "expect server msg error","",MB_OK);
		return -1;
	}	
	
	SendMsg(szUser);
	
	pGameMap	= new GameMap();
	pGameMap->Init(hdcScreen);

	
	memset(szData, 0 ,MsgLen);
	int len = datasock.Receive(szData, MsgLen);
	
	if (strcmp(szData,"[user is online]")==0)
	{
		MessageBox(hwnd_window, szData, "", MB_OK);
		return -1;
	}
	datasock.SetBlocking(false);
	
	severMessageBuffer.append(szData, len);
	
	HandleMsg();	

	return 0;
}

int GameWorld::Shutdown()
{
	SelectObject(hdcCanvas, bitOldCanvas);
	DeleteObject(bitCanvas);
	DeleteDC(hdcCanvas);

	vector<Sprite *>::iterator itr;

	for (itr = vecpSprite.begin(); itr != vecpSprite.end(); itr++)
	{
		delete (*itr);
		*itr = NULL;
	}	

	delete pGameMap;
	pGameMap = NULL;

	delete pMessageOut;
	pMessageOut = NULL;

	return 0;
}

int GameWorld::Main()
{		
	HandleMsg();	

	if (spMainRole == NULL)
	{
		return 0;
	}
	
	vector<Sprite *>::iterator iter;
	for (iter = vecpSprite.begin(); iter!=vecpSprite.end();iter++)
	{
		(*iter)->Animate();
	}
	
	pGameMap->MoveViewport(spMainRole->GetX(), spMainRole->GetY());

	char szDebugMessage[DebugMsgLen]={0};
	sprintf(szDebugMessage, "Viewport X:%d, Y:%d",ViewportPos_x,ViewportPos_y);
	vecDebugMessage[DebugMsg_Index_viewPort]=szDebugMessage;
	
	pGameMap->Draw(hdcCanvas);
	
	for (iter = vecpSprite.begin(); iter!=vecpSprite.end();iter++)
	{
		(*iter)->Draw(hdcCanvas);

		if ((*iter)->GetTypeID()==spMainRole->GetTypeID() && (*iter)->GetID() == spMainRole->GetID())
		{
			sprintf(szDebugMessage, "AnimIndex:%d, FrameNum :%d",spMainRole->GetAnimIndex(),spMainRole->GetFrameNum());
			vecDebugMessage[DebugMsg_Index_Animat]=szDebugMessage;
		}
		
	}	

	pGameMap->DrawMini(hdcCanvas, vecpSprite);
	Refresh();
	GenerateMsg();
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

int GameWorld::GenerateMsg()
{
// 	if (spMainRole->NeedRevive())
// 	{
// 		char szMessage[MsgLen]={0};
// 		sprintf(szMessage, "%d,%d,%d,%d,%d,%d,%d,%d", 
// 				spMainRole->GetTypeID(), spMainRole->GetID(), spMainRole->GetStatu(), SPRITEREVIVE, 0, 0, 320, 240);
// 		SendMsg(szMessage);	
// 	}

	return 0;
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
			if (spMainRole->AnimationBegin() || spMainRole->GetAction() == STAND)
			{
				FixToGrid(spMainRole, x, y);

				Sprite* pTarget = haveSprite(x, y);
				//typeid, id, statu, action, targetTypeID, targetid, x, y
				if (pTarget)
				{
					sprintf(szMessage, "%d,%d,%d,%d,%d,%d,%d,%d", 
							spMainRole->GetTypeID() ,spMainRole->GetID(), spMainRole->GetStatu(), ATTACK, 
							pTarget->GetTypeID(), pTarget->GetID(), x, y);
				}
				else
				{
					sprintf(szMessage, "%d,%d,%d,%d,%d,%d,%d,%d", 
							spMainRole->GetTypeID(), spMainRole->GetID(), spMainRole->GetStatu(), WALK, 0, 0, x, y);
				}
			}			

			break;
		default:
			break;
	}	
	SendMsg(szMessage);	
}

Sprite* GameWorld::haveSprite(int x, int y)
{
	// it will be replaced of find_if late
	vector<Sprite *>::iterator iter;
	for (iter = vecpSprite.begin(); iter!=vecpSprite.end();iter++)
	{
		if ( (*iter)->GetAction() != DIE
			&& (*iter)->GetStatu() != STATU_dormant
			&& ((*iter)->GetTypeID()!=spMainRole->GetTypeID() || (*iter)->GetID()!= spMainRole->GetID()) 
			&& (*iter)->GetX() == x && (*iter)->GetY()==y)
		{
			return (*iter);
		}
	}
	
	return NULL;
}

int GameWorld::DebugOut()
{
	pMessageOut->Draw(hdcCanvas, vecDebugMessage, DebugOut_X ,DebugOut_Y);

	return 0;
}

Sprite * GameWorld::CreateSprite(int TypeID)
{
	switch (TypeID)
	{
	case OBJ_Player:
		return new Player();
		break;
	case OBJ_wolf:
		return new wolf();
		break;
	default:
		return NULL;
		break;
	}	
}