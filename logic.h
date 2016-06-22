/************************************************************************************
 *  logic.h  
 *  
 *	descp : Game logic structs/prototypes
 *
 *  auth  : javery
 *  
 *  path  : C:\Program Files\Microsoft Visual Studio\MyProjects\PrimaryCubes3D\logic.h
 *
 * iDate  : 09/17/06 
 * version:
 ************************************************************************************/

#ifndef __LOGIC_H__
#define __LOGIC_H__

#include "cube.h"

#define GL_COLOR_TABLE_SIZE 500

#define GL_DIRECTION_LEFT     0
#define GL_DIRECTION_DOWN     1
#define GL_DIRECTION_RIGHT    2
#define GL_DIRECTION_UP       3

#define GL_GAME_SIZE_X          10
#define GL_GAME_SIZE_Y          10
#define GL_EASY_GAME_SIZE_X     10
#define GL_EASY_GAME_SIZE_Y     10
#define GL_NORM_GAME_SIZE_X     20
#define GL_NORM_GAME_SIZE_Y     20
#define GL_HARD_GAME_SIZE_X     40
#define GL_HARD_GAME_SIZE_Y     40

void GL_GenerateColorTable( void );
void GL_GenerateCube( pCube_t );
void GL_AddCube( pCube_t );
void GL_DeleteCube( pCube_t );
c_boolean GL_CheckDirections( pCube_t ,Cube_t[ GL_EASY_GAME_SIZE_X ][ GL_EASY_GAME_SIZE_Y ] );
//c_boolean GL_CheckDirections( pCube , Cube_t** );
void GL_ProcessGameGrid( Cube_t** );

#endif 