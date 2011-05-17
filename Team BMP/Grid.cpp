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
				clearEntity(i);
				//because the size of m_mapOfEntities is changing with each remove,
				//check to remove entities again until none have to be removed
				loop = true;
			}
		}
	}
}
void Grid::clearDeadEntities(World * a_world) 
{
	Entity * t_ent;
	bool loop = true;
	while(loop)
	{
		loop = false;
		for(int i = 0; i < m_mapOfEntities.size(); i++) 
		{
			t_ent = m_mapOfEntities.get(i);
			//checks to see if it is alive if not remove the damm thing
			if(!t_ent->getFlag(FLAG_DRAW) && t_ent->getStatNumber(HEALTH_CURRENT) <= 0)
			{
				if(t_ent->getType() == BOSS)
					clearABoss(a_world);
				clearEntity(i);
				//because the size of m_mapOfEntities is changing with each remove,
				//check to remove entities again until none have to be removed
				loop = true;
			}
		}
	}
}
void Grid::clearABoss(World * a_world){a_world->reduceBossCount();}