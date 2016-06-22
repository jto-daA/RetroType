
#ifndef __KAMIKAZE_H__
#define __KAMIKAZE_H__

#include <allegro.h>

#include "common.h"
#include "player.h"

#define KAMIKAZE_SIZE_X                 40
#define KAMIKAZE_SIZE_Y                 40

/* kami */
#define KAMIKAZE_FRAME_00                0
#define KAMIKAZE_FRAME_01                1
#define KAMIKAZE_FRAME_02                2
/* flames */
#define KAMIKAZE_FLAME_FRAME_00          3
#define KAMIKAZE_FLAME_FRAME_01          4
#define KAMIKAZE_FLAME_FRAME_02          5
/* explosion */
#define KAMIKAZE_EXPLODE_FRAME_00        6
#define KAMIKAZE_EXPLODE_FRAME_01        7
#define KAMIKAZE_EXPLODE_FRAME_02        8
#define KAMIKAZE_EXPLODE_FRAME_03        9
#define KAMIKAZE_EXPLODE_FRAME_04       10
#define KAMIKAZE_EXPLODE_FRAME_05       11
#define KAMIKAZE_EXPLODE_FRAME_06       12
#define KAMIKAZE_EXPLODE_FRAME_07       13

#define KAMIKAZE_FRAMES                 14

#define KAMIKAZE_RUNNING_SEQUENCE        0
#define KAMIKAZE_RUNNING_SEQUENCE_STEPS  4

#define KAMIKAZE_EXPLODE_SEQUENCE        1
#define KAMIKAZE_EXPLODE_SEQUENCE_STEPS  8
 
#define FLAME_FRAME_00                   0

#define FLAME_FRAMES                     1

#define KAMIKAZE_ANIMATION_RATE         30

#define FLAME_OFFSET                    22

#define KAMIKAZE_EXPLOSIONS             10
#define KAMIKAZE_EXPLOSION_LENGTH       10

typedef struct Kamikaze_t
{
	Entity_t ent;

	Explosion_t explosions[ KAMIKAZE_EXPLOSIONS ];

	int virtualPointer;
}*pKamikaze_t;

r_boolean Kamikaze_LoadFrames( void );

void Kamikaze_SpawnRandom( pKamikaze_t );
void Kamikaze_Spawn( pKamikaze_t , int , int , int );
void Kamikaze_Kill( pKamikaze_t );
void Kamikaze_Draw( pKamikaze_t );
r_boolean Kamikaze_Run( pKamikaze_t );

#endif