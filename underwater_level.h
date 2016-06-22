/************************************************************************************
 *  underwater_level.h  
 *  
 *	descp : restraint macros,prototypes for underwater stage
 *
 *  auth  : javery
 *  
 *  path  : C:\Program Files\Microsoft Visual Studio\MyProjects\PrimaryCubes3D\underwater_level.h
 *
 * iDate  : 10/15/06 
 * version:
 ************************************************************************************/

#ifndef __UNDERWATER_LEVEL_H__
#define __UNDERWATER_LEVEL_H__

#include "common.h"

#define LVL_WATER_FISHES             50
#define LVL_WATER_FISHES_MAX_X       100
#define LVL_WATER_FISHES_MAX_Y       200
#define LVL_WATER_FISHES_MAX_SPEED   5 

#define FISH_LEADER_01                0
#define FISH_IDLE_RADIUS             10

#define LVL_WATER_FISH_PERSONAL_SPACE 2 // thx mom

#define FISH_AI_BOUNDED             3

#define FISH_TARGET_TOLERANCE       10

typedef struct Fish_t
{
	Vect3_t position,target;
	Vect3_t vFactor01,vFactor02,vFactor03;
	c_boolean targetReached;	
		
	Vect3_t (*Think)( int , float );
}*pFish_t;

void LVL_UnderWater_LoadFrames( void );
// dynamic water
void LVL_UnderWater_CalcNewWater( void );
void LVL_UnderWater_PaintWater( float );
void LVL_UnderWater_Rain( void );
// fish boids
void LVL_UnderWater_InitFish( int );
void LVL_UnderWater_MoveFish( int );
void LVL_UnderWater_DrawFish( void );

// fish AI
static Vect3_t LVL_UnderWater_FindSharedCenter( int , float );
static Vect3_t LVL_UnderWater_MaintainDistance( int , float );
static Vect3_t LVL_UnderWater_MaintainVelocity( int , float );

static Vect3_t AI_FishThink_MoveToTarget( int , Vect3_t , float );
static Vect3_t AI_FishThink_Restrain( int , float , int , int , int , int );
static Vect3_t AI_FishThink_Idle( int , float ); 

#endif
