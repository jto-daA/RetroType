
#include <allegro.h>
#include <math.h>

#include "game.h"
#include "kamikaze.h"
#include "common.h"
#include "player.h"

extern BITMAP* doubleBuffer;
extern float masterScale;

static BITMAP* kamiData[ KAMIKAZE_FRAMES ];

static char* fileNames[] = 
{
	"content/gfx/enemies/kamikaze/kamikaze_1.bmp" , 
    "content/gfx/enemies/kamikaze/kamikaze_2.bmp" , 
	"content/gfx/enemies/kamikaze/kamikaze_3.bmp" ,
	"content/gfx/enemies/kamikaze/flame_1.bmp" ,
	"content/gfx/enemies/kamikaze/flame_2.bmp" ,
	
	"content/gfx/enemies/kamikaze/flame_3.bmp" ,
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
	KAMIKAZE_FRAME_00 , 
	KAMIKAZE_FRAME_01 , 
	KAMIKAZE_FRAME_02 , 
	KAMIKAZE_FRAME_01 
};

static int explosionSequence[] = 
{
	KAMIKAZE_EXPLODE_FRAME_00 , 
	KAMIKAZE_EXPLODE_FRAME_01 , 
	KAMIKAZE_EXPLODE_FRAME_02 , 
	KAMIKAZE_EXPLODE_FRAME_03 , 
	KAMIKAZE_EXPLODE_FRAME_04 , 
	KAMIKAZE_EXPLODE_FRAME_05 , 
	KAMIKAZE_EXPLODE_FRAME_06 , 
	KAMIKAZE_EXPLODE_FRAME_07
};

static SequenceInfo_t sequences[] = 
{
	{ KAMIKAZE_RUNNING_SEQUENCE_STEPS , 0 , runningSequence , 0 , 0 , -1 } ,
	{ KAMIKAZE_EXPLODE_SEQUENCE_STEPS , 0 , explosionSequence , 0 , 0 , -1 } 
};

r_boolean Kamikaze_LoadFrames( void )
{
	r_boolean status = Ent_LoadData( kamiData , fileNames , KAMIKAZE_FRAMES );
	
	return status;
}

void Kamikaze_SpawnRandom( pKamikaze_t pK )
{	
	P_CreateEntity( &pK->ent , 0 , 0 , 32 , 32 , 0 , 0 , 0 , 0 , ENTITY_TYPE_KAMIKAZE );
	
	pK->ent.position.y = rand() % SCREEN_H;
	pK->ent.position.x = SCREEN_W + 20;
	pK->ent.speed = rand() % 20   + 1;
	pK->ent.active = r_true;
	pK->ent.w = 32; // change later
	pK->ent.h = 32;

	pK->ent.frames = kamiData;
}

void Kamikaze_Spawn( pKamikaze_t pK , int x , int y , int speed )
{
	P_CreateEntity( &pK->ent , 0 , 0 , KAMIKAZE_SIZE_X , KAMIKAZE_SIZE_Y , x , y , speed , KAMIKAZE_FRAMES , ENTITY_TYPE_KAMIKAZE );
	
    pK->ent.position.x = x;
	pK->ent.position.y = y;
	pK->ent.speed =  speed;

	pK->ent.w = KAMIKAZE_SIZE_X;
	pK->ent.h = KAMIKAZE_SIZE_Y;

	pK->ent.minBounds.x = 0;
	pK->ent.minBounds.y = 0;
	pK->ent.maxBounds.x = 32;
	pK->ent.maxBounds.y = 32;

	pK->ent.frames = kamiData;

	Ent_UpdateAnimationSequence( &pK->ent , &sequences[ KAMIKAZE_RUNNING_SEQUENCE ] );
	pK->ent.animationData.rate = KAMIKAZE_ANIMATION_RATE;

	Ent_AddExplosions( &pK->ent , pK->explosions , KAMIKAZE_EXPLOSIONS , &sequences[ KAMIKAZE_EXPLODE_SEQUENCE ] );
}

void Kamikaze_Kill( pKamikaze_t pK )
{
	pK->ent.active = r_false;
	P_DestroyEntity( &pK->ent );
}

void Kamikaze_Draw( pKamikaze_t pK )
{
	if( !pK->ent.active )
	{
		return;
	}
	
	int color = pK->ent.collided ? makecol( 0 , 255 , 0 ) : makecol( 128 , 128 , 255 );
	
	//rectfill( doubleBuffer , pK->ent.position.x , pK->ent.position.y , pK->ent.position.x + 30 , pK->ent.position.y + 30 , color );
	//G_WriteToDebugWindow( pK->ent.onscreenTargetID , "%i" , pK->ent.position.x , pK->ent.position.y );
	
	G_WriteToDebugWindow( pK->ent.virtualPointerLink , "%i" , pK->ent.position.x  , pK->ent.position.y +20 );

	Ent_Animate( &pK->ent );
	
	// draw the flame
	masked_stretch_blit( pK->ent.frames[ pK->ent.animationData.currentFrame + KAMIKAZE_FLAME_FRAME_00 ] , 
						 doubleBuffer ,
						 0 ,
						 0 ,
						 pK->ent.frames[0]->w ,
						 pK->ent.frames[0]->h ,
						 pK->ent.position.x + FLAME_OFFSET * masterScale ,
						 pK->ent.position.y ,
						 pK->ent.w * masterScale ,
						 pK->ent.h * masterScale );

	// draw the kamikaze
	Ent_Draw( &pK->ent , 1 , 1 , 0 ); 

	// draw the explosions
	for( int explosion = 0;explosion < KAMIKAZE_EXPLOSIONS;explosion++ )
	{
		if( pK->explosions[ explosion ].ent.active )
		{
			pExplosion_t pEX = &pK->explosions[ explosion ];

			Ent_RunExplosion( &pK->ent , pEX );
			Ent_Animate( &pEX->ent );
			Ent_Draw( &pEX->ent , pEX->scale , pEX->scale , 0 );
		}
	}
}

r_boolean Kamikaze_Run( pKamikaze_t pK )
{
	if( pK->ent.position.x >= -( pK->ent.speed + pK->ent.w ) )
	//if( pK->ent.position.x > 20 )
	{
		pK->ent.position.x -= pK->ent.speed;
	}

	else
	{
		Kamikaze_Kill( pK );
		P_DestroyEntity( &pK->ent );
	}

	/* explode if necessary*/
	if( pK->ent.collided )
	{
		for(int explosion = 0;explosion < KAMIKAZE_EXPLOSIONS;explosion++ )
		{
			Ent_InitExplosion( &pK->ent , &pK->explosions[ explosion ] , KAMIKAZE_EXPLOSION_LENGTH );
		}
	}
	
	return pK->ent.active;	
}
