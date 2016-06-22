
#ifndef __BLASTO_H__
#define __BLASTO_H__

#include "common.h"
#include "player.h"
#include "game.h"

// frame-based ( based on a average 60 fps frame rate ).
// therefore , the basic blasto has a 20 sec memory : 
#define BLASTO_MEMORY_SIZE 175 //APPROX_SECONDS(20)  

#define BLASTO_SPEED 3
#define BLASTO_FRAMES 0

#define BLASTO_PROJECTILE_SPEED BLASTO_SPEED<<1
#define BLASTO_PROJECTILE_FRAMES  0 

#define PATTERN_STEP_SIZE  4

#define BLASTO_DOF         4

// pattern ID's
#define TEST_PATTERN 0
#define TEST_PATTERN_SIZE  5


// pattern steps
#define PATTERN_DIRECTION_LEFT     0
#define PATTERN_DIRECTION_RIGHT    1
#define PATTERN_DIRECTION_UP       2
#define PATTERN_DIRECTION_DOWN     3

typedef struct BlastoControls_t
{
	int directions[ BLASTO_DOF ];
}*pBlastoControls_t;

typedef struct Blasto_t
{
	Entity_t ent;
	Entity_t projectile;
	Vect2_t targetMemory[ BLASTO_MEMORY_SIZE ];
	
	int synapse;
	int patternSize,
		step,
		totalSteps;
	
	BlastoControls_t currentControlState;
	BlastoControls_t* currentPattern;
	
	r_boolean pulse;
}*pBlasto_t;

void Blasto_Spawn( pBlasto_t , int , int  );
void Blasto_Spawn( pBlasto_t , pSpawnPoint_t );

r_boolean Blasto_Destroy( pBlasto_t );
void Blasto_Kill( pBlasto_t );

r_boolean Blasto_Run( pBlasto_t , pPlayer_t );
static Vect2_t Blasto_PredictTarget( pBlasto_t , pPlayer_t );
static void Blasto_Shoot( pBlasto_t );
static r_boolean Blasto_ProcessBullet( pBlasto_t , pPlayer_t );
static r_boolean Blasto_ExecutePattern( pBlasto_t );

void Blasto_Draw( pBlasto_t );

#endif