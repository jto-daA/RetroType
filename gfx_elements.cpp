
/************************************************************************************
 *  gfx_elements.cpp
 *  
 *	descp : rendering routines for in-game objects.
 *
 *  auth  : javery
 *  
 *  path  : C:\Program Files\Microsoft Visual Studio\MyProjects\PrimaryCubes3D\gfx_elements.cpp
 *
 * iDate  : 09/21/06 
 * version:
 * TO DO : [ 10/15/06 ] Take code from GFX_ELE_DrawFullCube and transfer to G_SyncSprite
 ************************************************************************************/

#include "gfx_elements.h"
#include "game.h"

#include <allegro.h>

extern BITMAP* pageOne,*pageTwo,*pageThree;
BITMAP* cubes[ CUBE_COUNT ];

/*
========================================================================
GFX_ELE_DrawGameGrid ( debug )

Debugging version of game grid routine.
========================================================================
*/
void GFX_ELE_DrawGameGrid( pGame_t pGame )
{
	/*static*/ int rows = pGame->currentSession.yDimen,
		       cols = pGame->currentSession.xDimen,
	       xPadding = pGame->currentSession.xPadding,
		   yPadding = pGame->currentSession.yPadding,
		       xMax = pGame->gfxConfig.xDimen, // change for more flexablity.
			   yMax = pGame->gfxConfig.yDimen;
	         

	/*static*/ float deltaX = pGame->currentSession.deltaX,
		         deltaY = pGame->currentSession.deltaY;

	for( int y=0; y<=rows; y++ )
	{
		// horizontal lines
		line( pageOne , xPadding , 
			            ( deltaY * y ) + yPadding , 
					    xMax - xPadding , 
						( deltaY * y ) + yPadding , 
						G_COLOR_WHITE );		

				line( pageTwo , xPadding , 
			            ( deltaY * y ) + yPadding , 
					    xMax - xPadding , 
						( deltaY * y ) + yPadding , 
						G_COLOR_BLACK );		

				line( pageThree , xPadding , 
			            ( deltaY * y ) + yPadding , 
					    xMax - xPadding , 
						( deltaY * y ) + yPadding , 
						G_COLOR_WHITE );		
	}

	for( int x=0; x<=cols; x++ )
	{
		// vertical lines.
		line( pageOne , ( deltaX * x ) + xPadding , 
			            yPadding , 
						( deltaX * x ) + xPadding , 
						yMax - yPadding , 
						G_COLOR_WHITE );

		line( pageTwo , ( deltaX * x ) + xPadding , 
			            yPadding , 
						( deltaX * x ) + xPadding , 
						yMax - yPadding , 
						G_COLOR_BLACK );

		line( pageThree , ( deltaX * x ) + xPadding , 
			            yPadding , 
						( deltaX * x ) + xPadding , 
						yMax - yPadding , 
						G_COLOR_WHITE );

		
	}
}
/*
========================================================================
GFX_ELE_DrawCube

Draws game cube, position regulated by grid-space coordinates.
========================================================================
*/
void GFX_ELE_DrawCube( pGame_t pGame , pCube_t pCube )
{
	float   deltaX = pGame->currentSession.deltaX,
		    deltaY = pGame->currentSession.deltaY,
		  xPadding = pGame->currentSession.xPadding,
		  yPadding = pGame->currentSession.yPadding;

	float xInit = ( pCube->gridPosition.xCol * deltaX + xPadding ) ,
		  yInit = ( pCube->gridPosition.yRow * deltaY + yPadding ) ,
		  xTerm = ( pCube->gridPosition.xCol * deltaX + xPadding ) + deltaX, 
		  yTerm = ( pCube->gridPosition.yRow * deltaY + yPadding ) + deltaY;
	
	switch( pCube->frontFace )
	{
	case PRI_RED:
		{
			rectfill( pageOne , xInit , yInit , xTerm , yTerm , G_COLOR_RED );
			rect( pageOne , xInit , yInit , xTerm , yTerm , G_COLOR_BLACK );
			break;
		}
	case PRI_YELLOW:
		{
			rectfill( pageOne , xInit , yInit , xTerm , yTerm , G_COLOR_YELLOW );
			rect( pageOne , xInit , yInit , xTerm , yTerm , G_COLOR_BLACK );
			break;
		}
	case PRI_BLUE:
		{
			rectfill( pageOne , xInit , yInit , xTerm , yTerm , G_COLOR_BLUE );
			rect( pageOne , xInit , yInit , xTerm , yTerm , G_COLOR_BLACK );
			break;
		}
	case SEC_GREEN:
		{
			rectfill( pageOne , xInit , yInit , xTerm , yTerm , G_COLOR_GREEN );
			break;
		}
	case SEC_PURPLE:
		{
			rectfill( pageOne , xInit , yInit , xTerm , yTerm , G_COLOR_PURPLE );
			break;
		}
	case SEC_ORANGE:
		{
			rectfill( pageOne , xInit , yInit , xTerm , yTerm , G_COLOR_ORANGE );
			break;
		}
	case ALT_STONE:
		{
			rectfill( pageOne , xInit , yInit , xTerm , yTerm , G_COLOR_STONE );
			break;
		}
	}

	if ( pCube->frontFlagged )
	{
		rectfill( pageOne , 
				  xInit + ( deltaX/1.5 ) , yInit + ( deltaY/1.5 ) , 
			      xTerm - ( deltaX/1.5 ) , yTerm - ( deltaY/1.5 ) ,
				  G_COLOR_BLACK );
		
		rect(     pageOne , 
				  xInit + ( deltaX/1.5 ) , yInit + ( deltaY/1.5 ) , 
			      xTerm - ( deltaX/1.5 ) , yTerm - ( deltaY/1.5 ) ,
				  G_COLOR_WHITE );

	}
}

/*
=====================================================================================
GFX_ELE_DrawFullCube

Draws both sides of the cube.
=====================================================================================
*/
void GFX_ELE_DrawFullCube( pGame_t pGame ,pCube_t pCube )
{
	float   deltaX = pGame->currentSession.deltaX,
		    deltaY = pGame->currentSession.deltaY,
		  xPadding = pGame->currentSession.xPadding,
		  yPadding = pGame->currentSession.yPadding;

	float xInit = ( pCube->gridPosition.xCol * deltaX + xPadding ) ,
		  yInit = ( pCube->gridPosition.yRow * deltaY + yPadding ) ,
		  xTerm = ( pCube->gridPosition.xCol * deltaX + xPadding ) + deltaX, 
		  yTerm = ( pCube->gridPosition.yRow * deltaY + yPadding ) + deltaY;

	// take this code, and transfer it to G_SyncSprite
	pCube->cubeSprite[ CUBE_FRONT ].currentPosition.xPosition = xInit;
	pCube->cubeSprite[ CUBE_FRONT ].currentPosition.yPosition = yInit;
	pCube->cubeSprite[ CUBE_BACK ].currentPosition.xPosition = xInit;
	pCube->cubeSprite[ CUBE_BACK ].currentPosition.yPosition = yInit;
	switch( pCube->frontFace )
	{
	case PRI_RED:
		{
			rectfill( pageTwo , xInit , yInit , xTerm , yTerm , G_COLOR_RED );
			rect( pageTwo , xInit , yInit , xTerm , yTerm , G_COLOR_BLACK );
			break;
		}
	case PRI_YELLOW:
		{
			rectfill( pageTwo , xInit , yInit , xTerm , yTerm , G_COLOR_YELLOW );
			rect( pageTwo , xInit , yInit , xTerm , yTerm , G_COLOR_BLACK );
			break;

		}
	case PRI_BLUE:
		{
			rectfill( pageTwo , xInit , yInit , xTerm , yTerm , G_COLOR_BLUE );
			rect( pageTwo , xInit , yInit , xTerm , yTerm , G_COLOR_BLACK );
			break;
		}
	case SEC_GREEN:
		{
			rectfill( pageTwo , xInit , yInit , xTerm , yTerm , G_COLOR_GREEN );
			rect( pageTwo , xInit , yInit , xTerm , yTerm , G_COLOR_BLACK );
			break;
		}
	case SEC_PURPLE:
		{
			rectfill( pageTwo , xInit , yInit , xTerm , yTerm , G_COLOR_PURPLE );
			rect( pageTwo , xInit , yInit , xTerm , yTerm , G_COLOR_BLACK );
			break;
		}
	case SEC_ORANGE:
		{
			rectfill( pageTwo , xInit , yInit , xTerm , yTerm , G_COLOR_ORANGE );
			rect( pageTwo , xInit , yInit , xTerm , yTerm , G_COLOR_BLACK );
			break;
		}
	case ALT_STONE:
		{
			rectfill( pageTwo , xInit , yInit , xTerm , yTerm , G_COLOR_STONE );
			rect( pageTwo , xInit , yInit , xTerm , yTerm , G_COLOR_BLACK );
			break;
		}

	default:
		{
			break;
		}
	}

		// debug stuff... 
	if ( pCube->frontFlagged )
	{
		rectfill( pageTwo , 
				  xInit + ( deltaX/1.5 ) , yInit + ( deltaY/1.5 ) , 
			      xTerm - ( deltaX/1.5 ) , yTerm - ( deltaY/1.5 ) ,
				  G_COLOR_BLACK );
		
		rect(     pageTwo , 
				  xInit + ( deltaX/1.5 ) , yInit + ( deltaY/1.5 ) , 
			      xTerm - ( deltaX/1.5 ) , yTerm - ( deltaY/1.5 ) ,
				  G_COLOR_WHITE );

	}

	switch( pCube->rearFace )
	{
	case PRI_RED:
		{
			rectfill( pageThree , xInit , yInit , xTerm , yTerm , G_COLOR_RED );
			rect( pageThree , xInit , yInit , xTerm , yTerm , G_COLOR_BLACK );
			break;
		}
	case PRI_YELLOW:
		{
			rectfill( pageThree , xInit , yInit , xTerm , yTerm , G_COLOR_YELLOW );
			rect( pageThree , xInit , yInit , xTerm , yTerm , G_COLOR_BLACK );
			break;
		}
	case PRI_BLUE:
		{
			rectfill( pageThree , xInit , yInit , xTerm , yTerm , G_COLOR_BLUE );
			rect( pageThree , xInit , yInit , xTerm , yTerm , G_COLOR_BLACK );
			break;
		}
	case SEC_GREEN:
		{
			rectfill( pageThree , xInit , yInit , xTerm , yTerm , G_COLOR_GREEN );
			rect( pageThree , xInit , yInit , xTerm , yTerm , G_COLOR_BLACK );
			break;
		}
	case SEC_PURPLE:
		{
			rectfill( pageThree , xInit , yInit , xTerm , yTerm , G_COLOR_PURPLE );
			rect( pageThree , xInit , yInit , xTerm , yTerm , G_COLOR_BLACK );
			break;
		}
	case SEC_ORANGE:
		{
			rectfill( pageThree , xInit , yInit , xTerm , yTerm , G_COLOR_ORANGE );
			rect( pageThree , xInit , yInit , xTerm , yTerm , G_COLOR_BLACK );
			break;
		}
	case ALT_STONE:
		{
			rectfill( pageThree , xInit , yInit , xTerm , yTerm , G_COLOR_STONE );
			rect( pageThree , xInit , yInit , xTerm , yTerm , G_COLOR_BLACK );
			break;
		}
	}

	// debug stuff... 
	if ( pCube->rearFlagged )
	{
		rectfill( pageThree , 
				  xInit + ( deltaX/1.5 ) , yInit + ( deltaY/1.5 ) , 
			      xTerm - ( deltaX/1.5 ) , yTerm - ( deltaY/1.5 ) ,
				  G_COLOR_BLACK );
		
		rect(     pageThree , 
				  xInit + ( deltaX/1.5 ) , yInit + ( deltaY/1.5 ) , 
			      xTerm - ( deltaX/1.5 ) , yTerm - ( deltaY/1.5 ) ,
				  G_COLOR_WHITE );

	}
}
/*
=======================================================================================
GFX_ELE_DrawCubeSprite

Draws the sprite version of cube. Relys on cube sprites having first been syncronized.
=======================================================================================
*/
void GFX_ELE_DrawCubeSprite( pGame_t pGame , pCube_t pCube )
{
	GFX_SPR_DrawSprite( &pCube->cubeSprite[ CUBE_FRONT ] , pageTwo ,  1 , 1 );
	GFX_SPR_DrawSprite( &pCube->cubeSprite[ CUBE_BACK ] , pageThree , 1 , 1 );
}
