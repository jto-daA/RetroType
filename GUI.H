
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
 ************************************************************************************/

// argruably, this can be put into gfx_elements... 

#ifndef __GUI_H__
#define __GUI_H__

#include "common.h"
#include "session.h"

// GUI setups
#define GUI_EASY_CENTER       0
#define GUI_NORMAL_LEFT       1
#define GUI_NORMAL_RIGHT      2
// GUI element
#define GUI_ELE_SCORE_S       0
#define GUI_ELE_SCORE         1
#define GUI_ELE_HI_SCORE_S    2
#define GUI_ELE_HI_SCORE      3
#define GUI_ELE_STAGE         4
#define GUI_ELE_TIME          5
#define GUI_ELE_CUBE_FRONT    6
#define GUI_ELE_CUBE_BACK     7

typedef struct GUI_Element_t
{
	char* label;
	Vect3_t position;       // { x-upper left, y-upper left , length }
	float xFactor,yFactor;  // scaling factors
}*pGUI_Element_t;

c_boolean GUI_ELE_LoadGameGUI( void );
void GUI_ELE_DrawGameGUI( GameLevel_t );

void GUI_ELE_DrawScore( int );
void GUI_ELE_DrawHiScore( int );
void GUI_ELE_DrawNextCube( void );
void GUI_ELE_DrawGameStage( int );

char* UTL_ConvertIntToString( int );

#define NYBBLE_01( number ) ((number) & (0x0000000F))
#define NYBBLE_02( number ) ((number) & (0x000000F0))
#define NYBBLE_03( number ) ((number) & (0x00000F00))
#define NYBBLE_04( number ) ((number) & (0x0000F000))
#define NYBBLE_05( number ) ((number) & (0x000F0000))
#define NYBBLE_06( number ) ((number) & (0x00F00000))
#define NYBBLE_07( number ) ((number) & (0x0F000000))
#define NYBBLE_08( number ) ((number) & (0xF0000000))

#endif 