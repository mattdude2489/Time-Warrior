#pragma once
#include "World.h"
#include "Boss.h"
#include <string>
using namespace std;
#include "NPC.h"
#include "Magic.h"
#include "Obstacle.h"

#define NPC_ADD

World::World()
{
	clientPlayerIndex = 0; 
	maxWorldX = m_cCamera.w = SCREEN_WIDTH;
	maxWorldY = m_cCamera.h = SCREEN_HEIGHT;
	//Setting up the 16 grids. The number can easily be changed.
	for(int i = 0; i < NUM_GRIDS; i++)
	{
		Grid gridSys;
		m_mapOfEntities.add(gridSys);
	}
	for(int i =0; i < NUM_SPRITES; i++)
	{
		m_sprites[i].fileName = NULL;
		m_sprites[i].frameWidth = m_sprites[i].frameHeight = FRAME_SIZE;
		m_sprites[i].animSpeed = FRAME_RATE;
		m_sprites[i].rows = NUM_ROWS;
	}
	m_sprites[SLIME].fileName = "Sprites/slime.bmp";
	m_sprites[SLIME].frameWidth -= 1;
	m_sprites[SLIME].frameHeight -= 9;
	m_sprites[SKELETON].fileName = "Sprites/skeleton.bmp";
	m_sprites[SKELETON].frameWidth -= 8;
	m_sprites[GHOST].fileName = "Sprites/ghost.bmp";
	m_sprites[BOSS1].fileName = "Sprites/demon0.bmp";
	m_sprites[NPC1].fileName = "Sprites/greenguy.bmp";
	m_sprites[KNIGHT0].fileName = "Sprites/knight0.bmp";
	m_sprites[KNIGHT1].fileName = "Sprites/knight1.bmp";
	

	m_worldSprites[SINGLE] = new SDL_Sprite("Sprites/world_single.bmp", FRAME_SIZE, FRAME_SIZE, FRAME_RATE, NUM_WORLD_TILE_S);
	m_worldSprites[ANIMATION] = new SDL_Sprite("Sprites/world_animate.bmp", FRAME_SIZE, FRAME_SIZE, FRAME_RATE, NUM_WORLD_TILE_S);
	m_worldSprites[TREE_SPRITE] = new SDL_Sprite("Sprites/Tree.bmp", TREE_WIDTH, TREE_HEIGHT, FRAME_RATE, TREE_COUNT);
	m_worldSprites[ROCK_SHRUB] = new SDL_Sprite("Sprites/RocksandShrubs.bmp", FRAME_SIZE,FRAME_SIZE, FRAME_RATE, NUM_ROCK_SHRUB); 
	for(int i = 0; i < NUM_SPRITES_WORLD; ++i)
		m_worldSprites[i]->setTransparency(COLOR_TRANSPARENT);
	m_worldSprites[ANIMATION]->setLoopToBegin(true);
	m_worldSprites[ANIMATION]->start();

	m_success = setWorld("Maps/HubWorldMap.txt");
	m_closed = NULL;
	bossCount = 0;
	castleCount = 0;
}
World::~World()
{
	for(int i = 0; i < NUM_SPRITES_WORLD; ++i)
			delete m_worldSprites[i];
	delete m_closed;
}

int Tile::portalIndexNumber = 0; //I have to use global scope on this in order to use a static. That's just SAD.

bool World::setWorld(char * fileName)
{
	
	FILE * infile;
	fopen_s(&infile, fileName, "r");


	if(fileName == "Maps/HubWorldMap.txt")
		currentWorld = WORLD_HUB;
	else if(fileName == "Maps/MedEngMap.txt")
		currentWorld = WORLD_ENGLAND;
	else if(fileName == "Maps/Dungeon1.txt" || fileName == "Maps/Dungeon0.txt" ||fileName == "Maps/Dungeon2.txt" )
		currentWorld = WORLD_D1;
	else if(strcmp(fileName ,"Maps/Castle0.txt")||strcmp(fileName, "Maps/Castle1.txt"))
		currentWorld = WORLD_CASTLE;

		
	//Clear the previous map of the world, in order to create a better one.
	if(m_mapOfWorld.size() != 0)
		m_mapOfWorld.release();

	Tile::portalIndexNumber = 0; //EPIC. THIS FARKING WORKS.
	//start the actual loading of the textures.

	//for dungeon count
	int dcount = 0;
	bossCount = 0;

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
				hi.currentTexture = m_worldSprites[SINGLE];
				hi.pos.x = x * hi.currentTexture->getWidth();
				hi.pos.y = y * hi.currentTexture->getHeight();
				hi.collide = hi.portal = hi.dungeon = hi.spawnLocation = hi.bossLoc = hi.playerSpawn = hi.tree = hi.door = hi.stairs = hi.ddoor = hi.fdoor = false;
				x++;
				hi.collideBox.x = hi.pos.x;
				hi.collideBox.y = hi.pos.y;
				hi.collideBox.w = hi.currentTexture->getWidth();
				hi.collideBox.h = hi.currentTexture->getHeight();
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
			case 'P':
			case 'd':
			case 'p':
			case 'S':
			case 'b':
			case 'T':
				switch(currentWorld)
				{
				case WORLD_HUB:		hi.indexOfSpriteRow = TILE_METAL_QUAD;			break;
				case WORLD_ENGLAND:	hi.indexOfSpriteRow = TILE_GRASS;				break;
				case WORLD_D1:		hi.indexOfSpriteRow = rand()%2 + TILE_DIRT1;	break;
				
				}
				switch(c)
				{
				case 'P':	hi.portal = true;	hi.portalIndexNumber++;			break;
				case 'd':	hi.dungeon = true;	dcount++;						break;
				case 'p':	hi.playerSpawn = true;								break;
				case 'S':	hi.spawnLocation = true;							break;
				case 'b':	hi.bossLoc = true;									break;
				case 'T':	hi.tree = true;										break;
				}
				break;
			case 'D':
				switch(currentWorld)
				{
				case WORLD_HUB:		hi.indexOfSpriteRow = TILE_METAL_L;	hi.collide = true;	break;
				default:			hi.indexOfSpriteRow = TILE_DUST;
				}
				break;
			case 'H':
				switch(currentWorld)
				{
				case WORLD_HUB:		hi.indexOfSpriteRow = TILE_METAL_R;	hi.collide = true;	break;
				default:			hi.indexOfSpriteRow = TILE_DUST_D;
				}
				break;
			case 'V':
				switch(currentWorld)
				{
				case WORLD_HUB:		hi.indexOfSpriteRow = TILE_METAL_QUAD_L;	hi.collide = true;	break;
				default:			hi.indexOfSpriteRow = TILE_DUST_R;
				}
				break;
			case 'M':
				switch(currentWorld)
				{
				case WORLD_HUB:		hi.indexOfSpriteRow = TILE_METAL_QUAD_R;	hi.collide = true;	break;
				default:			hi.indexOfSpriteRow = TILE_DUST_CORNER;
				}
				break;
			case 'B':
				switch(currentWorld)
				{
				case WORLD_HUB:		hi.indexOfSpriteRow = TILE_METAL;				break;
				case WORLD_ENGLAND:	hi.indexOfSpriteRow = TILE_BLANK;				break;
				case WORLD_D1:		hi.indexOfSpriteRow = rand()%3 + TILE_SPIKE_LG;	break;
				}
				hi.collide = true;
				break;
			case 'W':
				hi.currentTexture = m_worldSprites[ANIMATION];
				hi.indexOfSpriteRow = TILE_WATER;
				hi.collide = true;
				break;
			case 'U':
				hi.currentTexture = m_worldSprites[ANIMATION];
				hi.indexOfSpriteRow = TILE_WATER_D;
				break;
			case 'u':
				hi.currentTexture = m_worldSprites[ANIMATION];
				hi.indexOfSpriteRow = TILE_WATER_U;
				break;
			case 'L':
				hi.currentTexture = m_worldSprites[ANIMATION];
				hi.indexOfSpriteRow = TILE_WATER_R;
				break;
			case 'l':
				hi.currentTexture = m_worldSprites[ANIMATION];
				hi.indexOfSpriteRow = TILE_WATER_L;
				break;
			case '+':
				hi.currentTexture = m_worldSprites[ANIMATION];
				hi.indexOfSpriteRow = TILE_BRIDGE_V;
				break;
			case '=':
				hi.currentTexture = m_worldSprites[ANIMATION];
				hi.indexOfSpriteRow = TILE_BRIDGE_H;
				break;
			case 'C':
				hi.indexOfSpriteRow = TILE_CASTLE_GROUND;
				break;
			case '|':
				hi.indexOfSpriteRow = TILE_CASTLE_WALL_V;
				hi.collide = true;
				break;
			case '-':
				hi.indexOfSpriteRow = TILE_CASTLE_WALL_H;
				hi.collide = true;
				break;
			case '[':
				hi.indexOfSpriteRow = TILE_CASTLE_DOOR_L;
				hi.door = true;
				break;
			case ']':
				hi.indexOfSpriteRow = TILE_CASTLE_DOOR_R;
				hi.door = true;
				break;
			case '^':
				hi.indexOfSpriteRow = TILE_CASTLE_STAIRS;
				hi.stairs = true;
				break;
			case 'Q':
				hi.indexOfSpriteRow = DESERT_WALL;
				hi.collide = true;
				break;
			case '{':
				hi.indexOfSpriteRow = DESERT_ARCH_L;
				hi.ddoor = true;
				break;
			case '}':
				hi.indexOfSpriteRow = DESERT_ARCH_R;
				hi.ddoor = true;
				break;
			case '<':
				hi.indexOfSpriteRow = FOREST_ARCH_L;
				hi.fdoor = true;
				break;
			case '>':
				hi.indexOfSpriteRow = FOREST_ARCH_R;
				hi.fdoor = true;
				break;
			case '@':
				hi.currentTexture = m_worldSprites[ROCK_SHRUB];
				hi.indexOfSpriteRow = rand()%2;
				hi.collide = true;
				break;
			case '#':
				hi.currentTexture = m_worldSprites[ROCK_SHRUB];
				hi.indexOfSpriteRow = BUSH;
				hi.collide = true;
				break;
			case '$':
				hi.currentTexture = m_worldSprites[ROCK_SHRUB];
				hi.indexOfSpriteRow = CACTUS;
				hi.collide = true;
				break;
			default:
				hi.indexOfSpriteRow = TILE_BLANK;
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
	maxWorldX = tileX * FRAME_SIZE;
	maxWorldY = tileY * FRAME_SIZE;
	//this should only happen once if the world has never been created
	if(!m_closed)
	{
		m_closed = new bool[dcount];
		for(int i = 0; i < dcount; i++)//set them all to not be closed at first
			m_closed[i] = false;
	}
	//resize the grids
	int x = 0, y = 0, w = 0, h = 0;
	w = getGridWidth();
	h = getGridHeight();
	for(int i = 0; i < m_mapOfEntities.size(); i++)
	{
		m_mapOfEntities.get(i).setLoc(x*w, y*h, h, w);
		x++;
		if(x == NUM_GRIDS_PER_ROW_COL)
		{
			y++;
			x = 0;
		}
	}
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
				m_mapOfEntities.get(i).getEntity(k)->getFlag(FLAG_NUDE))
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
	//c = fgetc(infile);
//	if(currentWorld != 0)
		fscanf_s(infile, "%i", &c);
	/*else
	{
		c = fgetc(infile);
		c -= 48;
	}*/
	//c -= 48; //48 is the range between 0 ASCII and NULL.
	//This system may work for the first few things...but then it'll blow up.
	while(c != currentWorld)
	{
		while(c != '#')
		{
			c = fgetc(infile);
			if(c == EOF) //Because this area is dangerous of becoming an infinite loop, give some escape code.
			{
				fclose(infile);
				return;
			}
		}
		fscanf_s(infile,"%i",&x); //The two integers.
		fscanf_s(infile,"%i",&y); //The two integers, x and y value.
		c = fgetc(infile); //Check to make sure that it's on the right world again. DANGEROUS OF INFINITE LOOP.
		//c = fgetc(infile);
		fscanf_s(infile, "%i", &c);
	//	c -= 48;
		if(c == EOF)
		{
			fclose(infile);
			return;
		}
	}
	char * charpoint = " ";
	//char * buff; //I ONLY NEED ONE CHARACTER...but I needed a *...and it wouldn't let me just use char *.
	//c = fgetc(infile);
	c = fgetc(infile); //This SHOULD make it get an integer.
	fscanf_s(infile, "%i", &c);
	int forLoopNum = c;
	for(int i = 0; i < forLoopNum; i++)
	{
		string s;
		c = fgetc(infile);
		while(c != '#')
		{
			if(c == '\n') //IGNORING.
				c = fgetc(infile);

			if(c == 92)
			{
				c = fgetc(infile);
				if(c == 'n')
				{
					c = 10; //The \n character.
				}
			}
//			charpoint = (char)c;
			//itoa(c, charpoint, 10); //Convert it to a char*.
			char buff = (char)c;
			s.append(1, buff);
			c = fgetc(infile);
		}
		fscanf_s(infile, "%i", &x);
		fscanf_s(infile, "%i", &y);
		//charpoint = &s;
		//strcpy_s(charpoint, strlen(s.c_str()) ,s.c_str());
		const char * buf = s.c_str();
		SDL_Sprite * newSprite = new SDL_Sprite(m_sprites[NPC1].fileName, m_sprites[NPC1].frameWidth, m_sprites[NPC1].frameHeight, m_sprites[NPC1].animSpeed, m_sprites[NPC1].rows);
		NonPlayerChar * newNPC = new NonPlayerChar(const_cast<char*>(buf), newSprite);
		newNPC->setNewed(true);
		newNPC->setLocation(x, y);
		add(newNPC);
		c = fgetc(infile);
	}
	fclose(infile);
}

/**
	The function that sets the world to have monsters and updates the Entity list of the grid to add them into.
**/
void World::setMonsters()
{
	int numMinions = 0;
	for(int i = 0; i < m_mapOfEntities.size(); i++)
	{
		for(int g = 0; g < m_mapOfEntities.size(); g++)
			m_mapOfEntities.get(g).clearAllEntities();
	}
	int dcount = 0;
	for(int i = 0; i < m_mapOfWorld.size(); i++)
	{
		SDL_Sprite * sprite = NULL;
		if(m_mapOfWorld.get(i).spawnLocation)
		{
			numMinions = (rand()%3)+1;
			for(int m = 0; m < numMinions; m++)
			{
				int spriteSheet = 0;
				e_material mtrl;
				spriteSheet = rand()%(NUM_SPRITE_SHEETS_TO_CHOOSE_FROM-1);
				switch(currentWorld)
				{
				case WORLD_ENGLAND:
					sprite = new SDL_Sprite(m_sprites[SLIME].fileName, m_sprites[SLIME].frameWidth, m_sprites[SLIME].frameHeight, m_sprites[SLIME].animSpeed, m_sprites[SLIME].rows);
					mtrl = MTRL_RUBBER;
					break;
				case WORLD_D1:
					switch(spriteSheet)
					{
					case 0:
						sprite = new SDL_Sprite(m_sprites[SKELETON].fileName, m_sprites[SKELETON].frameWidth, m_sprites[SKELETON].frameHeight, m_sprites[SKELETON].animSpeed, m_sprites[SKELETON].rows);
						mtrl = MTRL_DARK;
						break;
					case 1:
						sprite = new SDL_Sprite(m_sprites[GHOST].fileName, m_sprites[GHOST].frameWidth, m_sprites[GHOST].frameHeight, m_sprites[GHOST].animSpeed, m_sprites[GHOST].rows);
						mtrl = MTRL_DARK;
						break;
					}
					break;
				case WORLD_CASTLE:
					switch(spriteSheet)
					{
					case 0:
						sprite = new SDL_Sprite(m_sprites[KNIGHT0].fileName, m_sprites[KNIGHT0].frameWidth, m_sprites[KNIGHT0].frameHeight, m_sprites[KNIGHT0].animSpeed, m_sprites[KNIGHT0].rows);
						mtrl = MTRL_METAL;
						break;
					case 1:
						sprite = new SDL_Sprite(m_sprites[KNIGHT1].fileName, m_sprites[KNIGHT1].frameWidth, m_sprites[KNIGHT1].frameHeight, m_sprites[KNIGHT1].animSpeed, m_sprites[KNIGHT1].rows);
						mtrl = MTRL_METAL;
						break;
					}
					break;
				}
				Minion * newEntity = new Minion(sprite);
				newEntity->setNewed(true);
				newEntity->setMaterial(mtrl);
				newEntity->scaleToPlayer(m_player);
				newEntity->setLocation(m_mapOfWorld.get(i).pos);
				this->add(newEntity);
			}
		}
		if(m_mapOfWorld.get(i).bossLoc)
		{
			bossCount++;
			sprite = new SDL_Sprite(m_sprites[BOSS1].fileName, m_sprites[BOSS1].frameWidth, m_sprites[BOSS1].frameHeight, m_sprites[BOSS1].animSpeed, m_sprites[BOSS1].rows);
			Boss * newBoss = new Boss(sprite);
			newBoss->setMaterial(MTRL_FIRE);
			newBoss->setNewed(true);
			newBoss->scaleToPlayer(m_player);
			newBoss->setBossLoc(m_mapOfWorld.get(i).pos);
			this->add(newBoss);
			newBoss->setChip(FIRE, BASIC, this);
		}
		if(m_mapOfWorld.get(i).playerSpawn)
		{
			if(m_player->isLastWSet()&&currentWorld == WORLD_ENGLAND)
			{
				m_player->setCurrentLocToLast(this);
			}
			else{
				m_player->setLocation(m_mapOfWorld.get(i).pos);
			}
		}
		if(m_mapOfWorld.get(i).portal || m_mapOfWorld.get(i).dungeon)
		{
			bool check = false;
			if(m_closed && dcount)
				check = m_closed[dcount];
			if(check)
				sprite = new SDL_Sprite("Sprites/world_single.bmp", FRAME_SIZE, FRAME_SIZE, FRAME_RATE, NUM_WORLD_TILE_S);
			else
				sprite = new SDL_Sprite("Sprites/world_animate.bmp", FRAME_SIZE, FRAME_SIZE, FRAME_RATE, NUM_WORLD_TILE_S);
			Obstacle * obs = new Obstacle(sprite);
			obs->setNewed(true);
			obs->setIndex(dcount);
			dcount++;
			if(m_mapOfWorld.get(i).portal)
			{
				sprite->setRIndex(TILE_PORTAL);
				obs->setObstacleType(PORTAL);
			}
			else
			{
				if(check)
					sprite->setRIndex(TILE_DUNGEON_CLOSED);
				else
					sprite->setRIndex(TILE_DUNGEON);
				obs->setObstacleType(DUNGEON);
			}
			obs->setLocation(m_mapOfWorld.get(i).pos);
			this->add(obs);
		}
		if(m_mapOfWorld.get(i).tree)
		{
			sprite = new SDL_Sprite("Sprites/Tree.bmp", TREE_WIDTH, TREE_HEIGHT, FRAME_RATE, TREE_COUNT);
			Obstacle * obs = new Obstacle(sprite);
			sprite->setRIndex(rand()%3);
			obs->setObstacleType(TREE);
			obs->setNewed(true);
			obs->setLocation(m_mapOfWorld.get(i).pos);
			this->add(obs);
		}
		
	}
}

void World::update(Uint32 a_timePassed)
{
	//Making sure that the entities are all in their correct grids.
	Entity * cE = NULL;
	for(int z = 0; z < m_mapOfEntities.size(); z++)
	{
		for(int i = 0; i < m_mapOfEntities.get(z).getNumberOfEntities(); i++)
		{
			cE = m_mapOfEntities.get(z).getEntity(i);
			if(getLocationGrid(cE) != ERROR_CODE)
			{
				if(getLocationGrid(cE) != z && cE->getFlag(FLAG_DRAW))
				{
					m_mapOfEntities.get(getLocationGrid(cE)).setEntity(cE);
					m_mapOfEntities.get(z).remove(i);
				}
			}
		}
	}
	
	//Update entities based on where the current Camera is.
	for(int i = 0; i < m_mapOfEntities.size(); i++)
	{
		if(m_cCamera.intersects(m_mapOfEntities.get(i).getLoc()))
			m_mapOfEntities.get(i).update(a_timePassed, this);
	}
	for(int i = 0; i < m_mapOfEntities.size(); i++)
	{
		if(m_cCamera.intersects(m_mapOfEntities.get(i).getLoc()))
			m_mapOfEntities.get(i).clearDeadEntities(this);
	}
	m_worldSprites[ANIMATION]->update(a_timePassed);
	//WARNING: EXTREMELY CPU TAXING PROCESS AHEAD.
	//Make sure for each grid's sorting.
	for(int i = 0; i < m_mapOfEntities.size(); ++i)
		m_mapOfEntities.get(i).sortOnYPosition();
	m_cCamera.setPosition(*m_player->getCamera());
	//printf("plyr pos (%d,%d), plyr cam (%d,%d), world cam (%d,%d)\n", m_player->getLocation().x, m_player->getLocation().y, m_player->getCamera()->x, m_player->getCamera()->y, m_cCamera.getPosition().x, m_cCamera.getPosition().y);
}
void World::draw(SDL_Surface * a_screen)
{
	//Entities draw.
	for(int i = 0; i < m_mapOfWorld.size(); ++i)
	{
		if(m_cCamera.intersects(m_mapOfWorld.get(i).collideBox))
		{
			m_mapOfWorld.get(i).currentTexture->setRIndex(m_mapOfWorld.get(i).indexOfSpriteRow);
			m_mapOfWorld.get(i).currentTexture->draw(a_screen, m_mapOfWorld.get(i).getLocationScreen().x, m_mapOfWorld.get(i).getLocationScreen().y);
		}
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