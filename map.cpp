
#include <allegro.h>
#include <stdio.h>
#include <memory.h>

#include "map.h"
#include "game.h"   // for version control mainly. 
#include "player.h" // for collision stuff

extern BITMAP* doubleBuffer;
extern float masterScale;

int M_LoadTile( pTile_t tgtTile , char* fileName )
{
	static int tileRef = 0;

	tgtTile->data = load_bitmap( fileName , NULL );

	if( !tgtTile->data )
	{
		// add some error code here.
		allegro_exit();
	}

	tgtTile->x = 0;
	tgtTile->y = 0;
	tgtTile->w = tgtTile->data->w;
	tgtTile->h = tgtTile->data->h;
	tgtTile->position.x = 0;
	tgtTile->position.y = 0;
	tgtTile->attribs.type = TILE_TYPE_BACKGROUND;
	tgtTile->attribs.animated = 
	tgtTile->attribs.solid = 
	tgtTile->attribs.transparent = r_false;
	tgtTile->index = tileRef;

	// damage stuff...
	tgtTile->ent.healthStats.damageResistance = 100;
	tgtTile->ent.healthStats.health = 100;
	tgtTile->ent.healthStats.projectileStrength = 100;
	tgtTile->ent.healthStats.strength = 500;

	return tileRef++;
}

void M_LoadTestTiles( pTile_t testTiles )
{
	M_LoadTile( &testTiles[0] , "content/gfx/cubes_0006_stone.bmp" );
	M_LoadTile( &testTiles[1] , "content/gfx/cubes_0000_green.bmp" );
	M_LoadTile( &testTiles[2] , "content/gfx/cubes_0001_blue.bmp" );
	M_LoadTile( &testTiles[3] , "content/gfx/cubes_0002_red.bmp" );
	M_LoadTile( &testTiles[4] , "content/gfx/cubes_0003_yellow.bmp" );
	M_LoadTile( &testTiles[5] , "content/gfx/cubes_0004_orange.bmp" );
	M_LoadTile( &testTiles[6] , "content/gfx/cubes_0005_purple.bmp" );
}

void M_LoadStageOneTiles( pTile_t pTiles )
{
	M_LoadTile( &pTiles[0] , "content/gfx/stage_one/s1_brd.bmp" );
	M_LoadTile( &pTiles[1] , "content/gfx/stage_one/cw_v1.bmp" );
	M_LoadTile( &pTiles[2] , "content/gfx/stage_one/cw_v2.bmp" );
	M_LoadTile( &pTiles[3] , "content/gfx/stage_one/cw_v3.bmp" );
	M_LoadTile( &pTiles[4] , "content/gfx/stage_one/cwi_v1.bmp" );
	M_LoadTile( &pTiles[5] , "content/gfx/stage_one/cwi_v2.bmp" );
	M_LoadTile( &pTiles[6] , "content/gfx/stage_one/cwi_v3.bmp" );
	M_LoadTile( &pTiles[7] , "content/gfx/stage_one/cwh_v1.bmp" );
	M_LoadTile( &pTiles[8] , "content/gfx/stage_one/cwh_v2.bmp" );
	M_LoadTile( &pTiles[9] , "content/gfx/stage_one/cwh_v3.bmp" );


	M_LoadTile( &pTiles[10] , "content/gfx/stage_one/cwih_v1.bmp" );
	M_LoadTile( &pTiles[11] , "content/gfx/stage_one/cwih_v2.bmp" );
	M_LoadTile( &pTiles[12] , "content/gfx/stage_one/cwih_v3.bmp" );
	M_LoadTile( &pTiles[13] , "content/gfx/stage_one/cw_j1.bmp" );
	M_LoadTile( &pTiles[14] , "content/gfx/stage_one/cw_j2.bmp" );
	M_LoadTile( &pTiles[15] , "content/gfx/stage_one/cw_j3.bmp" );
	M_LoadTile( &pTiles[16] , "content/gfx/stage_one/cw_j4.bmp" );
	M_LoadTile( &pTiles[17] , "content/gfx/stage_one/cw_rw_d1.bmp" );
	M_LoadTile( &pTiles[18] , "content/gfx/stage_one/cw_rw_d2.bmp" );
	M_LoadTile( &pTiles[19] , "content/gfx/stage_one/cw_rw_d3.bmp" );


	M_LoadTile( &pTiles[20] , "content/gfx/stage_one/cw_rw_d4.bmp" );
	M_LoadTile( &pTiles[21] , "content/gfx/stage_one/cw_bg_d1.bmp" );
	M_LoadTile( &pTiles[22] , "content/gfx/stage_one/cw_bg_d2.bmp" );
	M_LoadTile( &pTiles[23] , "content/gfx/stage_one/cw_bg_d3.bmp" );
	M_LoadTile( &pTiles[24] , "content/gfx/stage_one/cw_bg_d4.bmp" );
	M_LoadTile( &pTiles[25] , "content/gfx/stage_one/frag_2_1.bmp" );
	M_LoadTile( &pTiles[26] , "content/gfx/stage_one/frag_2_2.bmp" );
	M_LoadTile( &pTiles[27] , "content/gfx/stage_one/frag_2_3.bmp" );
	M_LoadTile( &pTiles[28] , "content/gfx/stage_one/frag_2_4.bmp" );
	M_LoadTile( &pTiles[29] , "content/gfx/stage_one/frag_1_rw_1.bmp" );


	M_LoadTile( &pTiles[30] , "content/gfx/stage_one/frag_1_rw_2.bmp" );
	M_LoadTile( &pTiles[31] , "content/gfx/stage_one/frag_1_rw_3.bmp" );
	M_LoadTile( &pTiles[32] , "content/gfx/stage_one/frag_1_rw_4.bmp" );
	M_LoadTile( &pTiles[33] , "content/gfx/stage_one/frag_1_bg_1.bmp" );
	M_LoadTile( &pTiles[34] , "content/gfx/stage_one/frag_1_bg_2.bmp" );
	M_LoadTile( &pTiles[35] , "content/gfx/stage_one/frag_1_bg_3.bmp" );
	M_LoadTile( &pTiles[36] , "content/gfx/stage_one/frag_1_bg_4.bmp" );
	M_LoadTile( &pTiles[37] , "content/gfx/stage_one/rw_t.bmp" );
	M_LoadTile( &pTiles[38] , "content/gfx/stage_one/rw.bmp" );
	M_LoadTile( &pTiles[39] , "content/gfx/stage_one/rw_b.bmp" );

	M_LoadTile( &pTiles[40] , "content/gfx/stage_one/cwb_l1.bmp" );
	M_LoadTile( &pTiles[41] , "content/gfx/stage_one/cwb_l2.bmp" );
	M_LoadTile( &pTiles[42] , "content/gfx/stage_one/cwb_l3.bmp" );
	M_LoadTile( &pTiles[43] , "content/gfx/stage_one/cwt_l1.bmp" );
	M_LoadTile( &pTiles[44] , "content/gfx/stage_one/cwt_l2.bmp" );
	M_LoadTile( &pTiles[45] , "content/gfx/stage_one/cwt_l3.bmp" );
}

void M_Init( pMap_t pMap , pTile_t pTileSet )
{
	memset( pMap , 0 , sizeof(Map_t) );
	
	pMap->tiles = pTileSet;
}


void M_AllocMap( int*** tileArray , int rows , int cols )
{
	*tileArray = (int**)malloc(sizeof(int*)*rows );

	for( int r=0;r<rows;r++ )
	{
		**tileArray = (int*)malloc(sizeof(int)*cols);
		**tileArray+=r;
	}
}

// import RetroEditor rmap file
r_boolean M_LoadMap( pMap_t pMap , char* fileName )
{
	FILE* retroFile = fopen( fileName , "rb" );

	switch( pMap->rows ) // or switch to something damnit!!!
	{
		
	default:
		pMap->tileCount = 46;
		break;
	}

	if( retroFile == NULL)
	{
		allegro_exit();
		return r_false;
	}
// read in header.
	fread( &pMap->rows , sizeof(int) , 1 , retroFile );
	fread( &pMap->cols , sizeof(int) , 1 , retroFile );

	//M_AllocMap( &pMap->mapTiles[0][0] , pMap->rows , pMap->cols ); // hmmm... isn't this lovely?
	M_AllocMap( &pMap->pMapTiles , pMap->rows , pMap->cols );


// read in tile indicies
	for( int r=0;r<pMap->rows;r++ )
	{
		for( int c=0;c<pMap->cols;c++ )
		{
			fread( &pMap->mapTiles[r][c] ,  sizeof(int) , 1 , retroFile );
			//fread( &pMap->pMapTiles[r][c] , sizeof(int) , 1 , retroFile );
			
			int tile = pMap->mapTiles[r][c];

		}
	}	

// alloc and read loop triggers
	fread( &pMap->loopPointCount  , sizeof(int) , 1 , retroFile );
	pMap->loopPoints = (pLoopPoint_t)malloc(sizeof(LoopPoint_t)*pMap->loopPointCount);

	int trigger = 0;

	if( pMap->loopPointCount )
	// granted this is "offline" , but there is no need no any processing that doesn't need to be done.
	{
		for( ;trigger < pMap->loopPointCount;trigger++ )
		{
			fread( &pMap->loopPoints[ trigger ].mapLocation , sizeof(int) , 1 , retroFile );
			fread( &pMap->loopPoints[ trigger ].loopType , sizeof(enum) , 1 , retroFile );
			fread( &pMap->loopPoints[ trigger ].infinite , sizeof(r_boolean) , 1 , retroFile );
			fread( &pMap->loopPoints[ trigger ].iterations , sizeof(int) , 1, retroFile );
		}
	}
// "" scale points
	fread( &pMap->scalePointCount , sizeof(int) , 1 , retroFile );
	pMap->scalePoints = (ScalePoint_t*)malloc(sizeof(ScalePoint_t)*pMap->scalePointCount );
	
	if( pMap->scalePointCount )
	{
		for( trigger = 0;trigger < pMap->scalePointCount;trigger++ )
		{	
			fread( &pMap->scalePoints[ trigger ].mapLocation , sizeof(int) , 1 , retroFile );
			fread( &pMap->scalePoints[ trigger ].scaleFactor , sizeof(float) , 1 , retroFile );
			fread( &pMap->scalePoints[ trigger ].scaleSpeed , sizeof(float) , 1, retroFile );
		}
	}

// "" speed points
	fread( &pMap->accelerationPointCount , sizeof(int) , 1 , retroFile );
	pMap->accelerationPoints = (pAccelerationPoint_t)malloc(sizeof(AccelerationPoint_t)*pMap->accelerationPointCount );

	if( pMap->accelerationPointCount )
	{
		for( trigger=0;trigger < pMap->accelerationPointCount;trigger++ )
		{
			fread( &pMap->accelerationPoints[ trigger ].mapPosition , sizeof(int) , 1 , retroFile );
			fread( &pMap->accelerationPoints[ trigger ].speed , sizeof(int) , 1 , retroFile );
		}
	}

// spawn points
	fread( &pMap->spawnPointCount , sizeof(int) , 1 , retroFile );
	pMap->spawnPoints = (pSpawnPoint_t)malloc(sizeof(SpawnPoint_t)*pMap->spawnPointCount);

	if( pMap->spawnPointCount )
	{
	
		for( trigger=0;trigger < pMap->spawnPointCount;trigger++ )
		{
			fread( &pMap->spawnPoints[ trigger ].mapPosition , sizeof(int) , 1 , retroFile );
		
			//added on aug/18/07
			fread( &pMap->spawnPoints[ trigger ].mapCoords.x , sizeof(int) , 1 , retroFile );
			fread( &pMap->spawnPoints[ trigger ].mapCoords.y , sizeof(int) , 1 , retroFile );
		
			fread( &pMap->spawnPoints[ trigger ].entityType , sizeof(int) , 1 , retroFile );
			fread( &pMap->spawnPoints[ trigger ].entityGroupSize , sizeof(int) , 1 , retroFile );
			fread( &pMap->spawnPoints[ trigger ].startingEdge , sizeof(int) , 1 , retroFile );
			fread( &pMap->spawnPoints[ trigger ].startingDirection , sizeof(int) , 1 , retroFile );
			fread( &pMap->spawnPoints[ trigger ].formationType , sizeof(int) , 1 , retroFile );
			fread( &pMap->spawnPoints[ trigger ].formationSize , sizeof(int) , 1 , retroFile );
			fread( &pMap->spawnPoints[ trigger ].couplingStrenght , sizeof(int) , 1 , retroFile );

			// just to to the intermediate values in the debugger
			int mapCoordsX = pMap->spawnPoints[ trigger ].mapCoords.x;
			int mapCoordsY = pMap->spawnPoints[ trigger ].mapCoords.y;
		}
	}

	// added on [ 02272008 ]
	M_LoadSupplementsFromFile( pMap , retroFile );

	fclose( retroFile );

	return r_true;
}

/*
** added on [ 02272008 ]
*/
/*static*/ void M_LoadSupplementsFromFile( pMap_t pMap , FILE* fp )
{
	int tileCount = pMap->tileCount;

	for( int tile=0;tile < tileCount;tile++ )
	{
		fread( &pMap->tiles[ tile ].attribs.type , sizeof(TileType) , 1 , fp );
		fread( &pMap->tiles[ tile ].attribs.animated , sizeof(int) , 1 , fp );
		fread( &pMap->tiles[ tile ].attribs.transparent , sizeof(int) , 1, fp );
		fread( &pMap->tiles[ tile ].attribs.solid , sizeof(int) , 1 , fp );
	}
}

void M_Draw( pMap_t pMap )
{
	// Add "empty" tile to reference if either of these values to computed to be outside the bounds of the passed map.
	int maxRows=( (SCREEN_H)/(TILE_HIEGHT*masterScale) )+2,  // NOTE: '2' is added to blit potentially offscreen tiles due to global scale
		maxCols=( (SCREEN_W)/(TILE_WIDTH*masterScale) )+2;
	
	int xPosition = 0;
	int yPosition = 0;
	int parallaxFX = 0;
	
	float xFactor = masterScale;
	float yFactor = masterScale;
	
	static int xMapEdge = pMap->cols*TILE_WIDTH; 
	static int yMapEdge = pMap->rows*TILE_HIEGHT;

	// ADD TO MAP INIT CODE LATER ON
	pMap->maxPosition.x = xMapEdge;
	pMap->maxPosition.y = yMapEdge;
	
	int currentCol = (int)(pMap->position.x/TILE_WIDTH);
	int currentRow = (int)(pMap->position.y/TILE_HIEGHT);
	int xOffset = (pMap->position.x % TILE_WIDTH)*xFactor;
	int yOffset = (pMap->position.y % TILE_HIEGHT)*yFactor;

#ifdef __RETRO_TYPE_DEBUG_VERSION__
	for( int y=1;y<maxRows-5;y++) // allow for space on bottom for debug info
#else	
	for( int y=0; y<maxRows; y++ )
#endif	
	{
		for( int x=0; x<maxCols;x++ )
		{
			// if outside the bounds of the map, use a placeholder ( our boundary tile )
			int currentTileRef = // (currentCol+x>pMap->cols || currentRow+y>pMap->rows ) ?  // RIGHT 
								 (currentCol+x>pMap->cols || currentRow+y>pMap->rows ) ? // wrong but useful for now.
				                 (MAP_BOUNDARY_TILE) : 
								 (pMap->mapTiles[ currentRow+y ][ currentCol+x ]);

			if( currentTileRef == MAP_ALLOW_BACKGROUND )
			{
				continue;
			}

			if( currentTileRef == MAP_BOUNDARY_TILE )
			{
				// add more later on.				
				continue;
			}
			
			Tile_t currentTile = pMap->tiles[ currentTileRef ];

			xPosition = (TILE_WIDTH*x)*masterScale;
			yPosition = (TILE_HIEGHT*y)*masterScale;

			// for parallax
			switch( currentTile.attribs.type )
			{
			case TILE_TYPE_BACKGROUND:
				{
					parallaxFX = LAYER_BG_OFFSET;
					break;
				}

			case TILE_TYPE_LAYER_1:
				{
					parallaxFX = LAYER_1_OFFSET;
					break;
				}

			case TILE_TYPE_LAYER_2:
				{
					parallaxFX = LAYER_2_OFFSET;
					break;
				}

			case TILE_TYPE_LAYER_3:
				{
					parallaxFX = LAYER_3_OFFSET;
					break;
				}

			case TILE_TYPE_LAYER_PLAYING_FIELD:
				{
					parallaxFX = LAYER_PLAYING_FIELD_OFFSET;
					break;
				}

			case TILE_TYPE_LAYER_4:
				{
					parallaxFX = LAYER_4_OFFSET;
					break;
				}

			case TILE_TYPE_LAYER_5:
				{
					parallaxFX = LAYER_5_OFFSET;
					break;
				}

			case TILE_TYPE_LAYER_6:
				{
					parallaxFX = LAYER_6_OFFSET;
					break;
				}

			case TILE_TYPE_FOREGROUND:
				{
					parallaxFX = LAYER_FOREGROUND_OFFSET;
					break;
				}
			}

			// adjust for parallax effect.
//			xPosition += parallaxFX;

/*			masked_stretch_blit( pMap->tiles[ currentTileRef ].data , doubleBuffer , 
				          0 , 0 , TILE_WIDTH , TILE_HIEGHT ,
						  xPosition - xOffset,
						  yPosition - yOffset,
						  TILE_HIEGHT * xFactor , TILE_WIDTH * yFactor );
*/
				
			masked_stretch_blit( pMap->tiles[ currentTileRef ].data , doubleBuffer , 
								 0 , 0 , 
								 64 , 64 , 
								 xPosition - xOffset , 
								 yPosition - yOffset , 
								 32 * masterScale, 32 * masterScale );

		}
	}
}

// added [ 02/29/2008 ]

r_boolean M_P_CheckForCollision( pPlayer_t pP , pMap_t pMap )
{
/*	pVect2_t pPlayerPosition = &pP->ent.position;
	
	// first determine where to samplee the tile set from. 
	int mapRow = pMap->position.y >> 5;
	int mapCol = pMap->position.x >> 5;

	// now determine where player is in relation to normalized map coords.
	int playerRow = pPlayerPosition->y >> 5;
	int playerCol = pPlayerPosition->x >> 5;

	// sample tiles in moveable ( 4 ) directions around player , and the concurrent location
	int tileRefs[] = { 0 ,   0 ,  0  , 0  , 0 };
	//                conc  up   down left right
	TileType tileTypes[] = { TILE_TYPE_BACKGROUND , 
							 TILE_TYPE_BACKGROUND , 
				 			 TILE_TYPE_BACKGROUND , 
							 TILE_TYPE_BACKGROUND , 
							 TILE_TYPE_BACKGROUND };
	
	r_boolean solidTiles[] = { r_false , r_false , r_false , r_false , r_false };

	// get the tile references for all the surrounding tiles
	tileRefs[ MAP_DIRECTION_NULL ]  = pMap->mapTiles[ mapRow + playerRow ][ mapCol + playerCol ];		
	tileRefs[ MAP_DIRECTION_UP ]    = pMap->mapTiles[ (mapRow + playerRow) - 1 ][ mapCol + playerCol ];
	tileRefs[ MAP_DIRECTION_DOWN ]  = pMap->mapTiles[ (mapRow + playerRow) + 1 ][ mapCol + playerCol ];
	tileRefs[ MAP_DIRECTION_LEFT ]  = pMap->mapTiles[ mapRow + playerRow ][ (mapCol + playerCol) - 1 ];
	tileRefs[ MAP_DIRECTION_RIGHT ] = pMap->mapTiles[ mapRow + playerRow ][ (mapCol + playerCol) + 1 ];

	// ... based on the above , get the types of the surrounding tiles... 
	tileTypes[ MAP_DIRECTION_NULL ]  = pMap->tiles[ tileRefs[0] ].attribs.type;
	tileTypes[ MAP_DIRECTION_UP ]    = pMap->tiles[ tileRefs[1] ].attribs.type;
	tileTypes[ MAP_DIRECTION_DOWN ]  = pMap->tiles[ tileRefs[2] ].attribs.type;
	tileTypes[ MAP_DIRECTION_LEFT ]  = pMap->tiles[ tileRefs[3] ].attribs.type;
	tileTypes[ MAP_DIRECTION_RIGHT ] = pMap->tiles[ tileRefs[4] ].attribs.type;

	// ... now check to see if there are any solid tiles
	solidTiles[ MAP_DIRECTION_NULL ]  = pMap->tiles[ tileRefs[0] ].attribs.solid;
	solidTiles[ MAP_DIRECTION_UP ]    = pMap->tiles[ tileRefs[1] ].attribs.solid;
	solidTiles[ MAP_DIRECTION_DOWN ]  = pMap->tiles[ tileRefs[2] ].attribs.solid;
	solidTiles[ MAP_DIRECTION_LEFT ]  = pMap->tiles[ tileRefs[3] ].attribs.solid;
	solidTiles[ MAP_DIRECTION_RIGHT ] = pMap->tiles[ tileRefs[4] ].attribs.solid;
	
	// Check for collison with solid tiles. 
	for( int direction = 0;direction < MAP_DOF;direction++ )
	{
		if( solidTiles[ direction ] )
		{
			switch( direction ) 
			{
			case MAP_DIRECTION_NULL: // concurrent
				{			
					break;
				}

			case MAP_DIRECTION_UP:
				{
					pP->directions.up = r_false;
					break;
				}

			case MAP_DIRECTION_DOWN:
				{
					pP->directions.down = r_false;
					break;
				}

			case MAP_DIRECTION_LEFT:
				{
					pP->directions.left = r_false;
					break;
				}

			case MAP_DIRECTION_RIGHT:
				{
					pP->directions.right = r_false;
					break;
				}
			}
			
			if( Ent_TestForCollision( &pP->ent , &pMap->tiles[ tileRefs[ direction ] ].ent ) )
			{
				Ent_InflictDamage( pP , &pMap->tiles[ tileRefs[ direction ] ].ent , r_false , r_true );
			}
		}

		else
		{
			switch( direction )
			{
			case MAP_DIRECTION_NULL:
				{
					pP->directions.up = 
					pP->directions.down =
					pP->directions.left = 
					pP->directions.right = r_false;
					break;
				}

			case MAP_DIRECTION_UP:
				{
					pP->directions.up = r_true;
					break;
				}
				
			case MAP_DIRECTION_DOWN:
				{
					pP->directions.down = r_true;
					break;
				}
				
			case MAP_DIRECTION_LEFT:
				{
					pP->directions.left = r_true;
					break;
				}

			case MAP_DIRECTION_RIGHT:
				{
					pP->directions.right = r_true;
					break;
				}
			}
		}
	}
*/
	return pP->ent.collided;
}
