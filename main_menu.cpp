
#include <allegro.h>

#include "common.h"
#include "game.h"
#include "main_menu.h"

extern BITMAP* doubleBuffer;
extern volatile int secTicker;

const Vect2_t startElement = { 0 , 200 };
const Vect2_t optionsElement = { 0 , 250 };
const Vect2_t creditsElement = { 0 , 300 };
const Vect2_t exitElement = { 0 , 350 };

void MM_Init( pMainMenu_t pMenu , int timeToShow )
{
	pMenu->timeToDemo = timeToShow;
	pMenu->currentIndex = MAIN_MENU_ELEMENT_START;
}

void MM_HandleInput( pMainMenu_t pMenu )
{
	

}

void MM_AdvanceSelection( pMainMenu_t pMenu )
{
	// handle wrap around case
	if( pMenu->currentIndex == MAIN_MENU_ELEMENT_EXIT )
	{
		pMenu->currentIndex = MAIN_MENU_ELEMENT_START;
	}

	else if( pMenu->currentIndex <= MAIN_MENU_ELEMENT_EXIT && pMenu->currentIndex >= MAIN_MENU_ELEMENT_START )
	{
		pMenu->currentIndex++;
	}

	else
	{

	}
}

void MM_RetreatSelection( pMainMenu_t pMenu )
{
	if( pMenu->currentIndex == MAIN_MENU_ELEMENT_START )
	{
		pMenu->currentIndex = MAIN_MENU_ELEMENT_EXIT;
	}

	else if( pMenu->currentIndex <= MAIN_MENU_ELEMENT_EXIT && pMenu->currentIndex >= MAIN_MENU_ELEMENT_START )
	{
		pMenu->currentIndex--;
	}

	else
	{

	}
}

void MM_MakeSelection( pMainMenu_t pMenu )
{
	int selection = pMenu->currentIndex;

	switch( selection )
	{
	case MAIN_MENU_ELEMENT_START:
		{
			break;
		}

	case MAIN_MENU_ELEMENT_OPTIONS:
		{
			break;
		}

	case MAIN_MENU_ELEMENT_CREDITS:
		{
			break;
		}

	case MAIN_MENU_ELEMENT_EXIT:
		{
			break;
		}
	}
}

r_boolean MM_Run( pMainMenu_t pMenu )
{
	static int runningTime = secTicker;

	runningTime -= secTicker;

	// exit to demo if time
	if( runningTime >= MAIN_MENU_TIME_TO_DEMO )
	{
		return r_false;
	}

	// handle key presses
	if( key[ KEY_UP ] )
	{
		MM_RetreatSelection( pMenu );
	}

	else if( key[ KEY_DOWN ] ) 
	{
		MM_AdvanceSelection( pMenu );
	}

	else if( key[ KEY_ENTER ]  )
	{
		return r_false;
	}

	MM_RenderMenu( pMenu );
	
	return r_true;
}

void MM_RenderMenu( pMainMenu_t pMenu )
{
	for( int element = 0, color = 0;element < MAIN_MENU_ELEMENTS;element++ )
	{
		// render the selected element differently than the others.
		color = ( element == pMenu->currentIndex ) ? ( makecol( 255 , 255 , 255 ) ) : ( makecol( 128 , 128 , 128 ) );
		
		switch( element )
		{
		case MAIN_MENU_ELEMENT_START:
			{
				textout_ex( screen , font , "Start"	, startElement.x , startElement.y , color , 1 );	
				break;
			}

		case MAIN_MENU_ELEMENT_OPTIONS:
			{
				textout_ex( screen , font , "Options"	, optionsElement.x , optionsElement.y , color , 1 );	
				break;
			}

		case MAIN_MENU_ELEMENT_CREDITS:
			{
				textout_ex( screen , font , "Credits"	, creditsElement.x , creditsElement.y , color , 1 );	
				break;
			}

		case MAIN_MENU_ELEMENT_EXIT:
			{
				textout_ex( screen , font , "Exit"	, exitElement.x , exitElement.y , color , 1 );	
				break;
			}
		}
	}
}
