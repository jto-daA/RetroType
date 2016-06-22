/************************************************************************************
 *  font.h  
 *  
 *	descp :Font handling routines.
 *
 *  auth  : javery
 *  
 *  path  : C:\Program Files\Microsoft Visual Studio\MyProjects\PrimaryCubes3D\font.cpp
 *
 * iDate  : 10/13/06 
 * version:
 ************************************************************************************/

#include <allegro.h>

#include "font.h"

BITMAP* gameFont[ FONT_TABLE_SIZE ];
static char* fontFileName[ FONT_TABLE_SIZE ] = 
{ 
	"content/gfx/fonters0000.bmp" , 
	"content/gfx/fonters0001.bmp" ,
	"content/gfx/fonters0002.bmp" , 
	"content/gfx/fonters0003.bmp" , 
	"content/gfx/fonters0004.bmp" , 
	"content/gfx/fonters0005.bmp" , 
	"content/gfx/fonters0006.bmp" , 
	"content/gfx/fonters0007.bmp" , 
	"content/gfx/fonters0008.bmp" , 
	"content/gfx/fonters0009.bmp" , 
	"content/gfx/fonters0010.bmp" , 
	"content/gfx/fonters0011.bmp" , 
	"content/gfx/fonters0012.bmp" , 
	"content/gfx/fonters0013.bmp" , 
	"content/gfx/fonters0014.bmp" , 
	"content/gfx/fonters0015.bmp" , 
	"content/gfx/fonters0016.bmp" , 
	"content/gfx/fonters0017.bmp" , 
	"content/gfx/fonters0018.bmp" , 
	"content/gfx/fonters0019.bmp" , 
	"content/gfx/fonters0020.bmp" , 
	"content/gfx/fonters0021.bmp" , 
	"content/gfx/fonters0022.bmp" , 
	"content/gfx/fonters0023.bmp" , 
	"content/gfx/fonters0024.bmp" , 
	"content/gfx/fonters0025.bmp" , 
	"content/gfx/fonters0026.bmp" , 
	"content/gfx/fonters0027.bmp" , 
	"content/gfx/fonters0028.bmp" , 
	"content/gfx/fonters0029.bmp" , 
	"content/gfx/fonters0030.bmp" , 
	"content/gfx/fonters0031.bmp" , 
	"content/gfx/fonters0032.bmp" , 
	"content/gfx/fonters0033.bmp" , 
	"content/gfx/fonters0034.bmp" , 
	"content/gfx/fonters0035.bmp" , 
	"content/gfx/fonters0036.bmp" ,
	"content/gfx/fonters0037.bmp" 
	
};
/*
====================================================================
FT_LoadGameFont

Load the bitmaps used for game font.
====================================================================
*/
c_boolean FT_LoadGameFont( void )
{
	for( int index=0; index < FONT_TABLE_SIZE;index++ )
	{
		gameFont[ index ] = load_bitmap( fontFileName[ index ] , NULL );
		
		if( !gameFont[ index ] )
		{
			return c_false;
		}
	}
		return c_true;
}
/*
===================================================================
FT_RemoveGameFont

Frees game font.
===================================================================
*/
void FT_DeleteGameFont( void )
{
	for( int index=0; index < FONT_TABLE_SIZE;index++ )
	{
		//destroy_bitmap( gameFont[ index ] );
	}
}