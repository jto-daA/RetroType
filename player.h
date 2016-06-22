 
#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <allegro.h>

#include "common.h"

#define MAX_PROJECTILES 200

#define PLAYER_ANIMATION_RATE                          20
#define CANNON_ANIMATION_RATE                          40

#define PLAYER_FRAME_00  /* running */                  0
#define PLAYER_FRAME_01                                 1
#define PLAYER_FRAME_02  /* shooting */                 2
#define PLAYER_FRAME_03                                 3
#define PLAYER_FRAME_04  /* blaster */                  4
#define PLAYER_FRAME_05                                 5
#define PLAYER_FRAME_06                                 6
#define PLAYER_FRAME_07                                 7
#define PLAYER_FRAME_08  /* explosion */                8
#define PLAYER_FRAME_09                                 9
#define PLAYER_FRAME_10                                10
#define PLAYER_FRAME_11                                11 
#define PLAYER_FRAME_12                                12
#define PLAYER_FRAME_13                                13
#define PLAYER_FRAME_14                                14
#define PLAYER_FRAME_15                                15
#define PLAYER_FRAME_16  /* homing laser */            16
#define PLAYER_FRAME_17                                17
#define PLAYER_FRAME_18                                18
#define PLAYER_FRAMES                                  19

#define PLAYER_RUNNING_SEQUENCE                         0
#define PLAYER_IDLE_SEQUENCE_01                         1
#define PLAYER_SHOOTING_SEQUENCE_01                     2
#define PLAYER_SHOOTING_SEQUENCE_02                     3
#define PLAYER_CANNON_SEQUENCE_01                       4
#define PLAYER_EXPLOSION_SEQUENCE                       5
#define PLAYER_HOMING_LASER_SEQUENCE_01                 6

#define PLAYER_RUNNING_SEQUENCE_STEPS                   8
#define PLAYER_IDLE_01_SEQUENCE_STEPS                   2
#define PLAYER_SHOOTING_SEQUENCE_01_STEPS              12
#define PLAYER_SHOOTING_SEQUENCE_02_STEPS               3
#define PLAYER_CANNON_SEQUENCE_01_STEPS                 6
#define PLAYER_EXPLOSION_SEQUENCE_STEPS                 8
#define PLAYER_HOMING_LASER_SEQUENCE_01_STEPS           3

#define CANNON_AMMO                                     0
#define HOMING_AMMO                                     1
#define DROID_LASER_AMMO                                2

#define PLAYER_SIZE_X                                  40
#define PLAYER_SIZE_Y                                  40
#define CANNON_ROUND_SIZE_X                            20
#define CANNON_ROUND_SIZE_Y                            15
#define HOMING_ROUND_SIZE_X                            40
#define HOMING_ROUND_SIZE_Y                            20
#define DROID_LASER_SIZE_X                             40
#define DROID_LASER_SIZE_Y                             10
#define EXPLOSION_SIZE_X                               20
#define EXPLOSION_SIZE_Y                               20

#define DROID_FRAME_00                                  0
#define DROID_FRAME_01                                  1
#define DROID_FRAME_02                                  2
#define DROID_FRAME_03                                  3  
#define DROID_FRAMES                                    4

#define DROID_ANIMATION_RATE                           40

#define DROID_BLINKING_SEQUENCE                         0
#define DROID_BLINKING_SEQUENCE_STEPS                   7

#define MAX_CANNON_SHOTS                              200
#define MAX_PLASMA_BEAMS                                1
#define MAX_SPREAD_BLASTS                              10
#define MAX_HOMING_LASERS                              10 
#define MAX_DROID_LINKS                                 1

#define SPREAD_BURST_SHOT                               6

#define DROID_LINK_BEAM                                 0
#define PLASMA_BEAM_INDEX                               0

#define BULLET_CANNON_SPEED                            20
#define LASER_BEAM_SPEED                                8

#define DAMAGE_ENEMY                                    0
#define DAMAGE_PLAYER                                   1

#define INVINCEABLE                                    -1

#define MAX_EXPLOSIONS                                  5
#define MAX_EXPLOSION_SIZE                             10
#define EXPLOSION_LENGTH                               10
#define EXPLOSION_FRAMES                 EXPLOSION_LENGTH

#define PROJECTILE_HISTORY_LENGTH                      20
#define POSITIONAL_HISTORY_LENGTH                      50
typedef enum PowerUpType;

typedef enum
{
	CANNON,               // basic, munitons-based artilery ( default )
	PLASMA_CANNON,        // concentrated unidirectional plasma beam                 ( BLUE )
	SPREAD_BLASTER ,      // multi-directional burst weapon                          ( YELLOW )
	HOMING_LASER ,        // smart laser that scans area and destroys active targets ( RED )
	DROID_AMPLIFIER       // linking weapon that amplifies the attributes of droid   ( GREEN ) 
}WeaponType;

typedef enum
{
	CANNON_ROUND ,        // standard, single fire round
	CANNON_ROUND_PWR_2 ,
	CANNON_ROUND_PWR_3 , 
	CANNON_ROUND_PWR_MAX ,

	PLASMA_BEAM ,
	PLASMA_BEAM_PWR_2 , 
	PLASMA_BEAM_PWR_3 , 
	PLASMA_BEAM_PWR_MAX , 

	SPREAD_BURST , 
	SPREAD_BURST_PWR_2 , 
	SPREAD_BURST_PWR_3 , 
	SPREAD_BURST_PWR_MAX , 

	HOMING_BEAM ,
	HOMING_BEAM_PWR_2 , 
	HOMING_BEAM_PWR_3 , 
	HOMING_BEAM_PWR_MAX , 

	DROID_BEAM , 
	DROID_BEAM_PWR_2 , 
	DROID_BEAM_PWR_3 , 
	DROID_BEAM_PWR_MAX 
}BulletType;

typedef struct HealthData_t
{
	int health;
	int damageResistance;
	int strength;
	int projectileStrength;
}*pHealthData_t;

typedef struct SequenceInfo_t
{
	int steps;
	int currentStep;
	int* sequence;
	int sampleIndex;
	int sampleTrigger;
	int sampleChannel;
}*pSequenceInfo_t;

typedef struct AnimationData_t
{
	int currentFrame;
	int lastFrame;
	int rate;
	int timeOnCurrentFrame;
	int timeElapsed;
	int timeThisFrame;
	int timeLastFrame;

	SequenceInfo_t currentSequence;
}*pAnimationData_t;

typedef struct Entity_t
{
	Vect2_t minBounds,maxBounds;
	Vect2_t position;
	Vect2_t mapPosition;
	Vect2_t target;
	BITMAP** frames; int w,h; // remove when bitmaps are added.
	r_boolean collided,active;
	
	int frameCount;
	int currentFrame;
	int speed;

	int onscreenTargetID;     // used to identify onscreen targets

	int entityType;           // used to exclude certain types for targeting/unintended collision;

	int virtualPointerLink;   // Creates a link between virtual heap system, and onscreen-world entities ( via the virtual heap system ).

	r_boolean allocated;      // used for vh system.

	void* parent;             // cast to the specific type of the parent.

	HealthData_t healthStats;
	
	AnimationData_t animationData;
	
	PALETTE originalPallete,altPalette;
}*pEntity_t;

typedef struct Explosion_t
{
	Entity_t ent;
	int detonations;
	float scale;
}*pExplosion_t;

typedef struct Bullet_t
{
	Entity_t ent;
	BulletType bulletType;
}*pBullet_t;


// [ added 02/29/2008 ]
typedef struct MovementData_t
{
	r_boolean up,down,left,right;
}pMovementData_t;

typedef struct Droid_t
{
	Entity_t ent;
	int trigIndex;
	float scale;
}*pDroid_t;

typedef struct Player_t
// represents the user controled player; 
{
	WeaponType weaponType;
	MovementData_t directions;
	
	BulletType bulletType;
	Bullet_t projectiles[  MAX_PROJECTILES ];
	Vect2_t projectileHistory[ PROJECTILE_HISTORY_LENGTH ];
	Vect2_t   movementHistory[ POSITIONAL_HISTORY_LENGTH ];
	
	int currentBullet,currentBeam,currentLaser,currentBurst,currentLink;
	void (*Shoot)( Player_t* );
	void (*Draw)( Player_t* );

	Entity_t droid;           // automated,syncronized bot that works together w/player
	Entity_t ent;
	Droid_t buddy;
	Explosion_t explosions[ MAX_EXPLOSIONS ];

// lvls and power-ups
	int level;
}*pPlayer_t;

// basic stuff
int P_CreateEntity( pEntity_t , int , int , int , int , int , int , int , int , int );
int P_DestroyEntity( pEntity_t );

void P_SpawnNew( pPlayer_t );
void P_Kill( pPlayer_t );

void P_Draw( pPlayer_t );

void P_Move( pPlayer_t );
void P_Shoot( pPlayer_t );
void P_DestroyBullet( pPlayer_t , int );
static void P_ResortBullets( pPlayer_t );

void P_ShootCannon( pPlayer_t );
void P_ShootPlasmaCannon( pPlayer_t );
void P_ShootSpreadBlaster( pPlayer_t );
void P_ShootHomingLaser( pPlayer_t );
void P_ShootDroidAmplifier( pPlayer_t );

r_boolean P_LoadFrames( void );
r_boolean Droid_LoadFrames( void );

r_boolean Ent_TestForCollision( pEntity_t , pEntity_t );
void Ent_Draw( pEntity_t , float , float , int );

int Ent_InflictDamage( pPlayer_t , pEntity_t , r_boolean , r_boolean );

void Ent_InitHealthStats( pEntity_t , int , int , int , int );

r_boolean Ent_PursueEntity( pEntity_t , pEntity_t , int );

// power-ups and weapons
void P_SwitchWeapon( pPlayer_t , WeaponType );
void P_PowerUp( pPlayer_t , PowerUpType );

// animation system [ 03022008 ]
void Ent_Animate( pEntity_t );

// for frame loading [ 03032008 ]
r_boolean Ent_LoadData( BITMAP** , char** , int );

// slightly a hack.. added [ 03092008 ]
void Ent_UpdateAnimationSequence( pEntity_t , pSequenceInfo_t );
// to get rid of redundant code blocks [ 03192008 ] 
void Ent_SetAnimationParams( pEntity_t ,  BITMAP** , int , pSequenceInfo_t );
// explosion stuff [ 04042008 ]
void Ent_InitExplosion( pEntity_t , pExplosion_t , int );
r_boolean Ent_RunExplosion( pEntity_t , pExplosion_t );
void Ent_MoveExplosions( pExplosion_t , int , int , int );
void Ent_AddExplosions( pEntity_t , pExplosion_t , int , pSequenceInfo_t );
// save positional data [ 04192008 ]
void P_TrackPositionalData( pPlayer_t , pVect2_t , bool );
void P_ChangeAmmunition( pPlayer_t , BulletType );
// routines for each weapon fx. [ 04262008 ]
static void P_DrawCannonRound( pPlayer_t , int );
static void P_DrawHomingLaser( pPlayer_t , int );
static void P_DrawDroidBeam( pPlayer_t );
static void P_DrawPlasmaBeam( pPlayer_t );
static void P_DrawSpreadBurst( pPlayer_t , int  );



#endif