
#ifndef __BOSS_THREE_H__
#define __BOSS_THREE_H__

#include "common.h"
#include "player.h"
#include "map.h"
#include "boss_2.h" // for particle defs.

#define BOSS_THREE_FRAMES                                   1
#define BOSS_THREE_SPARKYS                                  2

#define SPARKY_MAX_X_SCALE                                 20
#define SPARKY_MAX_Y_SCALE                                 10
#define SPARKY_MIN_LIFESPAN                                10
#define SPARKY_MAX_LIFESPAN                                10
#define SPARKY_MAX_SPEED                                    4
#define SPARKY_FRAMES                                       1
#define SPARKY_GROUND_LEVEL                               600
#define SPARKY_LAUNCH_POWER                                10
#define SPARKY_LAUNCH_POWER_VARIATION                      10

// [ health stuffs ] 
#define BOSS_THREE_HEALTH                               50000
#define BOSS_THREE_PROJECTILE_STRENGTH                    200
#define BOSS_THREE_STRENGTH                               200
#define BOSS_THREE_DAMAGE_RESISTANCE                      100
      
#define SPARY_HEALTH                                      100
#define SPARKY_STRENGTH                                    50
#define SPARKY_DAMAGE_RESISTANCE                            0


// [ states ] 
#define BOSS_THREE_STATE_ONE                           (1<<0)
  
#define SPARK_STATE_ONE                                (1<<0)

// [ phases ] 
#define BOSS_THREE_INTRO_PHASE                             0
#define BOSS_THREE_PHASE_ONE                               1
#define BOSS_THREE_PHASE_TWO                               2
#define BOSS_THREE_PHASE_THREE                             3
#define BOSS_THREE_PHASE_FOUR                              4 
#define BOSS_THREE_PHASE_FINAL                             5

// [ particles ]
#define SPARKY_PARTICLES                                  20
#define SPARKY_PARTICLE_HISTORY                           10

typedef struct Sparky_t
{
	Entity_t ent;
	
	int state;
	int phase;
	int ttl;
	int direction;
	Particle_t pos;

	r_boolean airborne;
	
	float xScale,yScale;

	union attribs_t
	{
		int phases;
		r_boolean doingSomething;
		void* buf;
		void (*ai_callback)( int , int , ... );
	};

	Particle_t particleHistory[ SPARKY_PARTICLE_HISTORY ][ SPARKY_PARTICLES ];

}*pSparky_t;

typedef struct Boss_3t
{
	Entity_t projectiles[ 10 ];
	Entity_t head;
	Entity_t hand;	

	int state;
	int phase;

	// minnions
	Sparky_t minnions[ BOSS_THREE_SPARKYS ];

	// phase based vars

}*pBoss_3t;

// generic routines
void B3_Spawn( pBoss_3t , pSpawnPoint_t );
r_boolean B3_Kill( pBoss_3t );
void B3_Destroy( pBoss_3t );
r_boolean B3_Run( pBoss_3t , pPlayer_t );

//rendering 
void B3_Draw( pBoss_3t );
void Sparky_Draw( pSparky_t );

// [ phases ]
r_boolean B3_RunIntro( pBoss_3t , pPlayer_t );
r_boolean B3_RunPhaseOne( pBoss_3t , pPlayer_t );
r_boolean B3_RunPhaseTwo( pBoss_3t , pPlayer_t );
r_boolean B3_RunPhaseThree( pBoss_3t , pPlayer_t );
r_boolean B3_RunPhaseFour( pBoss_3t , pPlayer_t );
r_boolean B3_RunPhaseFive( pBoss_3t , pPlayer_t );

r_boolean Sparky_RunPhaseOne( pBoss_3t , int , pPlayer_t );
r_boolean Sparky_RunPhaseTwo( pBoss_3t , int , pPlayer_t );
r_boolean Sparky_RunPhaseThree( pBoss_3t , int , pPlayer_t );
r_boolean Sparky_RunPhaseFour( pBoss_3t , int , pPlayer_t );
r_boolean Sparky_RunPhaseFive( pBoss_3t , int , pPlayer_t );

// [ helpers ]
static void Sparky_Spawn( pSparky_t );
static void Sparky_Launch( pSparky_t , int );
static void Sparky_Propel( pSparky_t );


#endif 
