#include "World.h"

World::World(){m_success = setWorld("Maps/HubWorldMap.txt");}

World::~World()
{
		/*for(int i = 0; i < m_mapOfWorld.size(); i++)
			delete m_mapOfWorld.get(i).currentTexture;*/
	//because ALL of the tiles only hold ONE texture, it only needs to be deleted ONCE.
	delete m_mapOfWorld.get(0).currentTexture;// Hooray for trying to delete memory that's ALREADY GONE!?
}

bool World::setWorld(char * fileName)
{
	FILE * infile;
	fopen_s(&infile, fileName, "r");

	//Clear the previous map of the world, in order to create a better one.
	if(m_mapOfWorld.size() != 0)
		if(m_mapOfWorld.get(0).currentTexture->isSprite())
		{
			for(int i = 0; i < m_mapOfWorld.size(); i++)
				delete m_mapOfWorld.get(i).currentTexture;
		}
	m_mapOfWorld.release();
	//Screw it. I'm gonna do this Java style.
//	SDL_Sprite * sprite("Sprites/textureSetHub.bmp", 32, 32, 1, 5);
	SDL_Sprite * sprite = new SDL_Sprite("Sprites/textureSetHub.bmp", 32, 32, 1, 5);
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
			//Initial setup switch. Initializes ALL of the tiles. All of them. Dear god that's a lot of memory.
			switch(c)
			{
			case '\n':
				y++;
				x = 0;
				break;
			case 'G':
			case 'D':
			case 'H':
			case 'V':
			case 'M':
				hi.currentTexture = sprite;
				hi.pos.x = x;
				hi.pos.y = y;
				x++;
				break;
			}

			//"Anything else in particular" switch. So in other words, creations of entities, any particulars of the map.
			//Will happen in this switch statement.
			switch(c)
			{
			case 'G':
				hi.indexOfSpriteRow = 0;
				break;
			case 'D':
				hi.indexOfSpriteRow = 1;
				break;
			case 'H':
				hi.indexOfSpriteRow = 2;
				break;
			case 'V':
				hi.indexOfSpriteRow = 3;
				break;
			case 'M':
				hi.indexOfSpriteRow = 4;
				break;
			}
			m_mapOfWorld.add(hi);
			c = fgetc(infile);
		}
		m_success = true;
		fclose(infile);
	}
	return m_success;
}

void World::draw(SDL_Surface * a_screen)
{
	//Texture draw.
	for(int k = 0; k < m_mapOfWorld.size(); k++)
	{
		m_mapOfWorld.get(k).currentTexture->setRIndex(m_mapOfWorld.get(k).indexOfSpriteRow);
		m_mapOfWorld.get(k).currentTexture->draw(a_screen, 32*m_mapOfWorld.get(k).pos.x, 32*m_mapOfWorld.get(k).pos.y);
	}
	//Entities draw.
	for(int i = 0; i < m_mapOfEntities.size(); i++)
	{
		m_mapOfEntities.get(i)->draw(a_screen);
	}
}

void World::update(Uint32 a_timePassed)
{
	for(int i = 0; i < m_mapOfEntities.size(); i++)
		m_mapOfEntities.get(i)->update(a_timePassed);
	//WARNING: EXTREMELY CPU TAXING PROCESS AHEAD.
	sortOnYPosition();
}

void World::sortOnYPosition()
{
	//Selection sort, using the Y position. Dear god. Let's hope it doesn't slow it down too much.
	for(int i = 0; i < m_mapOfEntities.size(); ++i)
	{
		for(int k = i; k < m_mapOfEntities.size(); ++k)
		{
			if(m_mapOfEntities.get(i)->getLocationScreen().y > m_mapOfEntities.get(k)->getLocationScreen().y)
			{
				m_mapOfEntities.swap(i, k);
			}
		}
	}
}
