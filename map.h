
#ifndef __MAP_H__
#define __MAP_H__

#include <allegro.h>
#include <stdio.h>

#include "common.h"
#include "player.h"

#define MAX_TILE_COUNT 50

#define TILE_WIDTH  32
#define TILE_HIEGHT 32

#define MAP_HEADER_SIZE 68       // from editor
#define MAP_ALLOW_BACKGROUND -1  // also from editor.

#define MAP_BOUNDARY_TILE    -1  // consider making this stage specific

// even more stuff from the editor
#define FORMATION_EDGE_TOP           0x00000001
#define FORMATION_EDGE_BOTTOM        0x00000002
#define FORMATION_EDGE_LEFT          0x00000004
#define FORMATION_EDGE_RIGHT         0x00000008

#define FORMATION_DIRECTION_SOUTH    0x00000010
#define FORMATION_DIRECTION_NORTH    0x00000020
#define FORMATION_DIRECTION_WEST     0x00000040
#define FORMATION_DIRECTION_EAST     0x00000080

#define FORMATION_COUPLING_LOOSE     0x00000100
#define FORMATION_COUPLING_NORMAL    0x00000200
#define FORMATION_COUPLING_TIGHT     0x00000400

// this macros correspond to the drop down menus for entity type and formation type 
#define ENTITY_TYPE_KAMIKAZE             0
#define ENTITY_TYPE_RED_POWERUP          1
#define ENTITY_TYPE_GREEN_POWERUP        2
#define ENTITY_TYPE_BLUE_POWERUP         3
#define ENTITY_TYPE_YELLOW_POWERUP       4
#define ENTITY_TYPE_COCCUS               5
#define ENTITY_TYPE_BACILLI              6
#define ENTITY_TYPE_DIPLOBACILLI         7
#define ENTITY_TYPE_STREPTOBACILLI       8
#define ENTITY_TYPE_BLASTO               9 
#define ENTITY_TYPE_BOSS_ONE            10
#define ENTITY_TYPE_BOSS_ONE_ORB        11
#define ENTITY_TYPE_BOSS_TWO            12
#define ENTITY_TYPE_BOSS_THREE          13

#define ENTITY_DUMMY_TYPE              0xff00ee

#define ENTITY_TYPE_NON_COMBATIVE        9

#define FORMATION_TYPE_VERTICAL          0
#define FORMATION_TYPE_HORIZONTAL        1
#define FORMATION_TYPE_SQUARE            2

// this correspond to the radio buttons elements
#define ENTITY_GROUP_SIZE_SINGLE         0
#define ENTITY_GROUP_SIZE_MULTIPLE       1

#define STARTING_EDGE_LEFT               0
#define STARTING_EDGE_RIGHT              1
#define STARTING_EDGE_TOP                2
#define STARTING_EDGE_BOTTOM             3

#define STARTING_DIRECTION_EAST          0
#define STARTING_DIRECTION_WEST          1
#define STARTING_DIRECTION_SOUTH         2
#define STARTING_DIRECTION_NORTH         3

#define COUPLING_STRENGHT_TIGHT          0
#define COUPLING_STRENGHT_NORM           1
#define COUPLING_STRENGHT_LOOSE          2

#define LAYER_BG_OFFSET                  80
#define LAYER_1_OFFSET                   60
#define LAYER_2_OFFSET                   40
#define LAYER_3_OFFSET                   20
#define LAYER_PLAYING_FIELD_OFFSET        0
#define LAYER_4_OFFSET                  -20
#define LAYER_5_OFFSET                  -40
#define LAYER_6_OFFSET                  -60
#define LAYER_FOREGROUND_OFFSET         -80


#define MAP_DIRECTION_NULL                0
#define MAP_DIRECTION_UP                  1
#define MAP_DIRECTION_DOWN                2
#define MAP_DIRECTION_LEFT                3
#define MAP_DIRECTION_RIGHT               4  
#define MAP_DIRECTION_NW                  5
#define MAP_DIRECTION_NE                  6
#define MAP_DIRECTION_SW                  7
#define MAP_DIRECTION_SE                  8
#define MAP_DOF                           5

typedef enum TileType
// used for parallax layers
{
	TILE_TYPE_BACKGROUND = 0x00 ,
	TILE_TYPE_LAYER_1    = 0x01 ,
	TILE_TYPE_LAYER_2    = 0x02 ,
	TILE_TYPE_LAYER_3    = 0x03 ,
	TILE_TYPE_LAYER_PLAYING_FIELD    = 0x04 , 
	TILE_TYPE_LAYER_4    = 0x05 ,
	TILE_TYPE_LAYER_5    = 0x06 , 
	TILE_TYPE_LAYER_6    = 0x07 ,
	TILE_TYPE_FOREGROUND = 0x08 ,
	TILE_VAR_TRANSPARENT = 0x10  
};

typedef struct TileData_t
{
	TileType type;
	r_boolean transparent,solid,animated;
}*pTileData_t;

typedef struct Tile_t
{
	Entity_t ent;
	
	Vect2_t position;  // normalized map-space coordinates
	int x,y;           // actual x,y coordinates for upper-left of tile
	int w,h;           // dimensions of tile
	BITMAP* data;      // pixel data
	int index;         // index used to reference data from a list of loaded bitmaps
	TileData_t attribs;
}*pTile_t;

// data types to support triggers from editor
typedef struct LoopPoint_t
{
	enum LoopType
	{
		LOOP_POINT_ORIGIN,
		LOOP_POINT_TERM
	};

	LoopType loopType;
	int mapLocation,iterations;
	r_boolean infinite;

}*pLoopPoint_t;

typedef struct ScalePoint_t
{
	int mapLocation;
	float scaleFactor,scaleSpeed;
	r_boolean tgtScaleReached;
}*pScalePoint_t;

typedef struct AccelerationPoint_t
{
	int mapPosition,speed;
}*pAccelerationPoint_t;

typedef struct SpawnPoint_t
{
	int mapPosition , 
		entityType , 
		entityGroupSize , 
		startingEdge , 
		startingDirection , 
		formationType , 
		formationSize , 
		couplingStrenght;
	Vect2_t mapCoords;
}*pSpawnPoint_t;

typedef struct Map_t
{
	pTile_t tiles;
	int rows,cols,tileCount;
	int mapTiles[40][500];
	int** pMapTiles;
	Vect2_t position;    // view space coord for upper left of map.
	Vect2_t maxPosition; // view space coord for lower right of map.

	pLoopPoint_t loopPoints; 
	int loopPointCount;

	pScalePoint_t scalePoints;
	int scalePointCount;

	pAccelerationPoint_t accelerationPoints;
	int accelerationPointCount;

	pSpawnPoint_t spawnPoints;
	int spawnPointCount;

}*pMap_t;

int M_LoadTile( pTile_t , char* );
void M_Init( pMap_t , pTile_t );
void M_AllocMap( int*** , int , int );
void M_Draw( pMap_t );
void M_LoadTestTiles( pTile_t );
void M_LoadStageOneTiles( pTile_t );
r_boolean  M_LoadMap( pMap_t , char* );
/*static*/ void M_LoadSupplementsFromFile( pMap_t , FILE* );

r_boolean M_P_CheckForCollision( pPlayer_t , pMap_t );

#endif 