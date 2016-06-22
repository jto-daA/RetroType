
/*
** Coccus/Cocci entity types and morphographies....
*/

#ifndef __COCCI_H__
#define __COCCI_H__

#include "common.h"
#include "player.h"
#include "game.h"
#include "map.h"

// assumes a ent pointer... 
#define ENT_X_CENTER(ent) ( ( (ent->w) >> 1) )
#define ENT_Y_CENTER(ent) ( ( (ent->h) >> 1) )

#define BASIC_COCCUS_TIME_TO_MULTIPLY  200
#define BASIC_COCCUS_VELOCITY            5
#define BASIC_COCCUS_RADIUS             20

#define BASIC_COCCUS_PERSONAL_SPACE     50 // thanks again mom!

// cocci forms
#define COCCI_FORMATION_DI               1

#define CC_STARTING       0x00000001
#define CC_SEARCHING      0x00000002
#define CC_RUNNING        0x00000004
#define CC_MULTIPLYING    0x00000008
#define CC_DYING          0x00000010
#define CC_DEAD           0x00000020
#define CC_COLLISION      0x00000040

// animation stuffs.
#define COCCI_ANIMATION_RATE                  40

#define COCCI_FRAME_00                         0
#define COCCI_FRAME_01                         1
#define COCCI_FRAME_02                         2
#define COCCI_FRAME_03                         3
#define COCCI_FRAME_04                         4
#define COCCI_FRAME_05                         5
#define COCCI_FRAME_06                         6
#define COCCI_FRAME_07                         7
#define COCCI_FRAME_08                         8
#define COCCI_FRAME_09                         9
#define COCCI_FRAME_10                        10 
#define COCCI_FRAME_11                        11
#define COCCI_FRAME_12                        12
#define COCCI_FRAME_13                        13

#define COCCI_FRAMES                          14

#define COCCI_RUNNING_SEQUENCE                 0
#define COCCI_RUNNING_SEQUENCE_STEPS          14

#define COCCI_EXPLOSION_SEQUENCE               1
#define COCCI_EXPLOSION_SEQUENCE_STEPS         8

#define COCCI_EXPLOSIONS                      10
#define COCCI_EXPLOSION_LENGTH                10

typedef struct Coccus_t
{
	int timeToMultiply;
	int children;
	int state;
	int virtualPointer;
	
	Coccus_t* child;
	
	Entity_t ent;

	Explosion_t explosions[ COCCI_EXPLOSIONS ];

}*pCoccus_t;

struct Game_t;
typedef struct Game_t* pGame_t;

void Coccus_Spawn( pCoccus_t , int , float , float );
void Coccus_Spawn( pCoccus_t , int , pSpawnPoint_t );

static pCoccus_t Coccus_FindNextChild( pCoccus_t );
void Coccus_Multiply( pCoccus_t );

int Coccus_SearchForFriends( pCoccus_t , pCoccus_t );

// emergent behavior prototypes.
static Vect2_t Coccus_FindCommonCenter( pCoccus_t , pCoccus_t , int );
static int Coccus_CalcAverageVelocity( pCoccus_t , pCoccus_t , int );
static r_boolean Coccus_RegulateProximity( pCoccus_t , pCoccus_t ,  int );

static Vect2_t Coccus_AdjustCenter( pCoccus_t , pVect2_t );
static int Coccus_AdjustVelocity( pCoccus_t , int );

// main routines
r_boolean Coccus_Run( pCoccus_t , pCoccus_t , int );
r_boolean Coccus_Kill( pCoccus_t ); 

void Coccus_Destroy( pCoccus_t , pGame_t );

void Coccus_Draw( pCoccus_t );

// consider moving these to where they are more global
void SetStates( int* , int );
void ClearStates( int* , int );
r_boolean StateSet( int , int );

r_boolean Coccus_LoadFrames( void );

#endif 