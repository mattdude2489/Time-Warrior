#pragma once

#include "Entity.h"
#define	WANDER_DIST	50
enum e_states	{WANDER};

class Minion : public Entity
{
private:
	int m_lastDirectionChange, m_state;
public:
	Minion(){}
	Minion(int a_def, int a_int, int a_str, int a_health, int a_energy, int a_fRes, int a_iRes, int a_lRes, SDL_Sprite* a_sprite)
		:Entity(a_def, a_int, a_str, a_health, a_energy, a_fRes, a_iRes, a_lRes, a_sprite)
	{
		m_eType = MINION;
		m_state = WANDER;
		m_target.set(m_location);
		m_lastDirectionChange = TIME_WANDER;
	}
	void respawn()
	{
		heal(getStatNumber(HEALTH_MAX));
		setLocation(SCREEN_CENTER_X, SCREEN_CENTER_Y);
	}
	void updateUnique(int a_timePassed, World * a_world)
	{
		wander(a_timePassed);
		if(getStatNumber(HEALTH_CURRENT) <= 0)
			respawn();
	}
	void wander(int a_timePassed)
	{
		m_lastDirectionChange += a_timePassed;
		if(m_lastDirectionChange >= TIME_WANDER || m_location.equals(m_target))
		{
			m_lastDirectionChange = 0;
			//gives a + or - 50 change in wander
			m_target.setX(m_location.getX()+((rand()%(WANDER_DIST*2+1))-WANDER_DIST));
			m_target.setY(m_location.getY()+((rand()%(WANDER_DIST*2+1))-WANDER_DIST));
			if(m_target.x < m_location.x)
				m_sprite->setRIndex(3);
			else
			{
				if(m_target.x > m_location.x)
				{
					if(m_target.y < m_location.y)
						m_sprite->setRIndex(1);
					else
						m_sprite->setRIndex(2);
				}
				else
				{
					if(m_target.y < m_location.y)
						m_sprite->setRIndex(0);
					else
						m_sprite->setRIndex(2);
				}
			}
		}
		else
			moveToTarget(SPEED_MINION);
	}
};