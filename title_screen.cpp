/************************************************************************************
 *  title_screen.h  
 *  
 *	descp : display title screen , enter or exit game based on user input 
 *
 *  auth  : javery
 *  
 *  path  : C:\Program Files\Microsoft Visual Studio\MyProjects\PrimaryCubes3D\title_screen.h
 *
 * iDate  : 10/28/06 
 * version:
 ************************************************************************************/

#include <allegro.h>
#include <math.h>

#include "gfx_font.h"
#include "game.h"
#include "title_screen.h"

static char* titleFile = "content/gfx/title_screen.bmp";
static BITMAP* titleScreen,*distortBuffer;
static char* options[] = { "START GAME" , "EXIT","V D0.0" };
static TitleScreenStatus_t titleScreenStatus = { c_true , c_true , c_false , c_false };

extern BITMAP* pageOne;

/*
=========================================================================
TS_LoadTitleScreen

Loads image(s) needed for title screen
=========================================================================
*/
c_boolean TS_LoadTitleScreen( void )
{
	titleScreen = load_bitmap( titleFile , NULL );
	distortBuffer = create_bitmap( titleScreen->w , titleScreen->h );

	return ( titleScreen ) ? ( c_true ) : ( c_false );
}

void TS_CleanupTitleScreen( void ) 
{
	if( titleScreen ) 
	{
		destroy_bitmap( titleScreen );
	}
}

/*
=========================================================================
TS_DrawScreen 

Draws a distort version of title screen complet w/text
=========================================================================
*/
void TS_DrawScreen( void ) 
{
	blit( distortBuffer , pageOne , 0 , 0 , 0 , 0 , distortBuffer->w , distortBuffer->h );
	TS_TitleLogic();
	TS_DrawText();
	blit( pageOne , screen , 0 , 0 , 0, 0 , pageOne->w , pageOne->h );
	clear_to_color( distortBuffer , G_COLOR_BLACK );
}
/*
=========================================================================
TS_DrawText

Draw user choice strings.
=========================================================================
*/
static void TS_DrawText( void ) 
{
	static int xJiggle=1,yJiggle=1;

	GFX_FT_DrawString( pageOne , options[ OPTION_START_GAME ] , OPTION_START_GAME_X + 10*sin(xJiggle*3.14159/180*50) , OPTION_START_GAME_Y , 1 , 1 );
	GFX_FT_DrawString( pageOne , options[ OPTION_EXIT_GAME ] , OPTION_EXIT_GAME_X +10*sin(yJiggle*3.14159/180*50 ), OPTION_EXIT_GAME_Y , 1 , 1 );

	if( titleScreenStatus.newGameActive ) 
	{
		xJiggle++;
	}

	else if( titleScreenStatus.exitActive )
	{
		yJiggle++;
	}
}
/*
=========================================================================
TS_DistortScreen

Uses bitmap distort algo to make title screen look cool. 
=========================================================================
*/
void TS_DistortScreen( void )
{
	static int frame = 0;

	int xMax = titleScreen->w-2 ,
		yMax = titleScreen->h-2,
		xCoeffic=5,
		yCoeffic=3,
		distortX = 1,
		distortY = 1,
		color = 0,r=0,g=0,b=0;

	blit( titleScreen , distortBuffer , 0 , 0 , 0 , 0 , titleScreen->w , titleScreen->h );

	for( int y = 0;y<yMax;y++ )
	{
		for( int x = 0;x<xMax;x++ )
		{
			distortX = xCoeffic*(sin(frame*3.14159/180*9));
			distortY = yCoeffic*(sin(frame*3.14159/180*9));

			if( distortX < 0 )
			{
				distortX = 0;
			}
			if( distortX >= 640 )
			{
				distortX = 639;
			}

			if( distortY < 0 )
			{
				distortY = 0;
			}
			if( distortY >= 480 )
			{
				distortY = 479;
			}

			color = _getpixel16( titleScreen , x+distortX , y+distortY );

			r = GET_R(color);
			g = GET_G(color);
			b = GET_B(color);

			//color = r | g | b;

			_putpixel16( distortBuffer , x , y , color ) ;
			
		}

		frame++;
	}
}
/*
==========================================================================
TS_DistortScreen2

Sucks all pixels on screen toward a specified point
==========================================================================
*/
void TS_DistortScreen2( void )
{
	int xMax = 635,yMax = 470,xCenter=320,yCenter=238,color=0,x=5,y=5,r=0,g=0,b=0;
	static int xOffset=0,yOffset=0,frame=0;

	for( y=5;y<yMax;y++ )
	{
		for( x=5;x<xMax;x++ )
		{
			xOffset = x;
			yOffset = y;
			
			if( xOffset < 0 )
			{
				xOffset = 0;
			}
			else if( xOffset > xMax )
			{
				xOffset = xMax;
			}
			if( yOffset < 0 )
			{
				yOffset = 0;
			}
			if( yOffset > yMax )
			{
				yOffset = yMax;
			}

			color = _getpixel16( titleScreen , x ,y )<<(frame/10);
			
			_putpixel16( distortBuffer , x , y , color );
		}
	}

	frame++;
}

/*
==========================================================================
TS_TitleLogic

Preforms logic required to monitor user input, and enter/exit game as required.
==========================================================================
*/
int TS_TitleLogic( void )
{
	c_boolean newGameActive = titleScreenStatus.newGameActive,
		         exitActive = titleScreenStatus.exitActive;

	// check to see if a valid key has been hit
	if( key[ KEY_UP ] )
	{
		if( newGameActive )
		{
			titleScreenStatus.newGameActive  = c_false;
			titleScreenStatus.exitActive = c_true;
		}

		else if( exitActive ) 
		{
			titleScreenStatus.exitActive = c_false;
			titleScreenStatus.newGameActive = c_true;
		}	
	}

	else if( key[ KEY_DOWN ] )
	// using duplicate code, so that it will be easier to expand upon when more options are added.
	{
		if( newGameActive )
		{
			titleScreenStatus.newGameActive  = c_false;
			titleScreenStatus.exitActive = c_true;
		}

		else if( exitActive ) 
		{
			titleScreenStatus.exitActive = c_false;
			titleScreenStatus.newGameActive = c_true;
		}	

	}

	else if( key[ KEY_ENTER ] || key[ KEY_SPACE ] )
	{
		if( titleScreenStatus.exitActive )
		{
			// exit game
			titleScreenStatus.exitGameSelected = c_true;
			return -1;
		}

		else if( titleScreenStatus.newGameActive )
		{
			// start a new game
			key[ KEY_ESC ] = 1;
			titleScreenStatus.newGameSelected = c_true;
			return 1;
		}
	}
	
	return (int)titleScreenStatus.exitActive;
}