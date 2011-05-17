#include "Grid.h"
#include "Minion.h"
#include "World.h"

void Grid::clearAllEntities()
{
	bool remove, loop = true;
	while(loop)
	{
		loop = false;
		for(int i = 0; i < m_mapOfEntities.size(); i++)
		{
			switch(m_mapOfEntities.get(i)->getType())
			{
			case PLAYER:	remove = false;	break;
			case CHIP:		remove = !m_mapOfEntities.get(i)->getFlag(FLAG_OWNER_PLAYER);
							m_mapOfEntities.get(i)->setDrawOff();	break;
			default:		remove = true;
			}
			if(remove)
			{
				if(m_mapOfEntities.get(i)->getFlag(FLAG_NUDE))
					delete m_mapOfEntities.get(i); //GET RID OF IT. NAO.
				m_mapOfEntities.remove(i);
				//because the size of m_mapOfEntities is changing with each remove,
				//check to remove entities again until none have to be removed
				loop = true;
			}
		}
	}
}
void Grid::clearABoss(World * a_world){a_world->reduceBossCount();}