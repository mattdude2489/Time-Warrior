//World class. Contains the basic entities to easily update and draw to screen.
//Which of course means...SDL. Is necessary. GODDAMMIT.

//Created on March 10, 2011. Edited on March 10, 2011.

#include "Entity.h"

#pragma once

#define NUMBER_OF_ENTITIES 1

class World
{
private:
	Entity * m_mapOfEntities[NUMBER_OF_ENTITIES]; //TODO: MAGIC NUMBERS. Or maybe a vector list.
	int lowest;
public:
	World() {lowest = 0;}
	void draw(SDL_Surface * a_screen);
	void update(Uint32 timePassed);
	void setAnEntity(Entity * newEntity)
	{
		m_mapOfEntities[lowest] = newEntity;
		lowest++;
	}
};