
/************************************************************************************
 *  gfx_system.h
 *  
 *	descp : prototypes for system level graphics functionality ( timing as well )
 *
 *  auth  : javery
 *  
 *  path  : C:\Program Files\Microsoft Visual Studio\MyProjects\PrimaryCubes3D\gfx_system.h
 *
 * iDate  : 09/18/06 
 * version:
 ************************************************************************************/

#ifndef __GFX_SYSTEM_H__
#define __GFX_SYSTEM_H__

#include "common.h"
#include "game.h"

c_boolean GFX_SYS_SetVideoMode( pGame_t , int , int , int , c_boolean );
void GFX_SYS_TimerFunction( void );

#endif