#include "World.h"

World::World(){setWorld("Maps/HubWorldMap.txt");}

World::~World()
{
		for(int i = 0; i < m_mapOfWorld.getSize(); i++)
			delete m_mapOfWorld.get(i).currentTexture;
}

bool World::setWorld(char * fileName)
{
	FILE * infile;
	infile = fopen(fileName, "r");

	//Clear the previous map of the world, in order to create a better one.
	if(m_mapOfWorld.getSize() != 0)
		if(m_mapOfWorld.get(0).currentTexture->isSprite())
		{
			for(int i = 0; i < m_mapOfWorld.getSize(); i++)
				delete m_mapOfWorld.get(i).currentTexture;
		}
	m_mapOfWorld.release();

	//start the actual loading of the textures.
	if(infile == NULL)
		m_success = false;
	else
	{
		int c = 0, x = 0, y = 0;
		c = fgetc(infile);
		while(c != EOF)
		{
			Tile hi;
			switch(c)
			{
			case '\n':
				y++;
				x = 0;
				break;
			case 'G':
				hi.currentTexture = new SDL_Sprite("Sprites/grassTexture.bmp", 32, 32, 1, 1);
				hi.pos.x = x;
				hi.pos.y = y;
				x++;
				m_mapOfWorld.add(hi);
				break;
			}
			c = fgetc(infile);
		}
		m_success = true;
		fclose(infile);
	}
	return m_success;
}

void World::draw(SDL_Surface * a_screen)
{
	for(int k = 0; k < m_mapOfWorld.getSize(); k++)
		m_mapOfWorld.get(k).currentTexture->draw(a_screen, 32*m_mapOfWorld.get(k).pos.x, 32*m_mapOfWorld.get(k).pos.y);
	for(int i = 0; i < m_mapOfEntities.getSize(); i++)
		m_mapOfEntities.get(i)->draw(a_screen);
}

void World::update(Uint32 timePassed)
{
	for(int i = 0; i < m_mapOfEntities.getSize(); i++)
		m_mapOfEntities.get(i)->update(timePassed);
}