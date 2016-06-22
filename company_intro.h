
/************************************************************************************
 *  company_intro.h  
 *  
 *	descp : header for routines data management for Code Monkey intro screen
 *
 *  auth  : javery
 *  
 *  path  : C:\Program Files\Microsoft Visual Studio\MyProjects\PrimaryCubes3D\company_intro.h
 *
 * iDate  : 10/06/06 
 * version:
 ************************************************************************************/

#ifndef __COMPANY_INTRO_H__
#define __COMPANY_INTRO_H__

#include <allegro.h>

#include "common.h"
#include "gfx_sprite.h"

#define COMPANY_INTRO_FRAMES         11
#define COMPANY_MONKEY_FRAMES        11
#define COMPANY_INTRO_SMASHER_FRAMES 39
#define COMPANY_INTRO_DELAY
#define COMPANY_INTRO_PART1_LENGTH    10 
#define COMPANY_INTRO_PART2_LENGHT    

#define COMPANY_INTRO_MONKEYS         20 

#define MONKEY_SPEED                  8 
#define MAX_MONKEY_SPEED             50
#define MAX_MONKEY_STEPS             10
#define MAX_MONKEY_BREATHS           10

#define MONKEY_Y_OFFSET             200

#define COMPANY_INTRO_FINAL_DELAY     3

// frame macros
// IDLE
#define COMPANY_INTRO_IDLE_01         0
#define COMPANY_INTRO_IDLE_02         1
#define COMPANY_INTRO_IDLE_03         2
#define COMPANY_INTRO_IDLE_04         3
#define COMPANY_INTRO_IDLE_05         4
#define COMPANY_INTRO_IDLE_06         5
#define COMPANY_INTRO_IDLE_07         6

#define COMPANY_INTRO_IDLE_FRAMES     7

// WALKING
#define COMPANY_INTRO_WALK_01         6
#define COMPANY_INTRO_WALK_02         7
#define COMPANY_INTRO_WALK_03         8
#define COMPANY_INTRO_WALK_04         9

#define COMPANY_INTRO_WALK_FRAMES     4

// SMASHER
#define COMPANY_INTRO_SMASH_01        0 
#define COMPANY_INTRO_BG              0
#define COMPANY_INTRO_SMASH_02		  1
#define COMPANY_INTRO_SMASH_03        2
#define COMPANY_INTRO_SMASH_04        3
#define COMPANY_INTRO_SMASH_05        4
#define COMPANY_INTRO_SMASH_06        5 
#define COMPANY_INTRO_SMASH_07        6
#define COMPANY_INTRO_SMASH_08        7
#define COMPANY_INTRO_SMASH_09		  8
#define COMPANY_INTRO_SMASH_10        9
#define COMPANY_INTRO_SMASH_11       10
#define COMPANY_INTRO_SMASH_12       11
#define COMPANY_INTRO_SMASH_13       12
#define COMPANY_INTRO_SMASH_14       13
#define COMPANY_INTRO_SMASH_15       14
#define COMPANY_INTRO_SMASH_16       15
#define COMPANY_INTRO_SMASH_17       16
#define COMPANY_INTRO_SMASH_18       17
#define COMPANY_INTRO_SMASH_19       18
#define COMPANY_INTRO_SMASH_20       19
#define COMPANY_INTRO_SMASH_21       20
#define COMPANY_INTRO_SMASH_22       21
#define COMPANY_INTRO_SMASH_23       22
#define COMPANY_INTRO_SMASH_24       23
#define COMPANY_INTRO_SMASH_25       24
#define COMPANY_INTRO_SMASH_26       25
#define COMPANY_INTRO_SMASH_27       26
#define COMPANY_INTRO_SMASH_28       27

#define COMPANY_SMASHER_FRAMES       29

typedef struct Monkey_t
{
	BITMAP* monkeySprites[ COMPANY_MONKEY_FRAMES ]; 
	c_boolean walking,idle;
	Vect3_t position;      	// speed stored in z-component
	int currentFrame,lastFrame,stepsToRest,breathsToTake;
}*pMonkey_t;

void CIN_LoadFrames( void );
void CIN_DrawMonkey( int , int );
void CIN_MonkeyLogic( int );
void CIN_RunIntro( void );
void CIN_CleanUp( void );

#endif
