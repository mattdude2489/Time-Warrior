#pragma once
#include "World.h"
#include "Boss.h"
#include <string>
using namespace std;
#include "NPC.h"
#include "Magic.h"

//#define NPC_ADD

World::World()
{
	clientPlayerIndex = 0; 
	maxWorldX = SCREEN_WIDTH;
	maxWorldY = SCREEN_HEIGHT;
	//Setting up the 16 grids. The number can easily be changed.
	for(int i = 0; i < NUM_GRIDS; i++)
	{
		Grid gridSys;
		m_mapOfEntities.add(gridSys);	
	}
	m_sprites[SLIME].setSprite("Sprites/slime.bmp", FRAME_SIZE-1, 23, FRAME_RATE, NUM_ROWS);
	m_sprites[SKELETON].setSprite("Sprites/skeleton.bmp", 24, FRAME_SIZE, FRAME_RATE, NUM_ROWS);
	m_sprites[GHOST].setSprite("Sprites/ghost.bmp", FRAME_SIZE, FRAME_SIZE, FRAME_RATE, NUM_ROWS);
	m_sprites[BOSS1].setSprite("Sprites/demon0.bmp", FRAME_SIZE, FRAME_SIZE, FRAME_RATE, NUM_ROWS);
	for(int i =0; i < 3; i++)
	{
		m_sprites[i].setTransparency(COLOR_TRANSPARENT);
	}
	m_success = setWorld("Maps/HubWorldMap.txt");

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
	if(fileName == "Maps/Dungeon1.txt")
		currentWorld = WORLD_D1;
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
	SDL_Sprite * dungeon = new SDL_Sprite("Sprites/Dungeon1.bmp",FRAME_SIZE, FRAME_SIZE, FRAME_RATE, NUM_ROWS+2);
	dungeon->setTransparency(COLOR_TRANSPARENT); //SO MANY MEMORY LEAKS!?
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
				switch(currentWorld)
				{
				case WORLD_HUB: hi.currentTexture = hudSprite;break;
				case WORLD_ENGLAND:hi.currentTexture = sprite;break;
				case WORLD_D1:hi.currentTexture = dungeon;break;
				}

				hi.pos.x = x * hi.currentTexture->getWidth();
				hi.pos.y = y * hi.currentTexture->getHeight();
				hi.collide = hi.animate = hi.portal = hi.dungeon = hi.spawnLocation = hi.bossLoc= false;
				x++;
			}
			else
			{
				y++;
				tileX = x;
				x = 0;
			}
			tileY = y+1;
			int r_tile = 0;
			//"Anything else in particular" switch
			//Creations of entities & any particulars of the map
			switch(c)
			{
			case 'G':
				if(currentWorld == WORLD_HUB){
					hi.indexOfSpriteRow = 5;
				}
				else if(currentWorld == WORLD_ENGLAND){
					hi.indexOfSpriteRow = 0;
				}
				else if(currentWorld == WORLD_D1){
					r_tile = rand()%2;
					hi.indexOfSpriteRow = r_tile;
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
				else if(currentWorld == WORLD_ENGLAND){
				hi.indexOfSpriteRow = 5;
				hi.collide = true;
				}
				else if(currentWorld == WORLD_D1){
					r_tile = (rand()%3) + 2;
					hi.indexOfSpriteRow = r_tile;
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
			case 'd':
				hi.currentTexture = dungeon;
				hi.indexOfSpriteRow = 6;
				hi.animate = hi.dungeon = true;
				break;
			case'S':
					if(currentWorld == WORLD_D1){
						r_tile = rand()%2;
						hi.indexOfSpriteRow = r_tile;
						hi.spawnLocation = true;
					 }
					break;
			case 'b':
				if(currentWorld == WORLD_D1){
						r_tile = rand()%2;
						hi.indexOfSpriteRow = r_tile;
						hi.bossLoc = true;
				}
				break;

			default:
				hi.currentTexture = dungeon;
				hi.indexOfSpriteRow = 5; 
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
	//take into account that the outmost tiles are borders for entity containment,
	//so the valid movable world space is decreased by up/left/down/right tiles
	maxWorldX = (tileX-2) * FRAME_SIZE;
	maxWorldY = (tileY-2) * FRAME_SIZE;
	setMonsters();
#ifdef NPC_ADD
	setNPC();
#endif
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
	The function that calls of the NPC's based on the current world, gets rid of the previous ones, and sets new ones.
**/
void World::setNPC()
{
	//If there are already NPC's in the World, remove them and clean up the memory.
	for(int i = 0; i < m_mapOfEntities.size(); i++)
	{
		for(int k = 0; k < m_mapOfEntities.get(i).getNumberOfEntities(); k++)
		{
			if(m_mapOfEntities.get(i).getEntity(k)->getType() == NPC &&
				m_mapOfEntities.get(i).getEntity(k)->getNewed())
			{
				delete m_mapOfEntities.get(i).getEntity(k);
				m_mapOfEntities.get(i).remove(k);
			}
		}
	}
	//Now to open the file *, and abuse the atoi system.
	FILE * infile;
	fopen_s(&infile, "Maps/NPC Placements.txt", "r");
	int c = 0, x = 0, y = 0;
	c = fgetc(infile);
	c -= 48; //48 is the range between 0 ASCII and NULL.
	//This system may work for the first few things...but then it'll blow up.
	while(c != currentWorld)
	{
		while(c != '#')
			c = fgetc(infile);
		fscanf_s(infile,"%i",&x);
		fscanf_s(infile,"%i",&y);
		c = fgetc(infile); //Check to make sure that it's on the right world again. DANGEROUS OF INFINITE LOOP.
		c = fgetc(infile);
		c -= 48;
	}

	char * charpoint = " ";
	//char * buff; //I ONLY NEED ONE CHARACTER...but I needed a *...and it wouldn't let me just use char *.
	string s;
	c = fgetc(infile);
	c = fgetc(infile); //This SHOULD make it get an integer.
	int forLoopNum = c-48;
	for(int i = 0; i < forLoopNum; i++)
	{
		while(c != '#')
		{
			c = fgetc(infile);
			if(c == '\n') //IGNORING.
				c = fgetc(infile);
//			charpoint = (char)c;
			//itoa(c, charpoint, 10); //Convert it to a char*.
			char buff = (char)c;
			s.append(1, buff);
		}
		fscanf_s(infile, "%i", &x);
		fscanf_s(infile, "%i", &y);
		//charpoint = &s;
		//strcpy_s(charpoint, strlen(s.c_str()) ,s.c_str());
		SDL_Sprite * newSprite;
		newSprite = new SDL_Sprite("Sprites/greenguy.bmp", FRAME_SIZE, FRAME_SIZE, FRAME_RATE, NUM_ROWS);
		NonPlayerChar * newNPC;
		const char * buf = s.c_str();
		newNPC = new NonPlayerChar(const_cast<char*>(buf), newSprite);
		//newNPC->initSprite(newSprite); //Inits the entity first
		//newNPC->initNPC(charpoint); //Then inits the NPC portions.
		newNPC->setNewed(true);
		newNPC->setLocation(x, y);
		add(newNPC);
	}
	fclose(infile);
	/*SDL_Sprite * sdlsprite = new SDL_Sprite("Sprites/greenguy.bmp", FRAME_SIZE, FRAME_SIZE, FRAME_RATE, NUM_ROWS);
	NonPlayerChar * nonPlayer = new NonPlayerChar("Hi. Go through the portal NAO.", sdlsprite);
	nonPlayer->setNewed(true);
	add(nonPlayer);*/
}

/**
	The function that sets the world to have monsters and updates the Entity list of the grid to add them into.
**/
void World::setMonsters()
{
	int numMinions = 0;
	if(currentWorld == WORLD_ENGLAND){
		for(int i = 0; i < m_mapOfEntities.size(); i++)
		{
			m_mapOfEntities.get(i).setMonsters(currentWorld, i, maxWorldX, maxWorldY);
		}
	}
	else if(currentWorld == WORLD_D1){
		for(int g = 0; g< m_mapOfEntities.size(); g++)
		{
			m_mapOfEntities.get(g).clearAllEntities();
		}
		for(int i = 0; i < m_mapOfWorld.size(); i++)
		{
			SDL_Sprite * sprite;
			if(m_mapOfWorld.get(i).spawnLocation)
			{
				numMinions = (rand()%3)+3;
				for(int m = 0; m < numMinions; m++)
				{
					int spriteSheet = 0;
					spriteSheet = rand()%NUM_SPRITE_SHEETS_TO_CHOOSE_FROM;
					switch(spriteSheet)
					{
					case 0:
						sprite = new SDL_Sprite("Sprites/slime.bmp", FRAME_SIZE-1, 23, FRAME_RATE, NUM_ROWS);
						break;
					case 1:
						sprite = new SDL_Sprite("Sprites/skeleton.bmp", 24, FRAME_SIZE, FRAME_RATE, NUM_ROWS);
						break;
					case 2:
						sprite = new SDL_Sprite("Sprites/ghost.bmp", FRAME_SIZE, FRAME_SIZE, FRAME_RATE, NUM_ROWS);
						break;
					}
					Minion * newEntity = new Minion(100, 100, 3, 2, 5, 0, 0, 0, sprite);
					newEntity->setNewed(true);
					newEntity->setLocation(m_mapOfWorld.get(i).pos);
					this->add(newEntity);
				}
			}
			if(m_mapOfWorld.get(i).bossLoc)
			{
				sprite = new SDL_Sprite("Sprites/demon0.bmp", FRAME_SIZE, FRAME_SIZE, FRAME_RATE, NUM_ROWS);
				Boss * newBoss = new Boss(200,200,10,10,10,0,0,0,sprite);
				newBoss->setNewed(true);
				newBoss->setLocation(m_mapOfWorld.get(i).pos);
				this->add(newBoss);
				newBoss->setChip(FIRE, BASIC, this);
			}
		}
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