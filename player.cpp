
#include <allegro.h>
#include <memory.h>
#include <math.h>

#include "common.h"
#include "game.h"
#include "player.h"
#include "sys_gfx.h"
#include "droid.h"
#include "audio.h"
#include "boss_2.h"

extern BITMAP* doubleBuffer;
extern float masterScale;
extern volatile int msecTicker;

static BITMAP* playerFrames[ PLAYER_FRAMES ];

static char* playerFilenames[] = 
{
/* PLAYER_FRAME_00 */   "content/gfx/enemies/player/pl_r1.bmp" , 
/* PLAYER_FRAME_01 */	"content/gfx/enemies/player/pl_r2.bmp" , 
/* PLAYER_FRAME_02 */	"content/gfx/enemies/player/pl_s1.bmp" , 
/* PLAYER_FRAME_03 */   "content/gfx/enemies/player/pl_s2.bmp" , 
/* PLAYER_FRAME_04 */	"content/gfx/enemies/player/pl_s3.bmp" , 

/* PLAYER_FRAME_05 */	"content/gfx/enemies/player/munitions/mun_c1.bmp" , 
/* PLAYER_FRAME_06 */	"content/gfx/enemies/player/munitions/mun_c2.bmp" ,
/* PLAYER_FRAME_07 */	"content/gfx/enemies/player/munitions/mun_c3.bmp" , 
/* PLAYER_FRAME_08 */	"content/gfx/enemies/expl_1.bmp" , 
/* PLAYER_FRAME_09 */	"content/gfx/enemies/expl_2.bmp" , 

/* PLAYER_FRAME_10 */	"content/gfx/enemies/expl_3.bmp" , 
/* PLAYER_FRAME_11 */	"content/gfx/enemies/expl_4.bmp" , 
/* PLAYER_FRAME_12 */	"content/gfx/enemies/expl_5.bmp" , 
/* PLAYER_FRAME_13 */	"content/gfx/enemies/expl_6.bmp" , 
/* PLAYER_FRAME_14 */	"content/gfx/enemies/expl_7.bmp" , 

/* PLAYER_FRAME_15 */	"content/gfx/enemies/expl_8.bmp" , 
/* PLAYER_FRAME_16 */   "content/gfx/enemies/player/munitions/mun_hl1.bmp" , 
/* PLAYER_FRAME_17 */   "content/gfx/enemies/player/munitions/mun_hl2.bmp" , 
/* PLAYER_FRAME_18 */   "content/gfx/enemies/player/munitions/mun_hl3.bmp"  
};

static int playerRunningSequence[] = 
{
	PLAYER_FRAME_00 , 
	PLAYER_FRAME_00 , 
	PLAYER_FRAME_01 , 
	PLAYER_FRAME_01 , 
	PLAYER_FRAME_00 , 
	PLAYER_FRAME_00 , 
	PLAYER_FRAME_01 , 
	PLAYER_FRAME_01  
} ;

static int playerIdleSequence[] = 
{
	0 , 
	0
};

static int playerShootingSequence_01[] = 
{ 
	PLAYER_FRAME_00 ,
	PLAYER_FRAME_01 , 
	PLAYER_FRAME_00 , 
	PLAYER_FRAME_01 , 

	PLAYER_FRAME_02 , 
	PLAYER_FRAME_03 ,
	PLAYER_FRAME_04 ,
	PLAYER_FRAME_04 ,
	PLAYER_FRAME_04 ,
	PLAYER_FRAME_04 , 

	PLAYER_FRAME_03 , 
	PLAYER_FRAME_02 , 
	
	PLAYER_FRAME_00 , 
	PLAYER_FRAME_00 , 
};

static int playerShootingSequence_02[] = 
{ 
	PLAYER_FRAME_16 , 
	PLAYER_FRAME_17 ,
	PLAYER_FRAME_18
};

static int cannonShootingSequence[] = 
{
	PLAYER_FRAME_05 ,
	PLAYER_FRAME_05 , 
	PLAYER_FRAME_07 , 
	PLAYER_FRAME_07 ,
	PLAYER_FRAME_06 ,
	PLAYER_FRAME_06
};

static int plasmaShootingSequence[] = 
{
	0
};

static int homingShootingSequence[] = 
{
	PLAYER_FRAME_16 , 
	PLAYER_FRAME_16 , 
	PLAYER_FRAME_16
};

static int droidShootingSequence[] = 
{
	0
};

static int explosionSequence[] = 
{
	PLAYER_FRAME_08 , 
	PLAYER_FRAME_09 , 
	PLAYER_FRAME_10 , 
	PLAYER_FRAME_11 , 
	PLAYER_FRAME_12 , 
	PLAYER_FRAME_13 , 
	PLAYER_FRAME_14 , 
	PLAYER_FRAME_15 
};

static SequenceInfo_t playerSequences[] =
{
	{         PLAYER_RUNNING_SEQUENCE_STEPS , 0 ,     playerRunningSequence , 0 , 0 , -1 } , 
	{         PLAYER_IDLE_01_SEQUENCE_STEPS , 0 ,        playerIdleSequence , 0 , 0 , -1 } , 
	{     PLAYER_SHOOTING_SEQUENCE_01_STEPS , 0 , playerShootingSequence_01 , 1 , PLAYER_FRAME_02 , AUDIO_CHANNEL_PLAYER_WEAPON } ,
	{     PLAYER_SHOOTING_SEQUENCE_02_STEPS , 0 , playerShootingSequence_02 , 0 , 0 , -1 } , 
	{       PLAYER_CANNON_SEQUENCE_01_STEPS , 0 ,    cannonShootingSequence , 0 , 0 , -1 } , 
	{       PLAYER_EXPLOSION_SEQUENCE_STEPS , 0 ,         explosionSequence , 0 , 0 , -1 } ,
	{ PLAYER_HOMING_LASER_SEQUENCE_01_STEPS , 0 ,    homingShootingSequence , 0 , 0 , -1 }

};

static Vect2_t ammoSizes[] = 
{
	{ CANNON_ROUND_SIZE_X , CANNON_ROUND_SIZE_Y } , 
	{ HOMING_ROUND_SIZE_X , HOMING_ROUND_SIZE_Y } , 
	{  DROID_LASER_SIZE_X ,  DROID_LASER_SIZE_Y }
};

static BITMAP* droidFrames[ DROID_FRAMES ];

static char* fileNames[] = 
{
	"content/gfx/enemies/player/droid/droid_1.bmp" , 
	"content/gfx/enemies/player/droid/droid_2.bmp" , 
	"content/gfx/enemies/player/droid/droid_3.bmp" , 
	"content/gfx/enemies/player/droid/droid_4.bmp" 
};

static int blinkingSequence[] = 
{
	DROID_FRAME_00 , 
	DROID_FRAME_00 , 
	DROID_FRAME_00 , 
	DROID_FRAME_00 , 
	DROID_FRAME_00 , 
	DROID_FRAME_02 , 
	DROID_FRAME_03
};

static SequenceInfo_t droidSequences[] = 
{
	{ DROID_BLINKING_SEQUENCE_STEPS , 0 , blinkingSequence , 0 , 0 , -1 }
};



// consts

const int boss_strenght = 1000;
const int boss_damage_resistance = 100;

int P_CreateEntity( pEntity_t pEnt , int mmx , int mmy , int mxx , int mxy , int x , int y , int speed , int frames , int type )
{
	static int createdEntities=0;

	pEnt->currentFrame = 0;
	pEnt->frameCount = frames;
	//pEnt->frames = (BITMAP**)malloc(sizeof(BITMAP*)*frames);
	pEnt->minBounds.x = mmx;
	pEnt->minBounds.y = mmy;
	pEnt->maxBounds.x = mxx;
	pEnt->maxBounds.y = mxy;
	pEnt->speed = speed;
	pEnt->collided = r_false;

	pEnt->position.x = x;
	pEnt->position.y = y;

	// assrt members
	pEnt->onscreenTargetID = 0;
	pEnt->entityType = ENTITY_DUMMY_TYPE;
	pEnt->virtualPointerLink = 0;
	pEnt->allocated = r_true;

	// change this later.
	pEnt->w = abs( mmx - mxx );
	pEnt->h = abs( mmy - mxy );

	pEnt->entityType = type;	

	pEnt->active = r_true;

	// initialize palette info
	get_palette( pEnt->originalPallete );

	// calculate an alternate palette for damage effects ( consider passing paramters to dictate the difference )
	for( int color = 0;color < PAL_SIZE;color++ )
	{
		RGB originalColor , 
			     newColor;

		get_color( color , &originalColor );

		newColor.r = originalColor.r >> 3;
		newColor.g = originalColor.g << 3;
		newColor.b = originalColor.b >> 1;
		
		pEnt->altPalette[ color ].r = newColor.r;
		pEnt->altPalette[ color ].g = newColor.g;
		pEnt->altPalette[ color ].b = newColor.b;

	}

	return createdEntities++;
}

int P_DestroyEntity( pEntity_t pEnt )
{
	static int destroyedEntities=0;

	for( int frame=0;frame<pEnt->frameCount;frame++ )
	{
		if( pEnt->frames[ frame ]->dat )
		{
			//destroy_bitmap( pEnt->frames[ frame ] );
			//free( (void*)&pEnt->frames[ frame ] );
		}
	}
	
	//free( (void*)pEnt->frames );
	
	//memset( pEnt , 0 , sizeof(Entity_t) );
	
	pEnt->active = r_false;

	return destroyedEntities--;
}

void P_SpawnNew( pPlayer_t pPlayer )
{
	pPlayer->currentBullet = 0;
	pPlayer->ent.currentFrame = 0;

// player
	P_CreateEntity( &pPlayer->ent , 0 , 0 , PLAYER_SIZE_X , PLAYER_SIZE_Y , 0 , SCREEN_H>>1 , 5 , PLAYER_FRAMES , ENTITY_DUMMY_TYPE );
	P_SwitchWeapon( pPlayer , CANNON );
	Ent_InitHealthStats( &pPlayer->ent , 100 , 25 , 100 , 100 );

	pPlayer->ent.animationData.rate = PLAYER_ANIMATION_RATE;
	pPlayer->ent.frames = playerFrames;

	//Ent_UpdateAnimationSequence( &pPlayer->ent , &playerSequences[ PLAYER_RUNNING_SEQUENCE ] );
	Ent_UpdateAnimationSequence( &pPlayer->ent , &playerSequences[ 2 ] );

// droid
	P_CreateEntity( &pPlayer->buddy.ent ,
					0 , 0 , 
					DROID_SIZE_X ,
					DROID_SIZE_Y ,
					pPlayer->ent.position.x - DROID_COMFORT_DISTANCE,
					pPlayer->ent.position.y - DROID_COMFORT_DISTANCE,
					DROID_SPEED ,
					DROID_FRAMES , 
					ENTITY_DUMMY_TYPE );

	pPlayer->buddy.ent.animationData.rate = DROID_ANIMATION_RATE;
	pPlayer->buddy.ent.frames = droidFrames;
	pPlayer->buddy.trigIndex = 0;
	pPlayer->buddy.scale = 1.0f;
	
	Ent_UpdateAnimationSequence( &pPlayer->buddy.ent , &droidSequences[ DROID_BLINKING_SEQUENCE ] );

	Ent_AddExplosions( &pPlayer->ent , pPlayer->explosions , MAX_EXPLOSIONS , &playerSequences[ PLAYER_EXPLOSION_SEQUENCE ] );

// reset tracking history
	memset( &pPlayer->movementHistory , 0 , sizeof(Vect2_t)*POSITIONAL_HISTORY_LENGTH );
	memset( &pPlayer->projectileHistory , 0 , sizeof(Vect2_t)*PROJECTILE_HISTORY_LENGTH );

// load bullets ( cannon rounds by defualt )
	for( int bullet=0;bullet < MAX_PROJECTILES;bullet++ )
	{
		P_CreateEntity( &pPlayer->projectiles[ bullet ].ent , 
						0 , 0 , 
						ammoSizes[ CANNON_AMMO ].x , ammoSizes[ CANNON_AMMO ].y ,
						pPlayer->ent.position.x , pPlayer->ent.position.y , 
						0 , 
						PLAYER_FRAMES , 
						ENTITY_DUMMY_TYPE );

		Ent_SetAnimationParams( &pPlayer->projectiles[ bullet ].ent , playerFrames , PLAYER_ANIMATION_RATE , &playerSequences[ PLAYER_CANNON_SEQUENCE_01 ] );
	}
}

void P_Kill( pPlayer_t pPlayer )
{
	// Add death sounds / animations here.
	//P_SpawnNew( pPlayer );
}
void P_Draw( pPlayer_t pPlayer)
{
	int color = pPlayer->ent.collided==r_true ? makecol( 255 , 0 , 0 ) : makecol( 255 , 255 , 255 );
	float droidScale = pPlayer->buddy.scale;
	
	G_WriteToDebugWindow( pPlayer->ent.healthStats.health , "%i" , pPlayer->ent.position.x , pPlayer->ent.position.y + pPlayer->ent.h );
	
	// draw tracking
	for( int hist=0;hist < POSITIONAL_HISTORY_LENGTH;hist++ )
	{
		pVect2_t pV_P = &pPlayer->movementHistory[ hist ];
		pVect2_t pV_B = &pPlayer->projectileHistory[ hist ];

		circlefill( doubleBuffer , pV_P->x , pV_P->y , 10 , makecol( 255 , 255-hist , 255-hist ) );
		//circlefill( doubleBuffer , pV_B->x , pV_B->y , 10 , makecol( hist+255 , 255-hist , 255-hist ) );
	}
	
	// draw bullets 
	for( int bullet=0;bullet<pPlayer->currentBullet;bullet++ )
	{
		pEntity_t pBulletEnt = &pPlayer->projectiles[ bullet ].ent;

		if( pBulletEnt->active )
		{
 			switch( pPlayer->weaponType )
			{
			case CANNON:
				{
					P_DrawCannonRound( pPlayer , bullet );
					break;
				}

			case HOMING_LASER:
				{
					P_DrawHomingLaser( pPlayer , bullet );
					break;
				}

			case PLASMA_CANNON:
			case SPREAD_BLASTER:
			case DROID_AMPLIFIER:
				{
					break;
				}

			}
		}
	}

	// draw player
	Ent_Animate( &pPlayer->ent );
	Ent_Draw( &pPlayer->ent , 1 , 1 , 0 );

	// draw droid
	Ent_Animate( &pPlayer->buddy.ent );
	Ent_Draw( &pPlayer->buddy.ent , droidScale , droidScale , 0 );

	// draw explosions
	for( int explosion = 0;explosion < MAX_EXPLOSIONS;explosion++ )
	{
		if( pPlayer->explosions[ explosion ].ent.active )
		{
			pExplosion_t pEx = &pPlayer->explosions[ explosion ];
			
			Ent_RunExplosion( &pPlayer->ent , pEx );
			Ent_Animate( &pEx->ent );
			Ent_Draw( &pEx->ent , pEx->scale , pEx->scale , 0 );
		}
	}
}

void P_Move( pPlayer_t pPlayer)
// [ 02/29/2008 ] Now movements under directional restriction ( just *responds* to state changes )
// [ 04/19/2008 ] Record is kept of players past position. Tracks a size of POSITIONAL_HISTORY_LENGTH frames.
{
	int speed = pPlayer->ent.speed;
	int dx = 0 , dy = 0;
	
	r_boolean up = pPlayer->directions.up , 
		    down = pPlayer->directions.down , 
			left = pPlayer->directions.left , 
		   right = pPlayer->directions.right;

	
	pVect2_t pv = &pPlayer->ent.position;

	if( key[ KEY_UP ] && up )
	{
		if( pv->y > 0 )
		{
			pv->y -= speed;
			pPlayer->droid.position.y-=speed;
			pPlayer->buddy.ent.position.y -= speed;
			dy = -speed;
		}
	}

	if( key[ KEY_DOWN ] && down )
	{
		if( pv->y < SCREEN_H )
		{
			pv->y+=speed;
			pPlayer->droid.position.y+=speed;
			pPlayer->buddy.ent.position.y += speed;
			dy = speed;
		}
	}

	if( key[ KEY_LEFT ] && left )
	{
		if( pv->x > 0 )
		{
			pv->x-=speed;
			pPlayer->droid.position.x-=speed;
			pPlayer->buddy.ent.position.x-=speed;
			dx = -speed;
		}
	}

	if( key[ KEY_RIGHT ] && right )
	{
		if( pv->x < SCREEN_W )
		{
			pv->x+=speed;
			pPlayer->droid.position.x+=speed;
			pPlayer->buddy.ent.position.x+=speed;
			dx = speed;
		}
	}

	//Ent_MoveExplosions( pPlayer->explosions , MAX_EXPLOSIONS , dx , dy );

	if( key[ KEY_SPACE ] )
	{
		pPlayer->Shoot( pPlayer ); // setup with another weapon switching func.
		//P_ShootCannon( pPlayer );		
		//key[ KEY_SPACE ] = r_false;
	}

	// CHANGE THIS LATER
	if( key[ KEY_1 ] )
	{
		P_SwitchWeapon( pPlayer , CANNON );
	}

	if( key[ KEY_2 ] )
	{
		P_SwitchWeapon( pPlayer , PLASMA_CANNON );
	}

	if( key[ KEY_3 ] )
	{
		P_SwitchWeapon( pPlayer , SPREAD_BLASTER );
	}

	if( key[ KEY_4 ] )
	{
		P_SwitchWeapon( pPlayer , HOMING_LASER );
	}

	if( key[ KEY_5 ] )
	{
		P_SwitchWeapon( pPlayer , DROID_AMPLIFIER );
	}

	// Update movement history
	P_TrackPositionalData( pPlayer , pPlayer->movementHistory , r_true );
	P_TrackPositionalData( pPlayer , pPlayer->projectileHistory , r_false );
}

void P_DestroyBullet( pPlayer_t pPlayer , int bullet )
{
	pBullet_t pBullet = &pPlayer->projectiles[ bullet ];
	
	pBullet->ent.active = r_false;
	pBullet->ent.mapPosition.x = pBullet->ent.mapPosition.y = pBullet->ent.position.x = pBullet->ent.position.y = pBullet->ent.speed = -1 ;

	pPlayer->projectiles[ bullet ].ent.mapPosition.x =
	pPlayer->projectiles[ bullet ].ent.mapPosition.y =
	pPlayer->projectiles[ bullet ].ent.position.x =
	pPlayer->projectiles[ bullet ].ent.position.x = 0;

	pPlayer->currentBullet--;

	P_ResortBullets( pPlayer );
}
static void P_ResortBullets( pPlayer_t pPlayer )
// helper function that resorts the players bullet array
{
	for( int i=0;pPlayer->projectiles[ i ].ent.active;i++ )
	{

	}
	
	for( ;i<pPlayer->currentBullet;i++ )
	{
		pPlayer->projectiles[ i ] = pPlayer->projectiles[ i + 1 ];
	}
}
//
// CONSIDER : consolidating all these to a P_GenericShoot( pPlayer_t , int );
//
void P_ShootCannon( pPlayer_t pPlayer )
{
	int bulletIndex = pPlayer->currentBullet;

	if( bulletIndex == MAX_PROJECTILES )
	{
		return;
	}

	P_CreateEntity( &pPlayer->projectiles[ bulletIndex ].ent ,
		             0 , 0 , 
					 CANNON_ROUND_SIZE_X , 
					 CANNON_ROUND_SIZE_Y ,
					 ( pPlayer->ent.position.x + ( pPlayer->ent.w >> 1) ) /*+ BULLET_CANNON_SPEED */ , 
					 pPlayer->ent.position.y + ( pPlayer->ent.h >> 1 ) , 
					 BULLET_CANNON_SPEED , 
					 3 , 
					 ENTITY_DUMMY_TYPE );

//	pPlayer->projectiles[ bulletIndex ].ent.frames = playerFrames;
//	pPlayer->projectiles[ bulletIndex ].ent.animationData.rate = CANNON_ANIMATION_RATE;

//	Ent_UpdateAnimationSequence( &pPlayer->projectiles[ bulletIndex ].ent , &playerSequences[ PLAYER_CANNON_SEQUENCE_01 ] );
//	pPlayer->projectiles[ bulletIndex ].ent.animationData.currentSequence.currentStep = 0;

	pPlayer->currentBullet++;		
}

void P_ShootPlasmaCannon( pPlayer_t pPlayer )
{
	int bulletIndex = pPlayer->currentBullet;

	if( bulletIndex == MAX_PLASMA_BEAMS )
	{
		return;
	}

	

	pPlayer->currentBullet++;	
}

void P_ShootSpreadBlaster( pPlayer_t pPlayer )
// each shot adds 6 bullets to the enviorn.
{
	int bulletIndex = pPlayer->currentBullet;

	if( bulletIndex == MAX_PROJECTILES ) // bindex + 6... 
	{
		return;
	}

	//pPlayer->currentBullet += SPREAD_BURST_SHOT;
	pPlayer->currentBullet++;
}

void P_ShootHomingLaser( pPlayer_t pPlayer )
{
	int bulletIndex = pPlayer->currentBullet;

	if( bulletIndex == MAX_HOMING_LASERS )
	{
		return;
	}

	Ent_SetAnimationParams( &pPlayer->projectiles[ bulletIndex ].ent , pPlayer->ent.frames , PLAYER_ANIMATION_RATE , &playerSequences[ PLAYER_HOMING_LASER_SEQUENCE_01 ] );

	pPlayer->projectiles[ bulletIndex ].ent.position.x = pPlayer->ent.position.x;
	pPlayer->projectiles[ bulletIndex ].ent.position.y = pPlayer->ent.position.y;

	pEntity_t pBullet = &pPlayer->projectiles[ bulletIndex ].ent;
	pBullet->frames = playerFrames;


	pPlayer->currentBullet++;

}

void P_ShootDroidAmplifier( pPlayer_t pPlayer )
{
	int bulletIndex = pPlayer->currentBullet;
	
	if( bulletIndex == MAX_DROID_LINKS )
	{
		return;
	}

	line( doubleBuffer , pPlayer->ent.position.x , pPlayer->ent.position.y , pPlayer->droid.position.x , pPlayer->droid.position.y , makecol( 255 , 255 , 255 ) );

	pPlayer->currentBullet++;
}

r_boolean P_LoadFrames( void )
{
	return Ent_LoadData( playerFrames , playerFilenames , PLAYER_FRAMES );
}


r_boolean Droid_LoadFrames( void )
{
	return Ent_LoadData( droidFrames , fileNames , DROID_FRAMES );
}

	
r_boolean Ent_TestForCollision( pEntity_t e1 , pEntity_t e2 )
{
	r_boolean collided = r_false;

	// consider pre-calcing these and storing them in the ent-struct.
	int      xDim = abs( e1->minBounds.x - e1->maxBounds.x ) ,
		     yDim = abs( e1->minBounds.y - e1->maxBounds.y ) ,
        minDeltaX = abs( e1->position.x - e2->position.x ) ,
		maxDeltaX = abs( (e1->position.x+e1->w ) - (e2->position.x+e2->w) ) , 
		minDeltaY = abs( e1->position.y - e2->position.y ) , 
		maxDeltaY = abs( (e1->position.y+e1->h ) - (e2->position.y+e2->h) );

	if( (minDeltaX < xDim) && (maxDeltaX < xDim) ) 
	{
		if( (minDeltaY < yDim) && (maxDeltaY < yDim) )
		{
			collided = r_true;
			e1->collided = e1->collided = r_true;
		}
	}
	
	else
	{
		e1->collided = r_false;
		e2->collided = r_false;
	}

	G_WriteToDebugWindow( minDeltaX , "minDeltaX %d" , 10 , 460 );
	G_WriteToDebugWindow( minDeltaY , "minDeltaY %d" , 10 , 470 );
	G_WriteToDebugWindow( e1->position.x , "ent 1 x : %d" , 230 , 390 );
	G_WriteToDebugWindow( e1->position.y , "ent 2 y : %d" , 230 , 400 );
	
	return collided;
}

int Ent_InflictDamage( pPlayer_t pP , pEntity_t pE , r_boolean projectile , r_boolean self )
{
	// point to either the players or the enemies heath stats
	pHealthData_t pStats = ( self ) ? ( &pP->ent.healthStats ) : ( &pE->healthStats );
	
	// calculate damage due to projectile or body-to-body collision(s)
	int damage = ( projectile ) ? ( pStats->projectileStrength ) : ( pStats->strength );
		
	// dampen damage bansed on resistance
	pStats->damageResistance = pStats->damageResistance ? pStats->damageResistance : 1;
	//damage /= ( pStats->damageResistance );	
	// inflict the damge
	if( pStats->health != INVINCEABLE )
	{
		pStats->health -= damage;
	}

	return damage;
}

void Ent_InitHealthStats( pEntity_t pE , int health , int resistance , int strength , int projectileStrength )
{
	pE->healthStats.health = health;
	pE->healthStats.damageResistance = resistance;
	pE->healthStats.strength = strength;
	pE->healthStats.projectileStrength = projectileStrength;
}

r_boolean Ent_PursueEntity( pEntity_t pE1 , pEntity_t pE2 , int tolerance )
{
	pVect2_t pvTrk = &pE1->position;
	pVect2_t pvFle = &pE2->position;
	r_boolean xTgtFound = r_false;
	r_boolean yTgtFound = r_false;
	
	// adjust speed if necessary
	//int speed = (pE1->speed < pE2->speed ) ? (pE2->speed ) : ( pE1->speed);
	int speed = pE1->speed;
	

	if( ( pvTrk->x + tolerance )  < pvFle->x  )
	{
		pvTrk->x += speed;
	}

	else if( ( pvTrk->x - tolerance ) > pvFle->x  )
	{
		pvTrk->x -= speed;
	}

	else
	{
		xTgtFound = r_true;
	}

	if( ( pvTrk->y + tolerance ) < pvFle->y )
	{
		pvTrk->y += speed;
	}

	else if( ( pvTrk->y - tolerance ) > pvFle->y ) 
	{
		pvTrk->y -= speed;
	}

	else
	{
		yTgtFound = r_true;
	}
	
	return r_boolean( xTgtFound && yTgtFound );
}

void P_SwitchWeapon( pPlayer_t pPlayer , WeaponType newWeapon )
{
	pPlayer->weaponType = newWeapon;

	switch( newWeapon ) 
	{
		case DROID_AMPLIFIER:
			{
				pPlayer->Shoot = P_ShootDroidAmplifier;
				pPlayer->bulletType = DROID_BEAM;
				break;
			}

		case SPREAD_BLASTER:
		case PLASMA_CANNON:
		case CANNON:
			{
				pPlayer->Shoot = P_ShootCannon;
				pPlayer->bulletType = CANNON_ROUND;
				break;
			}

		
		case HOMING_LASER:
			{
				pPlayer->Shoot = P_ShootHomingLaser;
				pPlayer->bulletType = HOMING_BEAM;
				break;
			}
	}

	P_ChangeAmmunition( pPlayer , pPlayer->bulletType );
}

void P_PowerUp( pPlayer_t pPlayer , PowerUpType power )
{
	PowerUpType powerUp = (PowerUpType)power;
	WeaponType currentWeapon = pPlayer->weaponType;
	BulletType currentAmmo = pPlayer->bulletType;
	


	pPlayer->bulletType = currentAmmo;
}

void Ent_Animate( pEntity_t pEnt )
{
	pAnimationData_t pAnim = &pEnt->animationData;
	pSequenceInfo_t pSequence = &pAnim->currentSequence;
	
	// need to save the time spent inbetween calls
	static int timeElapsed = msecTicker;
	int swap = 0;

	if( pAnim->timeOnCurrentFrame >= pAnim->rate )
	{
		if( pSequence->currentStep < pSequence->steps-1 )
		{
			// move to the next step of the sequence
			pSequence->currentStep++;	
			
			// animate active explosions
		
		}

		else
		{
			// restart at the beggining of the sequence
			pSequence->currentStep = 0;
		}

		// reset timer
		pAnim->timeOnCurrentFrame = 0; 
		timeElapsed = msecTicker;
	}

	else
	{
		// stay on the current frame
	}

	// select the corresponding frame from the sequence
	pAnim->currentFrame = pSequence->sequence[ pSequence->currentStep ];

	// calculate the time spent on the current frame	
	pAnim->timeOnCurrentFrame = msecTicker - timeElapsed;

	// play sfx if time 
	if( pAnim->currentFrame == pAnim->currentSequence.sampleTrigger ) 
	{
		//AUD_PlaySample( pAnim->currentSequence.sampleChannel , pAnim->currentSequence.sampleIndex );
	}
}


void Ent_Draw( pEntity_t pE , float scale_x , float scale_y , int angle )
{
	int currentFrame = pE->animationData.currentFrame;

#ifdef __RETRO_TYPE_DEBUG_VERSION__
	//rectfill( doubleBuffer , pE->position.x , pE->position.y , pE->position.x + pE->w , pE->position.y + pE->h , makecol( 255 , 255 , 255 ) );
#else
	//stretch_blit( pE->frames[ pE->currentFrame ] , doubleBuffer , 0 , 0 , pE->w , pE->h , pE->position.x , pE->position.y , pE->w , pE->h );
#endif
	
	if( pE->collided )
	{
		//SYS_ChangePalette( 100 , 100 , 100 );
		set_palette( pE->altPalette );
	}

	if( angle )
	{
		int cx = pE->w >> 1 ,
			cy = pE->h >> 1;

		pivot_scaled_sprite( doubleBuffer ,
						     pE->frames[ currentFrame ] ,
							 pE->position.x + cx ,
							 pE->position.y + cy ,
							 cx ,
							 cy ,
							 itofix(angle) ,                // theta
							 ftofix(masterScale*scale_x)    // scale
						   ); 	
	}

	else
	{
		masked_stretch_blit( pE->frames[ pE->animationData.currentFrame ] ,
						     doubleBuffer ,
							 0 , 0 ,
							 pE->frames[ currentFrame ]->w , pE->frames[ currentFrame ]->h ,
							 pE->position.x , pE->position.y ,
							 pE->w * masterScale * scale_x,
							 pE->h * masterScale * scale_y );
	}

	if( pE->collided )
	{
		//SYS_ChangePalette( -100 , -100 , -100 );
		set_palette( pE->originalPallete );
	}
}

/*
** [ 03032008 ] Loads the frames for an entity
*/
r_boolean Ent_LoadData( BITMAP** pBitmapData , char** fileNames , int frames )
{
	PALETTE palette;
	
	for( int frame=0;frame < frames;frame++ )
	{
		pBitmapData[ frame ] = load_bitmap( fileNames[ frame ] , palette );

		if( !pBitmapData[ frame ] )
		{
			allegro_exit();
		}
	}

	return r_true;
}

/*
** [ 03092008 ] 
*/
void Ent_UpdateAnimationSequence( pEntity_t pEnt , pSequenceInfo_t pNewSequence )
{
	pEnt->animationData.currentSequence = *pNewSequence;
}

/*
** [ 03192008 ]
*/
void Ent_SetAnimationParams( pEntity_t pEnt , BITMAP** frames , int tgtRate , pSequenceInfo_t pSequence )
{
	pAnimationData_t pAnim = &pEnt->animationData;

	pAnim->rate = tgtRate;
	
	Ent_UpdateAnimationSequence( pEnt , pSequence );

	pEnt->frames = frames;
}
	
/*
** [ 04042008 ]
*/
void Ent_InitExplosion( pEntity_t pE , pExplosion_t pEx , int maxDuration )
{
	// create an explosion at a randomized location within entity.
	pEx->ent.position.x = (pE->position.x - pE->w ) + ( rand() % pE->w << 1 );	
	pEx->ent.position.y = (pE->position.y - pE->h ) + ( rand() % pE->h << 1 );

	// randomize the scale of the explosion
	pEx->scale = 0.85f +  ( 4 / ( 2 + rand() % MAX_EXPLOSION_SIZE ) );

	pEx->ent.animationData.currentSequence.currentStep = rand() % pEx->ent.animationData.currentSequence.steps;

	// randomize duration
	pEx->detonations = 1 + rand() % maxDuration;

	// flame on!!!
	pEx->ent.active = r_true;
}

r_boolean Ent_RunExplosion( pEntity_t pE , pExplosion_t pEx )
{
	pAnimationData_t pAnim = &pEx->ent.animationData;
	pSequenceInfo_t pSequence = &pAnim->currentSequence;

	// check to see if it's a burst have been completed.
	if( pSequence->currentStep == pSequence->steps - 1 )
	{
		// resituate explosion.
		pEx->ent.position.x = pE->position.x + rand() % pE->w;	
		pEx->ent.position.y = pE->position.y + rand() % pE->h;

		// rescale
		pEx->scale = 0.85f +  ( 4 / ( 2 + rand() % MAX_EXPLOSION_SIZE ) );
		
		// decrease amout of remaining detonations.
		pEx->detonations--;

		if( !pEx->detonations ) 
		{
			// deactive once all denotations have been completed.
			pEx->ent.active = r_false;
			// considering calling the parents kill routines a
		}
	}

	return pEx->ent.active;
}
	
void Ent_MoveExplosions( pExplosion_t pExplosions , int setSize , int x_diff , int y_diff )
{
	for( int explosion=0;explosion < setSize;explosion++ )
	{
		pVect2_t pV_Ex = &pExplosions[ explosion ].ent.position;

		pV_Ex->x += x_diff;
		pV_Ex->y += y_diff;
	}
}

/*
** [ 04132008 ]
** Come on. What sort of coder am I that I insist on rewriting the same old LOC when a routine would do??? 
** NOTE : this assumes that the parent's animation info has been properly initialized.
*/
void Ent_AddExplosions( pEntity_t pParentEnt , pExplosion_t pExplosionSet , int setSize , pSequenceInfo_t pExplosionSequence )
{
	for( int explosion = 0;explosion < setSize;explosion++ )
	{
		P_CreateEntity( &pExplosionSet[ explosion ].ent , 
						0 , 0 , 
						EXPLOSION_SIZE_X , EXPLOSION_SIZE_Y , 
						pParentEnt->position.x , pParentEnt->position.y , 
						pParentEnt->speed , 
						pParentEnt->frameCount , 
						ENTITY_DUMMY_TYPE );

		pExplosionSet[ explosion ].ent.active = r_false;

		Ent_SetAnimationParams( &pExplosionSet[ explosion ].ent , pParentEnt->frames , pParentEnt->animationData.rate , pExplosionSequence );
	}
}

/*
** [ 04192008 ]
** Used to keep track of where a the player , or it's projectiles have been ( to be used to progressive blending rendering )
*/
void P_TrackPositionalData( pPlayer_t pP , pVect2_t pHistoryArray , bool self )
{
	// determine the size of the passed
	int size = (self) ? (POSITIONAL_HISTORY_LENGTH) : (PROJECTILE_HISTORY_LENGTH);

	// save the current position in the first element
	pVect2_t pTrackingVect = (self) ? (&pP->ent.position) : (&pP->projectiles[0].ent.position);

	pHistoryArray[0] = *pTrackingVect;	
	
	// rotate the remaining positions.
	for( int pos=size-1;pos;pos-- )
	{
		pVect2_t pV_Curr = &pHistoryArray[ pos ];
		pVect2_t pV_Hist = &pHistoryArray[ pos - 1 ];

		*pV_Curr = *pV_Hist;		
	}
}
/*
** [ 04200839 ]
*/
void P_ChangeAmmunition( pPlayer_t pP , BulletType newAmmoType )
{
	int sequenceID = 0;
	Vect2_t ammoSize;

	// change bullet type
	pP->bulletType = newAmmoType;

	switch( newAmmoType )
	{
	case CANNON_ROUND:
	case CANNON_ROUND_PWR_2:
	case CANNON_ROUND_PWR_3:
	case CANNON_ROUND_PWR_MAX:
		{
			sequenceID = PLAYER_CANNON_SEQUENCE_01;
			ammoSize = ammoSizes[ CANNON_AMMO ];
			break;
		}

	case PLASMA_BEAM:
	case PLASMA_BEAM_PWR_2:
	case PLASMA_BEAM_PWR_3:
	case PLASMA_BEAM_PWR_MAX:
		{
			sequenceID = PLAYER_CANNON_SEQUENCE_01;
			ammoSize = ammoSizes[ CANNON_AMMO ];
			break;
		}

	case SPREAD_BURST:
	case SPREAD_BURST_PWR_2:
	case SPREAD_BURST_PWR_3:
	case SPREAD_BURST_PWR_MAX:
		{
			sequenceID = PLAYER_CANNON_SEQUENCE_01;
			ammoSize = ammoSizes[ CANNON_AMMO ];
			break;
		}

	case HOMING_BEAM:
	case HOMING_BEAM_PWR_2:
	case HOMING_BEAM_PWR_3:
	case HOMING_BEAM_PWR_MAX:
		{
			sequenceID = PLAYER_HOMING_LASER_SEQUENCE_01;
			//sequenceID = PLAYER_CANNON_SEQUENCE_01;
			ammoSize = ammoSizes[ HOMING_AMMO ];
			break;
		}

	case DROID_BEAM:
	case DROID_BEAM_PWR_2:
	case DROID_BEAM_PWR_3:
	case DROID_BEAM_PWR_MAX:
		{
			sequenceID = PLAYER_CANNON_SEQUENCE_01;
			ammoSize = ammoSizes[ CANNON_AMMO ];
			break;
		}
	}
	
	//for( int bullet = 0;bullet < pP->currentBullet;bullet++ )
	for( int bullet = 0;bullet < MAX_PROJECTILES;bullet++ )
	{
		/*P_CreateEntity( &pP->projectiles[ bullet ].ent ,
						0 , 0 , 
						ammoSize.x , ammoSize.y , 
						pP->ent.position.x , pP->ent.position.y , 
						5 , 
						PLAYER_FRAMES , 
						ENTITY_DUMMY_TYPE ); */
		
		// either do the above , or this way 
		pP->projectiles[ bullet ].ent.maxBounds.x = ammoSize.x;
		pP->projectiles[ bullet ].ent.maxBounds.y = ammoSize.y;

		// change bullet type internally
		pP->projectiles[ bullet ].bulletType =  newAmmoType;
		
		Ent_SetAnimationParams( &pP->projectiles[ bullet ].ent , playerFrames , PLAYER_ANIMATION_RATE , &playerSequences[ sequenceID ] );
	}
}

/*
** [ 04262008 ] Weapon fx renderers.
*/
static void P_DrawCannonRound( pPlayer_t pPlayer , int bulletIndex )
{
	pBullet_t pBullet = &pPlayer->projectiles[ bulletIndex ];

	// animate
	Ent_Animate( &pBullet->ent );

	// draw according to the power-level.
	switch( pPlayer->bulletType )
	{
	case CANNON_ROUND:
	case CANNON_ROUND_PWR_2:
	case CANNON_ROUND_PWR_3:
	case CANNON_ROUND_PWR_MAX:
		{
			Ent_Draw( &pBullet->ent , 1 , 1 , 0 );
			break;
		}
	}
}

static void P_DrawHomingLaser( pPlayer_t pPlayer , int bulletIndex )
/* NOTE : This hacks the animationdef struct , and is therefore dangerous. It changes frames of a sequence , but maintains
          the integrity otherwise. */
{
	pBullet_t pBullet = &pPlayer->projectiles[ bulletIndex ];
	
	// determine which frames to be used for the front/center/tail segments of laser
	int headFrame = pPlayer->ent.animationData.currentFrame;
	int tailFrame = headFrame + 1;
	int centFrame = centFrame + 1;
	
	// HACK!! depends on Ent_Animate to retain sequence info... May need to be changed!!!
	int* pTgtFrame = &pPlayer->ent.animationData.currentFrame;
	
	// used to rotate segment
	fixed fixedTheta = 0;

	// temporary entity
	Entity_t tempEntity;

	// angle between player and target 
	float theta =  0;

	// animate
	Ent_Animate( &pBullet->ent );

	for( int historic=0;historic < PROJECTILE_HISTORY_LENGTH;historic++ )
	{
		if( !historic ) // head segment 
		{
			*pTgtFrame = headFrame;
		}

		else if( historic+1 == PROJECTILE_HISTORY_LENGTH ) // tail segment
		{
			*pTgtFrame = tailFrame;
		}
		
		else
		{
			


		}	

		// draw based on the current power level and computed factors.
		switch( pBullet->bulletType )
		{
		case HOMING_BEAM:
		case HOMING_BEAM_PWR_2:
		case HOMING_BEAM_PWR_3:
		case HOMING_BEAM_PWR_MAX:
			{
				P_CreateEntity( &tempEntity , 
								0 , 0 , 
								pBullet->ent.w , pBullet->ent.h , 
								pPlayer->projectileHistory[ historic ].x , pPlayer->projectileHistory[ historic ].y , 
								0 , 
								0 , 
								ENTITY_DUMMY_TYPE );

				Vect2_t bulletDirection = { pPlayer->projectileHistory[0].x - pPlayer->projectileHistory[1].x , 
											pPlayer->projectileHistory[0].y - pPlayer->projectileHistory[1].y };

				// NOTE : this relies on an unsafe assumption that the target is *always* running from right to left ( vertical movement is disregarded ).
				Vect2_t targetDirection = { (pPlayer->ent.target.x - pPlayer->ent.speed) - pPlayer->ent.target.x , 
											pPlayer->ent.target.y };

				//bulletDirection = Normalize( &bulletDirection );
				//targetDirection = Normalize( &targetDirection );

				
				theta = DotProduct( &bulletDirection , &targetDirection );
				fixedTheta = asin( (fixed)theta );
				//fixedTheta = (fixed)theta%256;
				
				Ent_SetAnimationParams( &tempEntity , pPlayer->ent.frames , PLAYER_ANIMATION_RATE , &pBullet->ent.animationData.currentSequence );

				Ent_Animate( &tempEntity );
				Ent_Draw( &tempEntity , 1 , 1, theta );

				G_WriteToDebugWindow( fixedTheta , "%i" , tempEntity.position.x , tempEntity.position.y );
				
				break;
			}
		}
	}
}

static void P_DrawDroidBeam( pPlayer_t pPlayer )
{
	switch( pPlayer->bulletType )
	{
	case DROID_BEAM:
	case DROID_BEAM_PWR_2:
	case DROID_BEAM_PWR_3:
	case DROID_BEAM_PWR_MAX:
		{

			break;
		}
	}
}

