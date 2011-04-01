#pragma once

#include "Entity.h"
enum e_wander {WANDER_MAX = 100, WANDER_DIST = 50};
enum e_states	{WANDER};

class Minion : public Entity
{
private:
	int lastDirectionChange, m_state;
public:
	Minion(){}
	Minion(int a_def, int a_int, int a_str, int a_health, int a_energy, int a_fRes, int a_iRes, int a_lRes, SDL_Sprite* a_sprite)
		:Entity(a_def, a_int, a_str, a_health, a_energy, a_fRes, a_iRes, a_lRes, a_sprite)
	{
		m_eType = MINION;
		lastDirectionChange = 0;
		m_state = WANDER;
		m_target.set(m_location);
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
		lastDirectionChange += a_timePassed;
		if(lastDirectionChange > TIME_WANDER)
		{
			int tX, tY;
			tX = m_location.getX()+((rand()%WANDER_MAX)-WANDER_DIST);//gives a + or - 50 change in wander
			tY = m_location.getY()+((rand()%WANDER_MAX)-WANDER_DIST);
			m_target.set(tX, tY);
			lastDirectionChange = 0;
		}
		else
			moveToTarget(SPEED_MINION);
	}
};