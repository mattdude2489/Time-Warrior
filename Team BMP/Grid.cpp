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
	if(currentWorld == 1)
	{
		//TODO: CHANGE THIS TO BE PSUEDO RANDOM.
		srand(0); //I hope this doesn't come around to kill me!
		int numberOfMonsters = rand()%10;
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
				sprite = new SDL_Sprite("Sprites/slime.bmp", 32, 32, 32, 4);
			if(spriteSheet == 1)
				sprite = new SDL_Sprite("Sprites/skeleton.bmp", 24, 32, 32, 4);
			Minion * newEntity = new Minion(100, 100, 3, 2, 5, 0, 0, 0, sprite);
			newEntity->setNewed(true);
			int enX, enY;
			enX = rand()%pixelBaseX;
			enY = rand()%pixelBaseY;
			newEntity->move(enX + (pixelBaseX*gridX), enY + (pixelBaseY*gridY));

			m_mapOfEntities.add(newEntity);
		}
	}
}