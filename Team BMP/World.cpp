#pragma once
#include "World.h"
#include <string>
using namespace std;
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
	for(int i = 0; i < m_mapOfWorld.size(); i++)
	{
		if(m_mapOfWorld.get(i).portal)
		{
			delete m_mapOfWorld.get(i).currentTexture;
			break;
		}
	}
}

int Tile::portalIndexNumber = 0; //I have to use global scope on this in order to use a static. That's just SAD.

bool World::setWorld(char * fileName)
{
	FILE * infile;
	fopen_s(&infile, fileName, "r");
	if(fileName == "Maps/HubWorldMap.txt")
		currentWorld = WORLD_HUB;
	if(fileName == "Maps/MedEngMap.txt")
		currentWorld = WORLD_ENGLAND;
	//Clear the previous map of the world, in order to create a better one.
	if(m_mapOfWorld.size() != 0)
	{
		if(m_mapOfWorld.get(0).currentTexture->isSprite())
		{
			delete m_mapOfWorld.get(0).currentTexture;
			for(int i = 0; i < m_mapOfWorld.size(); i++)
			{
				if(m_mapOfWorld.get(i).portal)
				{
					delete m_mapOfWorld.get(i).currentTexture; //Delete this texture ONLY ONCE. It gets recreated soon anyways.
					break;
				}
			}
		}
		m_mapOfWorld.release();
	}
	SDL_Sprite * sprite = new SDL_Sprite("Sprites/textureSetHub.bmp", FRAME_SIZE, FRAME_SIZE, FRAME_RATE, NUM_ROWS+2);
	SDL_Sprite * hudSprite = new SDL_Sprite("Sprites/hub.bmp", FRAME_SIZE, FRAME_SIZE, FRAME_RATE, NUM_ROWS+2);
	sprite->setTransparency(COLOR_TRANSPARENT);
	SDL_Sprite *portalSprite = new SDL_Sprite("Sprites/textureSetHub.bmp", FRAME_SIZE, FRAME_SIZE, FRAME_RATE, NUM_ROWS+2);
	portalSprite->setTransparency(COLOR_TRANSPARENT);
	Tile::portalIndexNumber = 0; //EPIC. THIS FARKING WORKS.
	//The sprite used for the portal.
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
				if(currentWorld == WORLD_HUB){
					hi.currentTexture = hudSprite;
				}
				else{
					hi.currentTexture = sprite;
				}
				hi.pos.x = x * hi.currentTexture->getWidth();
				hi.pos.y = y * hi.currentTexture->getHeight();
				hi.collide = hi.animate = false;
				hi.portal = false;
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
				if(currentWorld == WORLD_HUB){
					hi.indexOfSpriteRow = 5;
				}
				else{
				hi.indexOfSpriteRow = WORLD_HUB;
				}
				break;
			case 'D':
				if(currentWorld == WORLD_HUB){
					hi.indexOfSpriteRow = 0;
					hi.collide = true;
				}
				else{
				hi.indexOfSpriteRow = 1;
				}
				break;
			case 'H':
				if(currentWorld == WORLD_HUB){
					hi.indexOfSpriteRow = 2;
					hi.collide = true;
				}
				else{
				hi.indexOfSpriteRow = 2;
				}
				break;
			case 'V':
				if(currentWorld == WORLD_HUB){
					hi.indexOfSpriteRow = 1;
					hi.collide = true;
				}
				else{
				hi.indexOfSpriteRow = 3;
				}
				break;
			case 'M':
				if(currentWorld == WORLD_HUB){
					hi.indexOfSpriteRow = 3;
					hi.collide = true;
				}
				else{
				hi.indexOfSpriteRow = 4;
				}
				break;
			case 'B':
				if(currentWorld == WORLD_HUB){
					hi.indexOfSpriteRow = 4;
					hi.collide = true;
				}
				else{
				hi.indexOfSpriteRow = 5;
				hi.collide = true;
				}
				break;
			case 'P':
				if(currentWorld == WORLD_HUB){
				hi.indexOfSpriteRow = 5;
				}
				else{
					hi.indexOfSpriteRow = 0;
				}
				//m_mapOfWorld.add(hi);
				//Change the sprite to the Portal Sprite, which can be used to update.
				hi.currentTexture = portalSprite;
				hi.indexOfSpriteRow = 6;
				hi.animate = hi.portal = true;
				hi.portalIndexNumber++;
				break;
			}
			if(c != '\n')
			{
				m_mapOfWorld.add(hi);
			}
			if(hi.animate)
				hi.currentTexture->start();
			c = fgetc(infile);
		}
		m_success = true;
		fclose(infile);
	}
	maxWorldX = tileX * FRAME_SIZE;
	maxWorldY = tileY * FRAME_SIZE;
	setMonsters();
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
void World::sortOnYPosition()
{
	for(int i = 0; i < m_mapOfEntities.size(); ++i)
		m_mapOfEntities.get(i).sortOnYPosition();
	for(int z = 0; z < m_mapOfEntities.size(); z++)
	{
		Entity * successfulPlayer;
		if(m_mapOfEntities.get(z).getPlayer(successfulPlayer))
			clientPlayerIndex = z; //Which grid the Player's in. once we know that, we can just use getPlayer.
	}
}

/**
	The function that sets the world to have monsters and updates the Entity list of the grid to add them into.
**/
void World::setMonsters()
{
	for(int i = 0; i < m_mapOfEntities.size(); i++)
	{
		m_mapOfEntities.get(i).setMonsters(currentWorld, i, maxWorldX, maxWorldY);
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
				if(cE->getType() == PLAYER)
					printf("(%d,%d) %d->%d\n", cE->getLocation().x, cE->getLocation().y, z, getLocationGrid(cE));
			}
		}
	}
	//Update all entities.
	for(int i = 0; i < m_mapOfEntities.size(); i++)
		m_mapOfEntities.get(i).update(a_timePassed, this);
	for(int i = 0; i < m_mapOfWorld.size(); ++i)
	{
		if(m_mapOfWorld.get(i).animate)
		{
			m_mapOfWorld.get(i).currentTexture->update(a_timePassed);
			if(m_mapOfWorld.get(i).currentTexture->getFrame() > m_mapOfWorld.get(i).currentTexture->getMaxFrames()-1)
				m_mapOfWorld.get(i).currentTexture->restart(m_mapOfWorld.get(i).indexOfSpriteRow);
		}
	}
	bool successPlayer;
	Entity * player;
	successPlayer = m_mapOfEntities.get(clientPlayerIndex).getPlayer(player); //This pointer will be erased soon afterwards.
	//It's merely there to take away the typing and make it easier to read.
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
Tile * World::getTile(int a_x,int a_y)
{
	//if a_x or a_y are off the map, return 1st tile
	if(a_x < 0 || a_x > tileX * FRAME_SIZE
		|| a_y < 0 || a_y > tileY * FRAME_SIZE)
		return getTile(0);
	else
		return getTile(a_x/FRAME_SIZE + ((a_y/FRAME_SIZE)*tileX));
}
char * World::convertAllEntitiesToCharBuffer()//omg death
{
	string s;
	char buffer[10000];
	Entity * t_ent;
	for(int g = 0;g < NUM_GRIDS; g++)
	{
		if(this->getGrid(g)->getNumberOfEntities() > 0)//this is extra don't judge me =(
		{
			for(int e =0; e < this->getGrid(g)->getNumberOfEntities(); e++)
			{
				t_ent = this->getGrid(g)->getEntity(e);
				sprintf_s(buffer, "%02i%02i%04i%04i", g, e, t_ent->getLocation().getX(), t_ent->getLocation().getY());
				s.append(buffer);
			//	printf("buffer: %s\n",buffer);

			}
		}
	}
	strcpy_s(buffer, s.c_str());
	printf("size of info: %i\n", strlen(buffer));
	return buffer;
}
void World::convertFromServer(char * omgServerInfo)
{
	char buffer[12], cbuff[12];
	int b = 0, g = 0, e = 0, x = 0, y = 0;
	printf("server length: %i\n",strlen(omgServerInfo));

	for(int i = 0; i < strlen(omgServerInfo)-16; i++, b++)
	{			
		buffer[b] = omgServerInfo[i];
		if(b == 11)
		{
			printf("buffer: %s\n", buffer);
			sprintf_s(cbuff, "%c%c", buffer[0],buffer[1]);
			g = atoi(cbuff);
			sprintf_s(cbuff, "%c%c", buffer[2],buffer[3]);
			e = atoi(cbuff);
			sprintf_s(cbuff, "%c%c%c%c", buffer[4],buffer[5],buffer[6],buffer[7]);
			x = atoi(cbuff);
			sprintf_s(cbuff, "%c%c%c%c", buffer[8],buffer[9],buffer[10],buffer[11]);
			y = atoi(cbuff);
			getGrid(g)->getEntity(e)->setLocation(x,y);
			b = -1;//this has to be done or it will set to 0 then be ++ right away and set the first diget to 0 and it throws it off by one = no bueno
		}
	}
}