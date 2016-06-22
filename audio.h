
/************************************************************************************
 *  audio.h  
 *  
 *	descp : FMOD/audio interface types/prototypes ( http://www.fmod.org/ )
 *
 *  auth  : javery
 *  
 *  path  : C:\Program Files\Microsoft Visual Studio\MyProjects\PrimaryCubes3D\audio.h
 *
 * iDate  : 10/26/06 
 * version:
 ************************************************************************************/

#ifndef __AUDIO_H__
#define __AUDIO_H__

#include "common.h"

#define MAX_FMOD_MUSIC_HANDLES      20
#define MAX_FMOD_SAMPLE_HANDLES     50
#define MAX_FMOD_STREAM_HANDLES     50

// system routines
c_boolean AUD_InitFMOD( int , int );
void AUD_ShutdownFMOD( void );
// user routines
int AUD_LoadSong( char* , c_boolean );
int AUD_LoadSample( char* );

void AUD_PlaySong( int );
void AUD_StopSong( int );
void AUD_FadeOutSong( int );

#endif
