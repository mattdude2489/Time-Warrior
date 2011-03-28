#include "World.h"

World::World()
{
	m_success = setWorld("Maps/HubWorldMap.txt"); 
	clientPlayerIndex = 0; 
	maxWorldX = SCREEN_WIDTH;
	maxWorldY = SCREEN_HEIGHT;
	//Setting up the 16 grids. The number can easily be changed.
	for(int i = 0; i < NUM_GRIDS; i++)
	{
		Grid gridSys;
		m_mapOfEntities.add(gridSys);	
	}
}

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
	if(fileName == "Maps/HubWorldMap.txt")
		currentWorld = 0;
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
void World::setCamera(SPoint * a_camera)
{
	//Dammit people, COMMENT.
	for(int i = 0; i < NUM_GRIDS; ++i)
	{
		for(int k = 0; k < m_mapOfEntities.get(i).getNumberOfEntities(); k++)
			m_mapOfEntities.get(i).getEntityAt(k)->setCamera(a_camera);
	}
	for(int i = 0; i < m_mapOfWorld.size(); ++i)
		m_mapOfWorld.get(i).cam = a_camera;
}
void World::sortOnYPosition()
{
	//Selection sort, using the Y position. Dear god. Let's hope it doesn't slow it down too much.
	/*for(int i = 0; i < m_mapOfEntities.size(); ++i)
	{
		for(int k = i; k < m_mapOfEntities.size(); ++k)
		{
			if(m_mapOfEntities.get(i)->getLocation().y > m_mapOfEntities.get(k)->getLocation().y)
			{
				m_mapOfEntities.swap(i, k);
			}
		}
	}*/
	for(int i = 0; i < m_mapOfEntities.size(); ++i)
		m_mapOfEntities.get(i).sortOnYPosition();
	for(int z = 0; z < m_mapOfEntities.size(); z++)
	{
		bool successfulPlayer;
		if(m_mapOfEntities.get(z).getPlayer(successfulPlayer)->getType() == 1)
			clientPlayerIndex = z; //Which grid the Player's in. once we know that, we can just use getPlayer.
	}
}
void World::add(Entity *newEntity)
{
	int a_x = (int)newEntity->getLocation().x / (int)maxWorldX/NUM_GRIDS_PER_ROW_COL;
	int a_y = (int)newEntity->getLocation().y / (int)maxWorldY/NUM_GRIDS_PER_ROW_COL;
	int gridXY = a_x + (4 * a_y);
	m_mapOfEntities.get(gridXY).setEntity(newEntity);
}

//Gets the entity in the grid, based on the x and y values passed in.
Entity * World::getEntity(int a_entity, int a_x, int a_y)
{
	a_x /= (int)(maxWorldX/NUM_GRIDS_PER_ROW_COL);
	a_y /= (int)(maxWorldY/NUM_GRIDS_PER_ROW_COL);
	return m_mapOfEntities.get(a_x + (NUM_GRIDS_PER_ROW_COL * a_y)).getEntityAt(a_entity);
}

int World::getNumEntities()
{
	int currentCount = 0;
	for(int i = 0; i < m_mapOfEntities.size(); i++)
	{
		currentCount += m_mapOfEntities.get(i).getNumberOfEntities();
	}
	return currentCount;
}

Grid * World::getGrid(int a_x, int a_y)
{
	int gridValueX = (int) a_x / (int)(maxWorldX/NUM_GRIDS_PER_ROW_COL);
	int gridValueY = (int) a_y / (int)(maxWorldY/NUM_GRIDS_PER_ROW_COL);
	int gridValue = gridValueX + (4 * gridValueY);
	return &m_mapOfEntities.get(gridValue);
}

void World::update(Uint32 a_timePassed)
{
	//static SPoint prevLoc = m_mapOfEntities.get(clientPlayerIndex)->getLocation();
	//Making sure that the entities are all in their correct grids.
	Entity * cE = NULL;
	for(int z = 0; z < m_mapOfEntities.size(); z++)
	{
		for(int i = 0; i < m_mapOfEntities.get(z).getNumberOfEntities(); i++)
		{
			cE = m_mapOfEntities.get(z).getEntityAt(i);
			int gridValueX = (int)cE->getLocation().x / (int)(maxWorldX/NUM_GRIDS_PER_ROW_COL);
			int gridValueY = (int)cE->getLocation().y / (int)(maxWorldY/NUM_GRIDS_PER_ROW_COL);
			int gridValue = gridValueX + (4 * gridValueY);
			if(gridValue != z)
			{
				printf("(%d, %d) = %d,%d\n", cE->getLocation().x, cE->getLocation().y, z, gridValue);
				m_mapOfEntities.get(gridValue).setEntity(cE);
				m_mapOfEntities.get(z).remove(i);
			}
		}
	}

	for(int i = 0; i < m_mapOfEntities.size(); i++)
		m_mapOfEntities.get(i).update(a_timePassed);
	//bool successPlayer;
	//Entity * cp = m_mapOfEntities.get(clientPlayerIndex).getPlayer(successPlayer); //This pointer will be erased soon afterwards.
	//It's merely there to take away the typing and make it easier to read.
	//static SPoint prevLoc = cp->getLocation();
	/*for(int i = 0; i < m_mapOfWorld.size(); i++)
	{
		if(((m_mapOfEntities.get(clientPlayerIndex)->getLocationWorld().x == m_mapOfWorld.get(i).pos.x) && 
			(m_mapOfEntities.get(clientPlayerIndex)->getLocationWorld().y == m_mapOfWorld.get(i).pos.y)) && 
			m_mapOfWorld.get(i).collide)
			m_mapOfEntities.get(clientPlayerIndex)->setLocation(0, prevLoc.x, prevLoc.y);
		if(m_mapOfEntities.get(clientPlayerIndex)->getSprite()->rectCollide(
			m_mapOfEntities.get(clientPlayerIndex)->getLocationWorld().x,
			m_mapOfEntities.get(clientPlayerIndex)->getLocationWorld().y,
			*m_mapOfWorld.get(i).currentTexture,
			m_mapOfWorld.get(i).pos.x,
			m_mapOfWorld.get(i).pos.y) && m_mapOfWorld.get(i).collide)
			m_mapOfEntities.get(clientPlayerIndex)->setLocation(1, prevLoc.x, prevLoc.y);
	}*/
	//prevLoc = cp->getLocation();
	//WARNING: EXTREMELY CPU TAXING PROCESS AHEAD.
	//Make sure for each grid's sorting.
	for(int i = 0; i < m_mapOfEntities.size(); ++i)
	{
		m_mapOfEntities.get(i).sortOnYPosition();
	}
}
void World::draw(SDL_Surface * a_screen)
{
	////Selection sort, using the Y position. Dear god. Let's hope it doesn't slow it down too much.
	//for(int i = 0; i < m_mapOfEntities.size(); ++i)
	//{
	//	for(int k = i; k < m_mapOfEntities.size(); ++k)
	//	{
	//		if(m_mapOfEntities.get(i)->getLocation().y > m_mapOfEntities.get(k)->getLocation().y)
	//		{
	//			m_mapOfEntities.swap(i, k);
	//		}
	//	}
	//}
	//Entities draw.
	for(int i = 0; i < m_mapOfWorld.size(); ++i)
	{
		m_mapOfWorld.get(i).currentTexture->setRIndex(m_mapOfWorld.get(i).indexOfSpriteRow);
		m_mapOfWorld.get(i).currentTexture->draw(a_screen, m_mapOfWorld.get(i).getLocationScreen().x, m_mapOfWorld.get(i).getLocationScreen().y);
	}
	for(int i = 0; i < m_mapOfEntities.size(); ++i)
	{
		m_mapOfEntities.get(i).draw(a_screen); //Why does it seem like the Entities are getting further and further away from direct access?
	}
}