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
	initWorld();
}

void World::initWorld()
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

	m_sprites[SLIME].setSprite("Sprites/slime.bmp", FRAME_SIZE -1, FRAME_SIZE -9, FRAME_RATE, NUM_ROWS);
	m_sprites[SKELETON].setSprite("Sprites/skeleton.bmp", FRAME_SIZE -8, FRAME_SIZE, FRAME_RATE, NUM_ROWS);
	m_sprites[GHOST].setSprite("Sprites/ghost.bmp", FRAME_SIZE , FRAME_SIZE, FRAME_RATE, NUM_ROWS);
	m_sprites[BOSS0].setSprite("Sprites/demon0.bmp", FRAME_SIZE , FRAME_SIZE, FRAME_RATE, NUM_ROWS);
	m_sprites[BOSS1].setSprite("Sprites/demon1.bmp", FRAME_SIZE , FRAME_SIZE, FRAME_RATE, NUM_ROWS);
	m_sprites[DRAGON].setSprite("Sprites/dragon.bmp", DRAGON_FRAME, DRAGON_FRAME, FRAME_RATE, NUM_ROWS);
	m_sprites[NPC1].setSprite("Sprites/greenguy.bmp", FRAME_SIZE , FRAME_SIZE, FRAME_RATE, NUM_ROWS);
	m_sprites[KNIGHT0].setSprite("Sprites/knight0.bmp", FRAME_SIZE , FRAME_SIZE, FRAME_RATE, NUM_ROWS);
	m_sprites[KNIGHT1].setSprite("Sprites/knight1.bmp", FRAME_SIZE , FRAME_SIZE, FRAME_RATE, NUM_ROWS);
	m_sprites[KNIGHT2].setSprite("Sprites/knight2.bmp", FRAME_SIZE , FRAME_SIZE, FRAME_RATE, NUM_ROWS);
	m_sprites[SHADOWDEMON].setSprite("Sprites/ShadowDemon.bmp", P_WIDTH, P_HEIGHT, FRAME_RATE, NUM_ROWS);
	m_sprites[DEMONKING].setSprite("Sprites/DemonKing.bmp", FRAME_SIZE, FRAME_SIZE, FRAME_RATE, NUM_ROWS);
	for(int i = 0; i < NUM_SPRITES; i++)
		m_sprites[i].setTransparency(COLOR_TRANSPARENT);
	for(int i = 0; i < NUM_SPRITES_WORLD; ++i)
		m_worldSprites[i] = NULL;
	fopen_s(&npc_loadNPCFile, "Maps/NPC Placements.txt", "r");
	fadeAmount = 2;
	m_success = setWorld("Maps/HubWorldMap.txt");
	bossCount = 0;
	castleCount = 0;
	for(int i = 0;  i < 8; i++)
	{	
		m_closed[i] = false;
	}
}

World::~World()
{
	destroyWorld();
}

void World::destroyWorld()
{
	for(int i = 0; i < NUM_SPRITES_WORLD; ++i)
	{
		if(m_worldSprites[i])
			delete m_worldSprites[i];
	}
	//delete m_closed;
	m_success = false; //Close off the world.
	if(npc_loadNPCFile)
		fclose(npc_loadNPCFile);
}

int Tile::portalIndexNumber = 0; //I have to use global scope on this in order to use a static. That's just SAD.

bool World::setWorld(char * fileName)
{
	m_worldTime = time(0);
	m_localTime = localtime(&m_worldTime);
	FILE * infile;
	fopen_s(&infile, fileName, "r");

	if(fileName == "Maps/HubWorldMap.txt")
		currentWorld = WORLD_HUB;
	else if(fileName == "Maps/MedEngMap.txt")
		currentWorld = WORLD_ENGLAND;
	else if(fileName == "Maps/Dungeon1.txt" || fileName == "Maps/Dungeon0.txt" ||fileName == "Maps/Dungeon2.txt" )
		currentWorld = WORLD_D1;
	else if(fileName == "Maps/Forest.txt")
		currentWorld = WORLD_FOREST;
	else if(fileName == "Maps/Desert.txt")
		currentWorld = WORLD_DESERT;
	else if(fileName == "Maps/Building2.txt")
		currentWorld = WORLD_TOWN2;
	else if(strcmp(fileName ,"Maps/Castle0.txt")||strcmp(fileName, "Maps/Castle1.txt"))
		currentWorld = WORLD_CASTLE;

	//re-create sprites so dark->light process is correct (& so only certain worlds fade)
		//keep in mind that all worlds use the same sprite sheet,
		//so as soon as one world fades it, all other worlds will also be faded
	for(int i = 0; i < NUM_SPRITES_WORLD; ++i)
	{
		if(m_worldSprites[i])
			delete m_worldSprites[i];
	}
	m_worldSprites[SINGLE] = new SDL_Sprite("Sprites/world_single.bmp", FRAME_SIZE, FRAME_SIZE, FRAME_RATE, NUM_WORLD_TILE_S);
	m_worldSprites[ANIMATION] = new SDL_Sprite("Sprites/world_animate.bmp", FRAME_SIZE, FRAME_SIZE, FRAME_RATE, NUM_WORLD_TILE_A);
	m_worldSprites[TREE_SPRITE] = new SDL_Sprite("Sprites/Tree.bmp", TREE_WIDTH, TREE_HEIGHT, FRAME_RATE, TREE_COUNT);
	m_worldSprites[ROCK_SHRUB] = new SDL_Sprite("Sprites/RocksandShrubs.bmp", FRAME_SIZE,FRAME_SIZE, FRAME_RATE, NUM_ROCK_SHRUB); 
	m_worldSprites[BUILDINGS] = new SDL_Sprite("Sprites/Buildings.bmp", BUILDING_W, BUILDING_H, FRAME_RATE, NUM_ROWS);
	for(int i = 0; i < NUM_SPRITES_WORLD; ++i)
		m_worldSprites[i]->setTransparency(COLOR_TRANSPARENT);
	m_worldSprites[ANIMATION]->setLoopToBegin(true);
	m_worldSprites[ANIMATION]->start();
	switch(currentWorld)
	{
	case WORLD_ENGLAND:
	case WORLD_FOREST:
	case WORLD_DESERT:
		fadeWorld();
		break;
	}
		
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
				hi.collide = hi.portal = hi.dungeon = hi.spawnLocation = hi.bossLoc = hi.npcLoc = hi.playerSpawn = hi.tree = hi.door = hi.stairs = hi.ddoor = hi.fdoor = hi.buildingh = hi.buildingv = hi.buildDoor = false;
				hi.npcNum = 0;
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
			int tempInt = 0;
			//"Anything else in particular" switch
			//Creations of entities & any particulars of the map
			switch(c)
			{
			case 'G'://moved it to seperat one cause of the desert area
				switch(currentWorld)
				{
				case WORLD_HUB:		hi.indexOfSpriteRow = TILE_METAL_QUAD;			break;
				case WORLD_ENGLAND:
				case WORLD_DESERT:
				case WORLD_FOREST:	hi.indexOfSpriteRow = TILE_GRASS;				break;
				case WORLD_D1:		hi.indexOfSpriteRow = rand()%2 + TILE_DIRT1;	break;
				case WORLD_TOWN2:	hi.indexOfSpriteRow = TILE_BUILDING_GROUND;		break;
				}
				break;
			case 'P':
			case 'd':
			case 'p':
			case 'S':
			case 'b':
			case 'T':
			case 'e':
			case 'E':
				switch(currentWorld)
				{
				case WORLD_HUB:		hi.indexOfSpriteRow = TILE_METAL_QUAD;			break;
				case WORLD_ENGLAND:
				case WORLD_FOREST:	hi.indexOfSpriteRow = TILE_GRASS;				break;
				case WORLD_DESERT:	hi.indexOfSpriteRow = TILE_DUST;				break;
				case WORLD_D1:		hi.indexOfSpriteRow = rand()%2 + TILE_DIRT1;	break;
				case WORLD_TOWN2:	hi.indexOfSpriteRow = TILE_BUILDING_GROUND;		break;
				}
				switch(c)
				{
				case 'P':	hi.portal = true;	hi.portalIndexNumber++;			break;
				case 'd':	hi.dungeon = true;	dcount++;						break;
				case 'p':	hi.playerSpawn = true;								break;
				case 'S':	hi.spawnLocation = true;							break;
				case 'b':	hi.bossLoc = true;									break;
				case 'T':	hi.tree = true;										break;
				case 'e':	hi.buildingh = true;								break;
				case 'E':	hi.buildingv = true;								break;
				}
				break;
			case 'D':
				switch(currentWorld)
				{
				case WORLD_HUB:		hi.indexOfSpriteRow = TILE_METAL_L;	hi.collide = true;			break;
				case WORLD_TOWN2:	hi.indexOfSpriteRow = TILE_BUILDING_DOOR; hi.buildDoor = true;	break;
				default:			hi.indexOfSpriteRow = TILE_DUST;
				}
				break;
			case 'H':
				switch(currentWorld)
				{
				case WORLD_HUB:		hi.indexOfSpriteRow = TILE_METAL_R;	hi.collide = true;	break;
				}
				break;
			case 'V':
				switch(currentWorld)
				{
				case WORLD_HUB:		hi.indexOfSpriteRow = TILE_METAL_QUAD_L;	hi.collide = true;	break;
				default:			hi.indexOfSpriteRow = TILE_ROAD;
				}
				break;
			case 'n':
				hi.indexOfSpriteRow = TILE_ROAD_TR;			break;
			case 'N':
				hi.indexOfSpriteRow = TILE_ROAD_TL;			break;
			case 'o':
				hi.indexOfSpriteRow = TILE_ROAD_BR;			break;
			case 'O':
				hi.indexOfSpriteRow = TILE_ROAD_BL;			break;
			case 'M':
				switch(currentWorld)
				{
				case WORLD_HUB:		hi.indexOfSpriteRow = TILE_METAL_QUAD_R;	hi.collide = true;	break;
				}
				break;
			case 'B':
				switch(currentWorld)
				{
				case WORLD_HUB:		hi.indexOfSpriteRow = TILE_METAL;							break;
				case WORLD_ENGLAND:	hi.indexOfSpriteRow = TILE_BLANK;							break;
				case WORLD_D1:		hi.indexOfSpriteRow = rand()%3 + TILE_SPIKE_LG;				break;
				case WORLD_TOWN2:	hi.indexOfSpriteRow = TILE_BUILDING_WOOD;hi.collide = true;	break;
				
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
			case ';':
				hi.currentTexture = m_worldSprites[ANIMATION];
				switch(currentWorld)
				{
				case WORLD_ENGLAND:		hi.indexOfSpriteRow = TILE_WATER_ANGLE_TR;		break;
				case WORLD_D1:			hi.indexOfSpriteRow = TILE_WATER_ANGLE_DTR;		break;
				}
				break;
			case '/':
				hi.currentTexture = m_worldSprites[ANIMATION];
				switch(currentWorld)
				{
				case WORLD_ENGLAND:		hi.indexOfSpriteRow = TILE_WATER_ANGLE_TL;		break;
				case WORLD_D1:			hi.indexOfSpriteRow = TILE_WATER_ANGLE_DTL;		break;
				}
				break;
				
			case ')':
				hi.currentTexture = m_worldSprites[ANIMATION];
				switch(currentWorld)
				{
				case WORLD_ENGLAND:		hi.indexOfSpriteRow = TILE_WATER_ANGLE_BR;		break;
				case WORLD_D1:			hi.indexOfSpriteRow = TILE_WATER_ANGLE_DBR;		break;
				}
				break;
			case '(':
				hi.currentTexture = m_worldSprites[ANIMATION];
				switch(currentWorld)
				{
				case WORLD_ENGLAND:		hi.indexOfSpriteRow = TILE_WATER_ANGLE_BL;		break;
				case WORLD_D1:			hi.indexOfSpriteRow = TILE_WATER_ANGLE_DBL;		break;
				}
				break;
			case 'y':
				hi.currentTexture = m_worldSprites[ANIMATION];
				switch(currentWorld)
				{
				case WORLD_ENGLAND:		hi.indexOfSpriteRow = TILE_WATER_CORNER_TR;		break;
				case WORLD_D1:			hi.indexOfSpriteRow = TILE_WATER_CORNER_DTR;	break;
				}
				hi.collide = true;
				break;
			case 'Y':
				hi.currentTexture = m_worldSprites[ANIMATION];
				switch(currentWorld)
				{
				case WORLD_ENGLAND:		hi.indexOfSpriteRow = TILE_WATER_CORNER_TL;		break;
				case WORLD_D1:			hi.indexOfSpriteRow = TILE_WATER_CORNER_DTL;	break;
				}
				hi.collide = true;
				break;
				
			case 'z':
				hi.currentTexture = m_worldSprites[ANIMATION];
				switch(currentWorld)
				{
				case WORLD_ENGLAND:		hi.indexOfSpriteRow = TILE_WATER_CORNER_BR;		break;
				case WORLD_D1:			hi.indexOfSpriteRow = TILE_WATER_CORNER_DBR;	break;
				}
				hi.collide = true;
				break;
				
			case 'Z':
				hi.currentTexture = m_worldSprites[ANIMATION];
				switch(currentWorld)
				{
				case WORLD_ENGLAND:		hi.indexOfSpriteRow = TILE_WATER_CORNER_BL;		break;
				case WORLD_D1:			hi.indexOfSpriteRow = TILE_WATER_CORNER_DBL;	break;
				}
				hi.collide = true;
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
				hi.indexOfSpriteRow = rand()%2+ROCK_BIG;
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
			case 'a':
				hi.indexOfSpriteRow = TILE_TOWN_WALL_H;
				hi.collide = true;
				break;
			case 'A':
				hi.indexOfSpriteRow = TILE_TOWN_WALL_V;
				hi.collide = true;
				break;
			case 'c':
				hi.indexOfSpriteRow = TILE_TOWN_DOOR;
				break;
				//the barrier tiles
			case 'i':
				if(m_player->getKeyLevel() > BARRIER_NONE)
					hi.indexOfSpriteRow = TILE_GRASS;
				else
				{
					hi.currentTexture = m_worldSprites[ANIMATION];
					hi.indexOfSpriteRow = TILE_BARRIER_RED;
					hi.collide = true;
				}
				break;
			case 'I':
				if(m_player->getKeyLevel() > BARRIER_NONE)
					hi.indexOfSpriteRow = TILE_GRASS;
				else
				{
					hi.currentTexture = m_worldSprites[ANIMATION];
					hi.indexOfSpriteRow = TILE_BARRIER_GREEN;
					hi.collide = true;
				}
				break;
			case 'j':
				if(m_player->getKeyLevel() > BARRIER_NONE)
					hi.indexOfSpriteRow = TILE_GRASS;
				else
				{
					hi.currentTexture = m_worldSprites[ANIMATION];
					hi.indexOfSpriteRow = TILE_BARRIER_YELLOW;
					hi.collide = true;
				}
				break;
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
				switch(currentWorld)
				{
				case WORLD_ENGLAND:		tempInt = TILE_GRASS;			break;
				case WORLD_DESERT:		tempInt = TILE_DUST;			break;
				case WORLD_HUB:			tempInt = TILE_METAL_QUAD;		break;
				case WORLD_CASTLE:		tempInt	= TILE_CASTLE_GROUND;	break;
				case WORLD_TOWN2:		tempInt = TILE_BUILDING_GROUND;	break;
				}
				hi.indexOfSpriteRow = tempInt;
				hi.npcLoc = true;
				hi.npcNum = c-48;
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
	for(int i = 0; i < m_mapOfEntities.size(); i++)
		{
			for(int k = 0; k < m_mapOfEntities.get(i).getNumberOfEntities(); k++)
			{
				switch(m_mapOfEntities.get(i).getEntity(k)->getType())
				{
				case PLAYER:	printf("Entity type is: PLAYER\n");	break;
				case NPC:		printf("Entity type is: NPC\n");
					//printf("Entity type is: %d\n", m_mapOfEntities.get(i).getEntity(k)->getType());
				break;
				}
			}
		}
		m_success = true;
		fclose(infile);
	}
	maxWorldX = tileX * FRAME_SIZE;
	maxWorldY = tileY * FRAME_SIZE;
	
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
	//SetNPC was here.
	if(npc_loadNPCFile)
		fclose(npc_loadNPCFile);
	printf("World made\n");
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
	The function that destroys all of the set NPC's. Call at the beginning of SetWorld.
**/
void World::destroyNPC()
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
}
/**
	The function that calls of the NPC's based on the current world, gets rid of the previous ones, and sets new ones.
**/
void World::setNPC(int cWorld, int NPCToGet , int npcX, int npcY)
{
	
	//Now to open the file *, and abuse the atoi system.
	int c = 1, x = 0, y = 0;
	//c = fgetc(infile);
//	if(currentWorld != 0)
	//Current World
	if(npc_loadNPCFile == NULL)
		fopen_s(&npc_loadNPCFile, "NPC Placements.txt", "r"); //To RESET the stream pointer.
	fscanf_s(npc_loadNPCFile, "%i", &c);
	while(c != currentWorld)
	{
		while(c != '#')
		{
			c = fgetc(npc_loadNPCFile);
			if(c == EOF) //Because this area is dangerous of becoming an infinite loop, give some escape code.
			{
				//fclose(npc_loadNPCFile);
				return;
			}
		}
		//fscanf_s(npc_loadNPCFile,"%i",&x); //The two integers.
	//	fscanf_s(npc_loadNPCFile,"%i",&y); //The two integers, x and y value.
		c = fgetc(npc_loadNPCFile); //Check to make sure that it's on the right world again. DANGEROUS OF INFINITE LOOP.
		//c = fgetc(infile);
		fscanf_s(npc_loadNPCFile, "%i", &c);
	//	c -= 48;
		if(c == EOF)
		{
			fclose(npc_loadNPCFile);
			return;
		}
	}
	char * charpoint = " ";
	//char * buff; //I ONLY NEED ONE CHARACTER...but I needed a *...and it wouldn't let me just use char *.
	//c = fgetc(infile);
	c = fgetc(npc_loadNPCFile); //This SHOULD make it get an integer.
	//fscanf_s(npc_loadNPCFile, "%i", &c);
	int forLoopNum = NPCToGet;
	for(int i = 0; i <= forLoopNum; i++)
	{
		string s;
		c = fgetc(npc_loadNPCFile);
		while(c != '#')
		{
			if(c == '\n') //IGNORING.
				c = fgetc(npc_loadNPCFile);

			if(c == 92)
			{
				c = fgetc(npc_loadNPCFile);
				if(c == 'n')
				{
					c = 10; //The \n character.
				}
			}
//			charpoint = (char)c;
			//itoa(c, charpoint, 10); //Convert it to a char*.
			char buff = (char)c;
			s.append(1, buff);
			c = fgetc(npc_loadNPCFile);
		}
		//fscanf_s(npc_loadNPCFile, "%i", &x);
		//fscanf_s(npc_loadNPCFile, "%i", &y);
		//charpoint = &s;
		//strcpy_s(charpoint, strlen(s.c_str()) ,s.c_str());
		const char * buf = s.c_str();
		NonPlayerChar * newNPC = new NonPlayerChar(const_cast<char*>(buf), &m_sprites[NPC1]);
		newNPC->setNewed(true);
		newNPC->setLocation(npcX*FRAME_SIZE, npcY*FRAME_SIZE);
		printf("NPC @ (%d,%d)\n", newNPC->getLocation().x, newNPC->getLocation().y);
		this->add(newNPC);
		c = fgetc(npc_loadNPCFile);
	}
	//fclose(infile);
}

/**
	The function that sets the world to have monsters and updates the Entity list of the grid to add them into.
**/
void World::setMonsters()
{
	int numMinions = 0;
	for(int g = 0; g < m_mapOfEntities.size(); g++)
		m_mapOfEntities.get(g).clearAllEntities();
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
					sprite = &m_sprites[SLIME];
					mtrl = MTRL_RUBBER;
					break;
				case WORLD_D1:
					switch(spriteSheet)
					{
					case 0:
						sprite = &m_sprites[SKELETON];
						mtrl = MTRL_DARK;
						break;
					case 1:
						sprite = &m_sprites[GHOST];
						mtrl = MTRL_DARK;
						break;
					}
					break;
				case WORLD_CASTLE:
					switch(castleCount)
					{
					case 0:
					case 1:
						switch(spriteSheet)
						{
						case 0:
							sprite = &m_sprites[KNIGHT0];
							mtrl = MTRL_METAL;
							break;
						case 1:
							sprite = &m_sprites[KNIGHT1];
							mtrl = MTRL_METAL;
							break;
						}
						break;
					case 2:
						switch(spriteSheet)
						{
						case 0:
							sprite = &m_sprites[KNIGHT2];
							mtrl = MTRL_METAL;
							break;
						case 1:
							sprite = &m_sprites[SHADOWDEMON];
							mtrl = MTRL_DARK;
							break;
						}
						break;
					}
					break;
				case WORLD_FOREST:
					switch(spriteSheet)
					{
					case 0:
						sprite = &m_sprites[SKELETON];
						mtrl = MTRL_DARK;
						break;
					case 1:
						sprite = &m_sprites[SLIME];
						mtrl = MTRL_RUBBER;
						break;
					}
					break;
				case WORLD_DESERT:
					switch(spriteSheet)
					{
					case 0:
						sprite = &m_sprites[GHOST];
						mtrl = MTRL_DARK;
						break;
					case 1:
						sprite = &m_sprites[SLIME];
						mtrl = MTRL_RUBBER;
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
			e_material t_type;
			switch(currentWorld)
			{
			case WORLD_FOREST:
			case WORLD_DESERT:
				sprite = &m_sprites[DRAGON];
				t_type = MTRL_AIR;
				break;
			case WORLD_CASTLE:
				if(castleCount < 3){
					sprite = &m_sprites[BOSS1];
					t_type = MTRL_DARK;
				}
				else{
					sprite = &m_sprites[DEMONKING];
					t_type = MTRL_LIGHT;
				}
				break;
			default:
				sprite = &m_sprites[BOSS0];
				t_type = MTRL_DARK;
				break;
			}
			Boss * newBoss = new Boss(sprite);
			newBoss->setMaterial(t_type);
			newBoss->setNewed(true);
			newBoss->scaleToPlayer(m_player);
			newBoss->setBossLoc(m_mapOfWorld.get(i).pos);
			this->add(newBoss);
			switch(t_type)
			{
			case MTRL_AIR:		newBoss->setChip(LIGHTNING, ADVANCED, this);	break;
			case MTRL_LIGHT:	newBoss->setChip(DIVINE, EXPERT, this);			break;
			case MTRL_DARK:
			default:			newBoss->setChip(FIRE, BASIC, this);			break;
			}
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
		if(m_mapOfWorld.get(i).dungeon)
		{
			bool check = false;
			check = m_closed[dcount];
			if(check)
				sprite = m_worldSprites[SINGLE];
			else
				sprite = m_worldSprites[ANIMATION];
			Obstacle * obs = new Obstacle(sprite);
			obs->setNewed(true);
			obs->setDIndex(dcount);
			dcount++;

			if(check)
				obs->setIndex(TILE_DUNGEON_CLOSED);
			else
				obs->setIndex(TILE_DUNGEON);
			obs->setObstacleType(DUNGEON);
		
			obs->setLocation(m_mapOfWorld.get(i).pos);
			this->add(obs);
		}
		if(m_mapOfWorld.get(i).portal )
		{
			sprite = m_worldSprites[ANIMATION];
			Obstacle * obs = new Obstacle(sprite);
			obs->setNewed(true);
			obs->setIndex(TILE_PORTAL);
			obs->setObstacleType(PORTAL);
			obs->setLocation(m_mapOfWorld.get(i).pos);
			this->add(obs);
		}
		if(m_mapOfWorld.get(i).tree)
		{
			sprite = m_worldSprites[TREE];
			Obstacle * obs = new Obstacle(sprite);
			obs->setIndex(rand()%3);
			obs->setObstacleType(TREE);
			obs->setNewed(true);
			obs->setLocation(m_mapOfWorld.get(i).pos);
			this->add(obs);
		}
		if(m_mapOfWorld.get(i).buildingv)
		{
			sprite = m_worldSprites[BUILDING];
			Obstacle * obs = new Obstacle(sprite);
			obs->setIndex(rand()%2);
			obs->setObstacleType(BUILDING);
			obs->setNewed(true);
			obs->setLocation(m_mapOfWorld.get(i).pos);
			this->add(obs);
		}
		if(m_mapOfWorld.get(i).buildingh)
		{
			int btype;
			sprite = m_worldSprites[BUILDING];
			Obstacle * obs = new Obstacle(sprite);
			btype = BUILDING_WOOD_RIGHT;
			obs->setIndex(btype);
			obs->setObstacleType(BUILDING);
			obs->setNewed(true);
			obs->setLocation(m_mapOfWorld.get(i).pos);
			obs->setBuildType(btype%2);//mod 2 cause if its a zero it will be wood otherwise stone
			obs->setDoor();
			this->add(obs);
		}
		if(m_mapOfWorld.get(i).npcLoc)
			setNPC(currentWorld, m_mapOfWorld.get(i).npcNum, m_mapOfWorld.get(i).pos.x/FRAME_SIZE, m_mapOfWorld.get(i).pos.y/FRAME_SIZE);
	}
}

void World::update(Uint32 a_timePassed, AudioHandler * ah)
{
	//Making sure that the entities are all in their correct grids.
	Entity * cE = NULL; //Empty entity pointer...
	for(int z = 0; z < m_mapOfEntities.size(); z++)
	{
		for(int i = 0; i < m_mapOfEntities.get(z).getNumberOfEntities(); i++)
		{
			cE = m_mapOfEntities.get(z).getEntity(i); //Get the entity here.
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
	for(int i = 0; i < m_mapOfEntities.size(); i++) //Check the grids...
	{
		if(m_cCamera.intersects(m_mapOfEntities.get(i).getLoc())) //If the grids intersect, update them.
			m_mapOfEntities.get(i).update(a_timePassed, this, ah);
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
void World::fadeWorld()
{
	int fade, hour = m_localTime->tm_hour;
	if(fadeAmount < 1)
		fadeAmount = 1;
	if(hour < HOURS_HALF_DAY)
		fade = ((HOURS_HALF_DAY-1) - hour) * fadeAmount;
	else
		fade = (hour - HOURS_HALF_DAY) * fadeAmount;
	//printf("hour: %d, fade: %d\n", hour, fade);
	if(fade > 0)
	{
		//for(int i = 0; i < NUM_SPRITES; i++)
		//	m_sprites[i].fade(fade);
		for(int i = 0; i < NUM_SPRITES_WORLD; i++)
			m_worldSprites[i]->fade((float)fade); 
	}
}