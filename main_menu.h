
#ifndef __MAIN_MENU_H__
#define __MAIN_MENU_H__

#include "common.h"

#define MAIN_MENU_ELEMENT_START                        0
#define MAIN_MENU_ELEMENT_OPTIONS                      1 
#define MAIN_MENU_ELEMENT_CREDITS                      2 
#define MAIN_MENU_ELEMENT_EXIT                         3
#define MAIN_MENU_ELEMENTS                             4

#define MAIN_MENU_TIME_TO_DEMO                         5

typedef struct MainMenu_t
{
	int currentIndex;
	int timeToDemo;

}*pMainMenu_t;

void MM_Init( pMainMenu_t , int );

void MM_HandleInput( pMainMenu_t );
void MM_AdvanceSelection( pMainMenu_t );
void MM_RetreatSelection( pMainMenu_t );
void MM_MakeSelection( pMainMenu_t );
r_boolean MM_Run( pMainMenu_t );

void MM_RenderMenu( pMainMenu_t );

#endif