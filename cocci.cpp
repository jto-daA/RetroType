
/*
** Coccus/Cocci routines for attacking,cocci formation,multiplying etc.
*/

#include <allegro.h>

#include "common.h"
#include "cocci.h"
#include "player.h"
#include "game.h"
#include "vr_heap.h"

extern BITMAP* doubleBuffer;
extern float masterScale;

static BITMAP* coccusFrames[ COCCI_FRAMES ];

static char* fileNames[] = 
{	
	"content/gfx/enemies/coccus/cc_1.bmp" ,
	"content/gfx/enemies/coccus/cc_2.bmp" ,
	"content/gfx/enemies/coccus/cc_3.bmp" ,
	"content/gfx/enemies/coccus/cc_4.bmp" ,
	"content/gfx/enemies/coccus/cc_5.bmp" , 
	
	"content/gfx/enemies/coccus/cc_6.bmp" ,
	"content/gfx/enemies/expl_1.bmp" , 
	"content/gfx/enemies/expl_2.bmp" , 
	"content/gfx/enemies/expl_3.bmp" , 
	"content/gfx/enemies/expl_4.bmp" , 

	"content/gfx/enemies/expl_5.bmp" , 
	"content/gfx/enemies/expl_6.bmp" , 
	"content/gfx/enemies/expl_7.bmp" , 
	"content/gfx/enemies/expl_8.bmp" 
};

static int runningSequence[] = 
{ 
	COCCI_FRAME_00 , 
	COCCI_FRAME_00 , 
	COCCI_FRAME_00 , 
	COCCI_FRAME_00 , 
	COCCI_FRAME_01 , 
	COCCI_FRAME_02 , 
	COCCI_FRAME_03 , 
	COCCI_FRAME_04 ,
	COCCI_FRAME_05 , 
	COCCI_FRAME_05 , 
	COCCI_FRAME_04 , 
	COCCI_FRAME_03 , 
	COCCI_FRAME_02 , 
	COCCI_FRAME_01 
};

static int explosionSequence[] = 
{
	COCCI_FRAME_06 , 
	COCCI_FRAME_07 , 
	COCCI_FRAME_08 , 
	COCCI_FRAME_09 , 
	COCCI_FRAME_10 , 
	COCCI_FRAME_11 , 
	COCCI_FRAME_12 , 
	COCCI_FRAME_13 
};

static SequenceInfo_t sequences[] = 
{
	{ COCCI_RUNNING_SEQUENCE_STEPS , 0 , runningSequence , 0 , 0 , -1 } , 
	{ COCCI_EXPLOSION_SEQUENCE_STEPS , 0 , explosionSequence , 0 , 0 , -1 }
};

void Coccus_Spawn( pCoccus_t pCC , int ttm , float xPosition , float yPosition )
{
	P_CreateEntity( &pCC->ent , 0 , 0 , 20 , 20 , xPosition , yPosition , 0 , 0 , ENTITY_TYPE_COCCUS );
	
	pCC->timeToMultiply = ttm;
	pCC->state = CC_STARTING | CC_SEARCHING;
	pCC->child = NULL;

	pCC->ent.frames = coccusFrames;

	pCC->ent.animationData.rate = COCCI_ANIMATION_RATE;

	Ent_UpdateAnimationSequence( &pCC->ent , &sequences[ COCCI_RUNNING_SEQUENCE ] );

	Ent_AddExplosions( &pCC->ent , pCC->explosions , COCCI_EXPLOSIONS , &sequences[ COCCI_EXPLOSION_SEQUENCE ] );
}

void Coccus_Spawn( pCoccus_t pCC , int ttm , pSpawnPoint_t pSpawnPoint )
{
	P_CreateEntity( &pCC->ent , 0 , 0 , 30 , 30 , SCREEN_W , pSpawnPoint->mapCoords.y , rand()%BASIC_COCCUS_VELOCITY+1 , 0 , ENTITY_TYPE_COCCUS );
		
	pCC->timeToMultiply = ttm;
	pCC->state = CC_STARTING | CC_SEARCHING;
	pCC->children = 0;
	pCC->child = NULL;

	pCC->ent.frames = coccusFrames;

	pCC->ent.animationData.rate = COCCI_ANIMATION_RATE;

	Ent_UpdateAnimationSequence( &pCC->ent , &sequences[ COCCI_RUNNING_SEQUENCE ] );

	Ent_AddExplosions( &pCC->ent , pCC->explosions , COCCI_EXPLOSIONS , &sequences[ COCCI_EXPLOSION_SEQUENCE ] );
}
/*
** returns the next child to add to the list
*/
static pCoccus_t Coccus_FindNextFreeChild( pCoccus_t pParent )
{
	pCoccus_t pChild = pParent->child;

	
	return pChild;
}

/*
** When it's time to multiply, the cocci multiplies into 2 cocci via fission.
*/
void Coccus_Multiply( pCoccus_t pCC )
{
	pCoccus_t pNewChild = Coccus_FindNextFreeChild( pCC );
	
	if( !pCC->child )
	{
		pCC->child = (pCoccus_t)malloc( sizeof(Coccus_t) );
	}
	
	Coccus_Spawn( pCC->child , pCC->timeToMultiply , pCC->ent.position.x + 5, pCC->ent.position.y + 5);
		
	pCC->children++;	
}

/*
** Looks around the game world for friends to pair up with
*/
int Coccus_SearchForFriends( pCoccus_t pCC , pCoccus_t pWorldCocci )
{
	return -1;
}

/*
** emergent behavoiral routine that creates the tendency of cocci to remain together
*/
static Vect2_t Coccus_FindCommonCenter( pCoccus_t pCoccus , pCoccus_t pWorldCocci , int activeCocci )
{
	Vect2_t avgCenter = {
							pCoccus->ent.position.x,
							pCoccus->ent.position.y
						};

	for( int coccus=0;coccus<activeCocci;coccus++ )
	{
		avgCenter.x += pWorldCocci[ coccus ].ent.position.x;
		avgCenter.y += pWorldCocci[ coccus ].ent.position.y;
	}

	// calculate the average of all active cocci ( consider sampling positional data from the center of each object instead it it's orgin )
	avgCenter.x /= activeCocci;
	avgCenter.y /= activeCocci;

	// keep the searching within the visible area 
	if( avgCenter.y < 0 )
	{
		avgCenter.y = 0;
	}

	else if( avgCenter.y > SCREEN_H )
	{
		avgCenter.y = SCREEN_H;
	}

	if( avgCenter.x < 0 )
	{
		avgCenter.x = 0;
	}

	else if( avgCenter.x > SCREEN_W )
	{
		avgCenter.x = SCREEN_W;
	}
			 

	
	return avgCenter;
}
/*
** "" velocity
*/
static int Coccus_CalcAverageVelocity( pCoccus_t pCoccus , pCoccus_t pWorldCocci , int activeCocci )
{
	int avgVelocity = pCoccus->ent.speed;

	for( int cocci=0;cocci<activeCocci;cocci++ )
	{
		avgVelocity += pWorldCocci[ cocci ].ent.speed;
	}

	avgVelocity /= activeCocci;

	return avgVelocity;
}
/*
** adjust coccus's position so it is closer to the common center
*/
static Vect2_t Coccus_AdjustCenter( pCoccus_t pCC , pVect2_t pvCenter )
{
	if( pCC->state & CC_SEARCHING )
	{
		if( pCC->ent.position.x < pvCenter->x )
		// if position is to the left
		{
			pCC->ent.position.x += pCC->ent.speed;
		}

		else if( pCC->ent.position.x > pvCenter->x )
		// if position is to the right
		{
			pCC->ent.position.x -= pCC->ent.speed;
		}

		else
		{
			ClearStates( &pCC->state , CC_SEARCHING );
		}

		if( pCC->ent.position.y < pvCenter->y )
		// if position is below 
		{
			pCC->ent.position.y += pCC->ent.speed;
		}

		else if( pCC->ent.position.y > pvCenter->y )
		// if position if above
		{
			pCC->ent.position.y -= pCC->ent.speed;
		}

		else 
		{
			ClearStates( &pCC->state , CC_SEARCHING );
		}
	}

	return pCC->ent.position;
}

static int Coccus_AdjustVelocity( pCoccus_t pCC , int avgVelocity )
{
	int speed = pCC->ent.speed;
	
	if( speed < avgVelocity )
	{
		pCC->ent.speed++;
	}

	else if( speed > avgVelocity )
	{
		pCC->ent.speed--;
	}

	else
	{

	}

	return abs( speed - avgVelocity );
}

/*
** Determines if the coccus is close enough to the rest of the existing cocci.
** Returns true if move movement is possible, false if the predetermined distance has been reached.
*/
static r_boolean Coccus_RegulateProximity( pCoccus_t pCC , pCoccus_t pWorldCocci , int groupSize )
{
	r_boolean ableToMove = r_false;
	
	if( !groupSize ) 
	{
		// if the coccus is all alone, allow it to move freely
		return r_true;
	}

	else
	{
		for( int wCoccus = 0;wCoccus < groupSize;wCoccus++ )
		// cycle through each neighboor, and determine if it is to close.
		{
			pCoccus_t pNeighboor = &pWorldCocci[ wCoccus ];

			if( abs( pCC->ent.position.x - pNeighboor->ent.position.x ) < BASIC_COCCUS_PERSONAL_SPACE )
			{
				if( abs( pCC->ent.position.y - pNeighboor->ent.position.y ) < BASIC_COCCUS_PERSONAL_SPACE )
				{
					ableToMove = r_false;
				}

				else
				{
					ableToMove = r_true;
				}
			}
		}

	}

	return ableToMove;
}
/*
** Each coccus decrements it's time to multiply on a FRAME-BY-FRAME basis. 
** Consider a TICK-BASED system... 
*/
r_boolean Coccus_Run( pCoccus_t pCC , pCoccus_t pWorldCocci , int activeCocci )
// note : The Cocci enemy class is one that runs dependant on it's own kind in the world; Hence, pointer to other cocci is passed.
{
	//pCC->ent.position.x-=pCC->ent.speed;

// Part one : Try to find other cocci to pair up with ( adjust speed and position )
	if( activeCocci > COCCI_FORMATION_DI && Coccus_RegulateProximity( pCC , pWorldCocci , activeCocci) )
	// merge, only if there are more than the minimum amount required, and the coccus in question is still out of range
	{
		Vect2_t center = Coccus_FindCommonCenter( pCC , pWorldCocci , activeCocci );
		int      speed = Coccus_CalcAverageVelocity( pCC , pWorldCocci , activeCocci );

		if( pCC->ent.position.x < center.x )
		{
			pCC->ent.position.x += pCC->ent.speed;
		}

		else if( pCC->ent.position.x > center.x )
		{
			pCC->ent.position.x -= pCC->ent.speed;
		}

		else
		{
	
		}

		if( pCC->ent.position.y < center.y )
		{
			pCC->ent.position.y += pCC->ent.speed;
		}

		else if( pCC->ent.position.y > center.y )
		{
			pCC->ent.position.y -= pCC->ent.speed;
		}

		else
		{

		}
	}

	else
	{
		pCC->ent.position.x -= pCC->ent.speed;
	}

	return (pCC->ent.position.x < 20 ) ? (r_false) : (r_true);
}

r_boolean Coccus_Kill( pCoccus_t pCC )
{
	r_boolean completelyDead = r_false;

	if( pCC->state & CC_DYING )
	{
		completelyDead = r_false;
	}

	else if( pCC->state & CC_DEAD )
	{
		completelyDead = r_true;
	}

	pCC->ent.active = r_false;

	return completelyDead;
}

/*
** Completely destroys coccs entity ( removes from game world and deallocates space in virtual heap )
*/
void Coccus_Destroy( pCoccus_t pCC , pGame_t pGame )
{
	Coccus_Kill( pCC );
	G_RemoveEntity( pGame , &pCC->ent );
	VH_Free( &pGame->gameHeap , VH_ENTITY_COCCUS , pCC->virtualPointer );
}

void Coccus_Draw( pCoccus_t pCC )
{
	if( !pCC->ent.active )
	{
		return;
	}
	
	int color = ( pCC->state & CC_STARTING ) ? ( makecol( 255 , 0 , 255 ) ) :
				( pCC->state & CC_SEARCHING ) ? ( makecol( 0 , 255 - pCC->ent.animationData.timeOnCurrentFrame , 255 ) ):
				( pCC->state & CC_RUNNING ) ? ( makecol( 0 , 0 , 0 ) ) :
				( pCC->state & CC_MULTIPLYING ) ? ( makecol( 0 , 0 , 0 ) ) :
				( pCC->state & CC_DYING ) ? ( makecol( 255 , 0 , 0 ) ) :
				( pCC->state & CC_DEAD ) ? ( makecol( 255 , 0 , 0 ) ) : 
				( pCC->state & CC_COLLISION ) ? ( makecol( 255 , 255 , 255 ) ): ( makecol( 255 , 255 , 255 ) );

	int children = pCC->children;
	
	circlefill( doubleBuffer , 
				( pCC->ent.position.x + ( pCC->ent.w >> 1 ) ) ,
				( pCC->ent.position.y + ( pCC->ent.h >> 1 ) ) ,
				BASIC_COCCUS_RADIUS * masterScale,
				color );
	
	G_WriteToDebugWindow( pCC->virtualPointer , "%d" , pCC->ent.position.x + ( pCC->ent.w/2) , pCC->ent.position.y + (pCC->ent.h/2) );
	G_WriteToDebugWindow( pCC->ent.onscreenTargetID , "%d" , pCC->ent.position.x + ( pCC->ent.w/2) , pCC->ent.position.y );

	Ent_Animate( &pCC->ent );
	Ent_Draw( &pCC->ent , 1 , 1 , 0 );
	
	if( pCC->child )
	{
		Coccus_Draw( pCC->child );
	}
}

void SetStates( int* stateVar , int activatedStates )
{
	*stateVar |= activatedStates;
}

void ClearStates( int* stateVar , int clearedStates )
{
	*stateVar &= ~clearedStates;
}

r_boolean StateSet( int flag , int state )
{
	return r_boolean( flag & state );
}

r_boolean Coccus_LoadFrames( void )
{
	return Ent_LoadData( coccusFrames , fileNames , COCCI_FRAMES );
}
