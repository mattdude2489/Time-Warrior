//World class. Contains the basic entities to easily update and draw to screen.
//Created on March 10, 2011. Edited on March 25, 2011.
#pragma once
#include "Entity.h"
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
public:
	World();
	~World();
	void add(Entity * newEntity); //TODO: FIX THIS LOL.
	void setCamera(SPoint * a_camera);
	void sortOnYPosition();
	void update(Uint32 timePassed);
	void draw(SDL_Surface * a_screen);
	bool setWorld(char * fileName);
	bool getSuccess() {return m_success;}
	int getNumEntities(int i) {return m_mapOfEntities.get(i).getNumberOfEntities();}
	int getNumEntities();
	Entity * getEntity(int a_entity, int a_grid){return m_mapOfEntities.get(a_grid).getEntityAt(a_entity);}
	Entity * getEntity(int a_entity, int a_x, int a_y); //So that each entity does NOT need to know which grid it's in!
};