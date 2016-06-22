
#ifndef __BOSS_TWO_H__
#define __BOSS_TWO_H__

#include "common.h"
#include "player.h"
#include "map.h"

// main stuff
#define BOSS_TWO_PROJECTILES                             5
#define BOSS_TWO_PARTICLES                            1000

#define BOSS_TWO_SEGMENTS                               14
#define BOSS_TWO_HEAD_SEGMENT                            0
#define BOSS_TWO_TORSO_SEGMENT                           1
#define BOSS_TWO_TAIL_SEGMENT    ( BOSS_TWO_SEGMENTS - 1 )

#define BOSS_TWO_SPEED                                   8
#define BOSS_TWO_RETICULE_SPEED                          2

#define BOSS_TWO_AIMING_TOLERANCE                       10
 
#define BOSS_TWO_PHASE_ONE_PLANE                       200

#define BOSS_TWO_SEGMENT_EXPLOSIONS                     20
#define BOSS_TWO_EXPLOSION_LENGTH                        5

// sizes 

#define BOSS_TWO_SIZE_X                                 80
#define BOSS_TWO_SIZE_Y                                 80

#define BOSS_TWO_RETICULE_SIZE_X                        50
#define BOSS_TWO_RETICULE_SIZE_Y                        50

#define BOSS_TWO_PROJECTILE_SIZE_X                      40
#define BOSS_TWO_PROJECTILE_SIZE_Y                      40 

#define BOSS_TWO_EXPLOSION_SIZE_X                       20
#define BOSS_TWO_EXPLOSION_SIZE_Y                       20

// animation stuff.
#define BOSS_TWO_ANIMATION_RATE                         30
#define BOSS_TWO_RETICULE_ANIMATION_RATE                30

#define BOSS_TWO_FRAME_00  /* segment */                 0
#define BOSS_TWO_FRAME_01  /* torso */                   1
#define BOSS_TWO_FRAME_02  /* head */                    2
#define BOSS_TWO_FRAME_03  /* reticule_01 */             3
#define BOSS_TWO_FRAME_04                                4
#define BOSS_TWO_FRAME_05                                5
#define BOSS_TWO_FRAME_06                                6
#define BOSS_TWO_FRAME_07                                7
#define BOSS_TWO_FRAME_08                                8
#define BOSS_TWO_FRAME_09                                9
#define BOSS_TWO_FRAME_10  /* reticule_02 */            10
#define BOSS_TWO_FRAME_11                               11
#define BOSS_TWO_FRAME_12                               12
#define BOSS_TWO_FRAME_13                               13
#define BOSS_TWO_FRAME_14                               14
#define BOSS_TWO_FRAME_15  /* projectile (front) */     15
#define BOSS_TWO_FRAME_16  /* projectile (side) */      16
#define BOSS_TWO_FRAME_17  /* explosion */              17
#define BOSS_TWO_FRAME_18                               18
#define BOSS_TWO_FRAME_19                               19
#define BOSS_TWO_FRAME_20                               20
#define BOSS_TWO_FRAME_21                               21	
#define BOSS_TWO_FRAME_22                               22
#define BOSS_TWO_FRAME_23                               23
#define BOSS_TWO_FRAME_24                               24
#define BOSS_TWO_FRAMES                                 25

#define BOSS_TWO_HEAD_SEQUENCE_01                        0
#define BOSS_TWO_HEAD_SEQUENCE_01_STEPS                  2

#define BOSS_TWO_TORSO_SEQUENCE_01                       1
#define BOSS_TWO_TORSO_SEQUENCE_01_STEPS                 2

#define BOSS_TWO_SEGMENT_SEQUENCE_01                     2
#define BOSS_TWO_SEGMENT_SEQUENCE_01_STEPS               2

#define BOSS_TWO_RETICULE_SEQUENCE_01                    3
#define BOSS_TWO_RETICULE_SEQUENCE_01_STEPS             16

#define BOSS_TWO_RETICULE_SEQUENCE_02                    4
#define BOSS_TWO_RETICULE_SEQUENCE_02_STEPS             10

#define BOSS_TWO_PROJECTILE_PROFILE_SEQUENCE             5
#define BOSS_TWO_PROJECTILE_PROFILE_SEQUENCE_STEPS       2

#define BOSS_TWO_PROJECTILE_FRONT_SEQUENCE               6
#define BOSS_TWO_PROJECTILE_FRONT_SEQUENCE_STEPS         2

#define BOSS_TWO_EXPLOSION_SEQUENCE                      7
#define BOSS_TWO_EXPLOSION_SEQUENCE_STEPS                8

// health
#define BOSS_TWO_HEALTH                                100
#define BOSS_TWO_STRENGTH                               10
#define BOSS_TWO_PROJECTILE_STRENGTH                    50
#define BOSS_TWO_DAMAGE_RESISTANCE                     100

// phases 
#define BOSS_TWO_PHASE_ONE                               0
#define BOSS_TWO_PHASE_TWO                               1  
#define BOSS_TWO_PHASE_THREE                             2
#define BOSS_TWO_BG_TRANSITION                           3
#define BOSS_TWO_FG_TRANSITION                           4
#define BOSS_TWO_PHASES                                  5

#define BOSS_TWO_PHASE_ONE_LENGTH                       20
#define BOSS_TWO_PHASE_TWO_LENGTH                       20
#define BOSS_TWO_PHASE_THREE_LENGTH                     20
#define BOSS_TWO_BG_TRANSITION_LENGTH                   10
#define BOSS_TWO_FG_TRANSITION_LENGTH                   10

// states
#define BOSS_TWO_TRANSITION_PHASE_ONE               (1<<0)
#define BOSS_TWO_TRANSITION_PHASE_TWO               (1<<1)

// cycles
#define BOSS_TWO_CYCLES                                 12

typedef struct Particle_t
{
	Vect2_t position;
	Vect2_t velocity;
	Vect2_t acceleration;

	int r,g,b; 
	int ttl;
	r_boolean active;
}*pParticle_t; 	

typedef struct Boss_2t
{
	Entity_t projectiles[ BOSS_TWO_PROJECTILES ];
	Entity_t segments[ BOSS_TWO_SEGMENTS ];
	Entity_t rocketPack;
	Entity_t reticule;

	int radius[ BOSS_TWO_SEGMENTS ];
	int distance[ BOSS_TWO_SEGMENTS ];
	r_boolean boomeranging[ BOSS_TWO_SEGMENTS ];
	int trigIndex[ BOSS_TWO_SEGMENTS ];
	
	Particle_t particles[ BOSS_TWO_PARTICLES ];

	float xPosition;
	float headScale;
	
	int state;
	int phase;
	int ticker;
	int cycle;
	r_boolean targetLocked;

	r_boolean (*currentCallback[ BOSS_TWO_CYCLES ])( Boss_2t* , pPlayer_t );
	int phases[ BOSS_TWO_CYCLES ];
	int phaseLengths[ BOSS_TWO_PHASES ];

	Explosion_t explosions[ BOSS_TWO_SEGMENTS ][ BOSS_TWO_SEGMENT_EXPLOSIONS ];

}*pBoss_2t;

void B2_Spawn( pBoss_2t , pSpawnPoint_t );
r_boolean B2_Kill( pBoss_2t );
void B2_Destroy( pBoss_2t );

void B2_Draw( pBoss_2t );

r_boolean B2_Run( pBoss_2t , pPlayer_t );

static r_boolean B2_RunPhaseOne( pBoss_2t , pPlayer_t );
static r_boolean B2_PhaseOneMovement( pBoss_2t , pPlayer_t );
static r_boolean B2_MoveToBackground( pBoss_2t , pPlayer_t );
static r_boolean B2_MoveToForeground( pBoss_2t , pPlayer_t );

static r_boolean B2_RunPhaseTwo( pBoss_2t , pPlayer_t );

static r_boolean B2_RunPhaseThree( pBoss_2t , pPlayer_t );

static void B2_LinkSegments( pBoss_2t );

// helpers
Vect2_t Project3Dto2D( float , float , float , int );
int CalcDistance( pVect2_t , pVect2_t );
float DotProduct( pVect2_t , pVect2_t );
Vect2_t Normalize( pVect2_t );

// particle system
void B2_InitParticle( pBoss_2t , int );
void B2_UpdateParticles( pBoss_2t ,  int , int );
r_boolean B2_RunParticle( pBoss_2t , int );
void B2_DrawParticles( pBoss_2t );

void B2_AdjustDifficulty( pBoss_2t , pPlayer_t );

r_boolean B2_LoadFrames( void );

#endif