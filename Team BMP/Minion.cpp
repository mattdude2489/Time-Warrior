#include "Minion.h"
#include "World.h"


void Minion::checkState(int a_timePassed, World * a_world)
{
	bool playerFound = false;
	Entity * t_player = NULL;
	for(int i = 0; i < a_world->getGrid(m_location.getX(), m_location.getY())->getNumberOfEntities()&&playerFound == false; i++)
	{
		if(a_world->getGrid(m_location.getX(), m_location.getY())->getPlayer(t_player))
		{
			if(t_player)
				isPlayerInRange(t_player);
			else
				m_state = WANDER;
		}
	}
	
	switch(m_state)
	{
	case WANDER:	wander(a_timePassed);						break;
	case CHASE:		if(t_player){updateTargPlayer(t_player);}	break;
	}
	moveToTarget((int)(SPEED_MINION*a_timePassed));
}