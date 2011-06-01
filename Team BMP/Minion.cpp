#include "Minion.h"
#include "World.h"

void Minion::checkState(int a_timePassed, World * a_world)
{
	Entity * t_player = a_world->getPlayer();
	if(!m_playerTargeted)isPlayerInRange(t_player);
	switch(m_state)
	{
	case WANDER:	wander(a_timePassed);										break;
	case CHASE:		updateTargPlayer(t_player, a_timePassed);					break;
	case GUARD:		break;
	}
	moveToTarget((int)(SPEED_MINION*a_timePassed));
}