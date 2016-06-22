
/************************************************************************************
 *  GUI.h
 *  
 *	descp : struts/associated... stuff for in-game GUI  (0_o)
 *
 *  auth  : javery
 *  
 *  path  : C:\Program Files\Microsoft Visual Studio\MyProjects\PrimaryCubes3D\GUI.h
 *
 * iDate  : 10/22/06 
 * version:
 * TO DO : [ 10/22/06 ] Make x/yJiggle factors increment on an event basis ( ie:big link )
 ************************************************************************************/

#include "gui.h"
#include "game.h"
#include "session.h"
#include "gfx_font.h"

#include <allegro.h>
#include <math.h>

extern BITMAP* pageOne,* pageTwo,*pageThree;

BITMAP *gameGUI[ 3 ];

char* gameGUIFiles[] = { "content/gfx/left_panel.bmp" , 
						 "content/gfx/left_panel.bmp" , 
						 "content/gfx/right_panel.bmp"};
extern Game_t currentGame;

static int xJiggle=0,yJiggle=0; // used to "jiggle" characters around

GUI_Element_t gameGUI_Elements[] = 
{
	{ "SCORE"    ,   { 70 , 26   , 0 } , 1 , .45 } ,  
	{ "SCORE_N"    , { 70 , 46 ,   0 } , 1 , .5 } , 
	{ "HISCORE" ,    { 95 , 3 ,    0 } , .5 , .145 } , 
	{ "HI SCORE_N" , { 50 , 20 ,   0 } , 1 , 1 } , 
	{ "STAGE"    ,   { 70 , 420 ,     0 } , .5 , .35} , 
	{ "TIME"     ,   { 0 , 0 ,     0 } , 1 , 1 } ,  
	{ "C_FRONT"  ,   { 99 , 410 ,  0 } , 2 , 2 } ,
	{ "C_BACK"   ,   { 177 , 410 , 0 } , 2 , 2 }
};

static baseTen[] = { 10000 , 19000 , 9000 , 15000 , 18000 , 100000 , 1000000 , 10000000 }; 

/*
==================================================================
GUI_ELE_LoadGameGUI

Loads in game GUIs
==================================================================
*/
c_boolean GUI_ELE_LoadGameGUI( void )
{
  	  gameGUI[ GUI_EASY_CENTER ] = load_bitmap( gameGUIFiles[ GUI_NORMAL_LEFT ] , NULL );
	  gameGUI[ GUI_NORMAL_LEFT ] = load_bitmap( gameGUIFiles[ GUI_NORMAL_LEFT ] , NULL );
     gameGUI[ GUI_NORMAL_RIGHT ] = load_bitmap( gameGUIFiles[ GUI_NORMAL_RIGHT] , NULL );	

	return c_true;
}
/*
===================================================================
GUI_ELE_DrawGUI

draws the specified GUI using info inside of current game
====================================================================
*/
void GUI_ELE_DrawGameGUI( GameLevel_t level )
{
	switch( level ) 
	{
	case LVL_EASY:
		{	
			masked_stretch_blit( gameGUI[ GUI_EASY_CENTER ] , pageOne ,  0 , 0 , gameGUI[ GUI_EASY_CENTER ]->w , gameGUI[ GUI_EASY_CENTER ]->h , 0 , 0 , pageOne->w , pageOne->h );
			break;
		}
	case LVL_NORM:
		{
			masked_stretch_blit( gameGUI[ GUI_NORMAL_LEFT ] , pageTwo   ,  0 , 0 , gameGUI[ GUI_NORMAL_LEFT ]->w , gameGUI[ GUI_NORMAL_LEFT ]->h , 0 , 0 , pageTwo->w , pageTwo->h );
			masked_stretch_blit( gameGUI[ GUI_NORMAL_RIGHT ] , pageThree , 0 , 0 , gameGUI[ GUI_NORMAL_RIGHT ]->w , gameGUI[ GUI_NORMAL_LEFT ]->h , 0 , 0 , pageThree->w , pageThree->h );
			GUI_ELE_DrawScore( currentGame.currentSession.currentScore );
			GUI_ELE_DrawHiScore( currentGame.currentSession.highScore );
			GUI_ELE_DrawNextCube();
			GUI_ELE_DrawGameStage( currentGame.currentSession.currentStage );
			break;
		}
	case LVL_HARD:
		{
			GUI_ELE_DrawScore( currentGame.currentSession.currentScore );
			GUI_ELE_DrawNextCube();
			masked_stretch_blit( gameGUI[ GUI_NORMAL_LEFT ] , pageTwo  ,  0 , 0 , gameGUI[ GUI_NORMAL_LEFT ]->w , gameGUI[ GUI_NORMAL_LEFT ]->h , 0 , 0 , pageTwo->w , pageTwo->h );
			masked_stretch_blit( gameGUI[ GUI_NORMAL_LEFT ] , pageThree , 0 , 0 , gameGUI[ GUI_NORMAL_LEFT ]->w , gameGUI[ GUI_NORMAL_LEFT ]->h , 0 , 0 , pageThree->w , pageThree->h );
			break;
		}
	}

	// make this conditional for some nice polish
	if( xJiggle == 360 )
	{
		xJiggle = yJiggle = 0;
	}
	else
	{
		xJiggle++;
		yJiggle+=2;
	}
	
}

/*
====================================================================
GUI_ELE_DrawScore

Draws bitmapped label and hiscore
====================================================================
*/
void GUI_ELE_DrawScore( int score ) 
{
	GFX_FT_DrawString( pageTwo , gameGUI_Elements[ GUI_ELE_SCORE_S ].label , 
								 gameGUI_Elements[ GUI_ELE_SCORE_S ].position.xPosition + 2*sin(xJiggle*3.14159/180/40), 
								 gameGUI_Elements[ GUI_ELE_SCORE_S ].position.yPosition + 2*cos(yJiggle*3.14159/180/50), 
								 gameGUI_Elements[ GUI_ELE_SCORE_S ].xFactor  , 
								 gameGUI_Elements[ GUI_ELE_SCORE_S ].yFactor );
	
	char* string = UTL_ConvertIntToString( score  );


	GFX_FT_DrawString( pageTwo , string , 
					   gameGUI_Elements[ GUI_ELE_SCORE ].position.xPosition , 
					   gameGUI_Elements[ GUI_ELE_SCORE ].position.yPosition , 
					   gameGUI_Elements[ GUI_ELE_SCORE ].xFactor , 
					   gameGUI_Elements[ GUI_ELE_SCORE ].yFactor );

}
/*
====================================================================
GUI_ELE_DrawHiScore

Draws high score and high score label
====================================================================
*/
void GUI_ELE_DrawHiScore( int hiScore )
{
	int delta = 20,letter = 0,
		      x=gameGUI_Elements[ GUI_ELE_HI_SCORE_S ].position.xPosition,
			  y=gameGUI_Elements[ GUI_ELE_HI_SCORE_S ].position.yPosition;

	float xS = gameGUI_Elements[ GUI_ELE_HI_SCORE_S ].xFactor , 
		  yS = gameGUI_Elements[ GUI_ELE_HI_SCORE_S ].yFactor;

	char* string = gameGUI_Elements[ GUI_ELE_HI_SCORE_S ].label;

	while( *string )
	{
		GFX_FT_DrawChar( pageTwo , *string , x + ( delta * letter ), y + sin(yJiggle*3.14159/180/10), xS   , yS );
		string++;
		letter++;
		yJiggle+=5;
	}      
}
/*
====================================================================
GUI_ELE_DrawNextCube

Draws the next cube to be put into game world.
====================================================================
*/
void GUI_ELE_DrawNextCube( void )
{
	pCube_t pNextCube = &currentGame.nextCube;

	masked_stretch_blit( pNextCube->cubeSprite[ CUBE_FRONT ].data , pageThree , 
						 0 , 0 , 
		                 pNextCube->cubeSprite[ CUBE_FRONT ].data->w ,
						 pNextCube->cubeSprite[ CUBE_FRONT ].data->h , 
						 gameGUI_Elements[ GUI_ELE_CUBE_FRONT ].position.xPosition , 
						 gameGUI_Elements[ GUI_ELE_CUBE_FRONT ].position.yPosition , 
						 pNextCube->cubeSprite[ CUBE_FRONT ].data->w * gameGUI_Elements[ GUI_ELE_CUBE_FRONT ].xFactor , 
						 pNextCube->cubeSprite[ CUBE_FRONT ].data->h * gameGUI_Elements[ GUI_ELE_CUBE_FRONT ].yFactor ); 

	masked_stretch_blit( pNextCube->cubeSprite[ CUBE_BACK ].data , pageThree , 0 , 0 , 
		                 pNextCube->cubeSprite[ CUBE_BACK ].data->w ,
						 pNextCube->cubeSprite[ CUBE_BACK ].data->h , 
						 gameGUI_Elements[ GUI_ELE_CUBE_BACK ].position.xPosition , 
						 gameGUI_Elements[ GUI_ELE_CUBE_BACK ].position.yPosition , 
						 pNextCube->cubeSprite[ CUBE_BACK ].data->w * gameGUI_Elements[ GUI_ELE_CUBE_BACK ].xFactor , 
						 pNextCube->cubeSprite[ CUBE_BACK ].data->h * gameGUI_Elements[ GUI_ELE_CUBE_BACK ].yFactor );
}
/*
======================================================================
UTL_ConvertIntToString

Takes an interger, and converts each didget to a alpha character.
=======================================================================
*/
char* UTL_ConvertIntToString( int number )
{
	char *numberString = currentGame.currentSession.sCurrentScore;	

	int quotient=2,remainder=0,swap=0,power=0,base=0;
	float smallQuotient=2;

	while( smallQuotient > 1 )
	{
		// cool base 10 conversion shortcut.
		base = baseTen[ 1 ];                  // load 10^n from lookup
		smallQuotient = ((float)number/(float)base );
		quotient = (int)( number/base );      // divide by 10^n, and truncate to isolate quotient
		remainder = number % 10;              // isolate remainder
		number /= 10;                         // divide number by 10 to prepare for next iteration

		if( smallQuotient < 1 )
		// this can be changed... 
		{
			swap = remainder;
			remainder = quotient;
			quotient = swap;
			quotient *= 10; // strange algo tweak.
		}

		switch ( remainder )
		{
		case 0:
			{
				numberString[power] = '0';
				break;
			}
		case 1:
			{
				numberString[power] = '1';
				break;
			}
		case 2:
			{
				numberString[power] = '2';
				break;
			}
		case 3:
			{
				numberString[power] = '3';
				break;
			}
		case 4:
			{
				numberString[power] = '4';
				break;
			}
		case 5:
			{
				numberString[power] = '5';
				break;
			}
		case 6:
			{
				numberString[power] = '6';
				break;
			}
		case 7:
			{
				numberString[power] = '7';
				break;
			}
		case 8:
			{
				numberString[power] = '8';
				break;
			}
		case 9:
			{
				numberString[power] = '9';
				break;
			}
		default:
			{
				numberString[power] = '*';
				break;
			}

		}

		power++;
	}

	while( numberString[ power ] )
	{
		numberString[ power++ ] = '0';
	}

	return numberString;
}
/*
===============================================================
G_DrawGameStage

Draws the current stage onto GUI
===============================================================
*/
void GUI_ELE_DrawGameStage( int  stage )
{
	char stageChar='0';

	switch( stage )
	{
	case 0:
		{
			stageChar = '0';
			break;
		}
	case 1:
		{
			stageChar = '1';
			break;
		}
	case 2:
		{
			stageChar = '2';
			break;
		}
	case 3:
		{
			stageChar = '3';
			break;
		}
	case 4:
		{
			stageChar = '4';
			break;
		}
	case 5:
		{
			stageChar = '5';
			break;
		}
	case 6:
		{
			stageChar = '6';
			break;
		}
	case 7:
		{
			stageChar = '7';
			break;
		}
	case 8:
		{
			stageChar = '8';
			break;
		}
	case 9:
		{
			stageChar = '9';
			break;
		}
	default:
		{
			stageChar = '0';
			break;
		}
	}
	
	
	GFX_FT_DrawString( pageTwo , gameGUI_Elements[ GUI_ELE_STAGE ].label , 
								 gameGUI_Elements[ GUI_ELE_STAGE ].position.xPosition , 
								 gameGUI_Elements[ GUI_ELE_STAGE ].position.yPosition , 
								 gameGUI_Elements[ GUI_ELE_STAGE ].xFactor , 
								 gameGUI_Elements[ GUI_ELE_STAGE ].yFactor );

	GFX_FT_DrawChar( pageTwo , stageChar , gameGUI_Elements[ GUI_ELE_STAGE ].position.xPosition + 70, 
										     gameGUI_Elements[ GUI_ELE_STAGE ].position.yPosition +20, 
											 gameGUI_Elements[ GUI_ELE_STAGE ].xFactor+1 , 
											 gameGUI_Elements[ GUI_ELE_STAGE ].yFactor );						    
}
