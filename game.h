/************************************************************************************
 *  game.h  
 *  
 *	descp : Frame-based logic/processing. Secondary wrapper to main.
 *
 *  auth  : javery
 *  
 *  path  : C:\Program Files\Microsoft Visual Studio\MyProjects\PrimaryCubes3D\game.h
 *
 * iDate  : 09/17/06 
 * version:
 ************************************************************************************/

#ifndef __GAME_H__
#define __GAME_H__

#include "common.h"
#include "session.h"
#include "cube.h"
#include "logic.h"

#define G_DEBUG_VERSION

#define G_GRID_PADDING_X                  0.15f
#define G_GRID_PADDING_Y                  0.15f
#define G_NORM_GRID_PADDING_X             0.02f
#define G_NORM_GRID_PADDING_Y             0.18f
#define G_HARD_GRID_PADDING_X             G_NORM_GRID_PADDING_X
#define G_HARD_GRID_PADDING_Y			  G_NORM_GRID_PADDING_Y

#define G_GLOBAL_TICK_RATE                   60
#define G_SECOND_TICK_RATE                   60

#define G_PRIMARY_POINTS                    100
#define G_SECONDARY_POINTS                  200

#define G_SMALLEST_LINK                      5

// colors
#define G_COLOR_RED    (makecol( 255 , 0 ,0  ))
#define G_COLOR_GREEN  (makecol( 0 , 205 , 0 ))
#define G_COLOR_BLUE   (makecol( 25 , 25 , 112 ))
#define G_COLOR_BLACK  (makecol( 0 , 0 , 0  ))
#define G_COLOR_WHITE  (makecol( 255,255,255))
#define G_COLOR_YELLOW (makecol( 255 , 255 , 0 ))
#define G_COLOR_PURPLE (makecol( 148 , 0 , 211 ))
#define G_COLOR_ORANGE (makecol( 255 , 140 , 0 ))
#define G_COLOR_STONE  (makecol( 105 , 105 , 105 ))

typedef struct GraphicsConfig_t
{
	int xDimen,yDimen,bitDepth;
}*pGraphicsConfig_t;

typedef struct InputConfig_t
{
	int mouseButtons;

}*pInputConfig;

typedef struct Game_t
{
	GraphicsConfig_t gfxConfig;
	InputConfig_t gameInputProfile;
	Session_t currentSession;
	
	Cube_t* pCubes;	
	Cube_t pGameCubes[10][10]; // debug array
//	Cube_t* pGameCubes;
	Cube_t pEasyGameCubes[ GL_EASY_GAME_SIZE_X ][ GL_EASY_GAME_SIZE_Y ]; // most unfortante... 
	Cube_t pNormGameCubes[ GL_NORM_GAME_SIZE_X ][ GL_NORM_GAME_SIZE_Y ];
	Cube_t pHardGameCubes[ GL_HARD_GAME_SIZE_X ][ GL_HARD_GAME_SIZE_Y ];
	Cube_t currentCube;
	Cube_t nextCube;
	
	c_boolean currentCubeMoving;

// CUSTOM HOOKS
	void (*DrawGrid)( int , int , int );
	void (*DrawCube)( pCube_t );
}*pGame_t;

void G_GameTimer( void );

c_boolean G_Init( pGame_t );
void G_ShutDown( pGame_t );

void G_MovePlayer( pGame_t );
int  G_FindLinks( pCube_t , pGame_t );
int  G_FindRearLinks( pCube_t , pGame_t ,c_boolean , c_boolean , c_boolean , c_boolean );
static int G_LinearSearch( pCube_t , pGame_t , int , int );
int G_FindAllLinks( CubeColor_t , pGame_t , int );
void G_ClearLinks( pGame_t );
int G_RunLogic( pGame_t );
void G_RunGraphics( pGame_t );
void G_DrawCubes( pGame_t );
c_boolean G_TestIfSessionOver( pGame_t );
void G_ResetSession( pGame_t );
void G_StartSession( pGame_t , GameLevel_t ); 
void G_RemoveLinks( pGame_t );
static c_boolean G_ResetCubes( pGame_t );
void G_SyncSprites( pGame_t );
void G_DrawBackGround( pGame_t );
static void G_LoadCubes( void );
void G_Delay( pGame_t );

#endif 