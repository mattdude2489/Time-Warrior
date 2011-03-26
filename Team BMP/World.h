//World class. Contains the basic entities to easily update and draw to screen.
//Which of course means...SDL. Is necessary. GODDAMMIT.
//Created on March 10, 2011. Edited on March 15, 2011.
#pragma once
#include "Entity.h"
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
	TemplateVector2<Entity*> m_mapOfEntities;
	TemplateVector2<Tile> m_mapOfWorld;
	bool m_success;
	int clientPlayerIndex;
public:
	World(){m_success = setWorld("Maps/HubWorldMap.txt"); clientPlayerIndex = 0;}
	~World();
	void setAnEntity(Entity * newEntity){m_mapOfEntities.add(newEntity);}
	void setCamera(SPoint * a_camera);
	void sortOnYPosition();
	void update(Uint32 timePassed);
	void draw(SDL_Surface * a_screen);
	bool setWorld(char * fileName);
	bool getSuccess() {return m_success;}
	int getNumEntities(){return m_mapOfEntities.size();}
	Entity * getEntity(int a_entity){return m_mapOfEntities.get(a_entity);}
};