
/************************************************************************************
 *  title_screen.h  
 *  
 *	descp : header for routines used for title screen
 *
 *  auth  : javery
 *  
 *  path  : C:\Program Files\Microsoft Visual Studio\MyProjects\PrimaryCubes3D\title_screen.h
 *
 * iDate  : 10/28/06 
 * version:
 ************************************************************************************/

#ifndef __TITLE_SCREEN_H__
#define __TITLE_SCREEN_H__

#include "common.h"

#define OPTION_START_GAME 0
#define OPTION_EXIT_GAME  1

#define OPTION_START_GAME_X 130
#define OPTION_START_GAME_Y 340
#define OPTION_EXIT_GAME_X  220
#define OPTION_EXIT_GAME_Y  400

typedef struct TitleScreenStatus_t
{
	c_boolean newGameActive,
		      newGameSelected,
			  exitActive, 
			  exitGameSelected;
}*pTitleScreenStatus_t;

c_boolean TS_LoadTitleScreen( void );
void TS_CleanupTitleSceen( void );
void TS_DrawScreen( void );
static void TS_DrawText( void );
void TS_DistortScreen( void );  // sinusodial distortion
void TS_DistortScreen2( void ); // vortex
int TS_TitleLogic( void );

#endif