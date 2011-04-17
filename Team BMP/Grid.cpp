#include "Grid.h"
#include "Minion.h"

void Grid::clearAllEntities()
{
	if(m_mapOfEntities.size() > 0) //There's SOMETHING in the grid.
		{
			for(int i = 0; i < m_mapOfEntities.size(); i++)
			{
				if(!(m_mapOfEntities.get(i)->getType() == PLAYER || m_mapOfEntities.get(i)->getType() == CHIP))
				{
					if(m_mapOfEntities.get(i)->getNewed())
					{
						delete m_mapOfEntities.get(i); //GET RID OF IT. NAO.
						m_mapOfEntities.remove(i);
					}
				}
			}
		}
}