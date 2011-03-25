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
	SPoint pos;
	int indexOfSpriteRow;
};

class World
{
private:
	TemplateVector2<Entity*> m_mapOfEntities;
	TemplateVector2<Tile> m_mapOfWorld;
	bool m_success;
	int clientPlayerIndex;
public:
	World();
	~World();
	void draw(SDL_Surface * a_screen);
	void update(Uint32 timePassed);
	void setAnEntity(Entity * newEntity){m_mapOfEntities.add(newEntity);}
	bool getSuccess() {return m_success;}
	bool setWorld(char * fileName);
	int getNumEntities(){return m_mapOfEntities.size();}
	Entity * getEntity(int a_entity){return m_mapOfEntities.get(a_entity);}
	void sortOnYPosition();
};