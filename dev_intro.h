/************************************************************************************
 *  dev_intro.h  
 *  
 *	descp : header for routines data management for developer(s) intro.
 *
 *  auth  : javery
 *  
 *  path  : C:\Program Files\Microsoft Visual Studio\MyProjects\PrimaryCubes3D\dev_intro.h
 *
 * iDate  : 10/06/06 
 * version:
 ************************************************************************************/

#ifndef __DEV_INTRO_H__
#define __DEV_INTRO_H__

#include "common.h"

// animation frames
#define DEV_INTRO_FRAME_01                    0
#define DEV_INTRO_FRAME_02					  1
#define DEV_INTRO_FRAME_03					  2
#define DEV_INTRO_FRAME_04					  3
#define DEV_INTRO_FRAME_05                 	  4
#define DEV_INTRO_FRAME_06                    5
#define DEV_INTRO_FRAME_07                    6
// bump maps
#define DEV_INTRO_BUMP_01                     7
#define DEV_INTRO_BUMP_02                     8
#define DEV_INTRO_BUMP_03                     9
#define DEV_INTRO_BUMP_04                    10
#define DEV_INTRO_BUMP_05                    11
#define DEV_INTRO_BUMP_06                    12
#define DEV_INTRO_BUMP_07                    13
// background
#define DEV_INTRO_BACKGROUND                 14

#define DEV_INTRO_FRAMES                      7
#define DEV_INTRO_LENGHT                      4
#define DEV_INTRO_DELAY                       1

#define DEV_INTRO_Y_OFFSET                  110

c_boolean DIN_LoadFrames( void );
void DIN_CleanUp( void );
static void DIN_DrawBumpedFrame( int , float , float );
void DIN_Animate( float ,float );

#endif 