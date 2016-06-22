
#include <allegro.h>

#include "game.h"
#include "blasto.h"
#include "player.h"

extern BITMAP* doubleBuffer;
extern float masterScale;
extern Game_t currentGame;

// patterns /////////////////////////////////////////////////////////////////////////////////////

BlastoControls_t testPattern[ TEST_PATTERN_SIZE ] = 
{
	{ 400 , 0 , 0 , 0 } ,  
	{ 0 , 0 , 200 , 0 } ,
	{ 10 , 0 , 0 , 0 } , 
	{ 0 , 0 , 0 , 200 } , 
	{ 1000 , 0 , 0 , 0 }
};

// implementation ///////////////////////////////////////////////////////////////////////////////

void Blasto_Spawn( pBlasto_t pB , int xPosition , int yPosition )
{
	P_CreateEntity( &pB->ent , 0 , 0 , 32 , 32 , xPosition , yPosition , BLASTO_SPEED , BLASTO_FRAMES , ENTITY_TYPE_BLASTO );
	P_CreateEntity( &pB->projectile , 0 , 0 , 20 , 10 , xPosition , yPosition , BLASTO_PROJECTILE_SPEED , BLASTO_PROJECTILE_FRAMES , ENTITY_DUMMY_TYPE );

	pB->ent.entityType = ENTITY_TYPE_BLASTO;

	// start with a fresh memory.
	pB->synapse = 0;
	pB->pulse = r_false;
	pB->step = 0;
	pB->totalSteps = 3;
	
	// init with the default pattern.
	pB->currentPattern = &testPattern[ pB->step ];
}

void Blasto_Spawn( pBlasto_t pB , pSpawnPoint_t pSpawnPoint )
{
	Blasto_Spawn( pB , SCREEN_W , pSpawnPoint->mapCoords.y );
}

r_boolean Blasto_Destroy( pBlasto_t pB )
{
	return r_true;
}

void Blasto_Kill( pBlasto_t pB )
{

}

r_boolean Blasto_Run( pBlasto_t pB , pPlayer_t pP )
{
	//pB->ent.position.x -= pB->ent.speed;

	// shoot when blasto's memory is full
	if( pB->synapse == ( BLASTO_MEMORY_SIZE - 1) )
	{
		Blasto_Shoot( pB );
		pB->synapse = 0;
	}

	else
	// run current pattern while tracking player.
	{
		if( pB->projectile.active )
		// process the old bullet until it's dead
		{
			
			Blasto_ProcessBullet( pB , pP );			
		}
		
		// track position of player
		pB->targetMemory[ pB->synapse ] = pP->ent.position;
		pB->synapse++;

		// update blasto based on it's current position.
		if( !Blasto_ExecutePattern( pB ) )
		{
			// move on to the next step
			if( pB->step < 4 )
			{
				pB->step++;
			}

			else 
			{
				pB->step = 0;				
			}
			
			pB->currentPattern = &testPattern[ pB->step ];
		}
	}

	return ( (pB->ent.position.x > -pB->ent.w) || (pB->ent.position.x < -pB->ent.w) )
		 ? ( r_true ) : ( r_false );
}

static Vect2_t Blasto_PredictTarget( pBlasto_t pB , pPlayer_t pP )
{
	Vect2_t target = { 0 , 0 };

	return target;
}

static void Blasto_Shoot( pBlasto_t pB )
// shoots at the target stored in memory
{
	// activate projectile
	pB->projectile.active = r_true;
	pB->projectile.position = pB->ent.position;
	//pB->projectile.position.x = pB->targetMemory[ BLASTO_MEMORY_SIZE - 1 ].x;
}

static r_boolean Blasto_ProcessBullet( pBlasto_t pB , pPlayer_t pP )
{
	if( pB->projectile.position.x > -pB->projectile.w )
	{
		pB->projectile.position.x -= BLASTO_PROJECTILE_SPEED;

		if( Ent_TestForCollision( &pP->ent , &pB->projectile ) )
		{
			pB->projectile.active = r_false; // damage player
		}
	}

	else
	{
		pB->projectile.active = r_false;	
	}

	return pB->projectile.active;
}

static r_boolean Blasto_ExecutePattern( pBlasto_t pB )
// return false when unable to complete the pattern
{
	r_boolean upComplete = r_false,
		    downComplete = r_false,
			leftComplete = r_false,
	   	   rightComplete = r_false,
	     patternIncomplete = r_true;

	if( pB->currentControlState.directions[ PATTERN_DIRECTION_LEFT ] < pB->currentPattern->directions[ PATTERN_DIRECTION_LEFT ] )
	{
		pB->ent.position.x -= pB->ent.speed;
		pB->currentControlState.directions[ PATTERN_DIRECTION_LEFT ] += pB->ent.speed;
	}

	else
	{
		leftComplete = r_true;
	}

	if( pB->currentControlState.directions[ PATTERN_DIRECTION_RIGHT ] < pB->currentPattern->directions[ PATTERN_DIRECTION_RIGHT ] )
	{
		pB->ent.position.x += pB->ent.speed;
		pB->currentControlState.directions[ PATTERN_DIRECTION_RIGHT ] += pB->ent.speed;
	}

	else
	{
		rightComplete = r_true;
	}

	if( pB->currentControlState.directions[ PATTERN_DIRECTION_UP ] < pB->currentPattern->directions[ PATTERN_DIRECTION_UP ] )
	{
		pB->ent.position.y -= pB->ent.speed;
		pB->currentControlState.directions[ PATTERN_DIRECTION_UP ] += pB->ent.speed;
	}

	else
	{
		upComplete = r_true;
	}

	if( pB->currentControlState.directions[ PATTERN_DIRECTION_DOWN ] < pB->currentPattern->directions[ PATTERN_DIRECTION_DOWN ] )
	{
		pB->ent.position.y += pB->ent.speed;
		pB->currentControlState.directions[ PATTERN_DIRECTION_DOWN ] += pB->ent.speed;
	}

	else
	{
		downComplete = r_true;
	}

	// determine if the pattern is completely completed.
	
	if( leftComplete && rightComplete && upComplete && downComplete ) 
	{
		patternIncomplete = r_false;
	}

	return patternIncomplete;
}

void Blasto_Draw( pBlasto_t pB )
{
	int color = ( pB->ent.collided ) ? makecol( 255 , 255 , 255 ) : makecol( 255 , 187 , 255 );

	// draw blasto
	rectfill( doubleBuffer ,
		      pB->ent.position.x , 
			  pB->ent.position.y , 
			  pB->ent.position.x + pB->ent.w , 
			  pB->ent.position.y + pB->ent.h , 
			  color );

	// draw bullet if its activeated.
	if( pB->projectile.active )
	{
		int bulletColor = (pB->pulse) ? (makecol(255,255,255)) : (makecol(255,0,0));

		if( pB->projectile.collided )
		{
			bulletColor = makecol( 0 , 0 , 0 );
		}

		pB->pulse = (pB->pulse) ? (r_false) : (r_true);

		rectfill( doubleBuffer , 
				  pB->projectile.position.x , 
				  pB->projectile.position.y , 
				  pB->projectile.position.x + pB->projectile.w , 
				  pB->projectile.position.y + pB->projectile.h , 
				  bulletColor 
				 );
	}

	// debug : draw blasto tracking 
	for( int p=0;p<pB->synapse-1;p++ )
	{
		int lineColor=makecol( 255-pB->synapse,0,0);
		
		line( doubleBuffer , pB->targetMemory[p].x , pB->targetMemory[p].y , pB->targetMemory[p+1].x , pB->targetMemory[p+1].y , makecol( 255 , 200 , 200 ) );
		line( doubleBuffer , pB->targetMemory[p].x+1 , pB->targetMemory[p].y+1 , pB->targetMemory[p+1].x+1 , pB->targetMemory[p+1].y+1 , makecol( 255 , 200 , 200 ) );
		line( doubleBuffer , pB->targetMemory[p].x-1 , pB->targetMemory[p].y-1 , pB->targetMemory[p+1].x-1 , pB->targetMemory[p+1].y-1 , makecol( 255 , 200 , 200 ) );
	}

	G_WriteToDebugWindow( pB->ent.onscreenTargetID , "%d" , pB->ent.position.x , pB->ent.position.y );
	G_WriteToDebugWindow( pB->ent.virtualPointerLink , "%d" , pB->ent.position.x + (pB->ent.w>>1) , pB->ent.position.y + ( pB->ent.h>>1) );
	G_WriteToDebugWindow( pB->step , "%d" , pB->ent.position.x + pB->ent.w, pB->ent.position.y );
}