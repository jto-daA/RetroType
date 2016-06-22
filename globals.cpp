

#include "game.h"

Game_t currentGame;
float masterScale=1.0f; // used for neo-geo style scaling effects.
int fps=0;

float cosineLookup[ TRIG_TABLE_SIZE ];
float   sineLookup[ TRIG_TABLE_SIZE ];