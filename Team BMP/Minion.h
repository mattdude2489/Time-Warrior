#pragma once

#include "Entity.h"
#define WANDERDIRECTIONTIME			3000
#define RANDMAX						100
#define WANDERDIST					50
enum e_states	{WANDER};

class Minion : public Entity
{
private:
	int lastDirectionChange;
	SPoint m_target;
	int m_state;
public:
	Minion(){}
	Minion(int a_def, int a_int, int a_str, int a_health, int a_energy, int a_fRes, int a_iRes, int a_lRes, SDL_Sprite* a_sprite)
	{
		init(a_def,  a_int,  a_str,  a_health,  a_energy,  a_fRes,  a_iRes,  a_lRes);
		m_sprite = a_sprite;
		lastDirectionChange = 0;
		m_state = WANDER;
	}
	void updateUnique(int a_timePassed)
	{
		m_sprite->update(a_timePassed);
		wander(a_timePassed);
		moveToTarget();
	}
	void wander(int a_timePassed)
	{
		if((a_timePassed - lastDirectionChange) > WANDERDIRECTIONTIME)
		{
			int tX, tY;
			tX = m_location.getX()+((rand()%RANDMAX)-WANDERDIST);//gives a + or - 50 change in wander
			tY = m_location.getY()+((rand()%RANDMAX)-WANDERDIST);
			m_target.set(tX, tY);
			lastDirectionChange = a_timePassed;
		}
	}
	void moveToTarget()
	{
		m_location.difference(m_target);
	}
};