
/************************************************************************************
 *  company_intro.cpp  
 *  
 *	descp : code for company intro
 *
 *  auth  : javery
 *  
 *  path  : C:\Program Files\Microsoft Visual Studio\MyProjects\PrimaryCubes3D\company_intro.cpp
 *
 * iDate  : 10/08/06 
 * version:
 ************************************************************************************/

#include <allegro.h>
#include <math.h>

#include "company_intro.h"
#include "game.h"

extern Game_t currentGame;
extern volatile int globalTickCount;
extern volatile int gameTickCount;
extern BITMAP* pageOne;

BITMAP* monkeyFrames[ COMPANY_MONKEY_FRAMES ];
BITMAP* smasherFrames[ COMPANY_SMASHER_FRAMES ]; // hehehehehehhh..
Monkey_t monkeys[ COMPANY_INTRO_MONKEYS ];

static char* smasherFiles[ COMPANY_SMASHER_FRAMES ] = 
{
	"content/gfx/smasher0000.bmp" , 
	"content/gfx/smasher0001.bmp" , 
	"content/gfx/smasher0002.bmp" , 
	"content/gfx/smasher0003.bmp" , 
	"content/gfx/smasher0004.bmp" , 
	"content/gfx/smasher0005.bmp" , 
	"content/gfx/smasher0006.bmp" , 
	"content/gfx/smasher0007.bmp" , 
	"content/gfx/smasher0008.bmp" , 
	"content/gfx/smasher0009.bmp" , 
	"content/gfx/smasher0010.bmp" , 
	"content/gfx/smasher0011.bmp" , 
	"content/gfx/smasher0012.bmp" , 
	"content/gfx/smasher0013.bmp" , 
	"content/gfx/smasher0014.bmp" , 
	"content/gfx/smasher0015.bmp" , 
	"content/gfx/smasher0016.bmp" , 
	"content/gfx/smasher0017.bmp" , 
	"content/gfx/smasher0018.bmp" , 
	"content/gfx/smasher0019.bmp" , 
	"content/gfx/smasher0020.bmp" , 
	"content/gfx/smasher0021.bmp" , 
	"content/gfx/smasher0022.bmp" , 
	"content/gfx/smasher0023.bmp" , 
	"content/gfx/smasher0024.bmp" , 
	"content/gfx/smasher0025.bmp" , 
	"content/gfx/smasher0026.bmp" , 
	"content/gfx/smasher0027.bmp" , 
	"content/gfx/smasher0028.bmp"  
};

/*
=============================================================================
CIN_LoadFrames

Loads bitmaps required for company intro.
=============================================================================
*/
void CIN_LoadFrames( void )
{
	monkeyFrames[ COMPANY_INTRO_IDLE_01 ] = load_bitmap( "content/gfx/cmp_intro_0000.bmp" , NULL );
	monkeyFrames[ COMPANY_INTRO_IDLE_02 ] = load_bitmap( "content/gfx/cmp_intro_0001.bmp" , NULL );
	monkeyFrames[ COMPANY_INTRO_IDLE_03 ] = load_bitmap( "content/gfx/cmp_intro_0002.bmp" , NULL );
	monkeyFrames[ COMPANY_INTRO_IDLE_04 ] = load_bitmap( "content/gfx/cmp_intro_0003.bmp" , NULL );
	monkeyFrames[ COMPANY_INTRO_IDLE_05 ] = load_bitmap( "content/gfx/cmp_intro_0004.bmp" , NULL );
	monkeyFrames[ COMPANY_INTRO_IDLE_06 ] = load_bitmap( "content/gfx/cmp_intro_0005.bmp" , NULL );
	monkeyFrames[ COMPANY_INTRO_IDLE_07 ] = load_bitmap( "content/gfx/cmp_intro_0006.bmp" , NULL );

	monkeyFrames[ COMPANY_INTRO_WALK_01 ] = load_bitmap( "content/gfx/cmp_intro_0006.bmp" , NULL );
	monkeyFrames[ COMPANY_INTRO_WALK_02 ] = load_bitmap( "content/gfx/cmp_intro_0007.bmp" , NULL );
	monkeyFrames[ COMPANY_INTRO_WALK_03 ] = load_bitmap( "content/gfx/cmp_intro_0008.bmp" , NULL );
	monkeyFrames[ COMPANY_INTRO_WALK_04 ] = load_bitmap( "content/gfx/cmp_intro_0009.bmp" , NULL );

	for( int index = 0;index < COMPANY_SMASHER_FRAMES;index++ )
	{
		smasherFrames[ index ] = load_bitmap( smasherFiles[ index ] , NULL );
	}	

	for( index = 0;index < COMPANY_INTRO_MONKEYS;index++ ) // now find a home... for every monkey :)
	{		
		for( int frame=0;frame < COMPANY_MONKEY_FRAMES;frame++ )
		{
			monkeys[ index ].monkeySprites[ frame ] = monkeyFrames[ frame ];

			// start off every monkey ready to run.
			monkeys[ index ].idle = c_false;
			monkeys[ index ].walking = c_true;
			
			monkeys[ index ].position.xPosition = index;
			monkeys[ index ].position.yPosition = MONKEY_Y_OFFSET + rand() % 30;
			monkeys[ index ].position.zPosition = rand() % 50; // speed

			if(!monkeys[ index ].position.zPosition )
			{ 
				monkeys[ index ].position.zPosition = 1;       // we shall have no lazy monkeys here... 
			}

			monkeys[ index ].stepsToRest = rand() % 5;
			monkeys[ index ].breathsToTake = 0;

			monkeys[ index ].currentFrame = COMPANY_INTRO_WALK_01;
		}
	}
}
/*
==========================================================================
CIN_DrawMonkey

Draws monkey at it's define location.
==========================================================================
*/
void CIN_DrawMonkey( int which , int frame )
{
	masked_stretch_blit( monkeys[ which ].monkeySprites[ frame ] , pageOne , 
		                0 , 0 , 
						monkeys[ which ].monkeySprites[ frame ]->w ,
						monkeys[ which ].monkeySprites[ frame ]->h ,
						monkeys[ which ].position.xPosition , 
						monkeys[ which ].position.yPosition , 
						monkeys[ which ].monkeySprites[ frame ]->w ,
						monkeys[ which ].monkeySprites[ frame ]->h ); 
}
/*
========================================================================
CIN_RunIntro

Runs company intro.
========================================================================
*/
void CIN_RunIntro( void )
{
	clear_to_color( pageOne , G_COLOR_WHITE );

	int i=0;
	
	
	// part one : MONKEYS EVERYWHERE!!!!!
	while( gameTickCount < COMPANY_INTRO_PART1_LENGTH )
	{		
		clear_to_color( pageOne , G_COLOR_WHITE );

		// draw background
		blit( smasherFrames[ COMPANY_INTRO_BG ] , pageOne , 0 , 0 , 0 , 0 , smasherFrames[ COMPANY_INTRO_BG ]->w , smasherFrames[ COMPANY_INTRO_BG ]->h ); 
		
		for( int monkey = 0;monkey < COMPANY_INTRO_MONKEYS;monkey++ )
		{
			CIN_MonkeyLogic( monkey );
			CIN_DrawMonkey( monkey , monkeys[ monkey ].currentFrame );			
		}	

		while( globalTickCount < MONKEY_SPEED )
		{
			// for a bit... 		
		}

		globalTickCount = 0;
		
		blit( pageOne , screen , 0 , 0 , 0 , 0 , 640 , 480 );
	}
	
	// part two : POOR MONKEYS... 
	for( i=0;i < COMPANY_SMASHER_FRAMES;i++ )
	{
		clear_to_color( pageOne , G_COLOR_WHITE );
		blit( smasherFrames[ i ] , pageOne , 0 , 0 , 0 , 0 , smasherFrames[ COMPANY_INTRO_BG ]->w , smasherFrames[ COMPANY_INTRO_BG ]->h );

		if( i < COMPANY_INTRO_SMASH_07 )
		{
			for( int monkey = 0;monkey < COMPANY_INTRO_MONKEYS;monkey++ )
			{
				CIN_MonkeyLogic( monkey );
				CIN_DrawMonkey( monkey , monkeys[ monkey ].currentFrame );			
			}	
		}

		blit( pageOne , screen , 0 , 0 , 0 , 0 , 640 , 480 );

		globalTickCount = 0;
		
		while( globalTickCount < MONKEY_SPEED/2 )
		{

		}

		if( i == COMPANY_INTRO_SMASH_07 )
		// let wall stay on the ground for a moment
		{
			globalTickCount = 0;

			while( globalTickCount < 30 )
			{

			}
		}

	}

	// final wait
	gameTickCount = 0;
	 
	while( gameTickCount < COMPANY_INTRO_FINAL_DELAY )
	{
	
	}
}
/*
=====================================================================
CIN_MonkeyLogic

Funny monkey AI routine....
=====================================================================
*/
void CIN_MonkeyLogic( int which )
{
	if( monkeys[ which ].position.xPosition > currentGame.gfxConfig.xDimen )
	// if the monkey goes to far, wrap it around the other side.
	{ 
		monkeys[ which ].position.xPosition = -5;
	}

	
	if( monkeys[ which ].stepsToRest ) // monkey needs to walk some more.
	{
		if( monkeys[ which ].currentFrame < COMPANY_INTRO_WALK_04 )
		{
			monkeys[ which ].lastFrame = monkeys[ which ].currentFrame;
			monkeys[ which ].currentFrame++;
			monkeys[ which ].position.xPosition += monkeys[ which ].position.zPosition;
			return;
		}
		
		if( monkeys[ which ].currentFrame == COMPANY_INTRO_WALK_04 ) 
		{
			monkeys[ which ].lastFrame = monkeys[ which ].currentFrame;
			monkeys[ which ].currentFrame = COMPANY_INTRO_WALK_01;
		}

		if( monkeys[ which ].lastFrame == COMPANY_INTRO_WALK_04 && monkeys[ which ].currentFrame == COMPANY_INTRO_WALK_01 )
		// monkey has completed a step
		{
			monkeys[ which ].stepsToRest--;
		}
	}

	else //monkey needs to rest.
	{
		if( monkeys[ which ].currentFrame < COMPANY_INTRO_IDLE_07 )
		{
			monkeys[ which ].lastFrame = monkeys[ which ].currentFrame;
			monkeys[ which ].currentFrame++;
			return;
		}
		
		if( monkeys[ which ].currentFrame == COMPANY_INTRO_IDLE_07 ) // loop animation back to beggining
		{
			monkeys[ which ].lastFrame = monkeys[ which ].currentFrame;
			monkeys[ which ].currentFrame = COMPANY_INTRO_IDLE_01;
		}

		if( monkeys[ which ].lastFrame == COMPANY_INTRO_IDLE_07 && monkeys[ which ].currentFrame == COMPANY_INTRO_IDLE_01 )
		// monkey just completed a step
		{
			monkeys[ which ].breathsToTake--;
		}
	}

	if( !monkeys[ which ].breathsToTake ) // if no more breaths to take, then start running.
	{
		monkeys[ which ].currentFrame = COMPANY_INTRO_WALK_01;
		monkeys[ which ].stepsToRest = rand() % MAX_MONKEY_STEPS;
		monkeys[ which ].position.zPosition = rand() % MAX_MONKEY_SPEED;

		if( !monkeys[ which ].position.zPosition )
		{
			monkeys[ which ].position.zPosition = 1;
		}

		monkeys[ which ].breathsToTake = 0;
		monkeys[ which ].idle = c_false;
		monkeys[ which ].walking = c_true;

	}

	else if( !monkeys[ which ].stepsToRest ) // if no more steps to take, then start breathing
	{
		monkeys[ which ].currentFrame = COMPANY_INTRO_IDLE_01;
		monkeys[ which ].breathsToTake = rand() % MAX_MONKEY_BREATHS;
		monkeys[ which ].stepsToRest = 0;
		monkeys[ which ].walking = c_false;
		monkeys[ which ].idle = c_true;
	}
}
/*
========================================================================================
CIN_CleanUp

Cleans up the resources loaded for company intro.
========================================================================================
*/
void CIN_CleanUp( void )
{
	for( int index=0; index < COMPANY_MONKEY_FRAMES;index++ )
	{
		destroy_bitmap( monkeyFrames[ index ] );
	}

	for(index=0;index < COMPANY_SMASHER_FRAMES;index++ )
	{
		destroy_bitmap( smasherFrames[ index ] );
	}
	
}