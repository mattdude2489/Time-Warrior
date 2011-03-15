//World class. Contains the basic entities to easily update and draw to screen.
//Which of course means...SDL. Is necessary. GODDAMMIT.
//Created on March 10, 2011. Edited on March 10, 2011.
#pragma once
#include "Entity.h"
#include "templateVector.h"

#define NUMBER_OF_ENTITIES 2

class World
{
private:
	//Entity * m_mapOfEntities[NUMBER_OF_ENTITIES]; //TODO: MAGIC NUMBERS. Or maybe a vector list.
	templateVector<Entity*> m_mapOfEntities;
public:
	World() {}
	void draw(SDL_Surface * a_screen);
	void update(Uint32 timePassed);
	void setAnEntity(Entity * newEntity)
	{
		m_mapOfEntities.add(newEntity);
	}
};