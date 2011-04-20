#include "Grid.h"
#include "Minion.h"
#include "World.h"

void Grid::clearAllEntities()
{
	if(m_mapOfEntities.size() > 0) //There's SOMETHING in the grid.
		{
			for(int i = 0; i < m_mapOfEntities.size(); i++)
			{
				if(!(m_mapOfEntities.get(i)->getType() == PLAYER || (m_mapOfEntities.get(i)->getType() == CHIP && m_mapOfEntities.get(i)->isChipOwnerPlayer())))
				{
					if(m_mapOfEntities.get(i)->getNewed())
					{
						delete m_mapOfEntities.get(i); //GET RID OF IT. NAO.
						m_mapOfEntities.remove(i);
					}
				}
				else if(m_mapOfEntities.get(i)->getType() == CHIP)//stops the crash
				{
					m_mapOfEntities.get(i)->setDrawOff();
				}
			}
			
		}
}