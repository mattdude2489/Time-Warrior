#include "World.h"

World::World(){m_success = setWorld("Maps/HubWorldMap.txt"); clientPlayerIndex = 0;}

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
	SDL_Sprite * sprite = new SDL_Sprite("Sprites/textureSetHub.bmp", SPRITE_SIZE, SPRITE_SIZE, SPRITE_SPEED, SPRITE_ROWS+1);
	sprite->setTransparency(COLOR_TRANSPARENT);
	//start the actual loading of the textures.
	if(infile == NULL)
		m_success = false;
	else
	{
		int c = 0, x = 0, y = 0;
		c = fgetc(infile);
		Tile hi;
		while(c != EOF)
		{
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
			case 'B':
				hi.currentTexture = sprite;
				hi.pos.x = x*32;
				hi.pos.y = y*32;
				x++;
				hi.collide = false;
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
			case 'B':
				hi.collide = true;
				hi.indexOfSpriteRow = 5;
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
		m_mapOfWorld.get(k).currentTexture->draw(a_screen, m_mapOfWorld.get(k).pos.x, m_mapOfWorld.get(k).pos.y);
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

	
	static SPoint prevLoc = m_mapOfEntities.get(clientPlayerIndex)->getLocation();
	
	//for(int i = 0; i < m_mapOfWorld.size(); i++)
	//{
	//	//if(((m_mapOfEntities.get(clientPlayerIndex)->getLocationWorld().x == m_mapOfWorld.get(i).pos.x) && 
	//	//	(m_mapOfEntities.get(clientPlayerIndex)->getLocationWorld().y == m_mapOfWorld.get(i).pos.y)) && 
	//	//	m_mapOfWorld.get(i).collide)
	//	//	m_mapOfEntities.get(clientPlayerIndex)->setLocation(0, prevLoc.x, prevLoc.y);
	//	if(m_mapOfEntities.get(clientPlayerIndex)->getSprite()->rectCollide(
	//		m_mapOfEntities.get(clientPlayerIndex)->getLocationWorld().x,
	//		m_mapOfEntities.get(clientPlayerIndex)->getLocationWorld().y,
	//		*m_mapOfWorld.get(i).currentTexture,
	//		m_mapOfWorld.get(i).pos.x,
	//		m_mapOfWorld.get(i).pos.y) && m_mapOfWorld.get(i).collide)

	//		m_mapOfEntities.get(clientPlayerIndex)->setLocation(1, prevLoc.x, prevLoc.y);
	//}

	/*for(int i = 0; i < m_mapOfEntities.size(); i++)
	{
		if(i != clientPlayerIndex)
			m_mapOfEntities.get(i)->move(LOC_SCREEN,-1*(m_mapOfEntities.get(clientPlayerIndex)->getLocationWorld().x - prevLoc.x), -1*(m_mapOfEntities.get(clientPlayerIndex)->getLocationWorld().y - prevLoc.y));
	}*/
	for(int i = 0; i < m_mapOfWorld.size(); i++)
	{
		m_mapOfWorld.get(i).pos.set(m_mapOfWorld.get(i).pos.x - (m_mapOfEntities.get(clientPlayerIndex)->getLocation().x - prevLoc.x), m_mapOfWorld.get(i).pos.y - (m_mapOfEntities.get(clientPlayerIndex)->getLocation().y - prevLoc.y));
	}
	prevLoc = m_mapOfEntities.get(clientPlayerIndex)->getLocation();
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
			if(m_mapOfEntities.get(i)->getLocation().y > m_mapOfEntities.get(k)->getLocation().y)
			{
				m_mapOfEntities.swap(i, k);
			}
		}
	}
	for(int z = 0; z < m_mapOfEntities.size(); ++z)
	{
		if(m_mapOfEntities.get(z)->getType() == 1)
		{
			clientPlayerIndex = z;
			break;
		}
	}
}
