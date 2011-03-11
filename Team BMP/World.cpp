#include "World.h"

void World::draw(SDL_Surface *a_screen)
{
	for(int i = 0; i < lowest; i++)
	{
		m_mapOfEntities[i]->draw(a_screen);
	}
}

void World::update(Uint32 timePassed)
{
	for(int i = 0; i < lowest; i++)
	{
		m_mapOfEntities[i]->update(timePassed);
	}
}