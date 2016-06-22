/************************************************************************************
 *  game.cpp  
 *  
 *	descp : Frame-based logic/processing,init,shutdown,'big-picture' routines.
 *
 *  auth  : javery
 *  
 *  path  : C:\Program Files\Microsoft Visual Studio\MyProjects\PrimaryCubes3D\game.h
 *
 * iDate  : 09/17/06 
 * version:
 * 
 * TO DO : [ 9/28/06 ] add helper routine to G_FindLinks using GL_GAME_DIRECTION_* macros *FIXED*
 *         [10/06/06 ] fix GL_FindAllLinks so that it works with one call.
 ************************************************************************************/

#include <allegro.h>
#include <memory.h>

#include "gfx_system.h"
#include "gfx_elements.h"
#include "cube.h"
#include "logic.h"
#include "game.h"
#include "common.h"
#include "dev_intro.h"
#include "company_intro.h"
#include "font.h"
#include "underwater_level.h"
#include "gui.h"
#include "audio.h"

extern volatile int globalTickCount;  
extern BITMAP* pageOne;               // primary graphics buffer
extern BITMAP* pageTwo;               // used for dual mode display ( left )
extern BITMAP* pageThree;             // used for dual mode display ( right )
extern Cube_t debugGameGrid[ GL_GAME_SIZE_X ][ GL_GAME_SIZE_Y ];
extern BITMAP* cubes[ CUBE_COUNT ];
extern BITMAP* waterLevel;

volatile int gameTickCount;

int stageSpeedFactor[] = 
{
	20,
	15,
	12,
	10,
	8,
	6,
	5,
	4,
	3,
	2,
};
int stageMaxScore[] =
{
	10000,
	40000,
	70000,
   100000,
   130000,
   160000,
   190000,
   220000,
   300000,
   400000
};
                                    
/*
========================================================================
G_Init

Preform precalcs,libary init,pre-caching etc. Assumes video mode and
'allegro_init' have been called.
========================================================================
*/
c_boolean G_Init( pGame_t pGame )
{
// [ Allegro ]=============================================================
	install_timer();

	install_keyboard();

	pGame->gameInputProfile.mouseButtons = install_mouse();

// [ timing routines ]=====================================================
	LOCK_FUNCTION( GFX_SysTimerFunction );
	LOCK_VARIABLE( globalTickCount );
	install_int_ex( GFX_SYS_TimerFunction , BPS_TO_TIMER( G_GLOBAL_TICK_RATE ) );

	LOCK_FUNCTION( G_GameTimer );
	LOCK_VARIABLE( gameTickCounter );
	install_int_ex( G_GameTimer , BPM_TO_TIMER( G_SECOND_TICK_RATE ) );

	LOCK_FUNCTION( DIN_Animate );
	LOCK_FUNCTION( CIN_RunIntro );
	LOCK_FUNCTION( G_Delay );
// [ session ]==============================================================
	G_StartSession( pGame , LVL_NORM );

	memset( (void**)pGame->pGameCubes, 0 , sizeof(Cube_t)* pGame->currentSession.xDimen * pGame->currentSession.yDimen );

	// Create off-screen buffer(s)	
	  pageOne = create_bitmap( pGame->gfxConfig.xDimen , pGame->gfxConfig.yDimen );   
	  pageTwo = create_bitmap( pGame->gfxConfig.xDimen / 2 , pGame->gfxConfig.yDimen );
	pageThree = create_bitmap( pGame->gfxConfig.xDimen / 2 , pGame->gfxConfig.yDimen );

	// [ resources/content ]================================================
	//load game fonts
	FT_LoadGameFont();
	// load game cubes
	G_LoadCubes();	
	
	// Generate color lookups.
	GL_GenerateColorTable();

	// generate first two cubes
	GL_GenerateCube( &pGame->currentCube );
	GL_GenerateCube( &pGame->nextCube );
	GL_AddCube( &pGame->currentCube );
//=====================================================================

	// run company intro
	//CIN_LoadFrames();
	//CIN_RunIntro();
	//CIN_CleanUp();
	
	// run developer intro
	//DIN_LoadFrames();
	//DIN_Animate( 1 , 1 );	
	//DIN_CleanUp();
// [ stages ]================================================================
	LVL_UnderWater_LoadFrames();
	LVL_UnderWater_InitFish(1);

// [ GUI ]===================================================================
	GUI_ELE_LoadGameGUI();
// [ AUDIO ]=================================================================
	AUD_InitFMOD( 44100 , 32 );
	return c_true;
}
/*
=======================================================================
G_Shutdown

Garbage collection. Deallocate resources. Shutdown APIs. etc.
=======================================================================
*/
void G_ShutDown( pGame_t pGame )
{
	destroy_bitmap( pageOne );
	destroy_bitmap( pageTwo );
	destroy_bitmap( pageThree );
	
	remove_int( GFX_SYS_TimerFunction );
	remove_int( G_GameTimer );

	FT_DeleteGameFont();
	AUD_ShutdownFMOD();
}
/*
=======================================================================
G_MovePlayer

Checks state of devices & reacts accordingly. Called after logic 
routine to determine movement possiblities
=======================================================================
*/ 
void G_MovePlayer( pGame_t pGame ) 
// kill preveious cell before moving on... 
{
	pCube_t pCurrentCube = &pGame->currentCube;

	GL_CheckDirections( pCurrentCube , pGame->pGameCubes );

	if( key[ KEY_RIGHT ] )
	{
		if( pCurrentCube->active && pCurrentCube->directions[ GL_DIRECTION_RIGHT ] )
		{
			GL_DeleteCube( &pGame->pGameCubes[ pCurrentCube->gridPosition.xCol ][ pCurrentCube->gridPosition.yRow ] );
			pCurrentCube->gridPosition.xCol++;
			pGame->pGameCubes[ pCurrentCube->gridPosition.xCol ][ pCurrentCube->gridPosition.yRow ] = *pCurrentCube;
		}
	}

	if( key[ KEY_LEFT ] )
	{
		if( pCurrentCube->active && pCurrentCube->directions[ GL_DIRECTION_LEFT ] )
		{
			GL_DeleteCube( &pGame->pGameCubes[ pCurrentCube->gridPosition.xCol ][ pCurrentCube->gridPosition.yRow ] );
			pCurrentCube->gridPosition.xCol--;
			pGame->pGameCubes[ pCurrentCube->gridPosition.xCol ][ pCurrentCube->gridPosition.yRow ] = *pCurrentCube;
		}
	}

	if( key[ KEY_DOWN ] )
	{
		if( pCurrentCube->active && pCurrentCube->directions[ GL_DIRECTION_DOWN ] )
		{
			GL_DeleteCube( &pGame->pGameCubes[ pCurrentCube->gridPosition.xCol ][ pCurrentCube->gridPosition.yRow ] );
			pCurrentCube->gridPosition.yRow++;
			pGame->pGameCubes[ pCurrentCube->gridPosition.xCol ][ pCurrentCube->gridPosition.yRow ] = *pCurrentCube;
		}
	}

	if( key[ KEY_SPACE ] )
	{
		CubeColor_t swap = pCurrentCube->frontFace;
		pCurrentCube->frontFace = pCurrentCube->rearFace;
		pCurrentCube->rearFace = swap;

		BITMAP* spriteSwap = pCurrentCube->cubeSprite[ CUBE_FRONT ].data;
		pCurrentCube->cubeSprite[ CUBE_FRONT ].data = pCurrentCube->cubeSprite[ CUBE_BACK ].data;
		pCurrentCube->cubeSprite[ CUBE_BACK ].data = spriteSwap;
	}
	
	else
	{
		if( pCurrentCube->directions[ GL_DIRECTION_DOWN ] )
		{
			GL_DeleteCube( &pGame->pGameCubes[ pCurrentCube->gridPosition.xCol ][ pCurrentCube->gridPosition.yRow ] );
			pCurrentCube->gridPosition.yRow++;
			pGame->pGameCubes[ pCurrentCube->gridPosition.xCol ][ pCurrentCube->gridPosition.yRow ] = *pCurrentCube;
		}

		// ... maybe play a sound...
	}

	// update coordinates
	int xCol =  pCurrentCube->gridPosition.xCol , yRow =  pCurrentCube->gridPosition.yMax;
	float   deltaX = pGame->currentSession.deltaX,
		    deltaY = pGame->currentSession.deltaY,
		  xPadding = pGame->currentSession.xPadding,
		  yPadding = pGame->currentSession.yPadding;

	/*pGame->pGameCubes[ xCol ][ yRow ].position.xPosition = (pGame->pGameCubes[ xCol ][ yRow ].gridPosition.xCol * deltaX )+xPadding;
	pGame->pGameCubes[ xCol ][ yRow ].position.yPosition = (pGame->pGameCubes[ xCol ][ yRow ].gridPosition.yRow * deltaY )+yPadding;
	
	pGame->pGameCubes[ xCol ][ yRow ].cubeSprite[ CUBE_FRONT ].currentPosition.xPosition = pGame->pGameCubes[ xCol ][ yRow ].position.xPosition;
	pGame->pGameCubes[ xCol ][ yRow ].cubeSprite[ CUBE_FRONT ].currentPosition.yPosition = pGame->pGameCubes[ xCol ][ yRow ].position.yPosition;

	pGame->pGameCubes[ xCol ][ yRow ].cubeSprite[ CUBE_BACK ].currentPosition.xPosition = pGame->pGameCubes[ xCol ][ yRow ].position.xPosition;
	pGame->pGameCubes[ xCol ][ yRow ].cubeSprite[ CUBE_BACK ].currentPosition.yPosition = pGame->pGameCubes[ xCol ][ yRow ].position.yPosition;*/
}
/*
========================================================================
G_GameTimer

Game interrupt routine. Called every second, used for in-game clock.
========================================================================
*/
void G_GameTimer( void ) 
{
	gameTickCount++;
}
END_OF_FUNCTION( G_GameTimer );
/*
=========================================================================
G_GameStartSession

Loads in default values for a game session.
==========================================================================
*/
void G_StartSession( pGame_t pGame ,GameLevel_t level )
{
	pGame->currentSession.currentGameLevel = level;
	pGame->currentSession.currentStage = 0;
	pGame->currentSession.currentScore = 0;
	pGame->currentSession.highScore = 0;
	pGame->currentSession.paused = c_false; 
	pGame->currentSession.timeToNextLevel = 100; // hard-coded... 
	pGame->currentSession.totalTime = 0;
	pGame->currentSession.linkSize = 0;
	pGame->currentSession.maxLink = 0;
	
	switch( level )
	{
	case LVL_EASY:
		{
			pGame->currentSession.xDimen = GL_EASY_GAME_SIZE_X;
			pGame->currentSession.yDimen = GL_EASY_GAME_SIZE_Y;
			//pGame->pGameCubes = (pCube_t)pEasyGameCubes;
			pGame->pCubes = (pCube_t)pGame->pEasyGameCubes;

			// calculate cell padding 
			pGame->currentSession.xPadding = pGame->gfxConfig.xDimen * G_GRID_PADDING_X;
			pGame->currentSession.yPadding = pGame->gfxConfig.yDimen * G_GRID_PADDING_Y;

			// calculate space in-between each cell
			pGame->currentSession.deltaX = ( (pGame->gfxConfig.xDimen-(pGame->currentSession.xPadding*2)) / (pGame->currentSession.xDimen) ) ;
			pGame->currentSession.deltaY = ( (pGame->gfxConfig.yDimen-(pGame->currentSession.yPadding*2)) / (pGame->currentSession.yDimen) );

			break;
		}

	case LVL_NORM:
		{
			pGame->currentSession.xDimen = GL_EASY_GAME_SIZE_X;   // CHANGE!!!
			pGame->currentSession.yDimen = GL_EASY_GAME_SIZE_Y;

			// calculate cell padding 
			pGame->currentSession.xPadding = pGame->gfxConfig.xDimen * G_NORM_GRID_PADDING_X;
			pGame->currentSession.yPadding = pGame->gfxConfig.yDimen * G_NORM_GRID_PADDING_Y;

			// calculate space in-between each cell
			pGame->currentSession.deltaX = ( ((pGame->gfxConfig.xDimen/2)-(pGame->currentSession.xPadding*2)) / (pGame->currentSession.xDimen) ) ;
			pGame->currentSession.deltaY = ( ( pGame->gfxConfig.yDimen-(pGame->currentSession.yPadding*2)) / (pGame->currentSession.yDimen) );

			break;
		}

	case LVL_HARD:
		{
			pGame->currentSession.xDimen = GL_HARD_GAME_SIZE_X;
			pGame->currentSession.yDimen = GL_HARD_GAME_SIZE_Y;
		
			// calculate cell padding 
			pGame->currentSession.xPadding = pGame->gfxConfig.xDimen * G_HARD_GRID_PADDING_X;
			pGame->currentSession.yPadding = pGame->gfxConfig.yDimen * G_HARD_GRID_PADDING_Y;

			// calculate space in-between each cell
			pGame->currentSession.deltaX = ( ((pGame->gfxConfig.xDimen/2)-(pGame->currentSession.xPadding*2)) / (pGame->currentSession.xDimen) ) ;
			pGame->currentSession.deltaY = ( (pGame->gfxConfig.yDimen-(pGame->currentSession.yPadding*2)) / (pGame->currentSession.yDimen) );

			break;
		}
	}
	
}
/*
===========================================================================
G_FindLinks

Searches through game grid for linked cells. Flags appropiate cells for 
score processing/deletion.
===========================================================================
*/
int G_FindLinks( pCube_t pCube , pGame_t pGame )
// Debug routine. Final version will have 3 seperate routines referenced via callback.
{
	int multiplier=0,left=0,right=0,up=0,down=0,
		row=pCube->gridPosition.yRow,
		col=pCube->gridPosition.xCol,
		i=0;
	
	c_boolean leftValid=c_false,
		     rightValid=c_false,
			    upValid=c_false,
			  downValid=c_false,
			  linkDone=c_false;

	// restrict searching by min/max dimensions of game grid.
	if( !col )
	{
		left = col;
		leftValid = ( pCube->frontFace == pGame->pGameCubes[ left ][ row ].frontFace ) ? c_true : c_false;
	}
	else
	{
		left = col - 1;
		leftValid = ( pCube->frontFace == pGame->pGameCubes[ left ][ row ].frontFace ) ? c_true : c_false;
	}

	if( (col + 1) == pCube->gridPosition.xMax )
	{
		right = col;
		rightValid = ( pCube->frontFace == pGame->pGameCubes[ right ][ row ].frontFace ) ? c_true : c_false;	
	}
	else
	{
		right = col + 1;
		rightValid = ( pCube->frontFace == pGame->pGameCubes[ right ][ row ].frontFace ) ? c_true : c_false;	
	}

	if( !row )
	{
		up = row;
		upValid = ( pCube->frontFace ==  pGame->pGameCubes[ col ][ up ].frontFace ) ? c_true : c_false;
	}
	else
	{
		up = row - 1;
		upValid = ( pCube->frontFace ==  pGame->pGameCubes[ col ][ up ].frontFace ) ? c_true : c_false;
	}

	if( (row + 1) == pCube->gridPosition.yMax )
	{
		down = row;
		downValid = ( pCube->frontFace == pGame->pGameCubes[ col ][ down ].frontFace ) ? c_true : c_false;
	}
	else
	{
		down = row + 1;
		downValid = ( pCube->frontFace == pGame->pGameCubes[ col ][ down ].frontFace ) ? c_true : c_false;
	}

	if( !upValid && !downValid && !leftValid && !rightValid )
	{
		return multiplier;
	}

	if( leftValid ) // a like cube exsists to the left of the current cube.
	{
		G_LinearSearch( pCube , pGame , GL_DIRECTION_LEFT , CUBE_FRONT );		
	}

	if( rightValid ) // a like cubes on in the cell to the right, so seach for more links.
	{
		G_LinearSearch( pCube , pGame , GL_DIRECTION_RIGHT , CUBE_FRONT );
	}

	if( upValid )  // going up... 
	{
		G_LinearSearch( pCube , pGame , GL_DIRECTION_UP , CUBE_FRONT  );
	}

	if( downValid ) // going down...
	{
		G_LinearSearch( pCube , pGame , GL_DIRECTION_DOWN , CUBE_FRONT );
	}

	// Hmmm... for some reason this doesn't pick up all the links on the first pass, so multiple passes are required... 
	/*G_FindAllLinks( pCube->frontFace , pGame , CUBE_FRONT );
	G_FindAllLinks( pCube->frontFace , pGame , CUBE_FRONT );
	G_FindAllLinks( pCube->frontFace , pGame , CUBE_FRONT );
	G_FindAllLinks( pCube->frontFace , pGame , CUBE_FRONT );
	G_FindAllLinks( pCube->frontFace , pGame , CUBE_FRONT );*/

	// stuff to handle link values
	multiplier = G_FindAllLinks( pCube->frontFace , pGame , CUBE_FRONT );
	// add the number of linked cubes on the rear side
	multiplier += G_FindRearLinks( pCube , pGame , leftValid , rightValid , downValid , upValid  );

	// update session variables
	pGame->currentSession.linkSize = multiplier;
	pGame->currentSession.maxLink = ( multiplier > pGame->currentSession.maxLink ) ? multiplier : pGame->currentSession.maxLink;

	// calculate/update game score.
	pGame->currentSession.currentScore += multiplier * G_PRIMARY_POINTS;
	
	textprintf_ex( pageOne , font , 5 , 420 ,  G_COLOR_WHITE , -1 , "col=%d  i=%d" , col , i  );
	textprintf_ex( screen , font , 150 ,420 , G_COLOR_RED , -1 , "%d cubes linked to active cube." , multiplier );

	return multiplier;	
}
/*
============================================================================
G_FindRearLinks

Same as G_FindLinks, but limited to rear game grid. Used 4 movement restricting
booleans passed from G_FindLinks
============================================================================
*/
int G_FindRearLinks( pCube_t pCube , pGame_t pGame , c_boolean left , c_boolean right , c_boolean down , c_boolean up )
{
	int multiplier = 0;

	if( !left && !right && !up && !down )
	{ 
		return multiplier;
	}
	
	if( left )
	{
		G_LinearSearch( pCube , pGame , GL_DIRECTION_LEFT , CUBE_BACK );
	}

	if( right )
	{
		G_LinearSearch( pCube , pGame , GL_DIRECTION_RIGHT , CUBE_BACK );

	}

	if( up )
	{
		G_LinearSearch( pCube , pGame , GL_DIRECTION_UP , CUBE_BACK );
	}

	if( down )
	{
		G_LinearSearch( pCube , pGame , GL_DIRECTION_DOWN , CUBE_BACK );
	}

	// this really needs to go... 
	/*G_FindAllLinks( pCube->rearFace , pGame , CUBE_BACK );
	G_FindAllLinks( pCube->rearFace , pGame , CUBE_BACK );
	G_FindAllLinks( pCube->rearFace , pGame , CUBE_BACK );
	G_FindAllLinks( pCube->rearFace , pGame , CUBE_BACK );*/

	multiplier = G_FindAllLinks( pCube->rearFace , pGame , CUBE_BACK );

	return multiplier;
}
/*
============================================================================
G_RunLogic

Main logic routine. Determines state of current cube, acts accordingly.
============================================================================
*/
int G_RunLogic( pGame_t pGame )
{	
	int link=0;
	
	// move player
	G_MovePlayer( pGame );
	G_SyncSprites( pGame );

	// once cube stops moving, run scoring routines
	if( pGame->currentCube.active == c_false )
	{
		G_ClearLinks( pGame );                        // clear flags from previous pass
		link = G_FindLinks( &pGame->currentCube , pGame );   // search and destory!
		
		if( pGame->currentSession.linkSize > G_SMALLEST_LINK )
		{
			G_RemoveLinks( pGame );                   // remove cubes
			G_ResetCubes( pGame );                    // push cubes into new position if necessary.
		}

		pGame->currentCube = pGame->nextCube;

		GL_GenerateCube( &pGame->nextCube );
		GL_AddCube( &pGame->currentCube );
	}	

	// check to see if session is over
	G_TestIfSessionOver( pGame );
	
	if( pGame->currentSession.sessionOver )
	{
		// Do more stuff here later... 
		G_ResetSession( pGame );
	}

	// advance to next stage,if applicable
	if( pGame->currentSession.currentScore > stageMaxScore[ pGame->currentSession.currentStage ] )
	{
		pGame->currentSession.currentStage++;
	}

	return link;
}
/*
============================================================================
G_RunGraphics

Draws a frame as dictated by the current level/stage etc.
============================================================================
*/
void G_RunGraphics( pGame_t pGame )
{
	// ripped from main
	clear_to_color( pageOne , G_COLOR_BLACK );
	clear_to_color( pageTwo , G_COLOR_WHITE );
	clear_to_color( pageThree , G_COLOR_BLACK );
		
	G_DrawBackGround( pGame );
		
	LVL_UnderWater_DrawFish();				
	LVL_UnderWater_CalcNewWater();
	LVL_UnderWater_Rain();

	LVL_UnderWater_PaintWater(0.0f );
	LVL_UnderWater_DrawFish();				
	

	G_DrawCubes( pGame );
		
	GUI_ELE_DrawGameGUI( pGame->currentSession.currentGameLevel );
	
	blit( pageTwo ,   pageOne , 0 , 0 , 0 ,   0 , pGame->gfxConfig.xDimen/2 , pGame->gfxConfig.yDimen );
	blit( pageThree , pageOne , 0 , 0 , 320 , 0 , pGame->gfxConfig.xDimen/2 , pGame->gfxConfig.yDimen );
	blit( pageOne , screen , 0 , 0 , 0 , 0 , pGame->gfxConfig.xDimen , pGame->gfxConfig.yDimen );
}

/*
============================================================================
G_DrawCubes

Loops through game grid and draws all active cubes.
============================================================================
*/
void G_DrawCubes( pGame_t pGame )
{
	for( int yRow=0; yRow<pGame->currentSession.yDimen;yRow++ )
	{
		for( int xCol=0; xCol<pGame->currentSession.xDimen;xCol++ )
		{
			if( pGame->pGameCubes[ xCol ][ yRow ].alive )
			{
				//GFX_ELE_DrawCube( pGame , &pGame->pGameCubes[ xCol ][ yRow ] );
				GFX_ELE_DrawFullCube( pGame , &pGame->pGameCubes[ xCol ][ yRow ] );
				GFX_ELE_DrawCubeSprite( pGame , &pGame->pGameCubes[ xCol ][ yRow ] );
			}

			else
			{
				
			}
		}
	}
}
/*
============================================================================
G_ClearLinks

Loops through and clears all flagged cells. 
============================================================================
*/
void G_ClearLinks( pGame_t pGame )
{
	for( int y=0;y<pGame->currentSession.yDimen;y++ )
	{
		for( int x=0;x<pGame->currentSession.xDimen;x++ )
		{
			if( pGame->pGameCubes[ x ][ y ].frontFlagged )
			{
				pGame->pGameCubes[ x ][ y ].frontFlagged = c_false;
			}

			if( pGame->pGameCubes[ x ][ y ].rearFlagged )
			{
				pGame->pGameCubes[ x ][ y ].rearFlagged = c_false;
			}
		}
	}
}
/*
===========================================================================
G_LinearSearch

Searches in a linear, specified direction from the passed cube, and returns
the number of like-colored faces.
===========================================================================
*/
static int G_LinearSearch( pCube_t pCube , pGame_t pGame , int direction , int side )
{
	int multiplier=0;
	int col = pCube->gridPosition.xCol,
		row = pCube->gridPosition.yRow;
	int x=0,y=0;
	c_boolean linkDone = c_false;

	switch( direction )
	{
	case GL_DIRECTION_LEFT:
		{
			x = -1;
			y = 0;
			break;
		}
	case GL_DIRECTION_RIGHT:
		{
			x = 1;
			y = 0;
			break;
		}
	case GL_DIRECTION_UP:
		{
			x = 0;
			y = -1;
			break;
		}
	case GL_DIRECTION_DOWN:
		{
			x = 0;
			y = 1;
			break;
		}
	}

	switch( side)
	{
	case CUBE_FRONT:
		{
			while( !linkDone )
			{
				if( pCube->frontFace == pGame->pGameCubes[ col + x ][ row + y ].frontFace && pGame->pGameCubes[ col ][ row ].alive )
				{
					pGame->pGameCubes[ col + x ][ row + y ].frontFlagged = c_true;
					multiplier++;
				}
				else
				{
					linkDone = c_true;
				}

				switch( direction )
				{
				case GL_DIRECTION_LEFT:
					{
						x--;
						break;
					}
				case GL_DIRECTION_RIGHT:
					{
						x++;
					    break;
					}
				case GL_DIRECTION_UP:
					{
						y--;
						break;
					}
				case GL_DIRECTION_DOWN:
					{
						y++;
						break;
					}
				}
			}
		}

		case CUBE_BACK:
		{
			while( !linkDone )
			{
				if( pCube->rearFace == pGame->pGameCubes[ col + x ][ row + y ].rearFace && pGame->pGameCubes[ col ][ row ].alive )
				{
					pGame->pGameCubes[ col + x ][ row + y ].rearFlagged = c_true;
					multiplier++;
				}
				else
				{
					linkDone = c_true;
				}

				switch( direction )
				{
				case GL_DIRECTION_LEFT:
					{
						x--;
						break;
					}
				case GL_DIRECTION_RIGHT:
					{
						x++;
					    break;
					}
				case GL_DIRECTION_UP:
					{
						y--;
						break;
					}
				case GL_DIRECTION_DOWN:
					{
						y++;
						break;
					}
				}
			}
		}
	}

	return multiplier;
}
/*
=================================================================================
G_FindAllLinks

Searches through game grid for faces of the passed color. Flags faces which are 
connected to current link.
=================================================================================
*/
int G_FindAllLinks( CubeColor_t color , pGame_t pGame , int side )
{
	int multiplier=0;
	int xCol=0,yRow=0;
	c_boolean flagged = c_false;

	// loop through and find all links.
	for( yRow = 0;yRow < pGame->currentSession.yDimen;yRow++ )
	{
		for( xCol = 0; xCol < pGame->currentSession.xDimen;xCol++ )
		{
			flagged = ( side == CUBE_FRONT ) ? ( pGame->pGameCubes[ xCol ][ yRow ].frontFlagged ) : ( pGame->pGameCubes[ xCol ][ yRow ].rearFlagged );
			
			if( flagged )
			{
				G_LinearSearch( &pGame->pGameCubes[ xCol ][ yRow ] , pGame , GL_DIRECTION_DOWN ,  side );
				G_LinearSearch( &pGame->pGameCubes[ xCol ][ yRow ] , pGame , GL_DIRECTION_UP ,    side );
				G_LinearSearch( &pGame->pGameCubes[ xCol ][ yRow ] , pGame , GL_DIRECTION_LEFT ,  side );
				G_LinearSearch( &pGame->pGameCubes[ xCol ][ yRow ] , pGame , GL_DIRECTION_RIGHT , side );				
			}
		}
	}

	// calculate and return number of linked cubes after completely finished searching.
	for( yRow=0;yRow<pGame->currentSession.yDimen;yRow++ )
	{
		for( xCol=0;xCol<pGame->currentSession.xDimen;xCol++ )
		{
			flagged = ( side == CUBE_FRONT ) ? ( pGame->pGameCubes[ xCol ][ yRow ].frontFlagged ) : ( pGame->pGameCubes[ xCol ][ yRow ].rearFlagged );
				
			if( flagged )
			{
				multiplier++;
			}
		}
	}

	if( multiplier < G_SMALLEST_LINK )
	{
		G_ClearLinks( pGame );
	}

	return multiplier;
}
/*
====================================================================================
G_TestIfSessionOver

Evaluates movement possiblities/position of current cube, and determines if the current
session is able to be played further.
====================================================================================
*/
c_boolean G_TestIfSessionOver( pGame_t pGame )
{
	if( !pGame->currentCube.gridPosition.yRow && (pGame->currentCube.gridPosition.xCol == pGame->currentSession.xDimen/2)) // cube is top row
	{
		if( !pGame->currentCube.directions[ GL_DIRECTION_DOWN ] || pGame->pGameCubes[ pGame->currentCube.gridPosition.xCol ][ pGame->currentCube.gridPosition.yRow + 1 ].alive ) // cube can't go down, so session over.
		{
			pGame->currentSession.sessionOver = c_true;
			return c_true;
		}
	}
	return c_false;
}
/*
====================================================================================
G_ResetSession

Resets the current session.
====================================================================================
*/
void G_ResetSession( pGame_t pGame )
{
	memset( (void**)pGame->pGameCubes, 0 , sizeof(Cube_t)* pGame->currentSession.xDimen * pGame->currentSession.yDimen );

	pGame->currentSession.currentScore = 1;
	pGame->currentSession.currentScore = 0;
	pGame->currentSession.linkSize = 0;
	pGame->currentSession.maxLink = 0;
	pGame->currentSession.paused = c_false;
	pGame->currentSession.currentStage = 0;
	
	// generate first two cubes
	GL_GenerateCube( &pGame->currentCube );
	GL_GenerateCube( &pGame->nextCube );
	GL_AddCube( &pGame->currentCube );

	pGame->currentSession.sessionOver = c_false;
}
/*
======================================================================================
G_RemoveLinks

Searches through game grid, removes all flagged faces.
=======================================================================================
*/
void G_RemoveLinks( pGame_t pGame )
{
	for( int yRow = 0;yRow < pGame->currentSession.yDimen;yRow++ )
	{
		for( int xCol = 0;xCol < pGame->currentSession.xDimen;xCol++ )
		{
			// evaluate the front side of the cube
			if( pGame->pGameCubes[ xCol ][ yRow ].frontFlagged )
			{
				if( pGame->pGameCubes[ xCol ][ yRow ].rearFlagged )
				{
					// if both sides are part of a link, delete
					GL_DeleteCube( &pGame->pGameCubes[ xCol ][ yRow ] ); 
					continue;
				}
				if( (pGame->pGameCubes[ xCol ][ yRow ].frontFace == pGame->pGameCubes[ xCol ][ yRow ].rearFace) && pGame->pGameCubes[ xCol ][ yRow ].frontFace == ALT_STONE )
				{
					// if both sides have been erased ( seperately or otherwise ), then delete it.
					GL_DeleteCube( &pGame->pGameCubes[ xCol ][ yRow ] );
					continue;
				}
				else
				{
					pGame->pGameCubes[ xCol ][ yRow ].frontFace = ALT_STONE;
					pGame->pGameCubes[ xCol ][ yRow ].cubeSprite[ CUBE_FRONT ].data = cubes[ G_CUBE_STONE ];
				}
			}
			
			// then the back... 
			else if( pGame->pGameCubes[ xCol ][ yRow ].rearFlagged )
			{
				if( pGame->pGameCubes[ xCol ][ yRow ].frontFlagged )
				{
					// delete if both sides have been flagged for deletion.
					GL_DeleteCube( &pGame->pGameCubes[ xCol ][ yRow ] );
					continue;
				}
				if( (pGame->pGameCubes[ xCol ][ yRow ].rearFace == pGame->pGameCubes[ xCol ][ yRow ].rearFace) && pGame->pGameCubes[ xCol ][ yRow ].rearFace == ALT_STONE )
				{
					// delete is both sides have been turned to stone.
					GL_DeleteCube( &pGame->pGameCubes[ xCol ][ yRow ] );
					continue;
				}
				else
				{
					pGame->pGameCubes[ xCol ][ yRow ].rearFace = ALT_STONE;
					pGame->pGameCubes[ xCol ][ yRow ].cubeSprite[ CUBE_BACK ].data = cubes[ G_CUBE_STONE ];

				}
					
			}
			
			// remove stones if both side are stoned...
			if( (pGame->pGameCubes[ xCol ][ yRow ].frontFace == pGame->pGameCubes[ xCol ][ yRow ].rearFace ) && pGame->pGameCubes[ xCol ][ yRow ].frontFace == ALT_STONE )
			{
				GL_DeleteCube( &pGame->pGameCubes[ xCol ][ yRow ] );
			}
		}
	}
}
/*
====================================================================================
G_ResetCubes

After links have been processed, called to 'push' remaining cubes down into thier 
new position. Returns c_true to recommend that game grid be recalculated.
====================================================================================
*/
static c_boolean G_ResetCubes( pGame_t pGame )
{
	int xDimen = pGame->currentSession.xDimen,
		yDimen = pGame->currentSession.yDimen;

	c_boolean recalculate = c_false;           

	for( int xCol = 0;xCol < xDimen;xCol++ )
	{
		for( int yRow = yDimen-2;yRow > 0;yRow-- )
		// start at the bottom and work up. ( excluding very bottom row )
		{
			if( pGame->pGameCubes[ xCol ][ yRow ].alive )
			{
				if( pGame->pGameCubes[ xCol ][ yRow + 1 ].alive )
				//  cube is supported.
				{
					continue;
				}
				else if( !pGame->pGameCubes[ xCol ][ yRow + 1 ].alive )
				// cube is floating... determine how many empty cells are beneath it.
				{
					int push = 1;

					while( !pGame->pGameCubes[ xCol ][ yRow + push ].alive && yRow + push < yDimen - 1  )
					// search for either another cube, or the bottom of the grid ( whichever comes first )
					{
						push++;
					}

					if( pGame->pGameCubes[ xCol ][ yRow + push ].alive ) 
					// only one cube per cell!!!!
					{
						pGame->pGameCubes[ xCol ][ yRow ].gridPosition.yRow += push-1; //  give it a little push
						pGame->pGameCubes[ xCol ][ yRow + push-1  ] = pGame->pGameCubes[ xCol ][ yRow ];
					}
					else
					{
						pGame->pGameCubes[ xCol ][ yRow ].gridPosition.yRow += push; //  give it a little push
						pGame->pGameCubes[ xCol ][ yRow + push ] = pGame->pGameCubes[ xCol ][ yRow ];						
					}

					pGame->pGameCubes[ xCol ][ yRow ].alive = c_false;
					recalculate = c_true;					
				}
			}
		}
	}	

	return recalculate;
}
/*
=====================================================================
G_LoadCubes

Loads cube bitmaps
=====================================================================
*/
static void G_LoadCubes( void )
{
	cubes[ G_CUBE_GREEN  ] = load_bitmap( "content/gfx/cubes_0000_green.bmp" , NULL );
	cubes[ G_CUBE_BLUE ]   = load_bitmap( "content/gfx/cubes_0001_blue.bmp" , NULL );
	cubes[ G_CUBE_RED  ]   = load_bitmap( "content/gfx/cubes_0002_red.bmp" , NULL );
	cubes[ G_CUBE_YELLOW ] = load_bitmap( "content/gfx/cubes_0003_yellow.bmp" , NULL );
	cubes[ G_CUBE_ORANGE ] = load_bitmap( "content/gfx/cubes_0004_orange.bmp" , NULL );
	cubes[ G_CUBE_PURPLE ] = load_bitmap( "content/gfx/cubes_0005_purple.bmp" , NULL );	
	cubes[ G_CUBE_STONE ]  = load_bitmap( "content/gfx/cubes_0006_stone.bmp" , NULL );
}
/*
====================================================================
G_SyncSprites

Updates sprite info with lastest data.
====================================================================
*/
void G_SyncSprites( pGame_t pGame )
{
	pCube_t pCube = &pGame->currentCube;
	
	float   deltaX = pGame->currentSession.deltaX,
		    deltaY = pGame->currentSession.deltaY,
		  xPadding = pGame->currentSession.xPadding,
		  yPadding = pGame->currentSession.yPadding;

	float xInit = ( pCube->gridPosition.xCol * deltaX + xPadding ) ,
		  yInit = ( pCube->gridPosition.yRow * deltaY + yPadding ) ,
		  xTerm = ( pCube->gridPosition.xCol * deltaX + xPadding ) + deltaX, 
		  yTerm = ( pCube->gridPosition.yRow * deltaY + yPadding ) + deltaY;
	
	// sync cube sprites
	for( int yRow=0;yRow < pGame->currentSession.yDimen;yRow++ )
	{
		for( int xCol=0;xCol < pGame->currentSession.xDimen;xCol++ )
		{
			pGame->pGameCubes[ xCol ][ yRow ].cubeSprite[ CUBE_FRONT ].currentPosition.xPosition = xInit;
			pGame->pGameCubes[ xCol ][ yRow ].cubeSprite[ CUBE_FRONT ].currentPosition.yPosition = yInit;

			pGame->pGameCubes[ xCol ][ yRow ].cubeSprite[ CUBE_BACK ].currentPosition.xPosition = xInit;
			pGame->pGameCubes[ xCol ][ yRow ].cubeSprite[ CUBE_BACK ].currentPosition.xPosition = yInit;
		}
	}
}
/*
====================================================================
G_DrawBackGround

Draws the current stage background.
====================================================================
*/
void G_DrawBackGround( pGame_t pGame )
{
	blit( waterLevel , pageTwo   , 0 , 0 , 0 , 0 , 320 , 480 );
	blit( waterLevel , pageThree , 0 , 0 , 0 , 0 , 320 , 480 );
}
/*
===================================================================
G_Delay

Creates a delay between the logic/rendering code. Used to progressively
speed-up game, as user advances through game.
===================================================================
*/
void G_Delay( pGame_t pGame )
{
	int delay = stageSpeedFactor[ pGame->currentSession.currentStage ];

	globalTickCount = 0;

	while( globalTickCount < delay )
	{
		// do graphics here. over-n-over again.		
		G_RunGraphics( pGame );
	}
}