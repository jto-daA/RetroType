
#include <allegro.h>
#include <math.h>

#include "droid.h"
#include "player.h"
#include "game.h"

extern volatile int msecTicker;
extern BITMAP* doubleBuffer;
extern float cosineLookup[];

void Droid_AttachToParent( pPlayer_t pP , Entity_t pDroid )
{

}

void Droid_UpdatePosition( pPlayer_t pPlayer )
{
	pDroid_t pDroid = &pPlayer->buddy;
	pVect2_t pv = &pDroid->ent.position;

	if( pDroid->trigIndex >= TRIG_TABLE_SIZE-1 )
	{
		pDroid->trigIndex = 0;
	}

	else
	{
		pDroid->trigIndex++;
	}

	if( CalcDistance( &pPlayer->ent.position , pv ) <= DROID_COMFORT_DISTANCE )
	{	// wobble it a lil-bit
		pv->y += DROID_AMPLITUDE * cosineLookup[ pDroid->trigIndex ];
	}

	else
	{
		//pv->y -= DROID_AMPLITUDE * cosineLookup[ pDroid->trigIndex ];
	}

}LOCK_FUNCTION( Droid_UpdatePosition)

void Droid_Draw( pEntity_t pDroid )
{
	//Ent_Draw( pDroid );
	rectfill( doubleBuffer , pDroid->position.x-10 , pDroid->position.y-10 , pDroid->position.x + 5 , pDroid->position.y + 5 , makecol( 0 , 0 , 255 ) );
}
