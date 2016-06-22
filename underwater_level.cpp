/************************************************************************************
 *  underwater_level.cpp  
 *  
 *	descp : routines to animate,display and handle underwater stage
 *
 *  auth  : javery
 *  
 *  path  : C:\Program Files\Microsoft Visual Studio\MyProjects\PrimaryCubes3D\underwater_level.h
 *
 * iDate  : 10/15/06 
 * version:
 ************************************************************************************/

#include <allegro.h>
#include <math.h>

#include "underwater_level.h"
#include "game.h"

static char* waterFileName = "content/gfx/water_bg.bmp";
BITMAP* waterLevel,*oldWaterBuffer,*newWaterBuffer;
Fish_t fishSchool[ LVL_WATER_FISHES ];

extern Game_t currentGame;
extern BITMAP *pageTwo,*pageThree;
/*
==========================================================================
LVL_UnderWater_LoadFrames

Loads the background image for the under water stage.
==========================================================================
*/
void LVL_UnderWater_LoadFrames( void )
{
	waterLevel = load_bitmap( waterFileName , NULL );
	
	oldWaterBuffer = create_bitmap( waterLevel->w , waterLevel->h );
	newWaterBuffer = create_bitmap( waterLevel->w , waterLevel->h );
	
	clear_to_color( oldWaterBuffer , G_COLOR_BLACK );
	clear_to_color( newWaterBuffer , G_COLOR_BLACK );
}
/*
==========================================================================
LVL_UnderWater_CalcNewWater

Calcuates water.
==========================================================================
*/
void LVL_UnderWater_CalcNewWater( void )
{
	static int xLimit = (currentGame.gfxConfig.xDimen/2 - 1);
	static int yLimit = (currentGame.gfxConfig.yDimen - 1);
	
	int average=0;
	
	for( int y=1; y<yLimit; y++ )
	{
		for( int x=1;x<xLimit;x++ )
		{
			average = ( _getpixel16( oldWaterBuffer , x + 1 , y ) +
					    _getpixel16( oldWaterBuffer , x - 1 , y ) +
					    _getpixel16( oldWaterBuffer , x , y + 1 ) +
					    _getpixel16( oldWaterBuffer , x , y - 1 ) 
						
						/*_getpixel16( oldWaterBuffer , x + 1 , y + 1 ) +
					    _getpixel16( oldWaterBuffer , x + 1 , y - 1 ) +
					    _getpixel16( oldWaterBuffer , x - 1,  y + 1 ) +
					    _getpixel16( oldWaterBuffer , x - 1,  y - 1 ) */
						
						
						)/2  - 			 
					    _getpixel16( newWaterBuffer , x , y ) ;
			
			average >>= 5;
			average *=.35;

			if( average <= 0 )
			{
				average = 3;
			
			}
			
			if( average > 255 )
			{
				average = 255-average;
				average %= average;
			}

			_putpixel16( newWaterBuffer , x , y , average  );
		}

	}
}
/*
===========================================================================
LVL_UnderWater_PaintWater

Simulated dynamic water in offscreen buffer. 
===========================================================================
*/
void LVL_UnderWater_PaintWater( float dampening )
{
	int deltaX=0,deltaY=0,indexX=0,indexY=0,color=0,texture=0;

	for( int y = 87;y < 394;y++ )
	{
		for( int x = 12;x < 304;x++ )
		{
			// calculate the differnce between adjacent water values
			deltaX = _getpixel16( newWaterBuffer , x , y ) - _getpixel( newWaterBuffer , x + 1 , y );
			deltaY = _getpixel16( newWaterBuffer , x , y ) - _getpixel( newWaterBuffer , x , y + 1 );

			// calcualate new offsets to be use to distort orginal image
			indexX = (deltaX) + (x);
			indexY = (deltaY) + (y);

			color = deltaX;

			if( indexY > 394 )
			{
				indexY = 391;
			}
			if( indexX > 304 )
			{
				indexX = 293;
			}
			if( indexY < 87 )
			{
				indexY = 86;
			}
			if( indexX < 12 )
			{
				indexX = 11;
			}
			
			if( color <= 0 )
			{
				color = 12;
			}
			if( color > 255 )
			{
				color = 255;
			}

			//color -= 255;

			texture = _getpixel16( waterLevel , indexX , indexY );
	
			_putpixel16( pageTwo , x , y , texture );
		}
	}
	blit( pageTwo ,pageThree , 0 , 0 , 0 , 0, 320 , 480 );

	// save water for next frame
	blit( newWaterBuffer , oldWaterBuffer , 0 , 0 , 0 , 0 , 320 , 480 );
}
/*
=========================================================================
LVL_UnderWater_Rain

Drips water into the water buffer 
=========================================================================
*/
void LVL_UnderWater_Rain( void )
{
	circlefill( newWaterBuffer , 1+rand()%318 , 1+rand()%478 , rand()%10 , rand()%5 );
}

/*
=========================================================================
LVL_UnderWater_InitFish

Places fish at random locations
=========================================================================
*/
void LVL_UnderWater_InitFish( int mutate )
{
	for( int fish=0;fish<LVL_WATER_FISHES;fish++ )
	{
		fishSchool[ fish ].position.xPosition = 1 + rand() % LVL_WATER_FISHES_MAX_X;
		fishSchool[ fish ].position.yPosition = 1 + rand() % LVL_WATER_FISHES_MAX_Y;
		fishSchool[ fish ].position.zPosition = 1 + rand() % LVL_WATER_FISHES_MAX_SPEED;
		
		// initialize to c_true, so new target may be assigned.
		fishSchool[ fish ].targetReached = c_true;
	}			
}
/*
=========================================================================
LVL_UnderWater_MoveFish

Calcuates new position for each fish using modified "boid" behavior algo(s).
=========================================================================
*/
void LVL_UnderWater_MoveFish( int which ) 
{
	float xVelocity=0,yVelocity=0,zVelocity=0,dVelocity=0;

	Vect3_t destination = { 0 , 0 , 0 },
			   restrain = { 0 , 0 , 0 }, 
			       idle = { 0 , 0 , 0 };

	// required boid calculations
	fishSchool[ which ].vFactor01 = LVL_UnderWater_FindSharedCenter( which , 0 );
	fishSchool[ which ].vFactor02 = LVL_UnderWater_MaintainDistance( which , 0 );
	fishSchool[ which ].vFactor03 = LVL_UnderWater_MaintainVelocity( which , 0 );
	
	// additional AI
   	   restrain = AI_FishThink_Restrain( which , 0 , 22 , 97 , 294 ,384 );
  //destination = AI_FishThink_MoveToTarget( which , destination ,  0 );
		   idle = AI_FishThink_Idle( which , 0 );

	xVelocity = fishSchool[ which ].vFactor01.xPosition + 
				fishSchool[ which ].vFactor02.xPosition + 
				fishSchool[ which ].vFactor03.xPosition + 
				destination.xPosition +
				restrain.xPosition +
				idle.xPosition;	
	
	yVelocity = fishSchool[ which ].vFactor01.yPosition + 
				fishSchool[ which ].vFactor02.yPosition + 
				fishSchool[ which ].vFactor03.yPosition + 
				destination.yPosition + 
				restrain.yPosition +
				idle.yPosition;
		
	zVelocity = fishSchool[ which ].vFactor01.zPosition + 
				fishSchool[ which ].vFactor02.zPosition +  
				fishSchool[ which ].vFactor03.zPosition;

	fishSchool[ which ].position.xPosition += xVelocity;
	fishSchool[ which ].position.yPosition += yVelocity;
	fishSchool[ which ].position.zPosition += zVelocity;
}
/*
=========================================================================
LVL_UnderWater_DrawFish

Draws all current fish.
=========================================================================
*/
void LVL_UnderWater_DrawFish( void )
{
	for( int fish = 0;fish < LVL_WATER_FISHES;fish++ )
	{
		LVL_UnderWater_MoveFish( fish );

		rectfill( pageTwo , fishSchool[ fish ].position.xPosition , fishSchool[ fish ].position.yPosition  , 
							fishSchool[ fish ].position.xPosition + 3 , fishSchool[ fish ].position.yPosition + 3 , 
							 G_COLOR_BLUE );
		rectfill( pageThree , fishSchool[ fish ].position.xPosition , fishSchool[ fish ].position.yPosition  , 
							fishSchool[ fish ].position.xPosition + 2 , fishSchool[ fish ].position.yPosition + 2 , 
							 G_COLOR_WHITE );

		rectfill( oldWaterBuffer , fishSchool[ fish ].position.xPosition , fishSchool[ fish ].position.yPosition  , 
								   fishSchool[ fish ].position.xPosition + 3 , fishSchool[ fish ].position.yPosition + 3 , 
							       makecol(255,0,255) );
	}
}
/*
=========================================================================
LVL_UnderWater_FindSharedCenter

Finds an averaged position for all fishes within a given tolerance range.
=========================================================================
*/
static Vect3_t LVL_UnderWater_FindSharedCenter( int which , float tolerance )
{
	Vect3_t averagePosition = { 0 , 0 , 0 };

	for( int fish=0;fish < LVL_WATER_FISHES;fish++ )
	{
		if( fish == which )
		// exclude passed fish from average.
		{
			continue;
		}
		else
		{
			averagePosition.xPosition += fishSchool[ fish ].position.xPosition;
			averagePosition.yPosition += fishSchool[ fish ].position.yPosition;
		}
	}

	averagePosition.xPosition /= LVL_WATER_FISHES-1;
	averagePosition.yPosition /= LVL_WATER_FISHES-1;

	averagePosition.xPosition -= fishSchool[ which ].position.xPosition + tolerance;
	averagePosition.yPosition -= fishSchool[ which ].position.yPosition + tolerance;

	averagePosition.xPosition /= 100;
	averagePosition.yPosition /= 100;

	return averagePosition;
}
/*
=========================================================================
LVL_UnderWater_MaintainDistance

Makes sure that the passed fish stays a certain distance away from all other fishes.
Fish strives to maintain a fixed distance on all sides.
=========================================================================
*/
static Vect3_t LVL_UnderWater_MaintainDistance( int which , float tolerance )
{
	Vect3_t targetDistance = { 0 , 0 , 0 };
	float deltaX=0,deltaY=0;

	for( int fish=0;fish < LVL_WATER_FISHES;fish++ )
	{
		if( fish == which )
		{
			// you can't stay away from yourself, now can you?
			continue;
		}
		else
		{
			deltaX = ( fishSchool[ which ].position.xPosition - fishSchool[ fish ].position.xPosition );
			deltaY = ( fishSchool[ which ].position.yPosition - fishSchool[ fish ].position.yPosition );

			if( deltaX > 0 )
			{
				if( abs(deltaX) < LVL_WATER_FISH_PERSONAL_SPACE )
				// to close to another fish, so fix.
				{
					targetDistance.xPosition = (LVL_WATER_FISH_PERSONAL_SPACE);
				}
			}

			else
			{
				if( abs(deltaX) < LVL_WATER_FISH_PERSONAL_SPACE )
				{
					targetDistance.xPosition = -(LVL_WATER_FISH_PERSONAL_SPACE);
				}
			}

			if( deltaY > 0 )
			{
				if( abs(deltaY) < LVL_WATER_FISH_PERSONAL_SPACE ) 
				// again, to close. 
				{
					targetDistance.yPosition = (LVL_WATER_FISH_PERSONAL_SPACE);
				}
			}
			else
			{
				if( abs(deltaY) < LVL_WATER_FISH_PERSONAL_SPACE )
				{
					targetDistance.yPosition = -(LVL_WATER_FISH_PERSONAL_SPACE);
				}
			}
		}
	}

	return targetDistance;
}
/*
=========================================================================
LVL_UnderWater_MaintainVelocity

Makes the current fish try and keep up with surrounding fish.
=========================================================================
*/
static Vect3_t LVL_UnderWater_MaintainVelocity( int which , float tolerance ) 
{
	Vect3_t targetVelocity = { 0 , 0 , 0 };

	for( int fish=0;fish < LVL_WATER_FISHES;fish++ )
	{
		if( fish == which )
		{ 
			continue;
		}
		else
		{
			targetVelocity.zPosition += fishSchool[ fish ].position.zPosition;
		}
	}

	targetVelocity.zPosition /= LVL_WATER_FISHES-1;
	targetVelocity.zPosition -= fishSchool[ fish ].position.zPosition;
	targetVelocity.zPosition /= 2;

	return targetVelocity;
}
/*
===========================================================================
AI_FishThink_MoveToTarget

Advances the specified fish indicie towards it's destination
===========================================================================
*/
static Vect3_t AI_FishThink_MoveToTarget( int fish , Vect3_t target , float tolerance )
{
	Vect3_t targetDestination = { 0 , 0 , 0 };
	float targetX=0,targetY=0;

	if( fishSchool[ fish ].targetReached )
	// add a random target
	{
		targetX = rand()%48;
		targetY = rand()%48;

		// set all fish to the same target
		for( int i=0;i < LVL_WATER_FISHES;i++ )
		{
			fishSchool[ i ].target.xPosition = targetX;
			fishSchool[ i ].target.yPosition = targetY;
		}

		fishSchool[ fish ].targetReached = c_false;

		return targetDestination;
	}

	if( abs( fishSchool[ fish ].position.xPosition - fishSchool[ fish ].target.xPosition ) < FISH_TARGET_TOLERANCE + tolerance &&
		abs( fishSchool[ fish ].position.yPosition - fishSchool[ fish ].target.yPosition ) < FISH_TARGET_TOLERANCE + tolerance )
	// just get it close *HACK* due to speed variances
	{
		fishSchool[ fish ].targetReached = c_true;
		return targetDestination;
	}

	else
	// move closer to target
	{
		if( fishSchool[ fish ].position.xPosition < fishSchool[ fish ].target.xPosition )
		{
			targetDestination.xPosition = fishSchool[ fish ].position.zPosition;
		}
		else if( fishSchool[ fish ].position.xPosition > fishSchool[ fish ].target.xPosition )
		{
			targetDestination.xPosition = -fishSchool[ fish ].position.zPosition;
		}

		if( fishSchool[ fish ].position.yPosition < fishSchool[ fish ].target.yPosition )
		{
			targetDestination.yPosition = fishSchool[ fish ].position.zPosition;
		}
		else if( fishSchool[ fish ].position.yPosition > fishSchool[ fish ].target.yPosition )
		{
			targetDestination.yPosition = -fishSchool[ fish ].position.zPosition;
		}
	}
	
	return targetDestination;
}

/*
===========================================================================
AI_FishThink_Restrain

Confines fish movement to specified extreme cords.
===========================================================================
*/
static Vect3_t AI_FishThink_Restrain( int which , float tolerance , int xMin , int yMin , int xMax , int yMax )
{
	Vect3_t restrain = { 0 , 0 , 0 };

	if( fishSchool[ which ].position.xPosition < xMin )
	{
		restrain.xPosition = FISH_AI_BOUNDED;
	}
	else if( fishSchool[ which ].position.xPosition > xMax )
	{
		restrain.xPosition = -FISH_AI_BOUNDED;
	}
	
	if( fishSchool[ which ].position.yPosition < yMin )
	{
		restrain.yPosition = FISH_AI_BOUNDED;
	}
	else if ( fishSchool[ which ].position.yPosition > yMax )
	{
		restrain.yPosition = -FISH_AI_BOUNDED;
	}
	
	return restrain;
}
/*
==========================================================================
AI_FishThink_Idle

If nothing special is going on, then just swim around in a simple pattern,
==========================================================================
*/
static Vect3_t AI_FishThink_Idle( int which , float tolerance )
{
	Vect3_t idle = { 0 , 0 , 0 };

	static int i=0;

	idle.xPosition = 2 * sin( i*3.14159/180/200 );
	idle.yPosition = 5 * cos( i*3.14159/180/100 );
	
	i++;
	
	return idle;
}