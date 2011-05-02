#include "Entity.h"
#include "World.h"
#include "Chip.h"

void Entity::update(int a_timePassed, World * a_world)
{
	//Update position.
	movePlayer(a_timePassed);
	//update stats
	m_timers[TIMER_REGEN] += a_timePassed;
	if(m_timers[TIMER_REGEN] >= TIME_SECOND_MS)
	{
		m_timers[TIMER_REGEN] = 0;
		regen(m_stats[ENERGY_REGEN]);
		if(m_eType == DUMMY)
			heal(m_stats[ENERGY_REGEN]);
	}
	//update timer
	m_timers[TIMER_GENERAL] += a_timePassed;
	switch(m_eType)
	{
	case CHIP:
	case OBSTACLE:
		break;
	default:
		if(m_timers[TIMER_GENERAL] >= TIME_INACTIVE && m_sprite->getFrame() == 1)
			m_sprite->stop();
	}
	//update health-bar & sprite
	m_hb.w = (Uint16)(((double)getStatNumber(HEALTH_CURRENT)/(double)getStatNumber(HEALTH_MAX))*(double)m_sprite->getWidth());
	m_hb.h = 5;
	m_sprite->update(a_timePassed);
	//check for world collision/tile collision
	if(m_flags[FLAG_DRAW])
	{
		if(a_world->getTile(m_location.x, m_location.y)->collide
			|| a_world->getTile(m_location.x+m_sprite->getWidth(), m_location.y)->collide
			|| a_world->getTile(m_location.x, m_location.y+m_sprite->getHeight())->collide
			|| a_world->getTile(m_location.x+m_sprite->getWidth(), m_location.y+m_sprite->getHeight())->collide)
		{
			switch(m_eType)
			{
			case CHIP:
				setDrawOff();
			case OBSTACLE:
			case TREE:
				break;
			default:
				move(m_prevLoc.x - m_location.x, m_prevLoc.y - m_location.y);
			}

		}
		switch(m_eType)
		{
		case PLAYER:
		case MINION:
		case BOSS:
			//Search through the entities in that particular grid. If there are any, check for collision with them.
			for(int i = 0; i < a_world->getGrid(m_location.x, m_location.y)->getNumberOfEntities(); i++)
			{
				if(collide(a_world->getEntity(i, m_location.x, m_location.y)))
				{
					if(epicCollide(a_world->getEntity(i, m_location.x, m_location.y)))
					{
						Entity * tmp = a_world->getEntity(i, m_location.x, m_location.y);
						if(tmp->getType() != OBSTACLE)
						{
							move(m_prevLoc.x - m_location.x, m_prevLoc.y - m_location.y);
						}
						else
						{
							if(tmp->isPortal())
							{
								if(m_eType == PLAYER)
								{
									//if(a_world->getCurrentWorld() == WORLD_D1)
									//	this->setLocation(1248, 128);
									if(a_world->getBossCount() == 0){
										a_world->setWorld("Maps/MedEngMap.txt");
										
										a_world->setCamera(this->m_camera);
									}
								}
							}
							else if(tmp->isDungeon())
							{
								if(m_eType == PLAYER)
								{
									if(!a_world->isDClosed(tmp->getIndex())){//if the dungeon is not closed open it
										int dungeon = rand()%3;//picks a random dungeon 

										this->setLastW();
										a_world->setDClosed(tmp->getIndex());
										switch(dungeon){
											case 0:
												a_world->setWorld("Maps/Dungeon1.txt");
												break;
											case 1:
												a_world->setWorld("Maps/Dungeon0.txt");
												break;
											case 2:
												a_world->setWorld("Maps/Dungeon2.txt");
												break;
										}
										a_world->setCamera(this->m_camera);
									}
								}
							}
						}
					}
				}
			}
		}
	}
	//update previous location & anything unique
	if(!m_prevLoc.equals(m_location))
		m_prevLoc = m_location;
	updateUnique(a_timePassed, a_world);
	useEffects(a_timePassed);
}
int Entity::getTotalDamageDealt(int a_amount, e_chipType a_type)
{
	int stat = 0;
	switch(a_type)
	{
	case MAGIC:		stat = getStatNumber(INTELLECT);	break;
	case WEAPON:	stat = getStatNumber(STRENGTH);		break;
	}
	if(a_amount > 0)
		return (int)(a_amount * ((double)(a_amount + stat) / a_amount));
	else
		return 0;
}
int Entity::getTotalDamageTaken(int a_amount, e_chipSubType a_type)
{
	int stat = 0;
	switch(a_type)
	{
	case DIVINE:	break;
	case LIGHTNING:	stat = getStatNumber(RESISTANCE_LIGHTNING);	break;
	case FIRE:		stat = getStatNumber(RESISTANCE_FIRE);		break;
	case ICE:		stat = getStatNumber(RESISTANCE_ICE);		break;
	default:		stat = getStatNumber(DEFENSE);
	}
	if(a_amount > 0)
		return (int)(a_amount * ((double)a_amount / (a_amount + stat)));
	else
		return 0;
}
void Entity::setCurrentLocToLast(World * a_world){this->setLocation(m_lastWLoc);}