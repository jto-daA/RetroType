
#include <allegro.h>
#include <math.h>

#include "boss_3.h"
#include "cocci.h"

// FEAR THE GOLIATH!!!!!!!!!!!!!!!

extern volatile int  secTicker;
extern volatile int msecTicker;
extern BITMAP* doubleBuffer;
extern float sineLookup[]; 
extern float cosineLookup[];

const int gravitationalConstant = 10;

void B3_Spawn( pBoss_3t pB3 , pSpawnPoint_t pSpawnPoint )
{
	P_CreateEntity( &pB3->head , 0 , 0 , 40 , 40 , pSpawnPoint->mapCoords.x , pSpawnPoint->mapCoords.y , 5 , 5 , ENTITY_TYPE_BOSS_THREE );

	for( int sparky=0;sparky < BOSS_THREE_SPARKYS;sparky++ )
	{
		P_CreateEntity( &pB3->minnions[ sparky ].ent , 0 , 0 , 10 , 10 , 0 , 400 , 0 , 0 , ENTITY_DUMMY_TYPE );
	}

	pB3->minnions[ 0 ].ent.position.x = SCREEN_W>>1;
	pB3->minnions[ 0 ].ent.position.y = 500;

	Sparky_Spawn( &pB3->minnions[ 0 ] );
	Sparky_Spawn( &pB3->minnions[ 1 ] );
	
	Sparky_Launch( &pB3->minnions[ 0 ] , 100 );
	Sparky_Launch( &pB3->minnions[ 0 ] , 200 );

	// test cases
	pB3->phase = BOSS_THREE_PHASE_THREE;
}

r_boolean B3_Kill( pBoss_3t pB3 )
{
	return r_true;
}

void B3_Destroy( pBoss_3t pB3 )
{

}

// [ rendering ] ====================================================================================

void B3_Draw( pBoss_3t pB3 )
{
	pVect2_t pv = &pB3->head.position;

	int color = ( pB3->head.collided ) ? ( makecol(255,255,255) ) : ( makecol(255,0,255) );
	
	rectfill( doubleBuffer , pv->x , pv->y , pv->x + 40 , pv->y+40, color );

	// draw sparkys
	for( int sparky=0;sparky < BOSS_THREE_SPARKYS;sparky++ )
	{
		Sparky_Draw( &pB3->minnions[ sparky ] );
	}
}

void Sparky_Draw( pSparky_t pS )
{
	pVect2_t pv = &pS->ent.position;

	int color = ( pS->ent.collided ) ? ( makecol( 255 , 255 , 255 ) ) : ( makecol( 128 , 128 , 128 ) );

	rectfill( doubleBuffer , pv->x , pv->y , pv->x + pS->xScale , pv->x + pS->yScale , color );
}

	




//===================================================================================================

r_boolean B3_Run( pBoss_3t pB3 , pPlayer_t pP )
{
	pVect2_t pv = &pB3->head.position;
	static int dir = 1;

	r_boolean (*ai_callback)( pBoss_3t , pPlayer_t ) = NULL;
	r_boolean (*sparky_ai_callback)( pBoss_3t , int , pPlayer_t ) = NULL;


	switch( pB3->phase)
	{
	case BOSS_THREE_INTRO_PHASE:
		{
			break;
		}
		
	default:
	case BOSS_THREE_PHASE_ONE:
		{
			break;
		}

	case BOSS_THREE_PHASE_TWO:
		{
			break;
		}

	case BOSS_THREE_PHASE_THREE:
		{
			break;
		}

	case BOSS_THREE_PHASE_FOUR:
		{
			break;
		}

	case BOSS_THREE_PHASE_FINAL:
		{
			break;
		}
	}

	if( pv->x > SCREEN_W || pv->x < 0 )
	{
		dir = -dir;
	}

	Sparky_Propel( &pB3->minnions[ 0 ] );
	Sparky_Propel( &pB3->minnions[ 1 ] );

	pv->x += dir;
	
	return r_true;
}	

// HERE WE GO... 

// [ INTRO PHASE ] ===========================================================================================

r_boolean  B3_RunIntro( pBoss_3t pB3 , pPlayer_t pP )
{
	return r_true;
}

r_boolean Sparky_RunIntro( pBoss_3t pB3 , int sparky , pPlayer_t pP )
{
	return r_true;
}

// [ PHASE ONE : SEE-SAW OF DOOM ] ===========================================================================

// [ PHASE TWO : ... STATIONARY-ROTATING-HEAD-OF-DOOM... ] ===================================================
r_boolean B3_RunPhaseTwo( pBoss_3t pB3 , pPlayer_t pP )
{
	Sparky_RunPhaseThree( pB3 , NO_OP , pP );

	return r_true;
}

r_boolean Sparky_RunPhaseThree( pBoss_3t pB3 , int sparky , pPlayer_t pP )
{
	return r_true;
}

// [ PHASE THREE .. TWIN TERRORS ] ===========================================================================

// [ PHASE FOUR ... DOUBLE-DUTCH OF DOOM ] ===================================================================

// [ PHASE FIVE ( FINAL ) ... GOLIATH REVEALED ] =============================================================

// [ HELPERS ] ===============================================================================================

static void Sparky_Spawn( pSparky_t pS )
{
	pS->ent.position.x = SCREEN_W>>1;
	pS->ent.position.y = 500;
	
	pS->xScale = 30;//1 + rand() % SPARKY_MAX_X_SCALE;
	pS->yScale = 30;//1 + rand() % SPARKY_MAX_Y_SCALE;
	pS->ttl = SPARKY_MIN_LIFESPAN + rand() % SPARKY_MAX_LIFESPAN;

	pS->ent.speed = 1 + rand() % SPARKY_MAX_SPEED;

	P_CreateEntity( &pS->ent , 0 , 0 , pS->xScale , pS->yScale , 0 , 500 , pS->ent.speed , SPARKY_FRAMES , ENTITY_DUMMY_TYPE );
}

static void Sparky_Launch( pSparky_t pS , int force )
{
	if( !pS->airborne )
	{
		pS->airborne = r_true;
	}

	// randomize the strength of the launch.
	pS->pos.acceleration.x = SPARKY_LAUNCH_POWER + rand() % SPARKY_LAUNCH_POWER_VARIATION;
	pS->pos.acceleration.y = SPARKY_LAUNCH_POWER + rand() % SPARKY_LAUNCH_POWER_VARIATION;
}

static void Sparky_Propel( pSparky_t pS )
{
	pVect2_t pv = &pS->ent.position;
	pVect2_t paccel = &pS->pos.acceleration;
	pVect2_t pvel = &pS->pos.velocity;

	int termVel = pS->xScale * pS->yScale / 10;

	// affect acceleration modulation due to the force of gravity
	paccel->y -= gravitationalConstant;
	paccel->x -= gravitationalConstant;

	// change velocity
	if( abs(pvel->x) < termVel && abs(pvel->y) < termVel )
	{
			
		pvel->x += paccel->x;
		pvel->y += paccel->y;
	}

	// update position
	pv->x += pvel->x;
	pv->y += pvel->y;

	// decrease time-to-live
	pS->ttl--;

	// check to see if sparky has hit the ground yet
	if( pv->y > SPARKY_GROUND_LEVEL )
	{
		pS->airborne = r_false;
		Sparky_Spawn( pS );
	}	

	G_WriteToDebugWindow( pv->y , "%i" , 0 , 0 );
	
	G_WriteToDebugWindow( pvel->y , "%i" , 0 , 10 );
	G_WriteToDebugWindow( termVel , "%i" , 50 , 10 );


	G_WriteToDebugWindow( paccel->y , "%i" , 0 , 20 );
}

