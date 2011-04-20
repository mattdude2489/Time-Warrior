//World class. Contains the basic entities to easily update and draw to screen.
//Created on March 10, 2011. Edited on March 25, 2011.
#pragma once
#include "Grid.h"
#include "templatevector.h"
#include "srect.h"

enum m_sprites			{SLIME, SKELETON, GHOST, BOSS1, NPC1, NUM_SPRITES};
enum m_worldsprites		{HUB, EUROPEAN, DUNGEON, WATER};
#define ERROR_CODE	-1

struct Tile
{
	SDL_Sprite * currentTexture;
	bool collide, animate, portal, dungeon, spawnLocation, bossLoc, playerSpawn;
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
	//TemplateVector2<Entity*> m_mapOfEntities;
	TemplateVector2<Grid> m_mapOfEntities;
	TemplateVector2<Tile> m_mapOfWorld;
	SpriteInfo m_sprites[NUM_SPRITES];
	//SpriteInfo m_worldSprites[10];

	bool m_success;
	int clientPlayerIndex;
	int currentWorld;
	int maxWorldX, maxWorldY; //how big the world is...maximally. Used for Grids.
	int tileX, tileY;
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
		int x = (a_x - FRAME_SIZE) / getGridWidth();
		int y = (a_y - FRAME_SIZE) / getGridHeight();
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

};