/************************************************************************************
 *  logic.cpp  
 *  
 *	descp : Game logic
 *
 *  auth  : javery
 *  
 *  path  : C:\Program Files\Microsoft Visual Studio\MyProjects\PrimaryCubes3D\logic.cpp
 *
 * iDate  : 09/18/06 
 * version:
 ************************************************************************************/

#include "common.h"
#include "cube.h"
#include "session.h"
#include "logic.h"
#include "game.h"
#include "gfx_elements.h"

#include <allegro.h>
#include <stdlib.h>
#include <memory.h>

static CubeColor_t colorTable[ GL_COLOR_TABLE_SIZE ];
Cube_t debugGameGrid[ GL_GAME_SIZE_X ][ GL_GAME_SIZE_Y ];
extern Game_t currentGame;
extern BITMAP* cubes[ CUBE_COUNT ];
pGame_t pCurrentGame = &currentGame;

/*
=============================================
GL_GenerateColorTable

Creates random color lookup table.
==============================================
*/
void GL_GenerateColorTable( void )
{
	for( int index = 0;index < GL_COLOR_TABLE_SIZE;index++ )
	{
		switch( rand() % 6 )
		{
		case 0:
			{
				colorTable[ index ] = PRI_RED;
				break;
			}

		case 1:
			{
				colorTable[ index ] = PRI_YELLOW;
				break;
			}

		case 2:
			{
				colorTable[ index ] = PRI_BLUE;
				break;
			}

		case 3:
			{
				colorTable[ index ] = SEC_GREEN;
				break;
			}

		case 4:
			{
				colorTable[ index ] = SEC_ORANGE;
				break;
			}
		case 5:
			{
				colorTable[ index ] = SEC_PURPLE;
				break;
			}
		}
	}
}
/*
===============================================
GL_GenerateCube

Creates a new cube, prepares for entry into game
grid. Displayed in next cube GUI element.
===============================================
*/
void GL_GenerateCube( pCube_t pCube )
{
	pCube->active = c_true;         // note : it is possible to be active ( while stagnant ) without being dead. 
	pCube->alive =  c_true;         // note : false ONLY eliminated as part of a score run.
	pCube->processed = c_false;     
	pCube->frontFlagged = c_false; 
	pCube->rearFlagged = c_false;
	
	// change for rendering puposes.
	pCube->position.xPosition = (float)pCurrentGame->currentSession.xDimen / 2;
	pCube->position.yPosition = 0;
	pCube->position.zPosition = 0;

	// grid coordinates
	pCube->gridPosition.yRow = 0;
	pCube->gridPosition.xCol = pCurrentGame->currentSession.xDimen / 2;
	
	pCube->gridPosition.xMax = pCurrentGame->currentSession.xDimen;
	pCube->gridPosition.yMax = pCurrentGame->currentSession.yDimen;

	// assign pseudo-random color to each face
	pCube->frontFace = colorTable[ rand() % GL_COLOR_TABLE_SIZE ];
	
	switch( pCube->frontFace )
	{
	case PRI_BLUE:
		{
			pCube->rearFace = SEC_ORANGE;
			pCube->cubeSprite[ CUBE_FRONT ].data = cubes[ G_CUBE_BLUE ];
			pCube->cubeSprite[ CUBE_BACK  ].data = cubes[ G_CUBE_ORANGE ];
			break;
		}
	case PRI_RED:
		{ 
			pCube->rearFace = SEC_PURPLE;
			pCube->cubeSprite[ CUBE_FRONT ].data = cubes[ G_CUBE_RED ];
			pCube->cubeSprite[ CUBE_BACK  ].data = cubes[ G_CUBE_PURPLE ];
			break;
		}
	case PRI_YELLOW:
		{
			pCube->rearFace = SEC_GREEN;
			pCube->cubeSprite[ CUBE_FRONT ].data = cubes[ G_CUBE_YELLOW ];
			pCube->cubeSprite[ CUBE_BACK  ].data = cubes[ G_CUBE_GREEN ];
			break;
		}
	case SEC_ORANGE:
		{
			pCube->rearFace = PRI_BLUE;
			pCube->cubeSprite[ CUBE_FRONT ].data = cubes[ G_CUBE_ORANGE ];
			pCube->cubeSprite[ CUBE_BACK ].data  = cubes[ G_CUBE_BLUE ];
			break;
		}
	case SEC_PURPLE:
		{
			pCube->rearFace = PRI_RED;
			pCube->cubeSprite[ CUBE_FRONT ].data = cubes[ G_CUBE_PURPLE ];
			pCube->cubeSprite[ CUBE_BACK ].data  = cubes[ G_CUBE_RED ];
			break;
		}
	case SEC_GREEN:
		{
			pCube->rearFace = PRI_YELLOW;
			pCube->cubeSprite[ CUBE_FRONT ].data = cubes[ G_CUBE_GREEN ];
			pCube->cubeSprite[ CUBE_BACK ].data =  cubes[ G_CUBE_YELLOW ];
			break;
		}
	}
			
	pCube->twoSided = ( pCube->frontFace == pCube->rearFace ) ? c_true : c_false;

	// set all directions to free
	pCube->directions[ GL_DIRECTION_LEFT ] = pCube->directions[ GL_DIRECTION_RIGHT ] = pCube->directions[ GL_DIRECTION_DOWN ] = c_true;

	// sync-sprite
	pCube->cubeSprite[ CUBE_FRONT ].currentPosition.xPosition = pCube->position.xPosition;
	pCube->cubeSprite[ CUBE_FRONT ].currentPosition.yPosition = pCube->position.yPosition;
	pCube->cubeSprite[ CUBE_FRONT ].xDimen = pCube->cubeSprite[ CUBE_FRONT ].data->w;
	pCube->cubeSprite[ CUBE_FRONT ].yDimen = pCube->cubeSprite[ CUBE_FRONT ].data->h;
	
	pCube->cubeSprite[ CUBE_BACK ].currentPosition.xPosition = pCube->position.xPosition;
	pCube->cubeSprite[ CUBE_BACK ].currentPosition.yPosition = pCube->position.yPosition;
	pCube->cubeSprite[ CUBE_BACK ].xDimen = pCube->cubeSprite[ CUBE_BACK ].data->w;
	pCube->cubeSprite[ CUBE_BACK ].yDimen = pCube->cubeSprite[ CUBE_BACK ].data->h;

}
/*
=================================================
GL_AddCube

Adds a generated cube to the current game.
=================================================
*/
void GL_AddCube( pCube_t pCube )
{
	pCube->active = pCube->alive = c_true;
	pCurrentGame->pGameCubes[ pCube->gridPosition.xCol ][ pCube->gridPosition.yRow ] = *pCube;
}
/*
=================================================
GL_DeleteCube
Removes a cube from the game.
=================================================
*/
void GL_DeleteCube( pCube_t pCube )
{
	//pCube->active = pCube->alive = c_false;	
 	memset( &pCurrentGame->pGameCubes[ pCube->gridPosition.xCol ][ pCube->gridPosition.yRow ] , 0 , sizeof(Cube_t) );
}
/*
=================================================
GL_CheckDirections

Checks the cells surround a cube, determines movement
options ( if any ). Returns 'c_true' is further 
movement is possible.
=================================================
*/
c_boolean GL_CheckDirections( pCube_t pCube , Cube_t pGameGrid[ GL_EASY_GAME_SIZE_X ][ GL_EASY_GAME_SIZE_Y ] )
{
	int col = pCube->gridPosition.xCol , 
		row = pCube->gridPosition.yRow , left=0,right=0,down=0;
	
	// SECTION 1 : CUBE IS NO LONGER ABLE TO MOVE ===============================================================
	if( !pCube->directions[ GL_DIRECTION_LEFT ] && !pCube->directions[ GL_DIRECTION_RIGHT ] && !pCube->directions[ GL_DIRECTION_DOWN ] )
	{
		pCube->active = c_false;
		return c_false;
	}
	
	// SECTION 2 : CUBE IN EXTREME ROW or COL ===================================================================
	   // CASE 1 : cube is on the left most wall.
	if( !pCube->gridPosition.xCol ) 
	{
		pCube->directions[ GL_DIRECTION_LEFT ] = c_false;
	}
	else
	{
		pCube->directions[ GL_DIRECTION_LEFT ] = c_true;
		left = col - 1;
	}

	   // CASE 2 : cube is on the right most wall.
	if( pCube->gridPosition.xCol + 1 == pCube->gridPosition.xMax ) 
	{
		pCube->directions[ GL_DIRECTION_RIGHT ] = c_false;
	}
	else
	{
		pCube->directions[ GL_DIRECTION_RIGHT ] = c_true;
		right = col + 1;
	}
	   // CASE 3 : cube is on the bottom wall
	if( pCube->gridPosition.yRow + 1 == pCube->gridPosition.yMax )  
	{
		pCube->directions[ GL_DIRECTION_DOWN ]  = c_false;
		pCube->directions[ GL_DIRECTION_LEFT ]  = c_false;
		pCube->directions[ GL_DIRECTION_RIGHT ] = c_false;
		pCube->active = c_false;                         // consider some brief movement in the bottom row
		return c_false;
	}
	else
	{
		pCube->directions[ GL_DIRECTION_DOWN ] = c_true;
		down = row + 1;
	}

	  // CASE 4 : cube is in top row
/*	if( !pCube->gridPosition.yRow )
	{
		if( pGameGrid[ col ][ row + 1 ].alive ) // cell beneath is occupied... 
		{
			pCube->directions[ GL_DIRECTION_DOWN ] = c_false;
		}
	}
	else
	{
		pCube->directions[ GL_DIRECTION_DOWN ] = c_true;
	}*/


	// SECTION 3 : USING MOVEMENT RESTRICTIONS FROM SEC. 1/2, RESTRICT ACCORDING TO NEIGHBOORING CELLS. ==========
	   // CASE 4 : cell directly below is occupied by a cube.
	if( pGameGrid[ col ][ down ].alive )
	{
		pCube->directions[ GL_DIRECTION_DOWN ] = c_false;
		pCube->directions[ GL_DIRECTION_LEFT ] = c_false;
		pCube->directions[ GL_DIRECTION_RIGHT ] = c_false;
		pCube->active = c_false;
		return c_false;
	}
	else
	{ 
		pCube->directions[ GL_DIRECTION_DOWN ] = c_true;
	}

	   // CASE 5 : cell to the left is occupied.
	if( pGameGrid[ left ][ row ].alive )
	{
		pCube->directions[ GL_DIRECTION_LEFT ] = c_false;
	}
	else
	{
		if( left )
		{
			pCube->directions[ GL_DIRECTION_LEFT ] = c_true;
		}
	}

	   // CASE 6 : cell to the right is occupied.
	if( pGameGrid[ right ][ row ].alive )
	{
		pCube->directions[ GL_DIRECTION_RIGHT ] = c_false;
	}
	else
	{
		if( right + 1 == pCube->gridPosition.xMax )
		{
			pCube->directions[ GL_DIRECTION_RIGHT ] = c_true;
		}
	}

	// if all else fails, just deny it.
	return c_false;
}





