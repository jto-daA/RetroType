
#ifndef __DROID_H__
#define __DROID_H__

#include "player.h"

#define DROID_PHASE_ONE_LENGHT   10
#define DROID_PHASE_TWO_LENGHT  100
#define DROID_PHASE_ONE_SPEED     1
#define DROID_PHASE_TWO_SPEED     2
#define DROID_MAX_DISTANCE      100
#define DROID_COMFORT_DISTANCE   20

#define DROID_SPEED               5

#define DROID_SIZE_X             20
#define DROID_SIZE_Y             20

#define DROID_AMPLITUDE          1

void Droid_AttachToParent( pPlayer_t , pEntity_t  );

void Droid_UpdatePosition( pPlayer_t );
void Droid_Draw( pEntity_t );



#endif