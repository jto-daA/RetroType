/************************************************************************************
 *  cube.h  
 *  
 *	descp : Defines game cube    
 *
 *  auth  : javery
 *  
 *  path  : C:\Program Files\Microsoft Visual Studio\MyProjects\PrimaryCubes3D\cube.h
 *
 * iDate  : 09/17/06 
 * version:
 ************************************************************************************/

#ifndef __CUBES_H__
#define __CUBES_H__

#include "common.h"

#include "gfx_sprite.h"

#define CUBE_FRONT 0
#define CUBE_BACK  1

typedef enum 
{
	PRI_RED,
	PRI_YELLOW,
	PRI_BLUE,
	SEC_ORANGE,
	SEC_PURPLE,
	SEC_GREEN,
	ALT_STONE
}CubeColor_t;

// cube
#define G_CUBE_GREEN    0
#define G_CUBE_BLUE     1
#define G_CUBE_RED      2
#define G_CUBE_YELLOW   3
#define G_CUBE_ORANGE   4
#define G_CUBE_PURPLE   5
#define G_CUBE_STONE    6
#define CUBE_COUNT 7

typedef struct Cell_t
{
	int yRow,xCol,xMax,yMax;
}pCell_t;

typedef struct Cube_t
{
	Vect3_t position;
	Cell_t gridPosition;
	CubeColor_t frontFace,rearFace;
	c_boolean processed,twoSided,active,alive;
	c_boolean directions[3];
	c_boolean frontFlagged,rearFlagged;

	Sprite_t cubeSprite[2];

}*pCube_t;


#endif 
