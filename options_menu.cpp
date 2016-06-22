
#include <allegro.h>
#include <memory.h>

#include "options_menu.h"
#include "audio.h"

extern BITMAP* doubleBuffer;

extern volatile int secTicker;

const Vect2_t resolutionElement = { 0 , 0 };
const Vect2_t musicElement = { 0 , 20 };
const Vect2_t sfxElement = { 0 , 40 };
const Vect2_t controlsElement = { 0 , 60 };
const Vect2_t returnToGameElement = { 0 , 80 };

void OPTM_Init( pOptionsMenu_t pOptMenu , int element )
{
	memset( (void*)pOptMenu , 0 , sizeof(OptionsMenu_t) );

	pOptMenu->activeElement = OPTIONS_MENU_ELEMENT_RESOLUTION;
	
	pOptMenu->musicLevel = OPTIONS_MENU_SELECTION_MED;
	pOptMenu->sfxLevel = OPTIONS_MENU_SELECTION_MED;
}

r_boolean OPTM_Run( pOptionsMenu_t pMenu )
{	
	OPTM_HandleInput( pMenu );
	OPTM_RenderMenu( pMenu );

	if( key[ KEY_ENTER ] )
	{
		return r_false;
	}
	
	return r_true;
}

void OPTM_HandleInput( pOptionsMenu_t pMenu )
{
	if( key[ KEY_UP ] )
	{
		AUD_PlaySample( AUDIO_CHANNEL_PLAYER_WEAPON , AUDIO_SAMPLE_TEST_0000 );
		OPTM_RetreatSelection( pMenu );
	}

	else if( key[ KEY_DOWN ] )
	{
		AUD_PlaySample( AUDIO_CHANNEL_PLAYER_WEAPON , AUDIO_SAMPLE_TEST_0000 );
		OPTM_AdvanceSelection( pMenu );
	}

	else if( key[ KEY_LEFT ] )
	{
		OPTM_RetreatOptionValue( pMenu );
	}

	else if( key[ KEY_RIGHT ] )
	{
		OPTM_AdvanceOptionValue( pMenu );
	}
}

void OPTM_AdvanceSelection( pOptionsMenu_t pMenu )
{
	// roll around
	if( pMenu->activeElement == OPTIONS_MENU_ELEMENT_RETURN_TO_GAME )
	{
		pMenu->activeElement = OPTIONS_MENU_ELEMENT_RESOLUTION;
	}

	else if( pMenu->activeElement < OPTIONS_MENU_ELEMENTS )
	{
		pMenu->activeElement++;
	}
}

void OPTM_RetreatSelection( pOptionsMenu_t pMenu )
{
	// roll around
	if( pMenu->activeElement == OPTIONS_MENU_ELEMENT_RESOLUTION )
	{
		pMenu->activeElement = OPTIONS_MENU_ELEMENT_RETURN_TO_GAME;
	}

	else if( pMenu->activeElement > OPTIONS_MENU_ELEMENT_RESOLUTION )
	{
		pMenu->activeElement--;
	}
}

r_boolean OPTM_AdvanceOptionValue( pOptionsMenu_t pMenu )
{
	int element = pMenu->activeElement;

	if( element == OPTIONS_MENU_ELEMENT_RESOLUTION )
	{
		

	}

	else if( element == OPTIONS_MENU_ELEMENT_MUSIC_LVL )
	{
		if( pMenu->musicLevel < OPTIONS_MENU_SELECTION_HIGH )
		{
			pMenu->musicLevel++;
		}

		else
		{
			// play error sound... or something.
		}

	}

	else if( element == OPTIONS_MENU_ELEMENT_SFX_LVL )
	{
		if( pMenu->sfxLevel < OPTIONS_MENU_SELECTION_HIGH )
		{
			pMenu->sfxLevel++;
		}

		else
		{

		}
	}
	
	return r_true;
}

r_boolean OPTM_RetreatOptionValue( pOptionsMenu_t pMenu )
{
	int element = pMenu->activeElement;

	switch( element )
	{
	case OPTIONS_MENU_ELEMENT_RESOLUTION:
		{
			break;
		}

	case OPTIONS_MENU_ELEMENT_MUSIC_LVL:
		{
			if( pMenu->musicLevel > OPTIONS_MENU_SELECTION_LOW )
			{
				pMenu->musicLevel--;
			}

			else
			{

			}

			break;

		}

	case OPTIONS_MENU_ELEMENT_SFX_LVL:
		{
			if( pMenu->sfxLevel > OPTIONS_MENU_SELECTION_LOW )
			{
				pMenu->sfxLevel--;
			}

			else
			{

			}

			break;
		}
	}

		
	return r_true;
}


void OPTM_MakeSelection( pOptionsMenu_t pMenu )
{
	int selection = pMenu->activeElement;

	switch( selection )
	{	
	case OPTIONS_MENU_ELEMENT_RESOLUTION:
		{
			break;
		}

	case OPTIONS_MENU_ELEMENT_MUSIC_LVL:
		{
			break;
		}

	case OPTIONS_MENU_ELEMENT_SFX_LVL:
		{
			break;
		}

	case OPTIONS_MENU_ELEMENT_CONTROLS:
		{
			break;
		}

	case OPTIONS_MENU_ELEMENT_RETURN_TO_GAME:
		{
			break;
		}
	}
}

void OPTM_RenderMenu( pOptionsMenu_t pMenu )
{
	for( int element = 0,color = 0,secColor=0;element < OPTIONS_MENU_ELEMENTS;element++ ) 
	{
		color = ( element == pMenu->activeElement ) ? ( makecol( 255 , 255 , 255 ) ) : ( makecol( 128 , 128 , 128 ) );
		
		switch( element )
		{
		case OPTIONS_MENU_ELEMENT_RESOLUTION:
			{
				textout_ex( screen , font , "Resolution" , resolutionElement.x , resolutionElement.y , color , -1 );
				break;
			}
			
		case OPTIONS_MENU_ELEMENT_MUSIC_LVL:
			{
				textout_ex( screen , font , "Music level" , musicElement.x , musicElement.y , color , -1 );
				
				secColor = ( pMenu->musicLevel == OPTIONS_MENU_SELECTION_LOW  ) ? ( makecol( 255 , 0 , 255 ) ) : ( makecol( 128 , 128 , 128 ) );
				textout_ex( screen , font , "Low" , LOW_COLUMN_OFFSET , musicElement.y , secColor , -1 );
				
				secColor = ( pMenu->musicLevel ==  OPTIONS_MENU_SELECTION_MED  ) ? ( makecol( 255 , 0 , 255 ) ) : ( makecol( 128 , 128 , 128 ) );
				textout_ex( screen , font , "Medium" , MED_COLUMN_OFFSET , musicElement.y , secColor , -1 );
				
				secColor = ( pMenu->musicLevel == OPTIONS_MENU_SELECTION_HIGH ) ? ( makecol( 255 , 0 , 255 ) ) : ( makecol( 128 , 128 , 128 ) );
				textout_ex( screen , font , "High" , HIGH_COLUMN_OFFSET , musicElement.y ,secColor , -1 );
				
				break;
			}

		case OPTIONS_MENU_ELEMENT_SFX_LVL:
			{
				textout_ex( screen , font , "Sound FX level" , sfxElement.x , sfxElement.y , color , -1 );

				// ugly code... 
				secColor = ( pMenu->sfxLevel == OPTIONS_MENU_SELECTION_LOW ) ? ( makecol( 255 , 0 , 255 ) ) : ( makecol( 128 , 128 , 128 ) );
				textout_ex( screen , font , "Low" , LOW_COLUMN_OFFSET , sfxElement.y , secColor , -1 );
				
				secColor = ( pMenu->sfxLevel == OPTIONS_MENU_SELECTION_MED ) ? ( makecol( 255 , 0 , 255 ) ) : ( makecol( 128 , 128 , 128 ) );
				textout_ex( screen , font , "Medium" , MED_COLUMN_OFFSET , sfxElement.y , secColor , -1 );
				
				secColor = ( pMenu->sfxLevel == OPTIONS_MENU_SELECTION_HIGH ) ? ( makecol( 255 , 0 , 255 ) ) : ( makecol( 128 , 128 , 128 ) );
				textout_ex( screen , font , "High" , HIGH_COLUMN_OFFSET , sfxElement.y , secColor , -1 );
				
				break;
			}

		case OPTIONS_MENU_ELEMENT_CONTROLS:
			{
				textout_ex( screen , font , "Configure Controls" , controlsElement.x , controlsElement.y , color , -1 );
				break;
			}

		case OPTIONS_MENU_ELEMENT_RETURN_TO_GAME:
			{
				textout_ex( screen , font , "Return to Game" , returnToGameElement.x , returnToGameElement.y , color , -1 );
				break;
			}
		}
	}
}

static r_boolean OPTM_ChangeSelectionValue( pOptionsMenu_t pMenu )
{
	int element = pMenu->activeElement;

	switch( element )
	{
	}


}

static r_boolean OPTM_ChangeInputValue( pOptionsMenu_t pMenu , int key )
{
	// output a prompt 
	
	pMenu->controls[ key ] = readkey();
}