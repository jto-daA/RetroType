/************************************************************************************
 *  gfx_font.h  
 *  
 *	descp : Game font rendering routines.
 *
 *  auth  : javery
 *  
 *  path  : C:\Program Files\Microsoft Visual Studio\MyProjects\PrimaryCubes3D\gfx_font.cpp
 *
 * iDate  : 010/13/06 
 * version:
 * TO DO : [10/13/06 ] Fix inconsistencies with numerical characters ( different size )
 ************************************************************************************/

#include <allegro.h>

#include "gfx_font.h"
#include "font.h"

extern BITMAP* gameFont[ FONT_TABLE_SIZE ];
extern BITMAP* pageOne , *pageTwo, *pageThree;

/*
============================================================================
GFX_FT_DrawChar

Draws scaled font sprite at specified location.
===========================================================================
*/
c_boolean GFX_FT_DrawChar( BITMAP* surface , char letter , int xPosition , int yPosition , float xFactor , float yFactor )
{
	int character=0,w=gameFont[8]->w,h=gameFont[8]->h;
	
	switch( letter )
	{
	case '0':
		{
			character = FONT_NUMBER_00;
			break;
		}
	case '1':
		{
			character = FONT_NUMBER_01;
			break;
		}
	case '2':
		{
			character = FONT_NUMBER_02;
			break;
		}
	case '3':
		{
			character = FONT_NUMBER_03;
			break;
		}
	case '4':
		{
			character = FONT_NUMBER_04;
			break;
		}
	case '5':
		{
			character = FONT_NUMBER_05;
			break;
		}
	case '6':
		{
			character = FONT_NUMBER_06;
			break;
		}
	case '7':
		{
			character = FONT_NUMBER_06;
			break;
		}
	case '8':
		{
			character = FONT_NUMBER_07;
			break;
		}
	case '9':
		{
			character = FONT_NUMBER_08;
			break;
		}
	case 'A':
		{
			character = FONT_LETTER_A;
			break;
		}
	case 'B':
		{
			character = FONT_LETTER_B;
			break;
		}
	case 'C':
		{
			character = FONT_LETTER_C;
			break;
		}
	case 'D':
		{
			character = FONT_LETTER_D;
			break;
		}
	case 'E':
		{
			character = FONT_LETTER_E;
			break;
		}
	case 'F':
		{
			character = FONT_LETTER_F;
			break;
		}
	case 'G':
		{
			character = FONT_LETTER_G;
			break;
		}
	case 'H':
		{
			character = FONT_LETTER_H;
			break;
		}
	case 'I':
		{
			character = FONT_LETTER_I;
			break;
		}
	case 'J':
		{
			character = FONT_LETTER_J;
			break;
		}
	case 'K':
		{
			character = FONT_LETTER_K;
			break;
		}
	case 'L':
		{
			character = FONT_LETTER_L;
			break;
		}
	case 'M':
		{
			character = FONT_LETTER_M;
			break;
		}
	case 'N':
		{
			character = FONT_LETTER_N;
			break;
		}
	case 'O':
		{
			character = FONT_LETTER_O;
			break;
		}
	case 'P':
		{
			character = FONT_LETTER_P;
			break;
		}
	case 'Q':
		{
			character = FONT_LETTER_Q;
			break;
		}
	case 'R':
		{
			character = FONT_LETTER_R;
			break;
		}
	case 'S':
		{
			character = FONT_LETTER_S;
			break;
		}
	case 'T':
		{
			character = FONT_LETTER_T;
			break;
		}
	case 'U':
		{
			character = FONT_LETTER_U;
			break;
		}
	case 'V':
		{
			character = FONT_LETTER_V;
			break;
		}
	case 'W':
		{
			character = FONT_LETTER_W;
			break;
		}
	case 'X':
		{
			character = FONT_LETTER_X;
			break;
		}
	case 'Y':
		{
			character = FONT_LETTER_Y;
			break;
		}
	case 'Z':
		{
			character = FONT_LETTER_Z;
			break;
		}
	default:
		{ 	
			character = FONT_CHAR_SPACE;
			break;
		}
	}

	masked_stretch_blit( gameFont[ character ] , surface , 0 , 0 , w , h , xPosition , yPosition , w * xFactor , h * yFactor ); 
			
	return c_true;
}
/*
============================================================================
GFX_FT_DrawString

Draws the specified string using game font-set.
===========================================================================
*/
c_boolean GFX_FT_DrawString( BITMAP* surface , char* string , int xPosition , int yPosition , float xFactor , float yFactor )
{
	int offset = gameFont[8]->w-GFX_FONT_SQUEEZE,letter=0;
	char currentLetter = *string;
	
	while( *string++ )
	{
		GFX_FT_DrawChar( surface , currentLetter , xPosition + ( letter * offset )  , yPosition , xFactor , yFactor );
		letter++;
		currentLetter = *string;
	}
	
	return c_true;
}