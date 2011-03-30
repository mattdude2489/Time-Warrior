//World class. Contains the basic entities to easily update and draw to screen.
//Created on March 10, 2011. Edited on March 25, 2011.
#pragma once
#include "Grid.h"
#include "templatevector.h"

struct Tile
{
	SDL_Sprite * currentTexture;
	bool collide;
	SPoint pos, *cam;
	int indexOfSpriteRow;
	SPoint getLocationScreen(){return pos.difference(*cam);}
};
class World
{
private:
	//TemplateVector2<Entity*> m_mapOfEntities;
	TemplateVector2<Grid> m_mapOfEntities;
	TemplateVector2<Tile> m_mapOfWorld;
	bool m_success;
	int clientPlayerIndex;
	int currentWorld;
	int maxWorldX, maxWorldY; //how big the world is...maximally. Used for Grids.
	int tileX, tileY;
public:
	World();
	~World();
	void add(Entity * newEntity){m_mapOfEntities.get(getLocationGrid(newEntity)).setEntity(newEntity);}
	void setCamera(SPoint * a_camera);
	void sortOnYPosition();
	void update(Uint32 timePassed);
	void draw(SDL_Surface * a_screen);
	bool setWorld(char * fileName);
	bool getSuccess() {return m_success;}
	int getGridWidth(){return maxWorldX/NUM_GRIDS_PER_ROW_COL;}
	int getGridHeight(){return maxWorldY/NUM_GRIDS_PER_ROW_COL;}
	int getLocationGrid(int a_x, int a_y){return (a_x / getGridWidth()) + (4 * (a_y / getGridHeight()));}
	int getLocationGrid(Entity * a_entity){return getLocationGrid(a_entity->getLocation().x, a_entity->getLocation().y);}
	int getNumEntities(int i) {return m_mapOfEntities.get(i).getNumberOfEntities();}
	int getNumEntities();
	Entity * getEntity(int a_entity, int a_grid){return m_mapOfEntities.get(a_grid).getEntity(a_entity);}
	Entity * getEntity(int a_entity, int a_x, int a_y){return m_mapOfEntities.get(getLocationGrid(a_x, a_y)).getEntity(a_entity);} //So that each entity does NOT need to know which grid it's in!
	Grid * getGrid(int a_index) {return &m_mapOfEntities.get(a_index);}
	Grid * getGrid(int a_x, int a_y){return &m_mapOfEntities.get(getLocationGrid(a_x, a_y));}
	Tile * getTile(int a_tile){return &m_mapOfWorld.get(a_tile);}
	Tile * getTile(int a_x,int a_y);
};