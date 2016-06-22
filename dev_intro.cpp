/************************************************************************************
 *  dev_intro.cpp  
 *  
 *	descp : Loads content for, and displays developer intro screen
 *
 *  auth  : javery
 *  
 *  path  : C:\Program Files\Microsoft Visual Studio\MyProjects\PrimaryCubes3D\dev_intro.cpp
 *
 * iDate  : 10/06/06 
 * version:
 ************************************************************************************/

#include <allegro.h>
#include <math.h>

#include "game.h"
#include "dev_intro.h"

static BITMAP* devIntroFrames[ DEV_INTRO_FRAMES ];
static BITMAP* devLightMap;
extern volatile int globalTickCount;
extern BITMAP* pageOne;
extern Game_t currentGame;
/*
======================================================================
DIN_LoadFrames

Loads in the animation frames for dev-intro.
======================================================================
*/
c_boolean DIN_LoadFrames( void )
{
	devIntroFrames[ DEV_INTRO_FRAME_01 ] = load_bitmap( "content/gfx/ahayweh_intro_0000.bmp" , NULL );
	devIntroFrames[ DEV_INTRO_FRAME_02 ] = load_bitmap( "content/gfx/ahayweh_intro_0001.bmp" , NULL );
	devIntroFrames[ DEV_INTRO_FRAME_03 ] = load_bitmap( "content/gfx/ahayweh_intro_0002.bmp" , NULL );
	devIntroFrames[ DEV_INTRO_FRAME_04 ] = load_bitmap( "content/gfx/ahayweh_intro_0003.bmp" , NULL );
	devIntroFrames[ DEV_INTRO_FRAME_05 ] = load_bitmap( "content/gfx/ahayweh_intro_0004.bmp" , NULL );
	devIntroFrames[ DEV_INTRO_FRAME_06 ] = load_bitmap( "content/gfx/ahayweh_intro_0005.bmp" , NULL );
	devIntroFrames[ DEV_INTRO_FRAME_07 ] = load_bitmap( "content/gfx/ahayweh_intro_0006.bmp" , NULL );

	devIntroFrames[ DEV_INTRO_BUMP_01 ] = load_bitmap( "content/gfx/ahayweh_bump_0000.bmp" , NULL );
	devIntroFrames[ DEV_INTRO_BUMP_02 ] = load_bitmap( "content/gfx/ahayweh_bump_0001.bmp" , NULL );
	devIntroFrames[ DEV_INTRO_BUMP_03 ] = load_bitmap( "content/gfx/ahayweh_bump_0002.bmp" , NULL );
	devIntroFrames[ DEV_INTRO_BUMP_04 ] = load_bitmap( "content/gfx/ahayweh_bump_0003.bmp" , NULL );
	devIntroFrames[ DEV_INTRO_BUMP_05 ] = load_bitmap( "content/gfx/ahayweh_bump_0004.bmp" , NULL );
	devIntroFrames[ DEV_INTRO_BUMP_06 ] = load_bitmap( "content/gfx/ahayweh_bump_0005.bmp" , NULL );
	devIntroFrames[ DEV_INTRO_BUMP_07 ] = load_bitmap( "content/gfx/ahayweh_bump_0006.bmp" , NULL );

	devIntroFrames[ DEV_INTRO_BACKGROUND ] = load_bitmap( "content/gfx/ahayweh_intro_lightmap.bmp" , NULL );

	// verify file load 
	for( int index = 0;index < DEV_INTRO_FRAMES*2;index++ )
	{
		if(!devIntroFrames[ index ] )
		{
			return c_false;
		}
	}

	// create light map
	int deltaX=0,deltaY=0,distance=0;
	devLightMap = create_bitmap( devIntroFrames[0]->w , devIntroFrames[0]->h );
	
	for( int y=0;y<devLightMap->h;y++ )
	{
		for( int x=0;x<devLightMap->w*4;x++ )
		{
			// calculate the strenght of the current pixel based on its distance away form center.
			deltaX = abs(devLightMap->w/2 - x);
			deltaY = abs(devLightMap->h/2 - y);

			distance = 1-sqrt( deltaX*deltaX + deltaY*deltaY );

			devLightMap->line[ y ][ x ] = distance*256;
		}
	}
	
	return c_true;
}
/*
===========================================================================
DIN_CleanUp

Releases the frames loaded for the dev intro.
===========================================================================
*/
void DIN_CleanUp( void )
{
	for( int index = 0;index < DEV_INTRO_FRAMES/**2*/   ;index++ )
	{
		destroy_bitmap( devIntroFrames[ index ] );
	}

	//destroy_bitmap( devLightMap );
}
/*
===========================================================================
DIN_Animate

Main routine for dev intro. loops through frames at a speed regulated by 
global ticker, for a predetermined time. 
==========================================================================
*/
void DIN_Animate( float xFactor , float yFactor  )
{
	int i=0;

	clear_to_color( pageOne , G_COLOR_WHITE );
	
	for( int cycleCount = 0;cycleCount < DEV_INTRO_LENGHT;cycleCount++ )
	{
		for( int frame = 0;frame < DEV_INTRO_FRAMES;frame++ )
		{			
			
			DIN_DrawBumpedFrame( frame , 0 , 0 );
			blit( devLightMap , pageOne , 0 , 0 , 0 , 0 , currentGame.gfxConfig.xDimen , currentGame.gfxConfig.yDimen );
			
			blit( devIntroFrames[ DEV_INTRO_BACKGROUND ] , pageOne ,  0 , 0 , 0 , 0 , currentGame.gfxConfig.xDimen , currentGame.gfxConfig.yDimen );
			
			masked_blit( devIntroFrames[ frame ] , pageOne ,  0 , 0 , 0 , 0 , currentGame.gfxConfig.xDimen , currentGame.gfxConfig.yDimen );
			blit( pageOne , screen , 0 , 0 , 0 , 0 , currentGame.gfxConfig.xDimen , currentGame.gfxConfig.yDimen );				 
			
			globalTickCount = 0;
			
			while( globalTickCount < DEV_INTRO_DELAY )
			{
				// wait for a bit.
			}

			clear_to_color( pageOne , G_COLOR_WHITE );
		}
	}
}
/*
==========================================================================
DIN_DrawBumpedFrame

Draws passed frames as peturbed by psuedo-bump map
==========================================================================
*/
static void DIN_DrawBumpedFrame( int frame , float lightX ,float lightY )
{
	int w=devIntroFrames[0]->w*4-1,  // ???? something to do with address mode... 
		h=devIntroFrames[0]->h-1,
	    r=0,g=0,b=0;
	
	int deltaX=0,deltaY=0,intensity=1;
	float normal=0; 

	for( int y=1;y<h;y++ )
	{
		for( int x=1;x<w;x++ )
		{
			// compute slope of vector at x,y using the data stored in bumpmap
			deltaX = devIntroFrames[ frame + DEV_INTRO_FRAMES ]->line[ y ][ x + 1 ] - 
				     devIntroFrames[ frame + DEV_INTRO_FRAMES ]->line[ y ][ x ];
			
			deltaY = devIntroFrames[ frame + DEV_INTRO_FRAMES ]->line[ y + 1 ][ x ] - 
				     devIntroFrames[ frame + DEV_INTRO_FRAMES ]->line[ y ][ x ];

			// clamp delta values
			if( deltaX < 0 || deltaX > w || ((deltaX + (w/2)) > w))
			{
				deltaX = 1;
			}
			
			if( deltaY < 0 || deltaY > h || ((deltaY +(h/2)) > h ))
			{
				deltaY = 1;
			}	
			
			// using calculated normal, compute the intensity of the light using the lightmap
			intensity = devLightMap->line[ deltaY + (h/2) ][ deltaX + ( w/2 ) ];
			
			r = getr( devIntroFrames[ frame ]->line[ y ][ x ] );
			g = getg( devIntroFrames[ frame ]->line[ y ][ x ] );
			b = getb( devIntroFrames[ frame ]->line[ y ][ x ] );
			
			r += intensity;
			g += intensity;
			b += intensity;

			pageOne->line[ y + DEV_INTRO_Y_OFFSET*2][ x ] = makecol( r , g , b );
			//_putpixel16( pageOne , x , y , makecol( r , g ,b ) );
		}
	}
}
