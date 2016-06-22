
#include <allegro/allegro.h>
#include <stdio.h>
#include <math.h>

#include "common.h"
#include "cube.h"
#include "game.h"
#include "logic.h"
#include "gfx_system.h"
#include "gfx_elements.h"
#include "gfx_sprite.h"
#include "dev_intro.h"
#include "gfx_font.h"
#include "underwater_level.h"
#include "gui.h"
#include "title_screen.h"
#include "audio.h"

extern volatile int globalTickCount;
extern volatile int gameTickCount;
extern BITMAP* pageOne,*pageTwo,*pageThree;
Game_t currentGame;

void main( void )
{
	int index=0,linkSize = 0;

	GFX_SYS_SetVideoMode( &currentGame , 640 , 480 , 16 , c_false );

	G_Init( &currentGame );	

	TS_LoadTitleScreen();

	//int songHandle = AUD_LoadSong( "content/audio/music/GAME_TITLE1.MID" , c_true );
	int songHandle = AUD_LoadSong( "content/audio/music/GAME_TITLE.MP3" , c_true );
	
	AUD_FadeOutSong( songHandle );
	AUD_PlaySong( songHandle );
	
	while( !key[ KEY_ESC ] ) 
	{
		TS_DistortScreen2();
		TS_DrawScreen();
	}

	key[ KEY_ESC ] = 0;
	AUD_StopSong( songHandle );

	while( !key[ KEY_ESC ] )
	{
		linkSize = G_RunLogic( &currentGame );	
		G_Delay( &currentGame );
	}

	G_ShutDown( &currentGame );	
}

END_OF_MAIN();
