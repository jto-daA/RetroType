
/************************************************************************************
 *  audio.h  
 *  
 *	descp : FMOD/audio interface ( http://www.fmod.org/ )
 *
 *  auth  : javery
 *  
 *  path  : C:\Program Files\Microsoft Visual Studio\MyProjects\PrimaryCubes3D\audio.cpp
 *
 * iDate  : 10/26/06 
 * version:
 ************************************************************************************/

#include <fmod/fmod.h>

#include "common.h"
#include "audio.h"

static FMUSIC_MODULE* fmodMusicModules[ MAX_FMOD_MUSIC_HANDLES ];
static FSOUND_SAMPLE* fmodSampleModules[ MAX_FMOD_SAMPLE_HANDLES ];
static FSOUND_STREAM*  fmodStreamModules[ MAX_FMOD_STREAM_HANDLES ];

/*
========================================================================
AUD_InitFMOD

Inits the FMOD libary.
========================================================================
*/
c_boolean AUD_InitFMOD( int rate , int channels )
{
	c_boolean success = (c_boolean)FSOUND_Init( rate , channels , FSOUND_INIT_ACCURATEVULEVELS /*| FSOUND_INIT_GLOBALFOCUS */);
	return success;
}
/*
========================================================================
AUDO_ShutDownFMOD

Shutsdown FMOD
========================================================================
*/
void AUD_ShutdownFMOD( void )
{
	FSOUND_Close();
}
/*
=========================================================================
AUD_LoadSong

Loads the specified songs into FMOD
=========================================================================
*/
int AUD_LoadSong( char* songFile , c_boolean looping )
{
	static int fmodSong=0;

	fmodSampleModules[ fmodSong ] =
	FSOUND_Sample_Load( 0 , songFile ,
		                FSOUND_LOOP_NORMAL | FSOUND_FORCEMONO | FSOUND_16BITS | FSOUND_STREAMABLE | FSOUND_2D , 
						0 , 0 );

	//FSOUND_SetLooping( fmodMusicModules[ fmodSong ] , looping );

	return fmodSong++;
}

/*
==========================================================================
AUD_PlaySong

Play music via FMOD
==========================================================================
*/
void AUD_PlaySong( int handleIndex )
{
	FSOUND_PlaySound( 0 , fmodSampleModules[ handleIndex ] );
}
/*
==========================================================================
AUD_StopSong

Stops the current song.
==========================================================================
*/
void AUD_StopSong( int handleIndex ) 
{
	FSOUND_StopSound( handleIndex );
}
/*
==========================================================================
AUD_FadeOutSong

Fades the passed song out to NULL dB
==========================================================================
*/
void AUD_FadeOutSong( int handleIndex )
{
	for( int vol=255;vol>0;vol--)
	{
		FSOUND_SetVolume( handleIndex , (int)vol );
	}
}

