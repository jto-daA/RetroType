/************************************************************************************
 *  gfx_sprite.h  
 *  
 *	descp : structs/prototypes for in-game sprites.
 *
 *  auth  : javery
 *  
 *  path  : C:\Program Files\Microsoft Visual Studio\MyProjects\PrimaryCubes3D\gfx_sprite.h
 *
 * iDate  : 09/03/06 
 * version:
 ************************************************************************************/

#ifndef __GFX_SPRITE_H__
#define __GFX_SPRITE_H__

#include <allegro.h>

#include "common.h"

typedef struct Sprite_t
{
	int xDimen,yDimen,bitDepth;
	BITMAP*data;
	Vect3_t lastPosition,currentPosition;
	float xFactor,yFactor;                 // scaling factors
	char* spriteID;
}*pSprite_t;

c_boolean GFX_SPR_LoadSprite( pSprite_t , char* );
void GFX_SPR_DeleteSprite( pSprite_t );
void GFX_SPR_DrawSprite( pSprite_t , BITMAP* , float , float  );

#endif 
