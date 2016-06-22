
#include <memory.h>
#include <allegro.h>

#include "common.h"
#include "input_logger.h"

/*
** prepars a file for storing a running demo
*/
r_boolean IL_InitDemoFile( pDemoFile_t pDemoFile , char* fileName )
{
	pDemoFile->pDemoFile = fopen( fileName , "w" );

	if( pDemoFile->pDemoFile == NULL )
	// unable to open new file on tgt system
	{
		ASSERT( pDemoFile->pDemoFile != NULL );
	}

	else
	{
		pDemoFile->frames = 0;
	}

	return r_true;
}
/*
** loads a stored demo file 
*/
r_boolean IL_LoadDemoFile( pDemoFile_t pDemoFile , char* fileName )
{
	pDemoFile->pDemoFile = fopen( fileName , "r" );
	
	if( pDemoFile->pDemoFile == NULL )
	{
		// it must not exsists... 
		ASSERT( pDemoFile->pDemoFile != NULL );
		return r_false;
	}

	else
	{
		// its all good. 
		pDemoFile->frames = 0;
	}

	return r_true;
}
/*
** Logs the current keystates. Note this, structures the demo system is such a way that it is *purely*
** input driven.
*/
void IL_LogFrame( pGame_t pGame , pDemoFile_t pDemoFile )
{
	char keyStates[ KEY_MAX ];

	memset( (void*)keyStates , 0 , sizeof(char)*KEY_MAX );

	
	if( keyboard_needs_poll() )
	{
		poll_keyboard();
	}
	
	memcpy( keyStates , (void*)key , KEY_MAX );

	keyStates[ KEY_LEFT ] = key[ KEY_LEFT ] ? key[ KEY_LEFT ] : 0;
	keyStates[ KEY_RIGHT ] = key[ KEY_RIGHT ] ? key[ KEY_RIGHT ] : 0;
	keyStates[ KEY_UP ] = key[ KEY_UP ] ? key[ KEY_UP ] : 0;
	keyStates[ KEY_DOWN ] = key[ KEY_DOWN ] ? key[ KEY_DOWN ] : 0;
	keyStates[ KEY_SPACE ] = key[ KEY_SPACE ] ? key[ KEY_SPACE ] : 0;
	keyStates[ KEY_1 ] = key[ KEY_1 ] ? key[ KEY_1 ] : 0;
	keyStates[ KEY_2 ] = key[ KEY_2 ] ? key[ KEY_2 ] : 0;
	keyStates[ KEY_3 ] = key[ KEY_3 ] ? key[ KEY_3 ] : 0;
	keyStates[ KEY_4 ] = key[ KEY_4 ] ? key[ KEY_4 ] : 0;

	// only print the keys required for gameplay to the file
	fprintf( pDemoFile->pDemoFile , "%d %c %c %c %c %c %c %c %c %c\n" ,
			 pDemoFile->frames , 
		 	 keyStates[ KEY_LEFT ] ,
			 keyStates[ KEY_RIGHT] ,
			 keyStates[ KEY_UP ]  ,
			 keyStates[ KEY_DOWN ] ,
			 keyStates[ KEY_SPACE ] ,
			 keyStates[ KEY_1 ]  ,
			 keyStates[ KEY_2 ] ,
			 keyStates[ KEY_3 ] ,
			 keyStates[ KEY_4 ] );

	fflush( pDemoFile->pDemoFile );

	pDemoFile->frames++;
}END_OF_FUNCTION( IL_LogFrame )
/*
** takes KEY_MAX chars from demo file and stuffs them into current keystates
** NOTE : due to Allegros asyncronous key state access methods , states req. being 
** explicitly set as to retain the values been resets... 
*/
r_boolean IL_LoadFrame( pGame_t pGame , pDemoFile_t pDemoFile )
{
	char storedKeyStates[ KEY_MAX ];
	int frame=0;
	int fileReadStatus=0;

	memset( (void*)storedKeyStates , 0 , sizeof(char)*KEY_MAX );

	fileReadStatus = 
	fscanf( pDemoFile->pDemoFile , "%d %c %c %c %c %c %c %c %c %c\n" ,
			&frame , 
		 	&storedKeyStates[ KEY_LEFT ] ,
			&storedKeyStates[ KEY_RIGHT] ,
			&storedKeyStates[ KEY_UP ]  ,
			&storedKeyStates[ KEY_DOWN ] ,
			&storedKeyStates[ KEY_SPACE ] ,
			&storedKeyStates[ KEY_1 ]  ,
			&storedKeyStates[ KEY_2 ] ,
			&storedKeyStates[ KEY_3 ] ,
			&storedKeyStates[ KEY_4 ] );

	if( fileReadStatus != EOF )
	{
		if( storedKeyStates[ KEY_LEFT ] )
		{	
			key[ KEY_LEFT ] = r_true;
		}

		else
		{
			key[ KEY_LEFT ] = r_false;
		}

		if( storedKeyStates[ KEY_RIGHT ] )
		{
			key[ KEY_RIGHT ] = r_true;
		}					
		else
		{ 
			key[ KEY_RIGHT ] = r_false;
		}

		if( storedKeyStates[ KEY_UP ] )
		{
			key[ KEY_UP ] = r_true;
		}

		else
		{ 
			key[ KEY_UP ] = r_false;
		}

		if( storedKeyStates[ KEY_DOWN ] )
		{
			key[ KEY_DOWN ] = r_true;
		}

		else
		{
			key[ KEY_DOWN ] = r_false;
		}

		if( storedKeyStates[ KEY_SPACE ] )
		{
			key[ KEY_SPACE ] = r_true;
		}

		else
		{ 
			key[ KEY_SPACE ] = r_false;
		}
	
		if( storedKeyStates[ KEY_1 ] )
		{
			key[ KEY_1 ] = r_true;
		}

		else
		{ 
			key[ KEY_1 ] = r_false;
		}

		if( storedKeyStates[ KEY_2 ] )
		{
			key[ KEY_2 ] = r_true;
		}

		else
		{ 
			key[ KEY_2 ] = r_false;
		}

		if( storedKeyStates[ KEY_3 ] )
		{
			key[ KEY_3 ] = r_true;
		}

		else
		{
			key[ KEY_3 ] = r_false;
		}

		if( storedKeyStates[ KEY_4 ] )
		{
			key[ KEY_4 ] = r_true;
		}

		else
		{ 
			key[ KEY_4 ] = r_false;
		}
	}

	else
	// end of demo file reached... 
	{
		// for now, just rewind it
		rewind( pDemoFile->pDemoFile );
		return r_false;
	}

	G_WriteToDebugWindow( frame , "%d" , 250 , 20 );
	G_WriteToDebugWindow( pDemoFile->pDemoFile->_cnt, " out of %d demo frames" , 280 , 20 );

	return r_true;
}