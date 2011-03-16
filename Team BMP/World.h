//World class. Contains the basic entities to easily update and draw to screen.
//Which of course means...SDL. Is necessary. GODDAMMIT.
//Created on March 10, 2011. Edited on March 15, 2011.
#pragma once
#include "Entity.h"
#include "templateVector.h"

#include <stdio.h>

struct Tile
{
	Sprite* currentTexture;
	bool collide;
	int posX, posY;
};

class World
{
private:
	//Entity * m_mapOfEntities[NUMBER_OF_ENTITIES]; //TODO: MAGIC NUMBERS. Or maybe a vector list.
	templateVector<Entity*> m_mapOfEntities;
	templateVector<Tile> m_mapOfWorld;
	bool success;
public:
	World();
	~World();
	void draw(SDL_Surface * a_screen);
	void update(Uint32 timePassed);
	void setAnEntity(Entity * newEntity)
	{
		m_mapOfEntities.add(newEntity);
	}
	bool getSuccess() {return success;}
};