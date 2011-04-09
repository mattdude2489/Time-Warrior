#include "Grid.h"
#include "Minion.h"

void Grid::setMonsters(int currentWorld, int currentGrid, int maxX, int maxY)
{
	if(m_mapOfEntities.size() > 0) //There's SOMETHING in the grid.
	{
		for(int i = 0; i < m_mapOfEntities.size(); i++)
		{
			if(!(m_mapOfEntities.get(i)->getType() == PLAYER || m_mapOfEntities.get(i)->getType() == CHIP))
				delete m_mapOfEntities.get(i); //GET RID OF IT. NAO.
		}
	}
	if(currentWorld == WORLD_ENGLAND)
	{
		int numberOfMonsters = rand()%1+1;
		int gridX, gridY;
		gridY = currentGrid/NUM_GRIDS_PER_ROW_COL;
		gridX = currentGrid-(NUM_GRIDS_PER_ROW_COL * gridY);
		int pixelBaseX, pixelBaseY;
		pixelBaseX = maxX/NUM_GRIDS_PER_ROW_COL;
		pixelBaseY = maxY/NUM_GRIDS_PER_ROW_COL;
		for(int k = 0; k < numberOfMonsters; k++)
		{
			int spriteSheet = 0;
			SDL_Sprite * sprite;
			spriteSheet = rand()%NUM_SPRITE_SHEETS_TO_CHOOSE_FROM;
			if(spriteSheet == 0)
				sprite = new SDL_Sprite("Sprites/slime.bmp", FRAME_SIZE-1, 23, FRAME_RATE, NUM_ROWS);
			if(spriteSheet == 1)
				sprite = new SDL_Sprite("Sprites/skeleton.bmp", 24, FRAME_SIZE, FRAME_RATE, NUM_ROWS);
			if(spriteSheet == 2)
				sprite = new SDL_Sprite("Sprites/ghost.bmp", FRAME_SIZE, FRAME_SIZE, FRAME_RATE, NUM_ROWS);
			Minion * newEntity = new Minion(100, 100, 3, 2, 5, 0, 0, 0, sprite);
			newEntity->setNewed(true);
			int enX, enY;
			enX = rand()%(pixelBaseX-FRAME_SIZE);
			enY = rand()%(pixelBaseY-FRAME_SIZE);
			newEntity->setLocation(FRAME_SIZE + enX + (pixelBaseX*gridX), FRAME_SIZE + enY + (pixelBaseY*gridY));
			m_mapOfEntities.add(newEntity);
		}
	}
}