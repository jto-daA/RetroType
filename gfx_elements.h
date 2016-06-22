
/************************************************************************************
 *  gfx_elements.h
 *  
 *	descp : prototypes for game element rendering
 *
 *  auth  : javery
 *  
 *  path  : C:\Program Files\Microsoft Visual Studio\MyProjects\PrimaryCubes3D\gfx_elements.h
 *
 * iDate  : 09/18/06 
 * version:
 ************************************************************************************/

#ifndef __GFX_ELEMENTS_H__
#define __GFX_ELEMENTS_H__

#include "game.h"
#include "cube.h"

#define GFX_GRID_PADDING_X  20
#define GFX_GRID_PADDING_Y  20

void GFX_ELE_DrawGameGrid( pGame_t  );
void GFX_ELE_DrawCube( pGame_t , pCube_t );
void GFX_ELE_DrawFullCube( pGame_t , pCube_t );
void GFX_ELE_DrawCubeSprite( pGame_t , pCube_t );
void GFX_ELE_NormalizeCoord( float , float , float , float );


#endif