
#include <allegro.h>
#include <math.h>

#include "common.h"
#include "cocci.h"
#include "player.h"
#include "boss_2.h"

extern BITMAP* doubleBuffer;
extern float sineLookup[];
extern float cosineLookup[];
extern volatile int msecTicker;
extern volatile int  secTicker;
extern float masterScale;

static const int xTermVel = 50;
static const int yTermVel = 50;

static BITMAP* frames[ BOSS_TWO_FRAMES ];

static char* fileNames[] = 
{
/* BOSS_TWO_FRAME_00 */	"content/gfx/enemies/boss_2/b2_seg.bmp" , 
/* BOSS_TWO_FRAME_01 */	"content/gfx/enemies/boss_2/b2_torso_1.bmp" , 
/* BOSS_TWO_FRAME_02 */	"content/gfx/enemies/boss_2/b2_head_1.bmp" ,
/* BOSS_TWO_FRAME_03 */	"content/gfx/enemies/boss_2/b2_ret_1_0.bmp" ,
/* BOSS_TWO_FRAME_04 */	"content/gfx/enemies/boss_2/b2_ret_1_1.bmp" ,

/* BOSS_TWO_FRAME_05 */	"content/gfx/enemies/boss_2/b2_ret_1_2.bmp" ,
/* BOSS_TWO_FRAME_06 */	"content/gfx/enemies/boss_2/b2_ret_1_3.bmp" ,
/* BOSS_TWO_FRAME_07 */	"content/gfx/enemies/boss_2/b2_ret_1_4.bmp" ,
/* BOSS_TWO_FRAME_08 */	"content/gfx/enemies/boss_2/b2_ret_1_6.bmp" ,
/* BOSS_TWO_FRAME_09 */	"content/gfx/enemies/boss_2/b2_ret_1_7.bmp" ,

/* BOSS_TWO_FRAME_10 */	"content/gfx/enemies/boss_2/b2_ret_2_0.bmp" ,
/* BOSS_TWO_FRAME_11 */	"content/gfx/enemies/boss_2/b2_ret_2_1.bmp" ,
/* BOSS_TWO_FRAME_12 */	"content/gfx/enemies/boss_2/b2_ret_2_2.bmp" ,
/* BOSS_TWO_FRAME_13 */	"content/gfx/enemies/boss_2/b2_ret_2_3.bmp" ,
/* BOSS_TWO_FRAME_14 */	"content/gfx/enemies/boss_2/b2_ret_2_4.bmp" ,

/* BOSS_TWO_FRAME_15 */	"content/gfx/enemies/boss_2/b2_proj_front_1.bmp" , 
/* BOSS_TWO_FRAME_16 */	"content/gfx/enemies/boss_2/b2_proj_pro_1.bmp" ,
/* BOSS_TWO_FRAME_17 */	"content/gfx/enemies/expl_1.bmp" , 
/* BOSS_TWO_FRAME_18 */	"content/gfx/enemies/expl_2.bmp" , 
/* BOSS_TWO_FRAME_19 */	"content/gfx/enemies/expl_3.bmp" , 

/* BOSS_TWO_FRAME_20 */	"content/gfx/enemies/expl_4.bmp" , 
/* BOSS_TWO_FRAME_21 */	"content/gfx/enemies/expl_5.bmp" , 
/* BOSS_TWO_FRAME_22 */	"content/gfx/enemies/expl_6.bmp" , 
/* BOSS_TWO_FRAME_23 */	"content/gfx/enemies/expl_7.bmp" , 
/* BOSS_TWO_FRAME_24 */	"content/gfx/enemies/expl_8.bmp"  
};

static int headSequence_01[] = 
{
	BOSS_TWO_FRAME_02 , 
	BOSS_TWO_FRAME_02 
};

static int torsoSequence_01[] = 
{
	BOSS_TWO_FRAME_01 , 
	BOSS_TWO_FRAME_01 
};

static int segmentSequence_01[] = 
{
	BOSS_TWO_FRAME_00 , 
	BOSS_TWO_FRAME_00
};

static int homingRoundSequence[] = 
{
	BOSS_TWO_FRAME_00 , 
	BOSS_TWO_FRAME_00 
};

static int projectileSequence[] = 
{
	BOSS_TWO_FRAME_03 , 
	BOSS_TWO_FRAME_04 
};

static int reticuleSequence_01[] = 
{
	BOSS_TWO_FRAME_03 , 
	BOSS_TWO_FRAME_04 , 
	BOSS_TWO_FRAME_05 , 
	BOSS_TWO_FRAME_06 , 
	BOSS_TWO_FRAME_07 , 
	BOSS_TWO_FRAME_08 , 
	BOSS_TWO_FRAME_09 , 
	BOSS_TWO_FRAME_10 , 
	BOSS_TWO_FRAME_10 , 
	BOSS_TWO_FRAME_10 , 
	BOSS_TWO_FRAME_09 , 
	BOSS_TWO_FRAME_08 , 
	BOSS_TWO_FRAME_07 , 
	BOSS_TWO_FRAME_06 , 
	BOSS_TWO_FRAME_05 , 
	BOSS_TWO_FRAME_04 	
};

static int reticuleSequence_02[] = 
{
	BOSS_TWO_FRAME_10 , 
	BOSS_TWO_FRAME_11 , 
	BOSS_TWO_FRAME_12 , 
	BOSS_TWO_FRAME_13 , 
	BOSS_TWO_FRAME_14 , 
	BOSS_TWO_FRAME_14 , 
	BOSS_TWO_FRAME_14 , 
	BOSS_TWO_FRAME_13 , 
	BOSS_TWO_FRAME_12 , 
	BOSS_TWO_FRAME_11 	
};

static int projectileProfileSequence[] = 
{
	BOSS_TWO_FRAME_15 , 
	BOSS_TWO_FRAME_15 
};

static int projectileFrontSequence[] = 
{
	BOSS_TWO_FRAME_16 , 
	BOSS_TWO_FRAME_16 
};

static int explosionSequence[] = 
{
	BOSS_TWO_FRAME_17 , 
	BOSS_TWO_FRAME_18 , 
	BOSS_TWO_FRAME_19 , 
	BOSS_TWO_FRAME_20 , 
	BOSS_TWO_FRAME_21 , 
	BOSS_TWO_FRAME_22 , 
	BOSS_TWO_FRAME_23 , 
	BOSS_TWO_FRAME_24 
};

static SequenceInfo_t boss_2_sequences[] = 
{
	{            BOSS_TWO_HEAD_SEQUENCE_01_STEPS , 0 ,           headSequence_01 , 0 , 0 , -1 } , 
	{           BOSS_TWO_TORSO_SEQUENCE_01_STEPS , 0 ,          torsoSequence_01 , 0 , 0 , -1 } , 
	{         BOSS_TWO_SEGMENT_SEQUENCE_01_STEPS , 0 ,        segmentSequence_01 , 0 , 0 , -1 } , 
	{        BOSS_TWO_RETICULE_SEQUENCE_01_STEPS , 0 ,       reticuleSequence_01 , 0 , 0 , -1 } ,
	{        BOSS_TWO_RETICULE_SEQUENCE_02_STEPS , 0 ,       reticuleSequence_02 , 0 , 0 , -1 } , 
	{ BOSS_TWO_PROJECTILE_PROFILE_SEQUENCE_STEPS , 0 , projectileProfileSequence , 0 , 0 , -1 } , 
	{   BOSS_TWO_PROJECTILE_FRONT_SEQUENCE_STEPS , 0 ,   projectileFrontSequence , 0 , 0 , -1 } ,
	{          BOSS_TWO_EXPLOSION_SEQUENCE_STEPS , 0 ,         explosionSequence , 0 , 0 , -1 } 

};


void B2_Spawn( pBoss_2t pB2 , pSpawnPoint_t pSpawnPoint )
{
	P_CreateEntity( &pB2->segments[ BOSS_TWO_HEAD_SEGMENT ] , 0 , 0 , BOSS_TWO_SIZE_X , BOSS_TWO_SIZE_Y , pSpawnPoint->mapCoords.x , pSpawnPoint->mapCoords.y , BOSS_TWO_SPEED , BOSS_TWO_FRAMES , ENTITY_TYPE_BOSS_TWO );
	P_CreateEntity( &pB2->segments[ BOSS_TWO_TORSO_SEGMENT ] , 0 , 0 , BOSS_TWO_SIZE_X , BOSS_TWO_SIZE_Y , pSpawnPoint->mapCoords.x , pSpawnPoint->mapCoords.y , BOSS_TWO_SPEED , BOSS_TWO_FRAMES , ENTITY_DUMMY_TYPE );

	// animation stuff 
	Ent_SetAnimationParams( &pB2->segments[ BOSS_TWO_HEAD_SEGMENT ] , frames , BOSS_TWO_ANIMATION_RATE , &boss_2_sequences[ BOSS_TWO_HEAD_SEQUENCE_01 ] );
	Ent_SetAnimationParams( &pB2->segments[ BOSS_TWO_TORSO_SEGMENT ] , frames , BOSS_TWO_ANIMATION_RATE , &boss_2_sequences[ BOSS_TWO_TORSO_SEQUENCE_01 ] );
	
	for( int segment = 0;segment < BOSS_TWO_SEGMENTS;segment++ )
	// each segment gets progressively smaller ( excluding the first two )
	{
		pB2->radius[ segment ] = BOSS_TWO_SIZE_X - ( segment<<2 );

		pB2->segments[ segment ].position.x = SCREEN_W >> 1;
		pB2->segments[ segment ].position.y = SCREEN_H >> 1;

		pB2->distance[ segment ] = 10;

		pB2->segments[ segment ].speed = 2;

		pB2->trigIndex[ segment ] = segment * 10;

		if( (segment != BOSS_TWO_HEAD_SEGMENT) && (segment != BOSS_TWO_TORSO_SEGMENT) )
		{
			P_CreateEntity( &pB2->segments[ segment ] , 0 , 0 ,
							BOSS_TWO_SIZE_X , BOSS_TWO_SIZE_Y , 
							SCREEN_W >> 1 , SCREEN_H >> 1 , 
							2 , 
							BOSS_TWO_FRAMES , 
							ENTITY_DUMMY_TYPE );

			Ent_SetAnimationParams( &pB2->segments[ segment ] , frames , BOSS_TWO_ANIMATION_RATE , &boss_2_sequences[ BOSS_TWO_SEGMENT_SEQUENCE_01 ] );
			//Ent_SetAnimationParams( &pB2->segments[ segment ] , frames , BOSS_TWO_ANIMATION_RATE , &boss_2_sequences[ BOSS_TWO_EXPLOSION_SEQUENCE ] );
		}

		Ent_InitHealthStats( &pB2->segments[ segment ] , BOSS_TWO_HEALTH , BOSS_TWO_DAMAGE_RESISTANCE , BOSS_TWO_STRENGTH , BOSS_TWO_PROJECTILE_STRENGTH );

//		pB2->segments[ segment ].explosions = 5;
	}

	for( int bullet = 0;bullet < BOSS_TWO_PROJECTILES;bullet++ )
	// bullets
	{
		P_CreateEntity( &pB2->projectiles[ bullet ] , 0 , 0 , BOSS_TWO_PROJECTILE_SIZE_X , BOSS_TWO_PROJECTILE_SIZE_Y , 0 , 0 , 4 , 0 , ENTITY_DUMMY_TYPE );
		Ent_SetAnimationParams( &pB2->projectiles[ bullet ] , frames , BOSS_TWO_ANIMATION_RATE , &boss_2_sequences[ BOSS_TWO_PROJECTILE_FRONT_SEQUENCE ] );
	}

	// init reticule
	P_CreateEntity( &pB2->reticule , 
					0 , 0 , 
					BOSS_TWO_RETICULE_SIZE_X , BOSS_TWO_RETICULE_SIZE_Y , 
					SCREEN_W >> 1 , SCREEN_H >> 1 , 
					BOSS_TWO_RETICULE_SPEED , 
					BOSS_TWO_FRAMES , 
					ENTITY_DUMMY_TYPE );

	Ent_SetAnimationParams( &pB2->reticule , frames , BOSS_TWO_RETICULE_ANIMATION_RATE , &boss_2_sequences[ BOSS_TWO_RETICULE_SEQUENCE_01 ] );

	// rocket pack
	P_CreateEntity( &pB2->rocketPack , 0 , 0 , 40 , 40 , 0 , 0 , 5 , 0 , ENTITY_DUMMY_TYPE );

	// particles
	for( int particle = 0;particle < BOSS_TWO_PARTICLES;particle++ )
	{
		B2_InitParticle( pB2 , particle );
	}

	// head scale
	pB2->headScale = 1;

	// start off in phase 1
	//pB2->phase = BOSS_TWO_PHASE_ONE;
	//pB2->phase = BOSS_TWO_PHASE_TWO;
    //pB2->phase = BOSS_TWO_PHASE_THREE;
	//pB2->phase = BOSS_TWO_FG_TRANSITION;
	//pB2->phase = BOSS_TWO_BG_TRANSITION;
	
	SetStates( &pB2->state , BOSS_TWO_TRANSITION_PHASE_ONE ); 

	pB2->boomeranging[ BOSS_TWO_TORSO_SEGMENT ] = r_true;

	// setup the order the phrases take place int
	pB2->currentCallback[ 0 ] = B2_RunPhaseOne;
	pB2->currentCallback[ 1 ] = B2_MoveToForeground;
	pB2->currentCallback[ 2 ] = B2_RunPhaseTwo;
	pB2->currentCallback[ 3 ] = B2_MoveToBackground;
	pB2->currentCallback[ 4 ] = B2_RunPhaseOne;
	pB2->currentCallback[ 5 ] = B2_MoveToForeground;
	pB2->currentCallback[ 6 ] = B2_RunPhaseTwo;
	pB2->currentCallback[ 7 ] = B2_MoveToBackground;
	pB2->currentCallback[ 8 ] = B2_RunPhaseOne;
	pB2->currentCallback[ 9 ] = B2_MoveToForeground;
	pB2->currentCallback[ 10] = B2_RunPhaseThree;
	pB2->currentCallback[ 11] = B2_MoveToBackground;

	pB2->phases[ 0 ] = BOSS_TWO_PHASE_ONE;
	pB2->phases[ 1 ] = BOSS_TWO_FG_TRANSITION;
	pB2->phases[ 2 ] = BOSS_TWO_PHASE_TWO;
	pB2->phases[ 3 ] = BOSS_TWO_BG_TRANSITION;
	pB2->phases[ 4 ] = BOSS_TWO_PHASE_ONE;
	pB2->phases[ 5 ] = BOSS_TWO_FG_TRANSITION;
	pB2->phases[ 6 ] = BOSS_TWO_PHASE_TWO;
	pB2->phases[ 7 ] = BOSS_TWO_BG_TRANSITION;
	pB2->phases[ 8 ] = BOSS_TWO_PHASE_ONE;
	pB2->phases[ 9 ] = BOSS_TWO_FG_TRANSITION;
	pB2->phases[ 10] = BOSS_TWO_PHASE_THREE;
	pB2->phases[ 11] = BOSS_TWO_BG_TRANSITION;

	// set the time to spend in each phase
	pB2->phaseLengths[ BOSS_TWO_PHASE_ONE ] = BOSS_TWO_PHASE_ONE_LENGTH;
	pB2->phaseLengths[ BOSS_TWO_PHASE_TWO ] = BOSS_TWO_PHASE_TWO_LENGTH;
	pB2->phaseLengths[ BOSS_TWO_PHASE_THREE ] = BOSS_TWO_PHASE_THREE_LENGTH;
	pB2->phaseLengths[ BOSS_TWO_BG_TRANSITION] = BOSS_TWO_BG_TRANSITION_LENGTH;
	pB2->phaseLengths[ BOSS_TWO_FG_TRANSITION] = BOSS_TWO_FG_TRANSITION_LENGTH;

	// explosion stuffs
	for( int seg = 0;seg < BOSS_TWO_SEGMENTS;seg++ )
	{
		Ent_AddExplosions( &pB2->segments[ seg ] , pB2->explosions[ seg ] , BOSS_TWO_SEGMENT_EXPLOSIONS , &boss_2_sequences[ BOSS_TWO_EXPLOSION_SEQUENCE ] );
	}
}

r_boolean B2_Kill( pBoss_2t pB2 )
{
	return r_true;
}

void B2_Destroy( pBoss_2t pB2 )
{

}

void B2_Draw( pBoss_2t pB2 )
{
	// particles
	B2_DrawParticles( pB2 );
	
	// segments
	for( int segment=BOSS_TWO_TAIL_SEGMENT;segment > 1;segment-- )
	{
		Ent_Animate( &pB2->segments[ segment ] );
		Ent_Draw( &pB2->segments[ segment ] , 1 , 1 , pB2->trigIndex[ segment ] );
		
		if( segment == 0 )
		{
			G_WriteToDebugWindow( pB2->headScale , "%d" , pB2->segments[ segment ].position.x , pB2->segments[ segment ].position.y );
		}

		G_WriteToDebugWindow( pB2->trigIndex[ segment ] , "%d" , pB2->segments[ segment ].position.x + 20, pB2->segments[ segment ].position.y );
	}

	// torso
	Ent_Animate( &pB2->segments[ BOSS_TWO_TORSO_SEGMENT ] );	
	Ent_Draw( &pB2->segments[ BOSS_TWO_TORSO_SEGMENT ] , 1 , 1 , 0 );
	
	// head
	Ent_Animate( &pB2->segments[ BOSS_TWO_HEAD_SEGMENT ] );
	Ent_Draw( &pB2->segments[ BOSS_TWO_HEAD_SEGMENT ] , 1 * pB2->headScale , 1 * pB2->headScale , 0 );


	if( pB2->phase == BOSS_TWO_PHASE_ONE )
	{
		Ent_Animate( &pB2->reticule );
		Ent_Draw( &pB2->reticule , 1 , 1 , 0 );
	}

	// projectiles 
	for( int bullet = 0;bullet < BOSS_TWO_PROJECTILES;bullet++ )
	{	
		float xScale = 0 , yScale = 0;
		int angle = 0;
		
		if( pB2->projectiles[ bullet ].active ) 
		{
			pVect2_t pv = &pB2->projectiles[ bullet ].position;

			if( pB2->phase == BOSS_TWO_PHASE_ONE )
			{
				//circlefill( doubleBuffer , pB2->projectiles[ bullet ].position.x , pB2->projectiles[ bullet ].position.y , 10 + pB2->projectiles[ bullet ].target.x , makecol( 255 , 0 , 0 ) );
				Ent_UpdateAnimationSequence( &pB2->projectiles[ bullet ] , &boss_2_sequences[ BOSS_TWO_PROJECTILE_PROFILE_SEQUENCE ] );
				xScale = yScale = 1 + pB2->projectiles[ bullet ].target.x/20;
				angle = pB2->projectiles[ bullet ].target.x;
			}

			else if( pB2->phase == BOSS_TWO_PHASE_THREE )
			{
				if( pv->x < pB2->segments[ BOSS_TWO_HEAD_SEGMENT ].position.x )
				{
					Ent_UpdateAnimationSequence( &pB2->projectiles[ bullet ] , &boss_2_sequences[ BOSS_TWO_PROJECTILE_FRONT_SEQUENCE ] );			
					xScale = 2 , yScale = 1;
					angle = 0;
				}
			}

			Ent_Animate( &pB2->projectiles[ bullet ] );
			Ent_Draw( &pB2->projectiles[ bullet ] ,
					  xScale , 
					  yScale ,
					  angle );	
		}
	}

	// explosions
	for( int seg = 0;seg < BOSS_TWO_SEGMENTS;seg++ )
	{
		for( int explosion = 0;explosion < BOSS_TWO_SEGMENT_EXPLOSIONS;explosion++ )
		{
			pExplosion_t pEx = &pB2->explosions[ seg ][ explosion ];

			if( pEx->ent.active )
			{
				Ent_RunExplosion( &pB2->segments[ seg ] , &pB2->explosions[ seg ][ explosion ] );
				Ent_Animate( &pEx->ent );
				Ent_Draw( &pEx->ent , pEx->scale , pEx->scale , 0 );
			}
		}
	}

	G_WriteToDebugWindow( pB2->phase , "%d : " , 0 , 0 );
	G_WriteToDebugWindow( pB2->state , "%p" , 30 , 0 );
}

r_boolean B2_Run( pBoss_2t pB2 , pPlayer_t pP )
{
	static int bossTicker = 0;
		
	// what haven't I been doing this all along... 
	r_boolean (*ai_callback)( pBoss_2t , pPlayer_t);

	bossTicker = secTicker;

	// change cycle when time 
	if( bossTicker >= pB2->phaseLengths[ pB2->phase ] )
	{
		if( pB2->cycle < (BOSS_TWO_CYCLES-1) )
		{
			// move to the next step of cycle
			pB2->cycle++;
			ai_callback = pB2->currentCallback[ pB2->cycle ];
			pB2->phase = pB2->phases[ pB2->cycle ];
			
			ClearStates( &pB2->state , 0xffffffff );
			SetStates( &pB2->state , BOSS_TWO_TRANSITION_PHASE_ONE );			
		}

		else
		{
			// make boss harder after each complete cycle
			B2_AdjustDifficulty( pB2 , pP );
			
			pB2->cycle = 0;
		}

		// reset timer vars
		bossTicker = secTicker = 0;
	}

	// execute current phase.
	switch( pB2->phase )
	{
	case BOSS_TWO_PHASE_ONE:
		{
			ai_callback = B2_RunPhaseOne;
			break;
		}

	case BOSS_TWO_PHASE_TWO:
		{
			ai_callback = B2_RunPhaseTwo;
			break;
		}

	case BOSS_TWO_PHASE_THREE:
		{
			ai_callback = B2_RunPhaseThree;
			break;
		}

	case BOSS_TWO_BG_TRANSITION:
		{
			ai_callback = B2_MoveToBackground;
			break;
		}

	case BOSS_TWO_FG_TRANSITION:
		{
			ai_callback = B2_MoveToForeground;
			break;
		}

	default:
		{
			ai_callback = B2_RunPhaseOne;
			break;
		}
	}

	// update particles 
	for( int particle = 0;particle < BOSS_TWO_PARTICLES;particle++ )
	{
		if( !B2_RunParticle( pB2 , particle ) )
		{
			B2_InitParticle( pB2 , particle );
		}
	}

	// set off explosions need be
	for( int seg = 0;seg < BOSS_TWO_SEGMENTS;seg++ )
	{
		if( pB2->segments[ seg ].collided )
		{
			for( int explode=0;explode < BOSS_TWO_SEGMENT_EXPLOSIONS;explode++ )
			{
				Ent_InitExplosion( &pB2->segments[ seg ] , &pB2->explosions[ seg ][ explode ] , BOSS_TWO_EXPLOSION_LENGTH );
			}
		}
	}

	G_WriteToDebugWindow( (int)ai_callback , "%p" , 0 , 20 );

	return ai_callback( pB2 , pP );
}

static r_boolean B2_RunPhaseOne( pBoss_2t pB2 , pPlayer_t pP )
// target the player from a safe distance
{
	static int startingTime = msecTicker;
	
// movement ================================================================

	// bosses' movement
	B2_PhaseOneMovement( pB2 , pP );

	// reticule movement
	if( Ent_PursueEntity( &pB2->reticule , &pP->ent , BOSS_TWO_AIMING_TOLERANCE ) )
	{
		Ent_UpdateAnimationSequence( &pB2->reticule , &boss_2_sequences[ BOSS_TWO_RETICULE_SEQUENCE_02 ] );
		pB2->targetLocked = r_true;
	}

	else
	{
		Ent_UpdateAnimationSequence( &pB2->reticule , &boss_2_sequences[ BOSS_TWO_RETICULE_SEQUENCE_01 ] );	
	}

// firing ===================================================================
	if( pB2->targetLocked )
	{
		if( !pB2->projectiles[ BOSS_TWO_PROJECTILES - 1 ].active )
		// only fire new bullets once the old ones are dead.
		{			
			for( int bullet = 0;bullet < BOSS_TWO_PROJECTILES;bullet++ )
			{
				// fire bullet
				pB2->projectiles[ bullet ].active = r_true;
				
				//  set initial positions for bullets
				pB2->projectiles[ bullet ].position = pB2->segments[ BOSS_TWO_HEAD_SEGMENT ].position;

				// replaces the old hack, with a new one... ( used to scale the sprite )
				pB2->projectiles[ bullet ].target.x = 1;
			}
		}

		else
		// update positions
		{
			for( int bullet=0;bullet < BOSS_TWO_PROJECTILES;bullet++ )
			{
				pEntity_t pBullet = &pB2->projectiles[ bullet ];

				if( pBullet->active )
				{
					if( Ent_PursueEntity( pBullet , &pB2->reticule , BOSS_TWO_AIMING_TOLERANCE ) || pBullet->target.x > 100 )
					// impact
					{
						pBullet->active = r_false;
						pB2->targetLocked = r_false;

						if( bullet == BOSS_ONE_PROJECTILES - 1 )
						{
							pB2->targetLocked = r_false;
						}
					}

					else // advance towards target
					{
						// make the sprite larger as it continues to move to it's target
						pBullet->target.x++;    
					}
				}
			}
		}		
	}

	// update ticker
	pB2->ticker = msecTicker - startingTime;		 

	return r_true;
}

static r_boolean B2_RunPhaseTwo( pBoss_2t pB2 , pPlayer_t pP )
// shoot out each segment as a projectile
{
	static activeSegment = 0;
	pVect2_t pv = NULL;
	int trigIndex = 0;
	int speed = 0;

	for( int seg = 1;seg < BOSS_TWO_SEGMENTS;seg++ )
	{
		if( pB2->boomeranging[ seg ] )
		// control the boomerang
		{
			// set the active segment
			activeSegment = seg;
			
			pv = &pB2->segments[ activeSegment ].position;
			
			// set to project at an acceleated rate
			speed  = pB2->segments[ activeSegment ].speed<<3;
			
			// each segment goes a little faster than the last.
			speed += seg; 

			if( StateSet( pB2->state , BOSS_TWO_TRANSITION_PHASE_ONE ) )
			// going towards player
			{
				if( pv->x < pP->ent.position.x )
				// time to come back
				{
					ClearStates( &pB2->state , BOSS_TWO_TRANSITION_PHASE_ONE );
					SetStates( &pB2->state , BOSS_TWO_TRANSITION_PHASE_TWO );
				}

				else
				{
					pv->x -= speed;
				}
			}

			else
			// return to home
			{
				if( (abs( pv->x  - pB2->segments[ BOSS_TWO_HEAD_SEGMENT ].position.x ) < speed ) )
				// if close enough , move to the next segment in line
				{
					ClearStates( &pB2->state , BOSS_TWO_TRANSITION_PHASE_TWO );
					SetStates( &pB2->state , BOSS_TWO_TRANSITION_PHASE_ONE );
					
					if( activeSegment < (BOSS_TWO_SEGMENTS - 1))
					{
						// move on down the line.
						pB2->boomeranging[ activeSegment ] = r_false;
						activeSegment++;
						pB2->boomeranging[ activeSegment ] = r_true;
						
					}

					else// if( activeSegment == (BOSS_TWO_SEGMENTS - 1) )
					{
						ClearStates( &pB2->state , BOSS_TWO_TRANSITION_PHASE_ONE | BOSS_TWO_TRANSITION_PHASE_TWO );
						SetStates( &pB2->state , BOSS_TWO_TRANSITION_PHASE_ONE );							  
						
						pB2->boomeranging[ 1 ] = r_true; // for next time.
						pB2->boomeranging[ activeSegment ] = r_false;
						
						activeSegment = 1;
						return r_false;

					}
				}

				else
				// advance segment back home toward target.
				{
					pv->x += speed;
				}
			}
		}

		else
		// let it wobble...
		{
			pv = &pB2->segments[ seg ].position;
			trigIndex = pB2->trigIndex[ seg ];

			// wobble.
			//pv->x +=  sineLookup[ trigIndex ];

			if( trigIndex < TRIG_TABLE_SIZE )
			{
				pB2->trigIndex[ seg ]++;
			}

			else
			{
				pB2->trigIndex[ seg ] = 0;
			}
		}
	}

	//B2_LinkSegments( pB2 );

	B2_UpdateParticles( pB2 , 0 , 0 );

	G_WriteToDebugWindow( activeSegment , "%d" , 0 , 10 );

	return r_true;
}

static r_boolean B2_RunPhaseThree( pBoss_2t pB2 , pPlayer_t pP )
// head of boss expands until it explodes ( while shooting out missile at the player )
{
	pEntity_t pHead = &pB2->segments[ BOSS_TWO_HEAD_SEGMENT ];
	static int volleyDelay = msecTicker;

// PART 1 : Inflate the head upon impact ===========================================	

	if( pHead->collided )
	{
		if( pB2->headScale < 200 )
		{
			pB2->headScale += 2;
		}
		
		pHead->minBounds.x -=2;
		pHead->minBounds.y -=2;

		pHead->maxBounds.x +=2;
		pHead->maxBounds.y +=2;

		if( pB2->headScale >= 200 )
		{
			return r_false;
		}
	}

	else
	{
		if( pB2->headScale >= 1 )
		{
			pB2->headScale-=0.25f;

			if( pHead->maxBounds.x >= BOSS_TWO_SIZE_X )
			{
				pHead->minBounds.x += 0.25f;
				pHead->minBounds.y += 0.25f;

				pHead->maxBounds.x -= 0.25f;
				pHead->maxBounds.y -= 0.25f; 
			}
		}
	}

// PART 2 : Shoot out homing missles ===============================================

	if( pB2->projectiles[ BOSS_TWO_PROJECTILES-1 ].active )
	// procssess all missles until they are deactivated
	{
		for( int missile = 0; missile < BOSS_TWO_PROJECTILES;missile++ )
		{
			pVect2_t pv = &pB2->projectiles[ missile ].position;
			pVect2_t pvTgt = &pP->ent.position;
			int speed = pB2->projectiles[ missile ].speed;		
			
			pv->x -= speed;
			
			// home y coord
			if( ( pv->y + speed ) < pvTgt->y )
			{
				pv->y += ( speed - missile );
			}

			else if( (pv->y + speed ) > pvTgt->y )
			{
				pv->y -= ( speed - missile );
			}
			
			// destroy when offscreen
			if( pB2->projectiles[ missile ].position.x < 0 )
			{
				pB2->projectiles[ missile ].active = r_false;
			}

			if( Ent_TestForCollision( &pP->ent , &pB2->projectiles[ missile ] ) )
			// destroy missile upon impact.
			{
				Ent_InflictDamage( pP , &pB2->projectiles[ missile ] , r_true , r_true );
				pB2->projectiles[ missile ].active = r_false;
			}
		}
	}

	else
	// fire new missiles
	{
		if( volleyDelay > 4500 )
		{
			volleyDelay = 0;
			msecTicker = 0;
			
			for( int missile = 0;missile < BOSS_TWO_PROJECTILES;missile++ )
			{
				pB2->projectiles[ missile ].position.x = pB2->segments[ BOSS_TWO_HEAD_SEGMENT ].position.x + ( missile * 50 ) ;
				pB2->projectiles[ missile ].position.y = pB2->segments[ BOSS_TWO_HEAD_SEGMENT ].position.y;

				pB2->projectiles[ missile ].active = r_true;
			}
		}

		else
		{
			volleyDelay = msecTicker;
		}
	}

	B2_UpdateParticles( pB2 , 0 , 0 );

	return r_true;
}

Vect2_t Project3Dto2D( float x , float y , float z , int plane )
{
	Vect2_t projected;

	projected.x = ( ( x / z ) + plane ) + ( SCREEN_W >> 1 );
	projected.y = ( ( y / z ) + plane ) + ( SCREEN_H >> 1 );

	return projected;
}

int CalcDistance( pVect2_t pv1 , pVect2_t pv2 )
{
	int dx = abs( pv1->x - pv2->x );
	int dy = abs( pv1->y - pv2->y );
	
	return sqrt( (dx*dx) + (dy*dy) );
}

float DotProduct( pVect2_t pv1 , pVect2_t pv2 )
{
	return (pv1->x * pv2->x ) + ( pv1->y * pv2->y );
}	

Vect2_t Normalize( pVect2_t pV )
{
	Vect2_t unitMaker = { 1 , 1 };
	float distance = CalcDistance( pV , &unitMaker );
	Vect2_t normalized = { pV->x / distance , pV->y / distance };

	return normalized;
}

static r_boolean B2_PhaseOneMovement( pBoss_2t pB2 , pPlayer_t pP )
{
	static int dir = 1;
	static int i=0;
	
	pVect2_t pvHead = &pB2->segments[ BOSS_TWO_HEAD_SEGMENT ].position;
	
	pvHead->x += dir;

	if( pvHead->y > 0 )
	{
		pvHead->y += sineLookup[ i / 2 ];
	}

	else
	{
		pvHead->y++;
	}

	B2_UpdateParticles( pB2 , -dir , 1 );

	if( i == TRIG_TABLE_SIZE - 1 )
	{
		i = 0;
	}

	else
	{
		i++;
	}

	if( pvHead->x < 0 || pvHead->x > SCREEN_W )
	{
		dir = -dir;
	}

	// link the remaining segments to the head.
	B2_LinkSegments( pB2 );	
	
	return r_true;
}

static r_boolean B2_MoveToBackground( pBoss_2t pB2 , pPlayer_t )
// transition routine # 1 : move into the background in prepreation for phase 1
{
	pVect2_t pvHead = &pB2->segments[ BOSS_TWO_HEAD_SEGMENT ].position;
	pVect2_t pvTail = &pB2->segments[ BOSS_TWO_TAIL_SEGMENT ].position;
	int speed = pB2->segments[ BOSS_TWO_HEAD_SEGMENT ].speed;

	if( StateSet( pB2->state , BOSS_TWO_TRANSITION_PHASE_ONE ) )
	// phase one : move up off screen
	{
		if( pvTail->y > -40 )
		{
			B2_UpdateParticles( pB2 , 0 , 1 );
			pvHead->y -= speed;
		}

		else 
		{
			ClearStates( &pB2->state , BOSS_TWO_TRANSITION_PHASE_ONE );
			SetStates( &pB2->state , BOSS_TWO_TRANSITION_PHASE_TWO );
		}
	}

	else 
	// move down into the new position
	{
		if( pvHead->y < 300 )
		{
			pvHead->y += speed;
			B2_UpdateParticles( pB2 , 0 , -1 );
		}

		else 
		{
			B2_UpdateParticles( pB2 , 0 , 0 );
			ClearStates( &pB2->state , BOSS_TWO_TRANSITION_PHASE_TWO );
			
			return r_true;
		}
	}

	B2_LinkSegments( pB2 );

	return r_false;
}

static r_boolean B2_MoveToForeground( pBoss_2t pB2 , pPlayer_t )
// transition routine # 2 : move into the foreground "" phase 2/3
{
	pVect2_t pvHead = &pB2->segments[ BOSS_TWO_HEAD_SEGMENT ].position;
	pVect2_t pvTail = &pB2->segments[ BOSS_TWO_TAIL_SEGMENT ].position;
	int speed = pB2->segments[ BOSS_TWO_HEAD_SEGMENT ].speed;

	if( StateSet( pB2->state , BOSS_TWO_TRANSITION_PHASE_ONE ) ) 
	{
		if( pvTail->y > -40 || pvHead->y > -40 )
		// move upward offscreen
		{
			pvHead->y -= speed;
			B2_UpdateParticles( pB2 , 0 , 1 );
		}

		else
		{
			// move to the bottom of the screen 
			//pvHead->y = SCREEN_H + 20;

			for( int seg = 0;seg < BOSS_TWO_SEGMENTS;seg++ )
			{
				pB2->segments[ seg ].position.y = SCREEN_H + ( seg * 10 ) + 100;
				pB2->segments[ seg ].position.x = SCREEN_W - 100;
			}

			ClearStates( &pB2->state , BOSS_TWO_TRANSITION_PHASE_ONE );
			SetStates( &pB2->state , BOSS_TWO_TRANSITION_PHASE_TWO );
			B2_LinkSegments( pB2 );
			return r_false;
		}
	}

	else
	// move upward into the foreground
	{
		if( pvHead->y > 100 )
		{
			pvHead->y -= speed;
			B2_UpdateParticles( pB2 , 0 , 1 );
		}

		else
		{
			B2_UpdateParticles( pB2 , 0 , speed );
			ClearStates( &pB2->state , BOSS_TWO_TRANSITION_PHASE_TWO );
			
			return r_true;
		}
	}

	B2_LinkSegments( pB2 );	

	return r_false;
}

static void B2_LinkSegments( pBoss_2t pB2 )
// keep all the segments linked together
{
	int maxDistance = 0;

	for( int segment = 1;segment < BOSS_TWO_SEGMENTS;segment++ )
	{
		maxDistance = pB2->distance[ segment ];

		if( CalcDistance( &pB2->segments[ segment ].position , &pB2->segments[ segment - 1 ].position ) >= maxDistance )
		// if the segment in question is outside the maximum range of it's parter ( adjacent segment )...
		{
			// ... follow the partner.
			Ent_PursueEntity( &pB2->segments[ segment ] , &pB2->segments[ segment - 1 ] , 25 );
		}
	}

}

void B2_InitParticle( pBoss_2t pB2 , int index )
{
	pVect2_t pvSrc = &pB2->segments[ BOSS_TWO_TORSO_SEGMENT ].position;
	pParticle_t pP = &pB2->particles[ index ];

	pP->position.x = pvSrc->x + rand() % 30;
	pP->position.y = pvSrc->y + rand() % 40;

	pP->acceleration.x = pP->acceleration.y = -2 + rand() % 3;
	pP->velocity.x = pP->velocity.y =  rand() % 3;

	pP->r = 150;
	pP->g = 150;
	pP->b = 255;

	pP->ttl = 20 + rand() % 30;

	pP->active = r_true;
}

void B2_UpdateParticles( pBoss_2t pB2  , int xAccel , int yAccel )
{
	for( int particle = 0;particle < BOSS_TWO_PARTICLES;particle++ )
	{
		pParticle_t pP = &pB2->particles[ particle ];
	
		pP->acceleration.x = xAccel;
		pP->acceleration.y = yAccel;
	}
}

r_boolean B2_RunParticle( pBoss_2t pB2 , int index )
{
	pParticle_t pP = &pB2->particles[ index ];
	pVect2_t pv = &pP->position;
	pVect2_t pVel = &pP->velocity;
	pVect2_t pAcel = &pP->acceleration;

	// adjust particle velocity ( acceleration affecting velocity )
	if( ( pVel->x + pAcel->x ) < xTermVel ) 
	{
		pVel->x += pAcel->x;
	}

	if( ( pVel->y + pAcel->y ) < yTermVel )
	{ 
		pVel->y += pAcel->y;
	}

	//. adjust particle position ( emitter ) 
	pv->x += pVel->x;
	pv->y += pVel->y;

	// set decay rate 
	int rate = 10;

	// decay color
	if( pP->r )
		pP->r -= rate;
	
	if( pP->g )
		pP->g -= rate;

	if( pP->b )
		pP->b -= rate;
/*	else
		pP->b = pP->g = 255;*/

	// decrease time-to-live
	pP->ttl--;	
	
	return ( pP->ttl <=0 ) ? ( r_false ) : ( r_true );
}

void B2_DrawParticles( pBoss_2t pB2 )
{
	pVect2_t pv = NULL;
	pParticle_t pP = NULL;
	
	for( int particle = 0;particle < BOSS_TWO_PARTICLES;particle++ )
	{
		pv = &pB2->particles[ particle ].position;
		pP = &pB2->particles[ particle ];
		
		//putpixel( doubleBuffer , pv->x , pv->y , makecol( pP->r , pP->g , pP->b ) );
		circlefill( doubleBuffer , pv->x , pv->y , masterScale * ((pP->r)>>2) , makecol( pP->r , pP->g , pP->b ) );
	}
}

void B2_AdjustDifficulty( pBoss_2t  pB2 , pPlayer_t pP )
{
	// make aiming reticule move faster
	if( pB2->reticule.speed < pP->ent.speed )
	{
		pB2->reticule.speed++;
	}

	// shorten phases 1 and 2
	if( pB2->phaseLengths[ BOSS_TWO_PHASE_ONE ] > 10 )
	{
		pB2->phaseLengths[ BOSS_TWO_PHASE_ONE ] -= 2;
		pB2->phaseLengths[ BOSS_TWO_PHASE_TWO ] -= 2;
	}
}

r_boolean B2_LoadFrames( void )
{
	return Ent_LoadData( frames , fileNames , BOSS_TWO_FRAMES );
}