#include <stdio.h>
#include "Sprite.h"
#include "GameWorld.h"

const int DIRCOUNT = 8;

//there arrays will be inited by the content from config file later.
int man_anims[8][8] = { {0,1,2,3,4,3,2,1,},
						{5,6,7,8,9,8,7,6,},
						{10,11,12,13,14,13,12,11,},
						{15,16,17,18,19,18,17,16,},
						{20,21,22,23,24,23,22,21,},
						{15,16,17,18,19,18,17,16,},
						{10,11,12,13,14,13,12,11,},
						{5,6,7,8,9,8,7,6,},};
// 						{25,26,27,28,29,28,27,26,},
// 						{30,31,32,33,34,33,32,31,},
// 						{35,36,37,38,39,38,37,36,},};

int man_walk[8][10] = {{40,41,42,43,44,45,46,47,48,49},
						{50,51,52,53,54,55,56,57,58,59},
						{60,61,62,63,64,65,66,67,68,69},
						{70,71,72,73,74,75,76,77,78,79},
						{80,81,82,83,84,85,86,87,88,89},
						{90,91,92,93,94,95,96,97,98,99},
						{100,101,102,103,104,105,106,107,108,109},
						{110,111,112,113,114,115,116,117,118,119},};

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

	pos_x	= GAME_WIDTH/2;
	pos_y	= GAME_HEIGHT/2;

}

Sprite::~Sprite()
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

int Sprite::Init()
{	
	Load_Frame(0, 25);

	for (int i=0;i<DIRCOUNT;i++)
	{
		Load_Animation(i, 8, man_anims[i]);
	}	

	return 0;
}

int Sprite::Load_Animation(int anim_index, int num_frames, int *sequence)
{
	animations[anim_index] = new int[num_frames+1];

	for (int i=0; i<num_frames; i++)
	{
		animations[anim_index][i] = sequence[i];
	}
	animations[anim_index][i] = -1;

	
	return 0;
}

int Sprite::Load_Frame(int nFrom, int nCount)
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

void Sprite::Animate()
{
	BitMapFrame++;
	if (animations[animIndex][BitMapFrame] == -1)
	{
		BitMapFrame	= 0;
	}
}

void Sprite::Draw(HDC hdcDest)
{
	if (pBitMap[animations[animIndex][BitMapFrame]])
	{
		if (Dir == NORTHEAST || Dir == EAST || Dir == SOUTHEAST)
		{
			pBitMap[animations[animIndex][BitMapFrame]]->Draw(hdcDest, pos_x, pos_y, true);
		}
		else
		{
			pBitMap[animations[animIndex][BitMapFrame]]->Draw(hdcDest, pos_x, pos_y);
		}
		
	}	
}

void Sprite::ChangeDir(int x, int y)
{
	if (x>pos_x)
	{
		if (y>pos_y)
		{
			Dir = SOUTHEAST;
		}
		else if (y<pos_y)
		{
			Dir = NORTHEAST;
		}
		else
		{
			Dir = EAST;
		}

	}
	else if (x<pos_x)
	{
		if (y>pos_y)
		{
			Dir = SOUTHWEST;
		}
		else if (y<pos_y)
		{
			Dir = NORTHWEST;
		}
		else
		{
			Dir = WEST;
		}
	}
	else
	{
		if (y>pos_y)
		{
			Dir = SOUTH;
		}
		else if (y<pos_y)
		{
			Dir = NORTH;
		}
	}

	animIndex = Dir;

	BitMapFrame = -1;

}

void Sprite::Move(int x, int y)
{
	ChangeDir(x, y);
}