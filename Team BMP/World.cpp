#include "World.h"

World::World()
{
	FILE * infile;
	infile = fopen("Maps/HubWorldMap.txt", "r");
	int c = 0;
	int x, y;
	x = y = 0;
	if(infile == NULL)
	{
		success = false;
		return;
	}
	c = fgetc(infile);
	while(c != EOF)
	{
		Tile hi;
		hi.currentTexture = new Sprite("Sprites/grassTexture.bmp", 1, 0, 1);
		if(c == 'G')
		{
			hi.currentTexture->setHIndex(0, 1);
		}
		if(c == '\n')
		{
			y++;
			x = 0;
		}
		hi.posX = x;
		hi.posY = y;
		x++;
		m_mapOfWorld.add(hi);
		c = fgetc(infile);
	}
	fclose(infile);
}

World::~World()
{
		for(int i = 0; i < m_mapOfEntities.getSize(); i++)
			delete m_mapOfWorld.get(i).currentTexture;
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