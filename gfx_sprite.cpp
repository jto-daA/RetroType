/************************************************************************************
 *  gfx_sprite.h  
 *  
 *	descp : routines to load/display/manipulate game sprites.
 *
 *  auth  : javery
 *  
 *  path  : C:\Program Files\Microsoft Visual Studio\MyProjects\PrimaryCubes3D\gfx_sprite.c
 *
 * iDate  : 09/03/06 
 * version:
 *
 *  TO DO : [ 09/03/06 ] Add logging and or screen prompt upon failed asset load.
 ************************************************************************************/

#include <allegro.h>
#include <memory.h>

#include "gfx_sprite.h"

/*
==================================================================================
GFX_SPR_LoadSprite

Loads the specified sprite. Initalized sprite sturct
==================================================================================
*/
c_boolean GFX_SPR_LoadSprite( pSprite_t pSprite , char* fileName )
{	
	memset( (void*)pSprite , 0 , sizeof(Sprite_t ) );
	
	pSprite->data = load_bitmap( fileName , NULL );
	pSprite->xDimen = pSprite->data->w;
	pSprite->yDimen = pSprite->data->h;
	pSprite->bitDepth = pSprite->data->vtable->color_depth;
	pSprite->spriteID = fileName;

	return ( pSprite->data ) ? c_true : c_false;
}
/*
===================================================================================
GFX_SPR_DeleteSprite

Frees resources allocated for sprite.
===================================================================================
*/
void GFX_SPR_DeleteSprite( pSprite_t pSprite )
{
	destroy_bitmap( pSprite->data );
}
/*
===================================================================================
GFX_SPR_DrawSprite
	
Draws the passed sprite onto the passed surface. Considers x/y scaling factors.
===================================================================================
*/
void GFX_SPR_DrawSprite( pSprite_t pSprite , BITMAP* surface ,float xFactor , float yFactor )
// hehehh... I named a variable 'x-factor'
{
	masked_stretch_blit( pSprite->data , surface ,
						 0 , 0 , 
						 pSprite->xDimen , pSprite->yDimen , 
						 pSprite->currentPosition.xPosition , pSprite->currentPosition.yPosition , 
						 pSprite->xDimen * xFactor, pSprite->yDimen * yFactor );

}

