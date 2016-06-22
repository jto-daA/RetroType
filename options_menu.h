
#ifndef __OPTIONS_MENU_H__
#define __OPTIONS_MENU_H__

#include "common.h"

#define OPTIONS_MENU_ELEMENT_RESOLUTION              0
#define OPTIONS_MENU_ELEMENT_MUSIC_LVL               1
#define OPTIONS_MENU_ELEMENT_SFX_LVL                 2
#define OPTIONS_MENU_ELEMENT_CONTROLS                3 
#define OPTIONS_MENU_ELEMENT_RETURN_TO_GAME          4
#define OPTIONS_MENU_ELEMENTS                        5

#define INPUT_KEY_UP                                 0 
#define INPUT_KEY_DOWN                               1
#define INPUT_KEY_LEFT                               2
#define INPUT_KEY_RIGHT                              3
#define INPUT_KEY_SPACE                              4
#define INPUT_KEY_1                                  5
#define INPUT_KEY_2                                  6
#define INPUT_KEY_3                                  7
#define INPUT_KEY_4                                  8
#define REQUIRED_KEYS                                9


// not the audio levels are made to jive w/MOD values 
#define AUDIO_LEVEL_LOW                             50
#define AUDIO_LEVEL_MEDIUM                         128	 
#define AUDIO_LEVEL_HIGH                           255

#define LOW_COLUMN_OFFSET                          300
#define MED_COLUMN_OFFSET                          400
#define HIGH_COLUMN_OFFSET                         500

#define OPTIONS_MENU_SELECTION_LOW                   0
#define OPTIONS_MENU_SELECTION_MED                   1
#define OPTIONS_MENU_SELECTION_HIGH                  2


typedef struct OptionsMenu_t
{
	int activeElement;

	int musicLevel;
	int sfxLevel;
	int controls[ REQUIRED_KEYS ];

}*pOptionsMenu_t;


void OPTM_Init( pOptionsMenu_t , int );

r_boolean OPTM_Run( pOptionsMenu_t );

void OPTM_HandleInput( pOptionsMenu_t );

void OPTM_AdvanceSelection( pOptionsMenu_t );
void OPTM_RetreatSelection( pOptionsMenu_t );

r_boolean OPTM_AdvanceOptionValue( pOptionsMenu_t );
r_boolean OPTM_RetreatOptionValue( pOptionsMenu_t );

void OPTM_MakeSelection( pOptionsMenu_t );

void OPTM_RenderMenu( pOptionsMenu_t );

static r_boolean OPTM_ChangeSelectionValue( pOptionsMenu_t );
static r_boolean OPTM_ChangeInputValue( pOptionsMenu_t , int );

#endif