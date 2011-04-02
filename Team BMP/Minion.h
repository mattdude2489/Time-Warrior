#pragma once

#include <math.h>
#include "Entity.h"


#define RANGETOENGAGE				100
#define SPEED						5
enum e_states	{WANDER, CHASE};
enum e_wander	{WANDER_DIST = 50, WANDER_MAX = 100,WANDERDIRECTIONTIME	= 3000};
class Minion : public Entity
{
private:
	int m_lastDirectionChange, m_state;
	bool m_playerTargeted;
public:
	Minion(){}
	Minion(int a_def, int a_int, int a_str, int a_health, int a_energy, int a_fRes, int a_iRes, int a_lRes, SDL_Sprite* a_sprite)
		:Entity(a_def, a_int, a_str, a_health, a_energy, a_fRes, a_iRes, a_lRes, a_sprite)
	{
		m_eType = MINION;
		m_lastDirectionChange = 0;
		m_state = WANDER;
		m_target.set(m_location);
		m_playerTargeted = false;
	}
	void respawn()
	{
		heal(getStatNumber(HEALTH_MAX));
		setLocation(SCREEN_CENTER_X, SCREEN_CENTER_Y);
	}
	void updateUnique(int a_timePassed, World * a_world)
	{
		checkState(a_timePassed, a_world);
		faceTargetDirection();
		if(getStatNumber(HEALTH_CURRENT) <= 0)
			respawn();
	}
	void wander(int a_timePassed)
	{
		m_lastDirectionChange += a_timePassed;

		if(m_lastDirectionChange > WANDERDIRECTIONTIME || m_location.equals(m_target))
		{
			int tX, tY;
			tX = m_location.getX()+((rand()%WANDER_MAX)-WANDER_DIST);//gives a + or - 50 change in wander
			tY = m_location.getY()+((rand()%WANDER_MAX)-WANDER_DIST);
			m_target.set(tX, tY);
			m_lastDirectionChange = 0;
		}
	}
	void checkState(int a_timePassed, World * a_world)
	{
		Entity * t_player = a_world->getPlayer();
		if(t_player)
		{
			isAplayerInRange(t_player);
		}
		switch(m_state)
		{
		case WANDER:		wander(a_timePassed);			break;
		case CHASE:			if(t_player){updateTargPlayer(t_player);}		break;
		
		}
		moveToTarget(SPEED);
	
	}
	void updateTargPlayer(Entity *a_player)
	{
		m_target.set(a_player->getLocation());
	}
	void isAplayerInRange(Entity *a_player)
	{

		double distance, diffx, diffy;
		diffx = m_location.x - a_player->getLocation().x;
		diffy = m_location.y - a_player->getLocation().y;
		distance = sqrt((diffx*diffx)+(diffy*diffy));//a^2 + b^2 = c^2 re ordered

		if(distance < RANGETOENGAGE){
			m_playerTargeted = true;
			m_state = CHASE;
		}
		else
		{
			m_playerTargeted = false;
			m_state = WANDER;
		}
	}
};