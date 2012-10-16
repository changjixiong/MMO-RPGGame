#include <stdio.h>
#include "Sprite.h"

const int DIRCOUNT = 8;
const int MaxFrameOfDie = 9;

const int spriteInfoWidth = 120;
const int spriteInfoHeight = 100;

//there arrays will be inited by the content from config file later.
int man_stand[8][8] = { {0,1,2,3,4,3,2,1,},
						{5,6,7,8,9,8,7,6,},
						{10,11,12,13,14,13,12,11,},
						{15,16,17,18,19,18,17,16,},
						{20,21,22,23,24,23,22,21,},
						{15,16,17,18,19,18,17,16,},
						{10,11,12,13,14,13,12,11,},
						{5,6,7,8,9,8,7,6,},};


int man_walk[8][10] = {{40,41,42,43,44,45,46,47,48,49},
						{50,51,52,53,54,55,56,57,58,59},
						{60,61,62,63,64,65,66,67,68,69},
						{70,71,72,73,74,75,76,77,78,79},
						{80,81,82,83,84,85,86,87,88,89},
						{70,71,72,73,74,75,76,77,78,79},
						{60,61,62,63,64,65,66,67,68,69},
						{50,51,52,53,54,55,56,57,58,59},};


int man_Attack[8][10] = {{120,121,122,123,124,125,126,127,128,129},
							{130,131,132,133,134,135,136,137,138,139},
							{140,141,142,143,144,145,146,147,148,149},
							{150,151,152,153,154,155,156,157,158,159},
							{160,161,162,163,164,165,166,167,168,169},
							{150,151,152,153,154,155,156,157,158,159},
							{140,141,142,143,144,145,146,147,148,149},
							{130,131,132,133,134,135,136,137,138,139},
							};

int man_Die[8][10] = {{280,281,282,283,284,285,286,287,288,289},
						{290,291,292,293,294,295,296,297,298,299},
						{300,301,302,303,304,305,306,307,308,309},
						{310,311,312,313,314,315,316,317,318,319},
						{320,321,322,323,324,325,326,327,328,329},
						{310,311,312,313,314,315,316,317,318,319},
						{300,301,302,303,304,305,306,307,308,309},
						{290,291,292,293,294,295,296,297,298,299},};
SpriteResource::SpriteResource()
{
	int index=0;
	// set all images to null
	for (index = 0; index<MAX_SPRITE_FRAMES; index++)
		pBitMap[index] = NULL;
	
	// set all animations to null
	for (index = 0; index<MAX_SPRITE_ANIMATIONS; index++)
		animations[index] = NULL;
}

SpriteResource::~SpriteResource()
{
	int i = 0;
	
	for (i=0 ;i<MAX_SPRITE_ANIMATIONS; i++)
	{
		if (animations[i] != NULL)
		{
			delete [] animations[i];
		}
		else
		{
			break;
		}
	}
	
	for (i=0; i<MAX_SPRITE_FRAMES; i++)
	{
		if (pBitMap[i] != NULL)
		{
			delete pBitMap[i];
		}
		else
		{
			break;
		}
	}

}

int SpriteResource::init()
{

	Load_Frame(0, 25);	
	for (int i=0;i<DIRCOUNT;i++)
	{
		Load_Animation(STAND+i, 8, man_stand[i]);
	}
	
	Load_Frame(40, 50);	
	for (i=0;i<DIRCOUNT;i++)
	{
		Load_Animation(WALK+i, 10, man_walk[i]);
	}

	Load_Frame(120, 50);	
	for (i=0;i<DIRCOUNT;i++)
	{
		Load_Animation(ATTACK+i, 10, man_Attack[i]);
	}

	Load_Frame(280, 50);	
	for (i=0;i<DIRCOUNT;i++)
	{
		Load_Animation(DIE+i, 10, man_Die[i]);
	}
	
	return 0;
}

int SpriteResource::Load_Frame(int nFrom, int nCount)
{
	char sztemp[120]={0};
	int x=0,y=0;
	for (int i=nFrom; i<nFrom+nCount; i++)
	{
		sprintf(sztemp,"./pic/man/C%05d.bmp",i);	
		pBitMap[i] = new MyBitMap((const char*)sztemp, true);	
		sprintf(sztemp,"./pic/man/C%05d.txt",i);
		FILE *fp;
		fp = fopen(sztemp,"r");
		if (fp)
		{
			fscanf(fp,"%d,%d", &x, &y);
			
			pBitMap[i]->SetOffSet(x,y);
			
			fclose(fp);
			fp=NULL;
		}		
	}
	
	return 0;
}

int SpriteResource::Load_Animation(int anim_index, int num_frames, int *sequence)
{
	animations[anim_index] = new int[num_frames+1];
	
	for (int i=0; i<num_frames; i++)
	{
		animations[anim_index][i] = sequence[i];
	}
	animations[anim_index][i] = -1;
	
	
	return 0;
}

Sprite::Sprite()
{
	int index=0;
	// set all images to null
	for (index = 0; index<MAX_SPRITE_FRAMES; index++)
		pBitMap[index] = NULL;
	
	// set all animations to null
	for (index = 0; index<MAX_SPRITE_ANIMATIONS; index++)
		animations[index] = NULL;


	BitMapFrame	= -1;
	animIndex	= 0;

 	pos_x	= -1;
 	pos_y	= -1;	

	action	= STAND;
	Dir		= SOUTH;

	anim_counter	= 0;
	anim_count_max	= 2;

	id=0;
}

Sprite::~Sprite()
{
	
}

int Sprite::GetResource(const SpriteResource & spriteResource)
{
	int i=0;
	for (i=0;i<MAX_SPRITE_ANIMATIONS;i++)
	{
		animations[i] = spriteResource.animations[i];
	}

	for (i=0;i<MAX_SPRITE_FRAMES;i++)
	{
		pBitMap[i] = spriteResource.pBitMap[i];
	}

	return 0;
}


int Sprite::Init(const char * szData, const SpriteResource & spriteResource)
{	
	int action;
	int idTarget;
	sscanf(szData,"[%d, %d, %d, %d, %d]", &id, &action, &idTarget, &pos_x, &pos_y);		

	GetResource(spriteResource);

	MiniPos_color = MiniPosColor_Role;

	pMessageOut = new MessageOut(spriteInfoWidth, spriteInfoHeight, 13, FW_THIN);

	roleInfoColor = RoleNormal;
	pMessageOut->SetTextColor(roleInfoColor);
	return 0;
}

void Sprite::SetMiniPosColor(MiniPosColor color)
{
	MiniPos_color = color;
}


void Sprite::Animate()
{
	if (BitMapFrame== -1)
	{
		BitMapFrame = 0;
		++anim_counter;
	}
	else
	{
		if (++anim_counter>=anim_count_max)
		{	
			BitMapFrame++;
			if (animations[animIndex][BitMapFrame] == -1)
			{
				switch (action)
				{
				case DIE:
					BitMapFrame = MaxFrameOfDie;
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
}

void Sprite::Draw(HDC hdcDest)
{

	if (pBitMap[animations[animIndex][BitMapFrame]])
	{
		if (Dir == NORTHEAST || Dir == EAST || Dir == SOUTHEAST)
		{
			pBitMap[animations[animIndex][BitMapFrame]]->Draw(hdcDest, pos_x - ViewportPos_x, pos_y - ViewportPos_y, true);
		}
		else
		{
			pBitMap[animations[animIndex][BitMapFrame]]->Draw(hdcDest, pos_x - ViewportPos_x, pos_y - ViewportPos_y);
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
	BitMapFrame = -1;

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
	BitMapFrame = -1;
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

bool Sprite::NeedRevive()
{ 
	return action == DIE && BitMapFrame == MaxFrameOfDie;
}