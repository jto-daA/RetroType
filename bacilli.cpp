
#include <allegro.h>

#include "game.h"
#include "bacilli.h"
#include "player.h"

extern BITMAP*  doubleBuffer;
extern float     masterScale;
extern float    sineLookup[]; // limit access to TRIG_TABLE_SIZE
extern float  cosineLookup[];

static BITMAP* bacilliFrames[ BACILLI_FRAMES ];

static char* fileNames[] = 
{
	"content/gfx/enemies/bacilli/bc_1.bmp" , 
	"content/gfx/enemies/bacilli/bc_2.bmp" , 
	"content/gfx/enemies/bacilli/bc_3.bmp" , 
	"content/gfx/enemies/bacilli/bc_4.bmp" , 
	"content/gfx/enemies/bacilli/bc_5.bmp" , 

	"content/gfx/enemies/bacilli/bc_seg_1.bmp" , 
	"content/gfx/enemies/bacilli/bc_seg_2.bmp" , 
	"content/gfx/enemies/bacilli/bc_seg_3.bmp" , 
	"content/gfx/enemies/bacilli/bc_seg_4.bmp" , 
	"content/gfx/enemies/bacilli/bc_seg_5.bmp" , 

	"content/gfx/enemies/expl_1.bmp" , 
	"content/gfx/enemies/expl_2.bmp" , 
	"content/gfx/enemies/expl_3.bmp" , 
	"content/gfx/enemies/expl_4.bmp" , 
	"content/gfx/enemies/expl_5.bmp" , 

	"content/gfx/enemies/expl_6.bmp" , 
	"content/gfx/enemies/expl_7.bmp" , 
	"content/gfx/enemies/expl_8.bmp" 
};

static int headRunningAnimation[] = 
{
	BACILLI_FRAME_00 , 
	BACILLI_FRAME_01 , 
	BACILLI_FRAME_02 , 
	BACILLI_FRAME_02 ,
	BACILLI_FRAME_03 , 
	BACILLI_FRAME_04 , 
	BACILLI_FRAME_03 , 
	BACILLI_FRAME_02 , 
	BACILLI_FRAME_02 , 
	BACILLI_FRAME_01
};

static int segmentRunningAnimation[] = 
{
	BACILLI_FRAME_05 , 
	BACILLI_FRAME_06 , 
	BACILLI_FRAME_07 , 
	BACILLI_FRAME_07 ,
	BACILLI_FRAME_08 , 
	BACILLI_FRAME_09 , 
	BACILLI_FRAME_08 , 
	BACILLI_FRAME_07 , 
	BACILLI_FRAME_07 , 
	BACILLI_FRAME_06
};

static int explosionSequence[] = 
{
	BACILLI_FRAME_10 , 
	BACILLI_FRAME_11 , 
	BACILLI_FRAME_12 , 
	BACILLI_FRAME_13 , 
	BACILLI_FRAME_14 , 
	BACILLI_FRAME_15 , 
	BACILLI_FRAME_16 , 
	BACILLI_FRAME_17
};

static SequenceInfo_t bacilliSequences[] = 
{
	{ BACILLI_HEAD_RUNNING_SEQUENCE_STEPS , 0 , headRunningAnimation , 0 , 0 , -1 } ,
	{ BACILLI_SEGM_RUNNING_SEQUENCE_STEPS , 0 , segmentRunningAnimation , 0 , 0 , -1 } , 
	{    BACILLI_EXPLOSION_SEQUENCE_STEPS , 0 ,    explosionSequence , 0 , 0 , -1 }

};

// BACILLI ////////////////////////////////////////////////////////////////////////////////////////////

void Bacilli_Spawn( pBacilli_t pBC , int xPosition , int yPosition )
{
	pBC->amplitude = rand()%MAX_BACILLI_AMPLITUDE;
	pBC->periodFactor = rand()%MAX_BACILLI_PERIOD_FACTOR;
	pBC->trigIndex = 0;

	if( !pBC->amplitude )
	{
		pBC->amplitude = 1;
	}

	if( !pBC->periodFactor )
	{
		pBC->periodFactor = 1;
	}

	P_CreateEntity( &pBC->ent , 0 , 0 , BACILLI_SIZE_X , BACILLI_SIZE_Y , xPosition , yPosition , BACILLI_SPEED , 0 , ENTITY_TYPE_BACILLI );

	pBC->ent.frames = bacilliFrames;
	pBC->ent.animationData.rate = BACILLI_ANIMATION_RATE;

	Ent_UpdateAnimationSequence( &pBC->ent , &bacilliSequences[ BACILLI_HEAD_RUNNING_SEQUENCE ] );

	Ent_AddExplosions( &pBC->ent , pBC->explosions , BACILLI_EXPLOSIONS , &bacilliSequences[ BACILLI_EXPLOSION_SEQUENCE ] );
}

void Bacilli_Spawn( pBacilli_t pBC , pSpawnPoint_t pSpawnPoint )
{
	pBC->amplitude = rand() % MAX_BACILLI_AMPLITUDE;
	pBC->periodFactor = rand() % MAX_BACILLI_PERIOD_FACTOR;
	pBC->trigIndex = 0;
	pBC->ent.entityType = ENTITY_TYPE_BACILLI;

	if( !pBC->amplitude ) pBC->amplitude = 1;

	if( !pBC->periodFactor ) pBC->periodFactor = 1;

	P_CreateEntity( &pBC->ent , 0 , 0 , BACILLI_SIZE_X , BACILLI_SIZE_Y , SCREEN_W , pSpawnPoint->mapCoords.y , BACILLI_SPEED , 0 ,  ENTITY_TYPE_BACILLI );

	pBC->ent.frames = bacilliFrames;
	pBC->ent.animationData.rate = BACILLI_ANIMATION_RATE;

	Ent_UpdateAnimationSequence( &pBC->ent , &bacilliSequences[ BACILLI_HEAD_RUNNING_SEQUENCE ] );

	Ent_AddExplosions( &pBC->ent , pBC->explosions , BACILLI_EXPLOSIONS , &bacilliSequences[ BACILLI_EXPLOSION_SEQUENCE ] );
}

/*
** returns true if it is completely dead, false otherwise
*/
r_boolean Bacilli_Kill( pBacilli_t pBC )
{
	return r_true;
}

void Bacilli_Destroy( pBacilli_t pBC )
{


}

r_boolean Bacilli_Run( pBacilli_t pBC )
{
	pBC->ent.position.x -= pBC->ent.speed;	

	if( (pBC->trigIndex * pBC->periodFactor ) >= TRIG_TABLE_SIZE )
	{
		pBC->trigIndex = 0;
	}

	else
	{
		pBC->trigIndex++;
	}
		
	// good ole trig-displacement path.
	pBC->ent.position.y += pBC->amplitude * cosineLookup[ pBC->trigIndex * pBC->periodFactor ];

	// explode if collided
	if( pBC->ent.collided )
	{
		for( int explosion = 0;explosion < BACILLI_EXPLOSIONS;explosion++ )
		{
			Ent_InitExplosion( &pBC->ent , &pBC->explosions[ explosion ] , BACILLI_EXPLOSION_LENGTH );
		}
	}

	// prepare to kill if sufficently offscreen
	return ( pBC->ent.position.x > -30 ) ? ( r_true ) : ( r_false );
}

void Bacilli_Draw( pBacilli_t pBC )
{
	int color = ( pBC->ent.collided ) ? makecol( 255 , 255 , 255 ) : makecol( 255 , 105 , 180 );
	
//	ellipsefill( doubleBuffer , pBC->ent.position.x , pBC->ent.position.y , 20 , 10 , color );	
	G_WriteToDebugWindow( pBC->ent.onscreenTargetID , "%i" , pBC->ent.position.x , pBC->ent.position.y );

	Ent_Animate( &pBC->ent );
	Ent_Draw( &pBC->ent , 1 , 1 , 0 );

	for( int explosion = 0;explosion < BACILLI_EXPLOSIONS;explosion++ )
	{
		if( pBC->explosions[ explosion ].ent.active )
		{
			pExplosion_t pEX = &pBC->explosions[ explosion ];

			Ent_RunExplosion( &pBC->ent , pEX );
			Ent_Animate( &pEX->ent );
			Ent_Draw( &pEX->ent , pEX->scale , pEX->scale , 0 );
		}
	}
}

// DIPLO BACILLI ////////////////////////////////////////////////////////////////////////////////////////////

void DiploBacilli_Spawn( pDiploBacilli_t pDB , int xPosition , int yPosition )
{
	
	pBacilli_t pHead = &pDB->segments[ BACILLI_HEAD_SEGMENT ];
	pBacilli_t pTail = &pDB->segments[ BACILLI_SEGMENT_1 ];
	
	// first spawn a randomized head.
	Bacilli_Spawn( pHead , xPosition , yPosition );
	Bacilli_Spawn( pTail , ( xPosition + pHead->ent.w ) , yPosition ); // some of these values will be overwritten... 

	// use the generated head to derived the tail segment
	pTail->amplitude = pHead->amplitude;
	pTail->periodFactor = pHead->periodFactor;
	pTail->trigIndex = 0;

	// set the ent types
	pHead->ent.entityType = ENTITY_TYPE_DIPLOBACILLI;
	pTail->ent.entityType = ENTITY_TYPE_DIPLOBACILLI;
	
	// set animation data 
	pHead->ent.frames = bacilliFrames;
	pHead->ent.animationData.rate = 
	pTail->ent.animationData.rate = BACILLI_ANIMATION_RATE;

	Ent_UpdateAnimationSequence( &pHead->ent , &bacilliSequences[ BACILLI_HEAD_RUNNING_SEQUENCE ] );
	Ent_UpdateAnimationSequence( &pTail->ent , &bacilliSequences[ BACILLI_SEGM_RUNNING_SEQUENCE ] );
}

void DiploBacilli_Spawn( pDiploBacilli_t pDB , pSpawnPoint_t pSpawnPoint )
{
	DiploBacilli_Spawn( pDB , SCREEN_W , pSpawnPoint->mapCoords.y );
}

r_boolean DiploBacilli_Kill( pDiploBacilli_t pDB )
{
	return r_true;
}

void DiploBacilli_Destroy( pDiploBacilli_t pDB )
{

}

r_boolean DiploBacilli_Run( pDiploBacilli_t pDB )
{
	r_boolean active = 

	// diplos are syncronous, so just call the bacilli routine on each segment
	Bacilli_Run( &pDB->segments[ BACILLI_SEGMENT_1 ] );
	Bacilli_Run( &pDB->segments[ BACILLI_HEAD_SEGMENT ] );
	
	return active;
}

void DiploBacilli_Draw( pDiploBacilli_t pDB )
{
	Bacilli_Draw( &pDB->segments[ BACILLI_SEGMENT_1 ] );
	Bacilli_Draw( &pDB->segments[ BACILLI_HEAD_SEGMENT ] );
}

// STREPTO BACILLI ////////////////////////////////////////////////////////////////////////////////////////////

void StreptoBacilli_Spawn( pStreptoBacilli_t pSBC ,  int xPosition , int yPosition )
{
	pBacilli_t head = &pSBC->segments[ BACILLI_HEAD_SEGMENT ];
	int seperation = 32;//head->ent.w;
	
	// create distance between each segment.
	Bacilli_Spawn( &pSBC->segments[ BACILLI_HEAD_SEGMENT ] ,  xPosition + ( seperation * 0 ) , yPosition ); // seperation == NO_OP
	Bacilli_Spawn( &pSBC->segments[ BACILLI_SEGMENT_1 ] ,     xPosition + ( seperation * 1 ) , yPosition );
	Bacilli_Spawn( &pSBC->segments[ BACILLI_SEGMENT_2 ] ,     xPosition + ( seperation * 2 ) , yPosition );
	Bacilli_Spawn( &pSBC->segments[ BACILLI_SEGMENT_3 ] ,     xPosition + ( seperation * 3 ) , yPosition );
	Bacilli_Spawn( &pSBC->segments[ BACILLI_TAIL_SEGMENT ] ,  xPosition + ( seperation * 4 ) , yPosition );

	// reassign this ( changed by Bacilli_Spawn )
	head->ent.entityType = ENTITY_TYPE_STREPTOBACILLI;	
	
	// head's animation stuff
	head->ent.frames = bacilliFrames;
	head->ent.animationData.rate = BACILLI_ANIMATION_RATE;
	Ent_UpdateAnimationSequence( &head->ent , &bacilliSequences[ BACILLI_HEAD_RUNNING_SEQUENCE ] );	

	// unify the segments according to the randomized movement data in the head.
	for( int segment = 1;segment < BASIC_STREPTOBACILLI_SEGMENTS;segment++ )
	{
		pSBC->segments[ segment ].amplitude =    head->amplitude;
		pSBC->segments[ segment ].periodFactor = head->periodFactor;

		// create a variance in phase-shift inorder to break syncronicity.
		pSBC->segments[ segment ].trigIndex = (segment * 10) / head->periodFactor;
		
		pSBC->segments[ segment ].ent.entityType = ENTITY_TYPE_STREPTOBACILLI;

		// animation stuphs
		pSBC->segments[ segment ].ent.animationData.rate = BACILLI_ANIMATION_RATE;
		pSBC->segments[ segment ].ent.frames = bacilliFrames;
		Ent_UpdateAnimationSequence( &pSBC->segments[ segment ].ent , &bacilliSequences[ BACILLI_SEGM_RUNNING_SEQUENCE ] ); 

	}
}

void StreptoBacilli_Spawn( pStreptoBacilli_t pSBC , pSpawnPoint_t pSpawnPoint )
{
	StreptoBacilli_Spawn( pSBC , SCREEN_W , pSpawnPoint->mapCoords.y );
}

r_boolean StreptoBacilli_Kill( pStreptoBacilli_t pSBC )
{
	return r_true;
}

void StreptoBacilli_Destroy( pStreptoBacilli_t pSBC )
{

}

r_boolean StreptoBacilli_Run( pStreptoBacilli_t pSBC )
{
	int speed = pSBC->segments[ BACILLI_HEAD_SEGMENT ].ent.speed;
	
	for( int segment = 0;segment < BASIC_STREPTOBACILLI_SEGMENTS;segment++ )
	{
		Bacilli_Run( &pSBC->segments[ segment ] );
	}
	
	// run until the tail is completely offscreen
	return ( pSBC->segments[ BACILLI_TAIL_SEGMENT ].ent.position.x > -10 ) ? ( r_true ) : ( r_false );
}

void StreptoBacilli_Draw( pStreptoBacilli_t pSBC )
{
	for( int segment = BASIC_STREPTOBACILLI_SEGMENTS-1;segment>-1;segment-- )
	{
		Bacilli_Draw( &pSBC->segments[ segment ] );
	}
}

r_boolean Bacilli_LoadFrames( void )
{
	return Ent_LoadData( bacilliFrames , fileNames , BACILLI_FRAMES );
}
