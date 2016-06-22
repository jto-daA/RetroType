/************************************************************************************
 *  font.h  
 *  
 *	descp : macros for in game character fonts. prototypes for font handler routines.
 *
 *  auth  : javery
 *  
 *  path  : C:\Program Files\Microsoft Visual Studio\MyProjects\PrimaryCubes3D\font.h
 *
 * iDate  : 10/13/06 
 * version:
 ************************************************************************************/

#ifndef __FONT_H__
#define __FONT_H__

#include "common.h"

// 'global' font macros
#define FONT_TABLE_SIZE     38
#define FONT_X_SIZE         50
#define FONT_Y_SIZE         50

// numerical
#define FONT_NUMBER_00		 0     
#define FONT_NUMBER_01		 1
#define FONT_NUMBER_02		 2	    
#define FONT_NUMBER_03		 3
#define FONT_NUMBER_04		 4
#define FONT_NUMBER_05		 5
#define FONT_NUMBER_06		 6
#define FONT_NUMBER_07		 7
#define FONT_NUMBER_08		 8
#define FONT_NUMBER_09		 9

// alpha
#define FONT_LETTER_A		10
#define FONT_LETTER_B		11
#define FONT_LETTER_C		12
#define FONT_LETTER_D		13
#define FONT_LETTER_E		14
#define FONT_LETTER_F		15
#define FONT_LETTER_G		16
#define FONT_LETTER_H		17	
#define FONT_LETTER_I		18	
#define FONT_LETTER_J		19	
#define FONT_LETTER_K		20
#define FONT_LETTER_L		21	
#define FONT_LETTER_M		22
#define FONT_LETTER_N		23
#define FONT_LETTER_O		24	
#define FONT_LETTER_P		25
#define FONT_LETTER_Q		26
#define FONT_LETTER_R		27
#define FONT_LETTER_S		28
#define FONT_LETTER_T		29
#define FONT_LETTER_U		30
#define FONT_LETTER_V		31
#define FONT_LETTER_W		32
#define FONT_LETTER_X		33
#define FONT_LETTER_Y		34
#define FONT_LETTER_Z		35
// etc
#define FONT_CHAR_ASTERIK   36
#define FONT_CHAR_SPACE     37

c_boolean FT_LoadGameFont( void );
void FT_DeleteGameFont( void );

#endif