
#ifndef __BOSS_1__
#define __BOSS_1__

#include "common.h"
#include "player.h"
#include "game.h"

// animation stuffs.
#define BOSS_ONE_FRAME_00                  0
#define BOSS_ONE_FRAME_01                  1 
#define BOSS_ONE_FRAME_02                  2
#define BOSS_ONE_FRAME_03                  3
#define BOSS_ONE_FRAME_04                  4
#define BOSS_ONE_FRAME_05                  5
#define BOSS_ONE_FRAME_06                  6
#define BOSS_ONE_FRAME_07                  7
#define BOSS_ONE_FRAME_08                  8 
#define BOSS_ONE_FRAME_09                  9
#define BOSS_ONE_FRAME_10                 10
#define BOSS_ONE_FRAME_11                 11
#define BOSS_ONE_FRAME_12                 12
#define BOSS_ONE_FRAME_13 /* explosion */ 13
#define BOSS_ONE_FRAME_14                 14
#define BOSS_ONE_FRAME_15                 15
#define BOSS_ONE_FRAME_16                 16
#define BOSS_ONE_FRAME_17                 17
#define BOSS_ONE_FRAME_18                 18
#define BOSS_ONE_FRAME_19                 19
#define BOSS_ONE_FRAME_20                 20
#define BOSS_ONE_FRAMES                   21 

#define BOSS_ONE_ANIMATION_RATE           30

#define BOSS_ONE_RUNNING_SEQUENCE             0
#define BOSS_ONE_EYE_RUNNING_SEQUENCE         1
#define BOSS_ONE_EYE_BROW_RUNNING_SEQUENCE    2
#define BOSS_ONE_MOUTH_RUNNING_SEQUENCE       3
#define BOSS_ONE_BULLET_RUNNING_SEQUENCE      4 
#define BOSS_ONE_ORB_BULLET_RUNNING_SEQUENCE  5 
#define BOSS_ONE_BESERKER_MOUTH_RUNNING_SEQUENCE 6
#define BOSS_ONE_ORB_EXPLOSION_SEQUENCE          7
#define BOSS_ONE_EXPLOSION_SEQUENCE              8

#define BOSS_ONE_RUNNING_SEQUENCE_STEPS          3
#define BOSS_ONE_EYE_RUNNING_SEQUENCE_STEPS      3
#define BOSS_ONE_EYE_BROW_RUNNING_SEQUENCE_STEPS 2
#define BOSS_ONE_MOUTH_RUNNING_SEQUENCE_STEPS    8
#define BOSS_ONE_BULLET_RUNNING_SEQUENCE_STEPS   6
#define BOSS_ONE_ORB_BULLET_RUNNING_SEQUENCE_STEPS 4
#define BOSS_ONE_BESERKER_MOUTH_RUNNING_SEQUENCE_STEPS 2
#define BOSS_ONE_ORB_EXPLOSION_SEQUENCE_STEPS          8
#define BOSS_ONE_EXPLOSION_SEQUENCE_STEPS              8

// boss main attribs.
#define BOSS_ONE_ORBS                     20
#define BOSS_ONE_PROJECTILES              10
#define BOSS_ONE_ORB_PROJECTILES           5

#define BOSS_ONE_SPEED                     5
#define BOSS_ONE_ORB_RADIAL_SPEED          1
#define BOSS_ONE_ORB_LINEAR_SPEED          1
#define BOSS_ONE_ORB_PROJECTILE_SPEED     10
#define BOSS_ONE_BESERKER_AMPLITURE        3

#define BOSS_ONE_MOUTH_OFFSET             60
#define BOSS_ONE_EYE_BROW_OFFSET          30
#define BOSS_ONE_CENTER_OFFSET            60
// sizes
#define BOSS_ONE_SIZE_X                  150
#define BOSS_ONE_SIZE_Y                  150

#define BOSS_ONE_ORB_SIZE_X               60
#define BOSS_ONE_ORB_SIZE_Y               60

#define BOSS_ONE_EYE_BROW_SIZE_X          150
#define BOSS_ONE_EYE_BROW_SIZE_Y          50

#define BOSS_ONE_MOUTH_SIZE_X             150
#define BOSS_ONE_MOUTH_SIZE_Y             40

#define BOSS_ONE_BULLET_SIZE_X             30
#define BOSS_ONE_BULLET_SIZE_Y             30

#define BOSS_ONE_ORB_BULLET_SIZE_X         40
#define BOSS_ONE_ORB_BULLET_SIZE_Y         30

#define BOSS_ONE_ORB_RANGE                400

#define BOSS_ONE_INTRO_LENGTH
#define BOSS_ONE_PROTECTED_LENGTH

#define BOSS_ONE_LEFT_EYEBALL               0
#define BOSS_ONE_RIGHT_EYEBALL              1  

// health/damage
#define BOSS_ONE_STRENGTH                 100
#define BOSS_ONE_PROJECTILE_STRENGTH       50
#define BOSS_ONE_BESERKER_STRENGTH        500
#define BOSS_ONE_HEALTH                  20000
#define BOSS_ONE_DAMAGE_RESISTANCE         90

#define ORB_STRENGTH                       25
#define ORB_PROJECTILE_STRENGTH            10
#define ORB_HEALTH                        500
#define ORB_DAMAGE_RESISTANCE              50

// states
// [ BOSS ]
// ==[ LOGIC STATES ]
#define BOSS_ONE_STATE_INTRO           (1<<0)
#define BOSS_ONE_STATE_PROTECTED       (1<<1)
#define BOSS_ONE_STATE_ROAMING         (1<<2)
#define BOSS_ONE_STATE_AGGRESIVE       (1<<3)
#define BOSS_ONE_STATE_BESERKER        (1<<4)
#define BOSS_ONE_STATE_BLIND           (1<<5)
#define BOSS_ONE_INIT_STATES           BOSS_ONE_STATE_INTRO | BOSS_ONE_STATE_PROTECTED 
#define BOSS_ONE_STATES                BOSS_ONE_STATE_INTRO | BOSS_ONE_STATE_PROTECTED | BOSS_ONE_STATE_ROAMING | BOSS_ONE_STATE_BLIND | BOSS_ONE_STATE_AGGRESIVE | BOSS_ONE_STATE_BESERKER 
// ==[ RENDERING STATES ]


// [ ORB ]
// ==[ LOGIC STATES ]
#define ORB_STATE_INTRO                (1<<0)
#define ORB_STATE_LONELY               (1<<1)
#define ORB_STATE_PROBING              (1<<2)
#define ORB_STATE_ORBITING             (1<<3)
#define ORB_STATE_SHOOTING             (1<<4)
#define ORB_STATE_FOUND_TARGET         (1<<5)
#define ORB_STATE_SEARCHING            (1<<6)
#define ORB_STATES                     ORB_STATE_INTRO | ORB_STATE_LONELY | ORB_STATE_PROBING | ORB_STATE_ORBITING | ORB_STATE_SHOOTING | ORB_STATE_FOUND_TARGET | ORB_STATE_SEARCHING
// ==[ RENDERING STATES ]

// explosions
#define BOSS_ONE_ORB_EXPLOSIONS            10
#define BOSS_ONE_ORB_EXPLOSION_LENGTH      10
#define BOSS_ONE_EXPLOSIONS                10
#define BOSS_ONE_EXPLOSION_LENGTH          10

typedef struct Boss1Orb_t
{
	Entity_t ent;
	Entity_t projectiles[ BOSS_ONE_ORB_PROJECTILES ];
	float rotationAngle;
	Vect2_t center;										
	int state;
	int ticker;
	Explosion_t explosions[ BOSS_ONE_ORB_EXPLOSIONS ];
}Orb_t,*pOrb_t;

typedef struct Boss_1
{
	Entity_t ent;
	Entity_t particleBeam;
	Entity_t projectiles[ BOSS_ONE_PROJECTILES ];
	Entity_t mouth;
	Entity_t eyebrows;
	Vect2_t slopes[ BOSS_ONE_PROJECTILES ];
	Orb_t orbs[ BOSS_ONE_ORBS ];
	int state;
	int ticker;
	int activeOrbs;
	int eyeballs[2];
	int inactiveBullets;
	Explosion_t explosions[ BOSS_ONE_EXPLOSIONS ];
}MegaDude_t,Boss_1t,*pBoss_1t,*pMegaDude_t;

void B1_Spawn( pMegaDude_t , pSpawnPoint_t );
r_boolean B1_Kill( pMegaDude_t );
void B1_Destroy( pMegaDude_t );

r_boolean B1_Run( pMegaDude_t , pPlayer_t );
static r_boolean B1_RunOrb( pBoss_1t ,  pOrb_t , pPlayer_t );

// state routines

int Orb_DetermineCurrentState( pOrb_t , pBoss_1t , pPlayer_t );
r_boolean Orb_RunIntro( pOrb_t , pBoss_1t , pPlayer_t );
r_boolean Orb_RunLonely( pOrb_t , pBoss_1t , pPlayer_t );
r_boolean Orb_RunProbing( pOrb_t , pBoss_1t , pPlayer_t );
r_boolean Orb_RunOrbit( pOrb_t , pBoss_1t , pPlayer_t );
r_boolean Orb_RunShooting( pOrb_t , pBoss_1t , pPlayer_t );
r_boolean Orb_RunSearching( pOrb_t , pBoss_1t , pPlayer_t );

int B1_DetermineCurrentState( pBoss_1t , pPlayer_t );
static int B1_SetOrbStates( pBoss_1t , int );
static int B1_ClearOrbStates( pBoss_1t , int );
r_boolean B1_RunIntro( pBoss_1t );
r_boolean B1_RunRoaming( pBoss_1t , pPlayer_t );
r_boolean B1_RunAggresive( pBoss_1t , pPlayer_t );
r_boolean B1_RunBeserker( pBoss_1t , pPlayer_t );

// rendering routines
void B1_Draw( pMegaDude_t );
static void B1_DrawOrb( pOrb_t );

// animation stuff 
r_boolean B1_LoadFrames( void );



#endif