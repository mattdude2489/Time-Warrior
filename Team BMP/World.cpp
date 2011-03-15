#include "World.h"

void World::draw(SDL_Surface *a_screen)
{
	for(int i = 0; i < m_mapOfEntities.getSize(); i++)
	{
		m_mapOfEntities.get(i)->draw(a_screen);
	}
}

void World::update(Uint32 timePassed)
{
	for(int i = 0; i < m_mapOfEntities.getSize(); i++)
	{
		m_mapOfEntities.get(i)->update(timePassed);
	}
}