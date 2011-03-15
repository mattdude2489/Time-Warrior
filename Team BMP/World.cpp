#include "World.h"

World::World()
{
	Tile hi;
	hi.currentTexture = new Sprite("Sprites/grassTexture.bmp", 1, 32, 1);
	m_mapOfWorld.add(hi);
}

World::~World()
{
	delete m_mapOfWorld.get(0).currentTexture;	
}
void World::draw(SDL_Surface *a_screen)
{
	m_mapOfWorld.get(0).currentTexture->draw(a_screen, 200, 200);
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