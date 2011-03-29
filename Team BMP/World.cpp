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
			//Initializes ALL of the tiles. All of them. Dear god that's a lot of memory.
			if(c != '\n')
			{
				hi.currentTexture = sprite;
				hi.pos.x = x * hi.currentTexture->getWidth();
				hi.pos.y = y * hi.currentTexture->getHeight();
				hi.collide = false;
				x++;
			}
			else
			{
				y++;
				tileX = x;
				x = 0;
			}
			tileY = y+1;
			//"Anything else in particular" switch
			//Creations of entities & any particulars of the map
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
				hi.indexOfSpriteRow = 5;
				hi.collide = true;
				break;
			}
			if(c != '\n')
				m_mapOfWorld.add(hi);
			c = fgetc(infile);
		}
		m_success = true;
		fclose(infile);
	}
	return m_success;
}
//sets up a camera for each entity & tile, so they correctly move relative to the player
void World::setCamera(SPoint * a_camera)
{
	for(int i = 0; i < NUM_GRIDS; ++i)
	{
		for(int k = 0; k < m_mapOfEntities.get(i).getNumberOfEntities(); k++)
			m_mapOfEntities.get(i).getEntity(k)->setCamera(a_camera);
	}
	for(int i = 0; i < m_mapOfWorld.size(); ++i)
		m_mapOfWorld.get(i).cam = a_camera;
}
Tile * World::getTile(int a_x,int a_y)
{
	int tileIndex = a_x/SPRITE_SIZE + ((a_y/SPRITE_SIZE)*tileX);//28 is the number of tiles per row. Switch to #define next chance.
	printf("index: %d, row: %d\n", tileIndex, m_mapOfWorld.get(tileIndex).indexOfSpriteRow);
	return &m_mapOfWorld.get(tileIndex);
}
void World::sortOnYPosition()
{
	for(int i = 0; i < m_mapOfEntities.size(); ++i)
		m_mapOfEntities.get(i).sortOnYPosition();
	for(int z = 0; z < m_mapOfEntities.size(); z++)
	{
		bool successfulPlayer;
		if(m_mapOfEntities.get(z).getPlayer(successfulPlayer)->getType() == PLAYER)
			clientPlayerIndex = z; //Which grid the Player's in. once we know that, we can just use getPlayer.
	}
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
			cE = m_mapOfEntities.get(z).getEntity(i);
			if(getLocationGrid(cE) != z)
			{
				m_mapOfEntities.get(getLocationGrid(cE)).setEntity(cE);
				m_mapOfEntities.get(z).remove(i);
			}
		}
	}

	for(int i = 0; i < m_mapOfEntities.size(); i++)
		m_mapOfEntities.get(i).update(a_timePassed);
	bool successPlayer;
	Entity * cp = m_mapOfEntities.get(clientPlayerIndex).getPlayer(successPlayer); //This pointer will be erased soon afterwards.
	//It's merely there to take away the typing and make it easier to read.
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
		m_mapOfEntities.get(i).sortOnYPosition();
}
void World::draw(SDL_Surface * a_screen)
{
	//Entities draw.
	for(int i = 0; i < m_mapOfWorld.size(); ++i)
	{
		m_mapOfWorld.get(i).currentTexture->setRIndex(m_mapOfWorld.get(i).indexOfSpriteRow);
		m_mapOfWorld.get(i).currentTexture->draw(a_screen, m_mapOfWorld.get(i).getLocationScreen().x, m_mapOfWorld.get(i).getLocationScreen().y);
	}
	for(int i = 0; i < m_mapOfEntities.size(); ++i)
		m_mapOfEntities.get(i).draw(a_screen); //Why does it seem like the Entities are getting further and further away from direct access?
}
int World::getNumEntities()
{
	int currentCount = 0;
	for(int i = 0; i < m_mapOfEntities.size(); i++)
		currentCount += getNumEntities(i);
	return currentCount;
}