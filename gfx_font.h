/************************************************************************************
 *  gfx_font.h  
 *  
 *	descp : prototypes for game font rendering routines.
 *
 *  auth  : javery
 *  
 *  path  : C:\Program Files\Microsoft Visual Studio\MyProjects\PrimaryCubes3D\gfx_font.h
 *
 * iDate  : 010/13/06 
 * version:
 ************************************************************************************/

#ifndef __GFX_FONT_H__
#define __GFX_FONT_H__

#include "common.h"
#include "font.h"

#include <allegro.h>

#define GFX_FONT_SQUEEZE 10

c_boolean GFX_FT_DrawChar( BITMAP* ,char , int , int , float , float );
c_boolean GFX_FT_DrawString( BITMAP* , char* , int , int , float , float );

#endif