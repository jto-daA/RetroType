
#include <math.h>

#include "common.h"
#include "cocci.h"   // for state routines
#include "boss_1.h"
#include "player.h"

extern float masterScale;
extern float   sineLookup[];
extern float cosineLookup[];
extern volatile int msecTicker;
extern volatile int  secTicker;
extern BITMAP* doubleBuffer; 

static BITMAP* boss_1_frames[ BOSS_ONE_FRAMES ];

static char* fileNames[] = 
{
	"content/gfx/enemies/boss_1/b1_e1.bmp" , 
	"content/gfx/enemies/boss_1/b1_b1.bmp" ,
	"content/gfx/enemies/boss_1/b1_eb1.bmp" , 
	"content/gfx/enemies/boss_1/b1_m1.bmp" ,
	"content/gfx/enemies/boss_1/b1_m2.bmp" , 
	
	"content/gfx/enemies/boss_1/b1_m3.bmp" , 
	"content/gfx/enemies/boss_1/b1_m4.bmp" , 
	"content/gfx/enemies/boss_1/b1_tear1.bmp" , 
	"content/gfx/enemies/boss_1/b1_tear2.bmp" , 
	"content/gfx/enemies/boss_1/b1_proj1.bmp" , 

	"content/gfx/enemies/boss_1/b1_proj2.bmp" , 
	"content/gfx/enemies/boss_1/b1_proj3.bmp" , 
	"content/gfx/enemies/boss_1/b1_proj4.bmp" , 
	"content/gfx/enemies/expl_1.bmp" , 
	"content/gfx/enemies/expl_2.bmp" , 
	
	"content/gfx/enemies/expl_3.bmp" , 
	"content/gfx/enemies/expl_4.bmp" , 
	"content/gfx/enemies/expl_5.bmp" , 
	"content/gfx/enemies/expl_6.bmp" , 
	"content/gfx/enemies/expl_7.bmp" , 
	
	"content/gfx/enemies/expl_8.bmp" 
};	

static int bossRunningSequence_01[] = 
{
	BOSS_ONE_FRAME_01 , 
	BOSS_ONE_FRAME_01 , 
	BOSS_ONE_FRAME_01 
};

static int eyeRunningSequence_01[] = 
{
	BOSS_ONE_FRAME_00 , 
	BOSS_ONE_FRAME_00 , 
	BOSS_ONE_FRAME_00 
};

static int eyebrowRunningSequence_01[] = 
{
	BOSS_ONE_FRAME_02 , 
	BOSS_ONE_FRAME_02
};

static int mouthRunningSequence_01[] = 
{
	BOSS_ONE_FRAME_03 , 
	BOSS_ONE_FRAME_04 , 
	BOSS_ONE_FRAME_05 , 
	BOSS_ONE_FRAME_04 , 
	BOSS_ONE_FRAME_03 , 
	BOSS_ONE_FRAME_04 ,
	BOSS_ONE_FRAME_05 , 
	BOSS_ONE_FRAME_04 
};

static int projectileSequence[] = 
{
	BOSS_ONE_FRAME_09 , 
	BOSS_ONE_FRAME_10 , 
	BOSS_ONE_FRAME_11 , 
	BOSS_ONE_FRAME_12 , 
	BOSS_ONE_FRAME_11 , 
	BOSS_ONE_FRAME_10 
};

static int orbTearSequence[] = 
{
	BOSS_ONE_FRAME_07 , 
	BOSS_ONE_FRAME_08 , 
	BOSS_ONE_FRAME_07 , 
	BOSS_ONE_FRAME_08
};

static int mouthBeserkerSequence[] = 
{
	BOSS_ONE_FRAME_06 , 
	BOSS_ONE_FRAME_06 
};		

static int orbExplosionSequence[] = 
{
	BOSS_ONE_FRAME_13 , 
	BOSS_ONE_FRAME_14 , 
	BOSS_ONE_FRAME_15 , 
	BOSS_ONE_FRAME_16 , 
	BOSS_ONE_FRAME_17 , 
	BOSS_ONE_FRAME_18 , 
	BOSS_ONE_FRAME_19 , 
	BOSS_ONE_FRAME_20
};

static int bossOneExplosionSequence[] = 
{
	BOSS_ONE_FRAME_13 , 
	BOSS_ONE_FRAME_14 , 
	BOSS_ONE_FRAME_15 , 
	BOSS_ONE_FRAME_16 , 
	BOSS_ONE_FRAME_17 , 
	BOSS_ONE_FRAME_18 , 
	BOSS_ONE_FRAME_19 , 
	BOSS_ONE_FRAME_20
};

static SequenceInfo_t boss_1_sequences[] = 
{
	{                BOSS_ONE_RUNNING_SEQUENCE_STEPS , 0 ,    bossRunningSequence_01 , 0 , 0 , -1 } , 
	{            BOSS_ONE_EYE_RUNNING_SEQUENCE_STEPS , 0 ,     eyeRunningSequence_01 , 0 , 0 , -1 } ,
	{       BOSS_ONE_EYE_BROW_RUNNING_SEQUENCE_STEPS , 0 , eyebrowRunningSequence_01 , 0 , 0 , -1 } , 
	{          BOSS_ONE_MOUTH_RUNNING_SEQUENCE_STEPS , 0 ,   mouthRunningSequence_01 , 0 , 0 , -1 } , 
	{         BOSS_ONE_BULLET_RUNNING_SEQUENCE_STEPS , 0 ,        projectileSequence , 0 , 0 , -1 } , 
	{     BOSS_ONE_ORB_BULLET_RUNNING_SEQUENCE_STEPS , 0 ,           orbTearSequence , 0 , 0 , -1 } ,
	{ BOSS_ONE_BESERKER_MOUTH_RUNNING_SEQUENCE_STEPS , 0 ,     mouthBeserkerSequence , 0 , 0 , -1 } , 
	{          BOSS_ONE_ORB_EXPLOSION_SEQUENCE_STEPS , 0 ,      orbExplosionSequence , 0 , 0 , -1 } , 
	{              BOSS_ONE_EXPLOSION_SEQUENCE_STEPS , 0 ,  bossOneExplosionSequence , 0 , 0 , -1 }
};

void B1_Spawn( pBoss_1t pB1 , pSpawnPoint_t pSpawnPoint )
{
	P_CreateEntity( &pB1->ent , 0 , 0 , BOSS_ONE_SIZE_X , BOSS_ONE_SIZE_Y , SCREEN_W , pSpawnPoint->mapCoords.y , BOSS_ONE_SPEED , BOSS_ONE_FRAMES , ENTITY_TYPE_BOSS_ONE);
	
	// eyebrows 
	P_CreateEntity( &pB1->eyebrows ,
					0 , 0 ,
					BOSS_ONE_EYE_BROW_SIZE_X ,
					BOSS_ONE_EYE_BROW_SIZE_Y ,
					pB1->ent.position.x , 
					pB1->ent.position.y - BOSS_ONE_EYE_BROW_OFFSET , 
					BOSS_ONE_SPEED , 
					BOSS_ONE_FRAMES , 
					ENTITY_DUMMY_TYPE );

	// mouth
	P_CreateEntity( &pB1->mouth , 
					0 , 0 , 
					BOSS_ONE_MOUTH_SIZE_X , 
					BOSS_ONE_MOUTH_SIZE_Y , 
					pB1->ent.position.x , 
					pB1->ent.position.y + ( pB1->ent.h - BOSS_ONE_MOUTH_OFFSET ), 
					BOSS_ONE_SPEED , 
					BOSS_ONE_FRAMES , 
					ENTITY_DUMMY_TYPE );
	// projectiles
	for( int bossProjectile=0;bossProjectile <  BOSS_ONE_PROJECTILES;bossProjectile++ )
	{
		P_CreateEntity( &pB1->projectiles[ bossProjectile ] , 
						0 , 0 , 
						BOSS_ONE_BULLET_SIZE_X , 
						BOSS_ONE_BULLET_SIZE_Y , 
						0 , 0 , 
						BOSS_ONE_ORB_PROJECTILE_SPEED , 
						BOSS_ONE_FRAMES , 
						ENTITY_DUMMY_TYPE );

		pB1->projectiles[ bossProjectile ].active = r_false;
		
		Ent_SetAnimationParams( &pB1->projectiles[ bossProjectile ] , boss_1_frames , BOSS_ONE_ANIMATION_RATE , &boss_1_sequences[ BOSS_ONE_BULLET_RUNNING_SEQUENCE ] );
	}

	// animation stuffs.
	Ent_SetAnimationParams( &pB1->ent ,      boss_1_frames , BOSS_ONE_ANIMATION_RATE , &boss_1_sequences[ BOSS_ONE_RUNNING_SEQUENCE ] );
	Ent_SetAnimationParams( &pB1->eyebrows , boss_1_frames , BOSS_ONE_ANIMATION_RATE , &boss_1_sequences[ BOSS_ONE_EYE_BROW_RUNNING_SEQUENCE ] );
	Ent_SetAnimationParams( &pB1->mouth ,    boss_1_frames , BOSS_ONE_ANIMATION_RATE , &boss_1_sequences[ BOSS_ONE_MOUTH_RUNNING_SEQUENCE ] );

	// explosions
	Ent_AddExplosions( &pB1->ent , pB1->explosions , BOSS_ONE_EXPLOSIONS , &boss_1_sequences[ BOSS_ONE_EXPLOSION_SEQUENCE ] );

	// center of boss
	Vect2_t center = { 
						( pB1->ent.position.x + (pB1->ent.w >> 1) ) - BOSS_ONE_CENTER_OFFSET , 
					    ( pB1->ent.position.y + (pB1->ent.h >> 1) ) - BOSS_ONE_CENTER_OFFSET
					};
	
	// arrange the orbs around the boss
	int spacing = float(TRIG_TABLE_SIZE / BOSS_ONE_ORBS);

	for( int orb=0;orb<BOSS_ONE_ORBS;orb++ )
	{
		// determine where orb is around the boss
		float angle = orb * spacing;
		
		// compute an offset along a circle around the boss
		int orbX = center.x * cosineLookup[ (int)angle ] - center.y *   sineLookup[ (int)angle ] , 
			orbY = center.x *   sineLookup[ (int)angle ] + center.y * cosineLookup[ (int)angle ];
		
		P_CreateEntity( &pB1->orbs[ orb ].ent , 
						0 , 0 ,
						BOSS_ONE_ORB_SIZE_X , BOSS_ONE_ORB_SIZE_Y , 
						orbX + pB1->ent.position.x ,
						orbY + pB1->ent.position.y , 
						BOSS_ONE_ORB_LINEAR_SPEED , 
						0 , 
						ENTITY_TYPE_BOSS_ONE_ORB );
		// animation stuffs.
		Ent_SetAnimationParams( &pB1->orbs[ orb ].ent , boss_1_frames ,BOSS_ONE_ANIMATION_RATE , &boss_1_sequences[ BOSS_ONE_EYE_RUNNING_SEQUENCE ] );

		Ent_AddExplosions( &pB1->orbs[ orb ].ent , pB1->orbs[ orb ].explosions , BOSS_ONE_ORB_EXPLOSIONS , &boss_1_sequences[ BOSS_ONE_ORB_EXPLOSION_SEQUENCE ] );

		// get the orb bullets
		for( int orbBullet=0;orbBullet < BOSS_ONE_ORB_PROJECTILES;orbBullet++ )
		{
			P_CreateEntity( &pB1->orbs[ orb ].projectiles[ orbBullet ] , 
							0 , 0 , 
							BOSS_ONE_ORB_BULLET_SIZE_X , 
							BOSS_ONE_ORB_BULLET_SIZE_Y , 
							pB1->orbs[ orb ].ent.position.x , 
							pB1->orbs[ orb ].ent.position.y , 
							BOSS_ONE_ORB_PROJECTILE_SPEED , 
							BOSS_ONE_FRAMES , 
							ENTITY_DUMMY_TYPE );

			// not so fast there buddy.
			pB1->orbs[ orb ].projectiles[ orbBullet ].active  = r_false;

			Ent_SetAnimationParams( &pB1->orbs[ orb ].projectiles[ orbBullet ] , boss_1_frames , BOSS_ONE_ANIMATION_RATE , &boss_1_sequences[ BOSS_ONE_ORB_BULLET_RUNNING_SEQUENCE ] );
		}

		
		pB1->orbs[ orb ].rotationAngle = angle;

		// set initial states
		SetStates( &pB1->orbs[ orb ].state , ORB_STATE_INTRO | ORB_STATE_ORBITING );
		Ent_InitHealthStats( &pB1->orbs[ orb ].ent , ORB_HEALTH , ORB_DAMAGE_RESISTANCE , ORB_STRENGTH , ORB_PROJECTILE_STRENGTH );
	}

	// explosions 
	

	// set initial states
	SetStates( &pB1->state , BOSS_ONE_STATE_INTRO | BOSS_ONE_STATE_ROAMING | BOSS_ONE_STATE_BLIND );
	Ent_InitHealthStats( &pB1->ent , BOSS_ONE_HEALTH , BOSS_ONE_DAMAGE_RESISTANCE , BOSS_ONE_STRENGTH , BOSS_ONE_PROJECTILE_STRENGTH );

	pB1->activeOrbs = BOSS_ONE_ORBS;
}

r_boolean B1_Kill( pBoss_1t pB1 )
{
	return r_true;
}

void B1_Destroy( pBoss_1t pB1 )
{

}

r_boolean B1_Run( pBoss_1t pB1 , pPlayer_t pP )
{
	int state = B1_DetermineCurrentState( pB1 , pP );

	if( StateSet( pB1->state , BOSS_ONE_STATE_INTRO ) )
	{
		B1_RunIntro( pB1 );
	}

	if( StateSet( pB1->state , BOSS_ONE_STATE_ROAMING ) )
	{
		B1_RunRoaming( pB1 , pP );
	}

	if( StateSet( pB1->state , BOSS_ONE_STATE_AGGRESIVE ) )
	{
		B1_RunAggresive( pB1 , pP );

		for( int bullet=0,inactiveBullets=0;bullet < BOSS_ONE_PROJECTILES;bullet++ )
		{
			if( !pB1->projectiles[ bullet ].active )
			{
				inactiveBullets++;
			}
		}

		if( inactiveBullets == BOSS_ONE_PROJECTILES - 1)
		// stop being aggresive if all bullets are deactivated.
		{
			ClearStates( &pB1->state , BOSS_ONE_STATE_AGGRESIVE );
		}	
		
		pB1->inactiveBullets = inactiveBullets;
	}

	if( StateSet( pB1->state , BOSS_ONE_STATE_BESERKER ) ) 
	{
		B1_RunBeserker( pB1 , pP );
	}

		// run orbs
	for( int orb=0;orb < BOSS_ONE_ORBS;orb++ )
	{
		if( !B1_RunOrb( pB1 , &pB1->orbs[ orb ] , pP ) )
		{

		}

		else
		{

		}
	}

	if( pB1->ent.collided )
	{
		for( int explode=0;explode < BOSS_ONE_EXPLOSIONS;explode++ )
		{
			Ent_InitExplosion( &pB1->ent , &pB1->explosions[ explode ] , BOSS_ONE_EXPLOSION_LENGTH );
		}
	}

	return r_true;
}

static r_boolean B1_RunOrb( pBoss_1t pB1 , pOrb_t pO , pPlayer_t pP )
// wee... my first FSM implementation
{
	if( pO->ent.healthStats.health <= 0 && pO->ent.healthStats.health != INVINCEABLE )
	{
		if( StateSet( pO->state , ORB_STATE_ORBITING ) )
		// instead of killing , respawn into a differnet state
		{
			Ent_InitHealthStats( &pO->ent , ORB_HEALTH , ORB_DAMAGE_RESISTANCE , ORB_STRENGTH , ORB_PROJECTILE_STRENGTH );
			ClearStates( &pO->state , ORB_STATES );
			
			SetStates( &pO->state , ORB_STATE_LONELY );
			SetStates( &pB1->state , BOSS_ONE_STATE_AGGRESIVE ); // boss gets mad if an orb is destroyed.
		}
		
		else
		{
			if( pO->ent.active )
			{
				pB1->activeOrbs--;
			}

			pO->ent.active = r_false;

			return r_false;
		}
	}

	// set the current state
	int state = Orb_DetermineCurrentState( pO , pB1 , pP );
	
	// perform the set state(s)
	if( StateSet( pO->state , ORB_STATE_LONELY ) )
	{
		Orb_RunLonely( pO , pB1 , pP );
	}

	if( StateSet( pO->state , ORB_STATE_SHOOTING ) )
	{
		Orb_RunShooting( pO , pB1 , pP );
	}

	if( StateSet( pO->state , ORB_STATE_INTRO ) )
	{
		Orb_RunIntro( pO , pB1 , pP );
	}

	if( StateSet( pO->state , ORB_STATE_PROBING ) )
	{
		Orb_RunProbing( pO , pB1 , pP );
	}

	if( StateSet( pO->state , ORB_STATE_SEARCHING ) )
	{
		Orb_RunSearching( pO , pB1 , pP );
	}

	else if( StateSet( pO->state , ORB_STATE_ORBITING ) )
	{
		Orb_RunOrbit( pO , pB1 , pP );
	}	
	
	return r_true;
}

void B1_Draw( pBoss_1t pB1 )
{
	int color = pB1->ent.collided ? makecol( 255 , 255 , 255 ) : makecol( 5 , 255 ,0 );
	
	// draw body
	G_WriteToDebugWindow( pB1->ent.healthStats.health , "%i" , pB1->ent.position.x + pB1->ent.w>>1 , pB1->ent.position.y + pB1->ent.h>>1);

	Ent_Animate( &pB1->ent );
	Ent_Draw( &pB1->ent , 1 , 1 , 0 );

	// draw explosions 
	for( int explode = 0;explode < BOSS_ONE_EXPLOSIONS;explode++ )
	{
		if( pB1->explosions[ explode ].ent.active )
		{
			pExplosion_t pEX = &pB1->explosions[ explode ];

			Ent_RunExplosion( &pB1->ent , pEX );
			Ent_Animate( &pEX->ent );
			Ent_Draw( &pEX->ent , pEX->scale , pEX->scale , 0 );
		}
	}

	// draw eyebrows
	Ent_Animate( &pB1->eyebrows );
	Ent_Draw( &pB1->eyebrows , 1 , 1 , 0 );

	// draw mouth
	Ent_Animate( &pB1->mouth );
	Ent_Draw( &pB1->mouth , 1 , 1 , 0 );

	// draw particle beam
	if( pB1->particleBeam.active )
	{
		//drawing_mode( DRAW_MODE_TRANS , doubleBuffer ,
		rectfill( doubleBuffer , 0 , pB1->ent.position.y , pB1->ent.position.x , pB1->ent.position.y + pB1->ent.h , makecol( 0 , 200 ,200 ) );
		line( doubleBuffer , pB1->ent.position.x , pB1->ent.position.y , 0 , pB1->ent.position.y , makecol( 255 , 255 , 255 ) );
		line( doubleBuffer , pB1->ent.position.x , pB1->ent.position.y + pB1->ent.h , 0 , pB1->ent.position.y + pB1->ent.h , makecol( 255 , 255 , 255 ) );
	}

	// draw projectiles
	if( pB1->projectiles[ 0 ].active )
	{
		for( int bullet=0;bullet < BOSS_ONE_PROJECTILES;bullet++ )
		{
/*			line( doubleBuffer , pB1->ent.position.x + ( pB1->ent.w >> 1  ) , 
								 pB1->ent.position.y + ( pB1->ent.h >> 1 ) , 
								 pB1->projectiles[ bullet ].position.x , 
								 pB1->projectiles[ bullet ].position.y , 
								 makecol( 255 , 255 , 0 ) 
								 ); */
			Ent_Animate( &pB1->projectiles[ bullet ] );
			Ent_Draw( &pB1->projectiles[ bullet ] , 1 , 1 , 0 );
			
		}
	}

	// draw orbs
	for( int orb=0;orb < BOSS_ONE_ORBS;orb++ )
	{
		if( pB1->orbs[ orb ].ent.active )
		{
			B1_DrawOrb( &pB1->orbs[ orb ] );

			for( int explode = 0;explode < BOSS_ONE_ORB_EXPLOSIONS;explode++ )
			{
				if( pB1->orbs[ orb ].explosions[ explode ].ent.active )
				{
					pExplosion_t pEX = &pB1->orbs[ orb ].explosions[ explode ];

					Ent_RunExplosion( &pEX->ent , pEX );
					Ent_Animate( &pEX->ent );
					Ent_Draw( &pEX->ent , pEX->scale , pEX->scale , 0 );
				}
			}
		}
	}

	G_WriteToDebugWindow( pB1->activeOrbs , "%i" , 0 , 0 );
}

static void B1_DrawOrb( pOrb_t pO  )
{
	int orbColor = pO->ent.collided ? makecol( 255 , 255 , 255 ) : makecol( 20 , 20 , 20 );

	Ent_Animate( &pO->ent );
	Ent_Draw( &pO->ent , 1 , 1 , 0 /*pO->rotationAngle*/ );

	// G_WriteToDebugWindow( pO->rotationAngle , "%d" , pO->ent.position.x , pO->ent.position.y );
	G_WriteToDebugWindow( pO->ent.healthStats.health , "%d" , pO->ent.position.x , pO->ent.position.y );

	// draw projectiles.
	for( int bullet=0;bullet < BOSS_ONE_ORB_PROJECTILES;bullet++ )
	{
		if( pO->projectiles[ bullet ].active )
		{
			
			// only draw when the bullet is in front of the orb
			if( pO->projectiles[ bullet ].position.x < pO->ent.position.x )
			{
				Ent_Animate( &pO->projectiles[ bullet ] );
				Ent_Draw( &pO->projectiles[ bullet ] , 1 , 1 , 0 );
			}
		}
	}
}

// ================= [ STATE ROUTINES ]===================================

int Orb_DetermineCurrentState( pOrb_t pO , pBoss_1t pB1 , pPlayer_t )
{
	// for now, orbs are unable to determine thier own state...
	return pO->state;
}

r_boolean Orb_RunIntro( pOrb_t pO , pBoss_1t pB1 , pPlayer_t pP )
{
	return r_true;
}

r_boolean Orb_RunLonely( pOrb_t pO , pBoss_1t pB1 , pPlayer_t pP )
// track the player , and shoot at it when it's time.
{
	// determines if the orb is in range of the player
	int range = abs( pO->ent.position.x - pP->ent.position.x );
	
// vertical movment
	if( pO->ent.position.y > pP->ent.position.y )
	{
		pO->ent.position.y -= pO->ent.speed;
	}

	else if( pO->ent.position.y < pP->ent.position.y )
	{	
		pO->ent.position.y += pO->ent.speed;
	}

	else
	// shoot at the player when directly arcoss from it
	{
		if( !pO->projectiles[ BOSS_ONE_ORB_PROJECTILES - 1 ].active )
		// shoot more bullets only if the last bullet is inactive
		{
			SetStates( &pO->state , ORB_STATE_SHOOTING );
			
			for( int bullet=0;bullet < BOSS_ONE_ORB_PROJECTILES;bullet++ )
			{	
				pO->projectiles[ bullet ].active = r_true;
				pO->projectiles[ bullet ].position.x = pO->ent.position.x + ( bullet * pO->ent.w<<1 );
				pO->projectiles[ bullet ].position.y = pO->ent.position.y + 5; // offset added for testing ( fixes off-collisions )
			}
		}
	}
// horizontal movement

	if( range > BOSS_ONE_ORB_RANGE )
	{
		pO->ent.position.x -= pO->ent.speed;

	}	

	else if( range < BOSS_ONE_ORB_RANGE )
	{
		pO->ent.position.x += pO->ent.speed;
	}

	else 
	{

	}

	return pO->ent.active;
}

r_boolean Orb_RunProbing( pOrb_t pO , pBoss_1t pB1 , pPlayer_t pP )
// confirm the presence of the player, and then return to host
{
	if( !StateSet( pO->state , ORB_STATE_FOUND_TARGET ) )
	// search for target if it's not alread found
	{
		int range = abs( pO->ent.position.x - pP->ent.position.x );
		
		if( range > BOSS_ONE_ORB_RANGE )
		// move orb within range of the player
		{
			pO->ent.position.x -= pO->ent.speed;
		}

		else
		{
			SetStates( &pO->state , ORB_STATE_FOUND_TARGET );
		}
	}

	else
	// flee the scene at an accelerated  rate back to the host once target has been found
	{
		pO->ent.position.x += pO->ent.speed<<3;

	}	

	// fail when the probe has located it's target and returned to it's host ( offscreen )
	return ( StateSet( pO->state , ORB_STATE_FOUND_TARGET ) && pO->ent.position.x > ( SCREEN_W + pO->ent.w ) ) ? ( r_false ) : ( r_true );
}

r_boolean Orb_RunOrbit( pOrb_t pO , pBoss_1t pB1 , pPlayer_t pP )
// make an orbit around the boss
{
		// compute center of boss 
	Vect2_t center = { 
						(pB1->ent.position.x + (pB1->ent.w >> 1)) , 
						(pB1->ent.position.y + (pB1->ent.h >> 1)) 
					};	
	
	if( pO->rotationAngle < TRIG_TABLE_SIZE - BOSS_ONE_ORB_RADIAL_SPEED )
	{
		pO->rotationAngle += BOSS_ONE_ORB_RADIAL_SPEED;
	}

	else
	{
		pO->rotationAngle = 0;
	}

	// rotate along circle
	pO->ent.position.x = center.x * cosineLookup[ (int)pO->rotationAngle ] - center.y *    sineLookup[ (int)pO->rotationAngle ];
	pO->ent.position.y = center.x *   sineLookup[ (int)pO->rotationAngle ] + center.y *  cosineLookup[ (int)pO->rotationAngle ];
	
	// radius hack	
	pO->ent.position.x /= 3;
	pO->ent.position.y /= 3;
	
	// offset orgin to that it is translated to the center of the boss
	pO->ent.position.x += ( center.x );
	pO->ent.position.y += ( center.y );

	return pO->ent.active;
}

r_boolean Orb_RunShooting( pOrb_t pO , pBoss_1t pB1 , pPlayer_t pP )
// update the fired bullets.
{
	for( int bullet=0;bullet < BOSS_ONE_ORB_PROJECTILES;bullet++ )
	{
		pO->projectiles[ bullet ].position.x -= BOSS_ONE_ORB_PROJECTILE_SPEED;
		pO->projectiles[ bullet ].position.y += sineLookup[ (int)pO->rotationAngle ];

		if( pO->projectiles[ bullet ].position.x < 0 )
		// kill if offscreen...
		{
			pO->projectiles[ bullet ].active = r_false;
		}

		else
		{
			if( pO->projectiles[ bullet ].position.x < pO->ent.position.x )
			// only test for collision for bullets in front of the orb
			{
				if( Ent_TestForCollision( &pP->ent , &pO->projectiles[ bullet ] ) )
				// or if collision is made with the player.
				{
					Ent_InflictDamage( pP , &pO->ent , r_true , r_true );
					pO->projectiles[ bullet ].active = r_false;
				}			
			}
		}
	}
	
	// fail only when the last projectile is deactivated.
	return pO->projectiles[ BOSS_ONE_ORB_PROJECTILES - 1 ].active;
}

r_boolean Orb_RunSearching( pOrb_t pO , pBoss_1t pB1 , pPlayer_t pP )
{
	// positions of the eye sockets
	Vect2_t leftTarget =  { pB1->ent.position.x, pB1->ent.position.y + pB1->ent.h * 0.20};
	Vect2_t rightTarget = { pB1->ent.position.x + pB1->ent.w * .5f ,pB1->ent.position.y + pB1->ent.h * .21 };
	pVect2_t pTarget = NULL;

	if( pO == &pB1->orbs[ pB1->eyeballs[ BOSS_ONE_LEFT_EYEBALL ] ] )
	// left eye ( R.I.P )
	{
		pTarget = &leftTarget;
	}

	else
	{
		pTarget = &rightTarget;
	}
	// horizontal			
	if( pO->ent.position.x < pTarget->x )
	{
		pO->ent.position.x += pO->ent.speed << 1;
	}

	else if( pO->ent.position.x > pTarget->x )
	{
		pO->ent.position.x -= pO->ent.speed << 1;
	}

	else
	{

	}

	//vertical
	if( pO->ent.position.y < pTarget->y )
	{
		pO->ent.position.y += pO->ent.speed << 1;
	}

	else if( pO->ent.position.y > pTarget->y )
	{
		pO->ent.position.y -= pO->ent.speed << 1;
	}

	else
	{

	}
	
	return StateSet( pO->state , ORB_STATE_SEARCHING );
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


int B1_DetermineCurrentState( pBoss_1t pB1 , pPlayer_t pP )
{		
	static r_boolean cureBlindness = r_false;

	// intro
	if( pB1->ent.position.x > SCREEN_W - ( pB1->ent.position.x + pB1->ent.w ) ) 
	{
		ClearStates( &pB1->state , BOSS_ONE_STATE_INTRO );
	}

	// roaming ( always roaming ) once the intro is complete
	if( !StateSet( pB1->state , BOSS_ONE_STATE_INTRO ) )
	{
		SetStates( &pB1->state , BOSS_ONE_STATE_ROAMING );
	}

	// aggresive ( set when an orb is destroyed )

	// beserker
	if( !StateSet( pB1->state , BOSS_ONE_STATE_BLIND ) )
	{
		// go into beserker mode if able to see.
		SetStates( &pB1->state , BOSS_ONE_STATE_BESERKER );
		Ent_UpdateAnimationSequence( &pB1->mouth , &boss_1_sequences[ BOSS_ONE_BESERKER_MOUTH_RUNNING_SEQUENCE ] );
	}

	// blindness
	if( pB1->activeOrbs == 2 && !cureBlindness ) // first time I've actually used NO_OP
	{
		// boss can see properly if he has 2 eyes.
		ClearStates( &pB1->state , BOSS_ONE_STATE_BLIND );

		// ugly code...
		if( !cureBlindness )
		{
			int* eyeballs = pB1->eyeballs;
			
			for( int orb=0;orb < BOSS_ONE_ORBS;orb++ )
			{
				if( pB1->orbs[ orb ].ent.active )
				{
					// make the 2 remaining orbs indestructable
					Ent_InitHealthStats( &pB1->orbs[ orb ].ent , INVINCEABLE , INVINCEABLE , ORB_STRENGTH , ORB_PROJECTILE_STRENGTH );
					ClearStates( &pB1->orbs[ orb ].state , ORB_STATES );
					SetStates( &pB1->orbs [ orb ].state , ORB_STATE_SEARCHING );
					*eyeballs = orb;
					eyeballs++;
				}
			}

			cureBlindness = r_true;
		}
	}
	
	return  pB1->state;
}

static int B1_SetOrbStates( pBoss_1t pB1 , int state )
{
	int activeOrbs = 0;
	
	for( int orb=0;orb < BOSS_ONE_ORBS;orb++ )
	{
		if( pB1->orbs[ orb ].ent.active )
		{
			SetStates( &pB1->orbs[ orb ].state , state );
			activeOrbs++;
		}
	}

	G_WriteToDebugWindow( activeOrbs , "%d" , 0 , 0 );

	return activeOrbs;
}

static int B1_ClearOrbStates( pBoss_1t pB1 , int state )
{
	int activeOrbs = 0;

	for( int orb=0;orb < BOSS_ONE_ORBS;orb++ )
	{
		if( pB1->orbs[ orb ].ent.active )
		{
			ClearStates( &pB1->orbs[ orb ].state , state );
			activeOrbs++;
		}
	}

	return activeOrbs;
}

r_boolean B1_RunIntro( pBoss_1t pB1 )
// slowly move boss onscreen
{
	if( pB1->ent.position.x > SCREEN_W - ( pB1->ent.w ) )
	{
		pB1->ent.position.x -= pB1->ent.speed >> 3;
	}

	else
	{
		return r_false;
	}
	
	return r_true;
}

r_boolean B1_RunRoaming( pBoss_1t pB1 , pPlayer_t pP )
// wander around
{
	static int i=-1;
	static int trigIndex=0;

	pB1->ent.position.x += i;
	pB1->eyebrows.position.x += i;
	pB1->mouth.position.x += i;

	if( pB1->ent.position.x < 0 )
	{
		i=1;
	}

	else if( pB1->ent.position.x > SCREEN_W - BOSS_ONE_SIZE_X )
	{
		i=-1;
	}

	if( StateSet( pB1->state , BOSS_ONE_STATE_BESERKER ) )
	{
		// move sinusodially if in beserker mode
		//pB1->ent.position.y += BOSS_ONE_BESERKER_AMPLITURE * sineLookup[ trigIndex ];
		
		if( trigIndex + 1 == TRIG_TABLE_SIZE-1 )
		{
			trigIndex = 0;
		}

		else
		{
			trigIndex++;
		}
	}

	
	return r_false;
}

r_boolean B1_RunAggresive( pBoss_1t pB1 , pPlayer_t pP )
// shoots out a series of projectiles along an arc of 180-360 degrees
{
	Vect2_t center = { pB1->ent.position.x + ( pB1->ent.w >> 1 ) , 
					   pB1->ent.position.y + ( pB1->ent.h >>1 ) };

	r_boolean outOfAmmo = r_false;

	int spacing = int( 180 / BOSS_ONE_PROJECTILES );
	int activeBullets = 0;
	r_boolean volleyActive = r_true;
	
	for( int bullet=0;bullet < BOSS_ONE_PROJECTILES;bullet++ )
	{
		if( !pB1->projectiles[ bullet ].active )
		// create new bullets
		{
			int angle = 179 + ( bullet * spacing );
			
			if( angle > 359 )
			{
				angle = 359;
			}

			// calc position along arc
			float a = pB1->projectiles[ bullet ].position.x = center.x * cosineLookup[ angle ] - center.y *   sineLookup[ angle ];
			float b = pB1->projectiles[ bullet ].position.y = center.x *   sineLookup[ angle ] + center.y * cosineLookup[ angle ];

			pB1->projectiles[ bullet ].position.x /= 10;
			pB1->projectiles[ bullet ].position.y /= 10;

			pB1->projectiles[ bullet ].position.x += center.x;
			pB1->projectiles[ bullet ].position.y += center.y;			

			// calc slope of line segment from the boss position to the created projectile
			float run = pB1->slopes[ bullet ].x = center.x - pB1->projectiles[ bullet ].position.x;
			float rize = pB1->slopes[ bullet ].y = center.y - pB1->projectiles[ bullet ].position.y;

			// normalize slopes
			run /= 100;
			rize /= 100;
			pB1->slopes[ bullet ].x /= 10;
			pB1->slopes[ bullet ].y /= 10;

			// fire projectile
			pB1->projectiles[ bullet ].active = r_true; 
			
		}

		else
		// update actiaved bullets
		{
			//pB1->projectiles[ bullet ].position.x -= pB1->slopes[ bullet ].x/100;
			pB1->projectiles[ bullet ].position.y -= pB1->slopes[ bullet ].y/100;
			pB1->projectiles[ bullet ].position.x-=2;

			if( pB1->projectiles[ bullet ].position.x < 0 )
			{
				pB1->projectiles[ bullet ].active = r_false;
				volleyActive = r_false;

			}

			else if( Ent_TestForCollision( &pP->ent , &pB1->projectiles[ bullet ] ) )
			{
				Ent_InflictDamage( pP , &pB1->projectiles[ bullet ] , r_true , r_true );
				pB1->projectiles[ bullet ].active = r_false;
			}
		}
	}
	
	return volleyActive;	
}

r_boolean B1_RunBeserker( pBoss_1t pB1 , pPlayer_t pP )
// shoot out a massive , obliterating particle beam
{
	pB1->ticker = secTicker;

	Entity_t beamArea;
	
	// create a temporary bounding volume
	P_CreateEntity( &beamArea , 0 , pB1->ent.position.y , pB1->ent.position.x , pB1->ent.position.y + pB1->ent.h , 0 ,0 , 0 , 0 , ENTITY_DUMMY_TYPE );

	rectfill( doubleBuffer , beamArea.minBounds.x , beamArea.minBounds.y , beamArea.maxBounds.x , beamArea.maxBounds.y , makecol( 255 , 0 , 0 ) );
	
	if( pB1->particleBeam.active )
	{
		if( pB1->ticker > 100 )
		{
			pB1->particleBeam.active = r_false;
			ClearStates( &pB1->state , BOSS_ONE_STATE_BESERKER );
		}
	}

	else
	{
		// fire projectile
		pB1->particleBeam.active = r_true;

		if( Ent_TestForCollision( &beamArea , &pP->ent ) )
		{
			Ent_InflictDamage( pP , &pB1->ent , r_true , r_false );
		}
	}
	
	return pB1->particleBeam.active;
}	

// [ 03192008 ] 
r_boolean B1_LoadFrames( void ) 
{
	return Ent_LoadData( boss_1_frames , fileNames , BOSS_ONE_FRAMES );
}
