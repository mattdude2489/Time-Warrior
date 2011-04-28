//World class. Contains the basic entities to easily update and draw to screen.
//Created on March 10, 2011. Edited on March 25, 2011.
#pragma once
#include "Grid.h"
#include "templatevector.h"
#include "srect.h"

enum e_sprites			{SLIME, SKELETON, GHOST, BOSS1, NPC1, NUM_SPRITES};
enum e_worldsprites		{SINGLE, ANIMATION, TREE_SPRITE, NUM_SPRITES_WORLD};
enum e_trees			{TREE_BARE, TREE_FALL, TREE_NORMAL, TREE_PALM, TREE_PINE, TREE_COUNT, TREE_WIDTH = 66, TREE_HEIGHT = 96};
enum e_worldTileS		{TILE_BLANK, TILE_METAL_L, TILE_METAL_QUAD_L, TILE_METAL_R, TILE_METAL_QUAD_R, TILE_METAL, TILE_METAL_QUAD, TILE_GRASS, TILE_DUST, TILE_DUST_D, TILE_DUST_R, TILE_DUST_CORNER, TILE_DIRT1, TILE_DIRT2, TILE_SPIKE_LG, TILE_ROCKS, TILE_SPIKE_SM, TILE_DUNGEON_CLOSED, NUM_WORLD_TILE_S};
enum e_worldTileA		{TILE_PORTAL, TILE_DUNGEON, TILE_WATER, TILE_WATER_D, TILE_WATER_U, TILE_WATER_R, TILE_WATER_L, TILE_BRIDGE_V, TILE_BRIDGE_H, NUM_WORLD_TILE_A};
#define ERROR_CODE	-1

struct Tile
{
	SDL_Sprite * currentTexture;
	bool collide, portal, dungeon, spawnLocation, bossLoc, playerSpawn, tree;
	SPoint pos, *cam;
	int indexOfSpriteRow;
	static int portalIndexNumber;
	SPoint getLocationScreen(){return pos.difference(*cam);}
	Entity * m_player;
	SRect collideBox;
};

struct SpriteInfo
{
	char * fileName;
	int frameWidth, frameHeight, animSpeed, rows;
};

class World
{
private:
	TemplateVector2<Grid> m_mapOfEntities;
	TemplateVector2<Tile> m_mapOfWorld;
	SpriteInfo m_sprites[NUM_SPRITES];
	SDL_Sprite * m_worldSprites[NUM_SPRITES_WORLD];

	bool m_success, * m_closed;//m_closed to determine if dungeon is closed
	int clientPlayerIndex, currentWorld, maxWorldX, maxWorldY, tileX, tileY, bossCount;
	Entity * m_player;
	SRect m_cCamera; //The client's camera.
public:
	World();
	~World();
	void add(Entity * newEntity){m_mapOfEntities.get(getLocationGrid(newEntity)).setEntity(newEntity);if(newEntity->getType() == PLAYER) m_player = newEntity;}
	void setCamera(SPoint * a_camera);
	void sortOnYPosition();
	void setMonsters();
	void setNPC();
	void update(Uint32 a_timePassed);
	void draw(SDL_Surface * a_screen);
	bool setWorld(char * fileName);
	bool getSuccess() {return m_success;}
	int getGridWidth(){return maxWorldX/NUM_GRIDS_PER_ROW_COL;}
	int getGridHeight(){return maxWorldY/NUM_GRIDS_PER_ROW_COL;}
	int getLocationGrid(int a_x, int a_y)
	{
		int x = a_x / getGridWidth();
		int y = a_y / getGridHeight();
		int temp = (x + (NUM_GRIDS_PER_ROW_COL * y));
		if(x < 0 || y < 0
		|| x > (NUM_GRIDS_PER_ROW_COL-1) || y > (NUM_GRIDS_PER_ROW_COL-1))
			return	ERROR_CODE;
		else
			return temp;
	}
	int getLocationGrid(Entity * a_entity){return getLocationGrid(a_entity->getLocation().x, a_entity->getLocation().y);}
	int getNumEntities(int i) {return m_mapOfEntities.get(i).getNumberOfEntities();}
	int getNumEntities();
	int getCurrentWorld(){return currentWorld;}
	int getWorldWidth(){return maxWorldX;}
	int getWorldHeight(){return maxWorldY;}
	int getNumTiles(){return m_mapOfWorld.size();}
	Entity * getEntity(int a_entity, int a_grid){return m_mapOfEntities.get(a_grid).getEntity(a_entity);}
	Entity * getEntity(int a_entity, int a_x, int a_y){return m_mapOfEntities.get(getLocationGrid(a_x, a_y)).getEntity(a_entity);} //So that each entity does NOT need to know which grid it's in!
	Entity * getPlayer(){return m_player;}
	Grid * getGrid(int a_index) {return &m_mapOfEntities.get(a_index);}
	Grid * getGrid(int a_x, int a_y){return &m_mapOfEntities.get(getLocationGrid(a_x, a_y));}
	Tile * getTile(int a_tile){return &m_mapOfWorld.get(a_tile);}
	Tile * getTile(int a_x,int a_y);
	//my networking disastor
	char * convertAllEntitiesToCharBuffer();
	void convertFromServer(char * omgServerInfo);
	//networking stuff
	void updateFromServer(char * a_in);
	void setDClosed(int in){m_closed[in] = true;}
	bool isDClosed(int in){return m_closed[in];}
	int getBossCount(){return bossCount;}
	void reduceBossCount(){bossCount--;}
};