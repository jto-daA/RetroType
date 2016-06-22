
/************************************************************************************
 *  gfx_system.cpp
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

#include <allegro.h>

#include "common.h"
#include "gfx_system.h"

volatile int globalTickCount = 0;                              // controls overall game speed
BITMAP* pageOne,*pageTwo,*pageThree;

/*
================================================================
GFX_SYSTEM_SetVideoMode

Sets the specified video mode. Returns 'c_false' upon error.
================================================================
*/
c_boolean GFX_SYS_SetVideoMode( pGame_t pGame , int xRes , int yRes , int depth , c_boolean fullScreen )
{
	allegro_init();
	
	set_color_depth( depth );

	pGame->gfxConfig.xDimen = xRes;
	pGame->gfxConfig.yDimen = yRes;
	pGame->gfxConfig.bitDepth = depth;	

	if( fullScreen )
	{
		if( set_gfx_mode( GFX_AUTODETECT , xRes , yRes , 0 , 0 ) < 0 )
		{
			allegro_message( "Unable to set specified video mode : %s\n" , allegro_error );
		}
	}

	else
	{
		if( set_gfx_mode( GFX_AUTODETECT_WINDOWED , xRes , yRes , 0 , 0 ) < 0 )
		{
			allegro_message( "Unable to set specified video mode : %s\n" , allegro_error );
			return c_false;
		}

	}

	return c_true;
}
/*
================================================================
GFX_SYS_TimerFunction

Timer function required by Allegro. Controls/normalizes overall
game speed.
================================================================
*/
void GFX_SYS_TimerFunction( void )
{
	globalTickCount++;
}
END_OF_FUNCTION( GFX_SYS_TimerFunction );



