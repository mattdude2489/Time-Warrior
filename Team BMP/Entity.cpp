#include "Entity.h"
#include "World.h"
#include "Chip.h"

void Entity::update(int a_timePassed, World * a_world)
{
	//Update position.
	movePlayer(a_timePassed);
	//update stats
	m_timeToRegen += a_timePassed;
	if(m_timeToRegen >= TIME_SECOND_MS)
	{
		m_timeToRegen = 0;
		regen(m_stats[ENERGY_REGEN]);
		if(m_eType == DUMMY)
			heal(m_stats[ENERGY_REGEN]);
	}
	//update timer
	m_timer += a_timePassed;
	if(m_eType != CHIP)
	{
		if(m_timer >= TIME_INACTIVE && m_sprite->getFrame() == 1)
			m_sprite->stop();
	}
	//update health-bar & sprite
	m_hb.w = (Uint16)(((double)getStatNumber(HEALTH_CURRENT)/(double)getStatNumber(HEALTH_MAX))*(double)m_sprite->getWidth());
	m_hb.h = 5;
	m_sprite->update(a_timePassed);
	//check for world collision/tile collision
	if(m_shouldDraw)
	{
		if(a_world->getTile(m_location.x, m_location.y)->collide
			|| a_world->getTile(m_location.x+m_sprite->getWidth(), m_location.y)->collide
			|| a_world->getTile(m_location.x, m_location.y+m_sprite->getHeight())->collide
			|| a_world->getTile(m_location.x+m_sprite->getWidth(), m_location.y+m_sprite->getHeight())->collide)
		{
			if(m_eType != CHIP)
				move(m_prevLoc.x - m_location.x, m_prevLoc.y - m_location.y);
			else
				m_shouldDraw = false;
		}
		if(a_world->getTile(m_location.x, m_location.y)->portal
			|| a_world->getTile(m_location.x+m_sprite->getWidth(), m_location.y)->portal
			|| a_world->getTile(m_location.x, m_location.y+m_sprite->getHeight())->portal
			|| a_world->getTile(m_location.x+m_sprite->getWidth(), m_location.y+m_sprite->getHeight())->portal)
		{
			if(m_eType == PLAYER)
			{
				if(a_world->getCurrentWorld() == WORLD_D1)
					this->setLocation(1248, 128);
				a_world->setWorld("Maps/MedEngMap.txt");
				
				a_world->setCamera(this->m_camera);
			}
		}
		if(a_world->getTile(m_location.x, m_location.y)->dungeon
			|| a_world->getTile(m_location.x+m_sprite->getWidth(), m_location.y)->dungeon
			|| a_world->getTile(m_location.x, m_location.y+m_sprite->getHeight())->dungeon
			|| a_world->getTile(m_location.x+m_sprite->getWidth(), m_location.y+m_sprite->getHeight())->dungeon)
		{
			if(m_eType == PLAYER)
			{
				int dungeon = rand()%3;//picks a random dungeon 
				this->setLastW();
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
		if(m_eType != CHIP)
		{
			//Search through the entities in that particular grid. If there are any, check for collision with them.
			for(int i = 0; i < a_world->getGrid(m_location.x, m_location.y)->getNumberOfEntities(); i++)
			{
				if(collide(a_world->getEntity(i, m_location.x, m_location.y)))
				{
					if(epicCollide(a_world->getEntity(i, m_location.x, m_location.y)))
						move(m_prevLoc.x - m_location.x, m_prevLoc.y - m_location.y);
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
	case MAGIC:
		stat = getStatNumber(INTELLECT);
		break;
	case WEAPON:
		stat = getStatNumber(STRENGTH);
		break;
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
	case DIVINE:
		break;
	case LIGHTNING:
		stat = getStatNumber(RESISTANCE_LIGHTNING);
		break;
	case FIRE:
		stat = getStatNumber(RESISTANCE_FIRE);
		break;
	case ICE:
		stat = getStatNumber(RESISTANCE_ICE);
		break;
	default:
		stat = getStatNumber(DEFENSE);
	}
	if(a_amount > 0)
		return (int)(a_amount * ((double)a_amount / (a_amount + stat)));
	else
		return 0;
}
void Entity::setCurrentLocToLast(World * a_world)
{
	this->setLocation(m_lastWLoc);
	//a_world->setCamera(this->m_camera);
}