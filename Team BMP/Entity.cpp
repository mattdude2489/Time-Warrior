#include "Entity.h"
#include "World.h"

void Entity::update(int a_timePassed, World * a_world)
{
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
	//update health-bar
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
	//update previous location & anything unique
	m_prevLoc = m_location;
	updateUnique(a_timePassed, a_world);
}
