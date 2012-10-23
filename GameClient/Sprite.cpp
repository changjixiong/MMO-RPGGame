#include <stdio.h>
#include <time.h>
#include "Sprite.h"

const int DIRCOUNT = 8;
const int MaxFrameOfDie = 9;

const int spriteInfoWidth = 120;
const int spriteInfoHeight = 100;

#define SPRITE_ATTR_VISIBLE        16  // sprite is visible
#define SPRITE_ATTR_CLONE          256 // the sprite is a clone


//there arrays will be inited by the content from config file later.
int man_stand[8][8] = { {0,1,2,3,4,3,2,1,},
						{5,6,7,8,9,8,7,6,},
						{10,11,12,13,14,13,12,11,},
						{15,16,17,18,19,18,17,16,},
						{20,21,22,23,24,23,22,21,},
						{15,16,17,18,19,18,17,16,},
						{10,11,12,13,14,13,12,11,},
						{5,6,7,8,9,8,7,6,},};


int man_walk[8][10] = { {25,26,27,28,29,30,31,32,33,34},
						{35,36,37,38,39,40,41,42,43,44},
						{45,46,47,48,49,50,51,52,53,54},
						{55,56,57,58,59,60,61,62,63,64},
						{65,66,67,68,69,70,71,72,73,74},
						{55,56,57,58,59,60,61,62,63,64},
						{45,46,47,48,49,50,51,52,53,54},
						{35,36,37,38,39,40,41,42,43,44}, 
                      };



int man_Attack[8][10] = {	{75, 76, 77, 78, 79, 80, 81, 82, 83, 84},
							{85, 86, 87, 88, 89, 90, 91, 92, 93, 94},
							{95, 96, 97, 98, 99, 100, 101, 102, 103, 104},
							{105, 106, 107, 108, 109, 110, 111, 112, 113, 114},
							{115, 116, 117, 118, 119, 120, 121, 122, 123, 124},
							{105, 106, 107, 108, 109, 110, 111, 112, 113, 114},
							{95, 96, 97, 98, 99, 100, 101, 102, 103, 104},
							{85, 86, 87, 88, 89, 90, 91, 92, 93, 94},
							};


int man_Die[8][10] = {	{125, 126, 127, 128, 129, 130, 131, 132, 133, 134},
						{135, 136, 137, 138, 139, 140, 141, 142, 143, 144},
						{145, 146, 147, 148, 149, 150, 151, 152, 153, 154},
						{155, 156, 157, 158, 159, 160, 161, 162, 163, 164},
						{165, 166, 167, 168, 169, 170, 171, 172, 173, 174},
						{155, 156, 157, 158, 159, 160, 161, 162, 163, 164},
						{145, 146, 147, 148, 149, 150, 151, 152, 153, 154},
						{135, 136, 137, 138, 139, 140, 141, 142, 143, 144},};
//===================================================================================
int wolf_stand[DIRCOUNT][8] = 
						{	{0,1,2,3,4,3,2,1,},
							{5,6,7,8,9,8,7,6,},
							{10,11,12,13,14,13,12,11,},
							{15,16,17,18,19,18,17,16,},
							{20,21,22,23,24,23,22,21,},
							{15,16,17,18,19,18,17,16,},
							{10,11,12,13,14,13,12,11,},
							{5,6,7,8,9,8,7,6,},};


int wolf_walk[DIRCOUNT][10] = 
						{	 {25,26,27,28,29,30,31,32,33,34},
							{35,36,37,38,39,40,41,42,43,44},
							{45,46,47,48,49,50,51,52,53,54},
							{55,56,57,58,59,60,61,62,63,64},
							{65,66,67,68,69,70,71,72,73,74},
							{55,56,57,58,59,60,61,62,63,64},
							{45,46,47,48,49,50,51,52,53,54},
							{35,36,37,38,39,40,41,42,43,44}, 
						};


int wolf_Attack[DIRCOUNT][10] = 
							{	{75, 76, 77, 78, 79, 80, 81, 82, 83, 84},
								{85, 86, 87, 88, 89, 90, 91, 92, 93, 94},
								{95, 96, 97, 98, 99, 100, 101, 102, 103, 104},
								{105, 106, 107, 108, 109, 110, 111, 112, 113, 114},
								{115, 116, 117, 118, 119, 120, 121, 122, 123, 124},
								{105, 106, 107, 108, 109, 110, 111, 112, 113, 114},
								{95, 96, 97, 98, 99, 100, 101, 102, 103, 104},
								{85, 86, 87, 88, 89, 90, 91, 92, 93, 94},							
							};



int wolf_Die[DIRCOUNT][15] = 

						{	{125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 175,176,177,178,179},
							{135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 180,181,182,183,184},
							{145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 185,186,187,188,189},
							{155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 190,191,192,193,194},
							{165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 195,196,197,198,199},
							{155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 190,191,192,193,194},
							{145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 185,186,187,188,189},
							{135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 180,181,182,183,184},
						};

bool Sprite::resouceLoaded			= false;
int **Sprite::spAnimationsResource	= NULL;
MyBitMap **Sprite::spBitMapResource	= NULL;


int **Player::pAnimations		= NULL;
MyBitMap **Player::pBitMap		= NULL;
bool Player::resourceAttached	= false;


int **wolf::pAnimations		= NULL;
MyBitMap **wolf::pBitMap	= NULL;
bool wolf::resourceAttached	= false;

Sprite::Sprite()
{
	BitMapFrame	= 0;
	animIndex	= 0;

 	pos_x	= -1;
 	pos_y	= -1;	

	action	= STAND;

	Dir		= SOUTH;


	anim_counter	= 0;
	anim_count_max	= 2;

	id=0;
	TypeID = 0;

	sprite_atrr = 0;	

	obj_statu = STATU_normal;
}

Sprite::~Sprite()
{
}


int Sprite::Load_Frame(int indexNo, int nFromPicNo, int nCount, const char * pzFolderPath)
{
	char sztemp[120]={0};
	int x=0,y=0;
	for (int i=0; i<nCount; i++)
	{
		sprintf(sztemp,"%s/C%05d.bmp",pzFolderPath,nFromPicNo+i);	
		spBitMapResource[indexNo + i] = new MyBitMap((const char*)sztemp, true);	
		sprintf(sztemp,"%s/C%05d.txt",pzFolderPath,nFromPicNo+i);
		FILE *fp;
		fp = fopen(sztemp,"r");
		if (fp)
		{
			fscanf(fp,"%d,%d", &x, &y);
			
			spBitMapResource[indexNo + i]->SetOffSet(x,y);
			
			fclose(fp);
			fp=NULL;
		}		
	}
	
	return 0;
}

int Sprite::Load_Animation(int anim_index, int num_frames, int *sequence)
{
	spAnimationsResource[anim_index] = new int[num_frames+1];
	
	for (int i=0; i<num_frames; i++)
	{
		spAnimationsResource[anim_index][i] = sequence[i];
	}
	spAnimationsResource[anim_index][i] = -1;
	
	
	return 0;
}

int Sprite::Load_Resource()
{
	if (resouceLoaded)
	{
		return 0;
	}	

	spAnimationsResource	= new int*[MAX_SPRITE_ANIMATIONS];
	spBitMapResource		= new MyBitMap*[MAX_SPRITE_FRAMES];

	int index=0;
	// set all images to null
	for (index = 0; index<MAX_SPRITE_FRAMES; index++)
		spBitMapResource[index] = NULL;
	
	// set all animations to null
	for (index = 0; index<MAX_SPRITE_ANIMATIONS; index++)
		spAnimationsResource[index] = NULL;


	Load_Frame(0, 0, 25, "./pic/man");	
	Load_Frame(25, 40, 50, "./pic/man");	
	Load_Frame(75, 120, 50, "./pic/man");
	Load_Frame(125, 280, 50, "./pic/man");	

	for (int i=0;i<DIRCOUNT;i++)
	{
		Load_Animation(STAND+i, 8, man_stand[i]);
		Load_Animation(WALK+i, 10, man_walk[i]);
		Load_Animation(ATTACK+i, 10, man_Attack[i]);
		Load_Animation(DIE+i, 10, man_Die[i]);
	}

	Load_Frame(175+0, 400+0, 25, "./pic/animal");	//wolf stand
	Load_Frame(175+25, 400+40, 50, "./pic/animal");	//wolf walk
	Load_Frame(175+75, 400+200, 50, "./pic/animal");//wolf attack
	Load_Frame(175+125, 400+280, 50, "./pic/animal");	//wolf die
	Load_Frame(175+175, 400+360, 25, "./pic/animal");	//wolf rot

	for (i=0;i<DIRCOUNT;i++)
	{
		Load_Animation(32+STAND+i, 8, wolf_stand[i]);
		Load_Animation(32+WALK+i, 10, wolf_walk[i]);
		Load_Animation(32+ATTACK+i, 10, wolf_Attack[i]);
		Load_Animation(32+DIE+i, 15, wolf_Die[i]);
	}

	resouceLoaded = true;
		
	return 0;
}


int Sprite::Init(const char * szData)
{	
	int action;
	int targetID;
	int targetTypeID;
	//typeid, id, statu, action, targetTypeID, targetid, x, y
	sscanf(szData,"[%d,%d,%d,%d,%d,%d,%d,%d]", &TypeID, &id, &obj_statu, &action, &targetTypeID, &targetID, &pos_x, &pos_y);		

	MiniPos_color = MiniPosColor_Role;

	pMessageOut = new MessageOut(spriteInfoWidth, spriteInfoHeight, 13, FW_THIN);

	roleInfoColor = RoleNormal;
	pMessageOut->SetTextColor(roleInfoColor);	


	spAnimations	= getpAnimations();
	spBitMap		= getpBitMap();

	switch (TypeID)
	{
	case OBJ_Player:
		SetMiniPosColor(MiniPosColor_Player);
		break;
	case OBJ_wolf:
		SetMiniPosColor(MiniPosColor_animal);
		break;
	default:
		break;
	}


	return 0;
}

void Sprite::SetMiniPosColor(MiniPosColor color)
{
	MiniPos_color = color;
}


void Sprite::Animate()
{
	if (obj_statu == STATU_dormant)
		return;

	if (++anim_counter>=anim_count_max)
	{	
		BitMapFrame++;
		if (spAnimations[animIndex][BitMapFrame] == -1)
		{
			switch (action)
			{
			case DIE:
				obj_statu = STATU_dormant;
				BitMapFrame =0;
				break;
			default:
				ChangeAction(STAND);
				BitMapFrame = 0;				
				break;
			}
			
		}
		
		if (action == WALK)
		{
			MovePos();
		}	
		
		anim_counter = 0;
 	}

}

void Sprite::Draw(HDC hdcDest)
{
	if (obj_statu == STATU_dormant)
		return;

	if (spBitMap[spAnimations[animIndex][BitMapFrame]])
	{
		if (Dir == NORTHEAST || Dir == EAST || Dir == SOUTHEAST)
		{
			spBitMap[spAnimations[animIndex][BitMapFrame]]->Draw(hdcDest, pos_x - ViewportPos_x, pos_y - ViewportPos_y, true);
		}
		else
		{
			spBitMap[spAnimations[animIndex][BitMapFrame]]->Draw(hdcDest, pos_x - ViewportPos_x, pos_y - ViewportPos_y);
		}

		
		DrawInfo(hdcDest);
	}		
}

void Sprite::DrawInfo(HDC hdcDest)
{
	
	char szRoleInof[32]={0};
	sprintf(szRoleInof,"ID:%d [%d,%d]", id, pos_x, pos_y);	


	pMessageOut->Draw(hdcDest,
					szRoleInof,
					pos_x - 40 - ViewportPos_x,pos_y - 52 - ViewportPos_y);

}

void Sprite::RandDir()
{
	srand((unsigned)time(NULL));
	switch (rand()%8)
	{
	case SOUTH:
		Dir = SOUTH;
		break;
	case SOUTHWEST:
		Dir = SOUTHWEST;
		break;
	case WEST:
		Dir = WEST;
		break;
	case NORTHWEST:
		Dir = NORTHWEST;
		break;
	case NORTH:
		Dir = NORTH;
		break;
	case NORTHEAST:
		Dir = NORTHEAST;
		break;
	case EAST:
		Dir = EAST;
		break;
	case SOUTHEAST:
		Dir = SOUTHEAST;
		break;
	}
}

void Sprite::ChangeDir(int x, int y)
{
	int dx = x - pos_x;
	int dy = y- pos_y;

	if (dx > 0)
	{
		//abs(dy)/dx < stepLen_y/(3*stepLen_x)  =>  abs(dy)*(3*stepLen_x) < stepLen_y*dx
		if (abs(dy)*(3*stepLen_x) < stepLen_y*dx)
		{
			Dir = EAST;
		}
		else
		{
			if (abs(dy)*stepLen_x > 4*stepLen_y*dx)
			{
				if (dy>0)
				{
					Dir = SOUTH;
				}
				else
				{
					Dir = NORTH;
				}				
			}
			else
			{
				if (dy>0)
				{
					Dir = SOUTHEAST;
				}
				else
				{
					Dir = NORTHEAST;
				}
				
			}
		}
	}
	else if (dx < 0)
	{
		if (abs(dy)*(3*stepLen_x) < stepLen_y*abs(dx))
		{
			Dir = WEST;
		}
		else 
		{
			if (abs(dy)*stepLen_x > 4*stepLen_y*abs(dx))
			{
				if (dy > 0)
				{
					Dir = SOUTH;
				}
				else
				{
					Dir = NORTH;
				}
				
			}
			else
			{
				if (dy > 0)
				{
					Dir = SOUTHWEST;
				}
				else
				{
					Dir = NORTHWEST;
				}
			}
			
		}
		
	}
	else
	{
		if (dy > 0)
		{
			Dir = SOUTH;
		}
		else if (dy < 0)
		{
			Dir = NORTH;
		}
	}

	animIndex = action + Dir;
	BitMapFrame		= 0;
	anim_counter	= 0;

}

void Sprite::Move(int x, int y)
{
	if (x != pos_x || y != pos_y)
	{
		ChangeAction(WALK);
		ChangeDir(x, y);		
	}	
}

void Sprite::Attack(int x, int y)
{
	if (x != pos_x || y != pos_y)
	{
		ChangeAction(ATTACK);
		ChangeDir(x, y);		
	}	
}

void Sprite::Die(int x, int y)
{
	ChangeAction(DIE);
}

void Sprite::ChangeAction(Action act)
{	
	action = act;
	animIndex = action + Dir;
	BitMapFrame = 0;
	anim_counter= 0;
	obj_statu = STATU_normal;
}

Action Sprite::GetAction() const
{
	return action;
};

DIR Sprite::GetDir() const
{
	return Dir;
};

void Sprite::MovePos()
{
	switch (Dir)
	{
	case SOUTH:		
	case SOUTHEAST:
	case SOUTHWEST:
		pos_y = (pos_y/stepLen_y)*stepLen_y + (BitMapFrame + 1)*stepLen_y/walkFrames;
	};
	
	switch (Dir)
	{
	case NORTH:		
	case NORTHEAST:
	case NORTHWEST:
		pos_y =  ((pos_y + stepLen_y -1)/stepLen_y)*stepLen_y - (BitMapFrame + 1)*stepLen_y/walkFrames ;
	};
	
	switch (Dir)
	{
	case EAST:
	case NORTHEAST:
	case SOUTHEAST:
		pos_x = (pos_x/stepLen_x)*stepLen_x + (BitMapFrame + 1)*stepLen_x/walkFrames;
	};
	
	switch (Dir)
	{
	case WEST:
	case NORTHWEST:
	case SOUTHWEST:
		pos_x = ((pos_x + stepLen_x -1)/stepLen_x)*stepLen_x -  (BitMapFrame + 1)*stepLen_x/walkFrames ;
	};
	
};

Player::Player()
{
	bitmapFolderPath = "./pic/man";
	bitmapIndexOffset= 0;
	animationIndexoffset = 0;

	if (resourceAttached == false)
	{
		AttachAnimations();
		AttachBitMaps();
		resourceAttached = true;
	}
}

Player::~Player()
{

}

void Player::AttachAnimations()
{
	pAnimations	= new int*[MAX_SPRITE_ANIMATIONS];

	for (int i=0;i<MAX_SPRITE_ANIMATIONS;i++)
	{
		pAnimations[i] = Sprite::spAnimationsResource[i];
	}
}

void Player::AttachBitMaps()
{
	pBitMap		= new MyBitMap*[MAX_SPRITE_FRAMES];
	for (int i=0;i<175;i++)
	{
		pBitMap[i] = Sprite::spBitMapResource[i];
	}
}

wolf::wolf()
{
	bitmapIndexOffset = 175;
	animationIndexoffset = 32;
	if (resourceAttached == false)
	{
		AttachAnimations();
		AttachBitMaps();
		resourceAttached = true;
	}
}

wolf::~wolf()
{

}

void wolf::AttachAnimations()
{
	pAnimations	= new int*[MAX_SPRITE_ANIMATIONS];
	
	for (int i=0;i<MAX_SPRITE_ANIMATIONS;i++)
	{
		pAnimations[i] = Sprite::spAnimationsResource[animationIndexoffset + i];
	}	

}

void wolf::AttachBitMaps()
{
	pBitMap		= new MyBitMap*[MAX_SPRITE_FRAMES];
	for (int i=0;i<(25+50+50+50+25);i++)
	{
		pBitMap[i] = Sprite::spBitMapResource[bitmapIndexOffset + i];
	}
}