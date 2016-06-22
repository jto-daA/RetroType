
#ifndef __BACILLI_H__
#define __BACILLI_H__

#include "map.h"
#include "player.h"

#define MAX_BACILLI_AMPLITUDE             4
#define MAX_BACILLI_PERIOD_FACTOR         8

#define BACILLI_SIZE_X                   64
#define BACILLI_SIZE_Y                   32

#define BASIC_DIPLOBACILLI_SEGMENTS       2
#define BASIC_STREPTOBACILLI_SEGMENTS     5

#define BACILLI_SPEED                     3
#define BACILLI_SPEED_VARIANCE            5

#define DIPLOBACILLI_SPEED               10      
#define STREPTOBACILLI_SPEED              5
  
#define BACILLI_ANIMATION_RATE           30
 
#define BACILLI_FRAME_00                  0
#define BACILLI_FRAME_01                  1
#define BACILLI_FRAME_02                  2
#define BACILLI_FRAME_03                  3
#define BACILLI_FRAME_04                  4

#define BACILLI_FRAME_05                  5
#define BACILLI_FRAME_06                  6
#define BACILLI_FRAME_07                  7
#define BACILLI_FRAME_08                  8
#define BACILLI_FRAME_09                  9

#define BACILLI_FRAME_10                 10
#define BACILLI_FRAME_11                 11
#define BACILLI_FRAME_12                 12
#define BACILLI_FRAME_13                 13
#define BACILLI_FRAME_14                 14
#define BACILLI_FRAME_15                 15
#define BACILLI_FRAME_16                 16
#define BACILLI_FRAME_17                 17

#define BACILLI_FRAMES                   18

#define BACILLI_HEAD_RUNNING_SEQUENCE     0
#define BACILLI_SEGM_RUNNING_SEQUENCE     1
#define BACILLI_EXPLOSION_SEQUENCE         2

#define BACILLI_HEAD_RUNNING_SEQUENCE_STEPS  5
#define BACILLI_SEGM_RUNNING_SEQUENCE_STEPS  5
#define BACILLI_EXPLOSION_SEQUENCE_STEPS     8

#define BACILLI_HEAD_SEGMENT              0
#define BACILLI_SEGMENT_1                 1  
#define BACILLI_SEGMENT_2                 2
#define BACILLI_SEGMENT_3                 3
#define BACILLI_TAIL_SEGMENT              4

#define BACILLI_EXPLOSIONS               10
#define BACILLI_EXPLOSION_LENGTH         10

typedef struct Bacilli_t
{
	Entity_t ent;
	int amplitude;
	int periodFactor;
	int trigIndex;

	Explosion_t explosions[ BACILLI_EXPLOSIONS ];
}*pBacilli_t;

typedef struct DiploBacilli_t
{
	Bacilli_t segments[ BASIC_DIPLOBACILLI_SEGMENTS ];
}*pDiploBacilli_t;

typedef struct StreptoBacilli_t
{
	Bacilli_t segments[ BASIC_STREPTOBACILLI_SEGMENTS ];
}*pStreptoBacilli_t;

// one bacilli
void Bacilli_Spawn( pBacilli_t , int , int  );
void Bacilli_Spawn( pBacilli_t , pSpawnPoint_t );
r_boolean Bacilli_Kill( pBacilli_t );
void Bacilli_Destroy( pBacilli_t );
r_boolean Bacilli_Run( pBacilli_t );
void Bacilli_Draw( pBacilli_t );

// two syncronized bacilli
void DiploBacilli_Spawn( pDiploBacilli_t , int , int );
void DiploBacilli_Spawn( pDiploBacilli_t ,  pSpawnPoint_t );
r_boolean DiploBacilli_Kill( pDiploBacilli_t );
void DiploBacilli_Destroy( pDiploBacilli_t );
r_boolean DiploBacilli_Run( pDiploBacilli_t );
void DiploBacilli_Draw( pDiploBacilli_t );

// five asyncronous bacilli
void StreptoBacilli_Spawn( pStreptoBacilli_t , int , int  );
void StreptoBacilli_Spawn( pStreptoBacilli_t , pSpawnPoint_t );
r_boolean StreptoBacilli_Kill( pStreptoBacilli_t );
void StreptoBacilli_Destroy( pStreptoBacilli_t );
r_boolean StreptoBacilli_Run( pStreptoBacilli_t );
void StreptoBacilli_Draw( pStreptoBacilli_t );

r_boolean Bacilli_LoadFrames( void );

#endif
