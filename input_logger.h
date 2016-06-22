
#ifndef __INPUT_LOGGER_H__
#define __INPUT_LOGGER_H__

#include <stdio.h>

#include "common.h"
#include "game.h"

typedef struct DemoFile_t
{
	FILE* pDemoFile; 
	int frames;

}*pDemoFile_t;

r_boolean IL_InitDemoFile( pDemoFile_t , char* );
r_boolean IL_LoadDemoFile( pDemoFile_t , char* );
void IL_LogFrame( pGame_t , pDemoFile_t ); 
r_boolean IL_LoadFrame( pGame_t , pDemoFile_t );

#endif
