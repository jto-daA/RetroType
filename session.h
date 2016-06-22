/************************************************************************************
 *  session.h  
 *  
 *	descp : Defines current game session
 *
 *  auth  : javery
 *  
 *  path  : C:\Program Files\Microsoft Visual Studio\MyProjects\PrimaryCubes3D\session.h
 *
 * iDate  : 09/17/06 
 * version:
 ************************************************************************************/

#ifndef __SESSION_H__ 
#define __SESSION_H__

typedef enum
{ 
	LVL_EASY, 
	LVL_NORM,
	LVL_HARD
}GameLevel_t;

typedef struct Session_t
{
	GameLevel_t currentGameLevel;	
	int currentStage;
	int linkSize,maxLink;

	// grid size ,grid padding , space in-between cells
	int xDimen,yDimen,xPadding;
	float yPadding,deltaX,deltaY;

	// time
	int totalTime,
		timeToNextLevel;

	// score
	int currentScore,
		highScore;
	char sCurrentScore[8],
		 sHighScore[8];

	// booleans
	c_boolean paused,sessionOver;
}*pSession_t;

#endif 
