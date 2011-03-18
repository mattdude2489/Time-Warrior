#include "World.h"

World::World()
{
	setWorld("Maps/HubWorldMap.txt");
}

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
	int c = 0;
	int x, y;
	x = y = 0;
	if(infile == NULL)
	{
		success = false;
		return success;
	}
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
			//hi.currentTexture->setHIndex(0, 1);
			hi.posX = x;
			hi.posY = y;
			x++;
			m_mapOfWorld.add(hi);
			break;
		}
		c = fgetc(infile);
	}
	success = true;
	fclose(infile);
	return success;
}

void World::draw(SDL_Surface *a_screen)
{
	for(int k = 0; k < m_mapOfWorld.getSize(); k++)
	{
		m_mapOfWorld.get(k).currentTexture->draw(a_screen, 32*m_mapOfWorld.get(k).posX, 32*m_mapOfWorld.get(k).posY);
	}
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