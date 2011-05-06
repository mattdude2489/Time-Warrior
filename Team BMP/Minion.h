#pragma once

#include <math.h>
#include "Entity.h"
#include "Chip.h"

enum e_range	{ENGAGE_RANGE = 100, HIT_DELAY = 1000, BOSS_ENGAGE = 150, BOSS_ATK_RANGE = 50};
enum e_states	{WANDER, CHASE, GUARD};
enum e_wander	{WANDER_DIST = 50, WANDER_MAX = WANDER_DIST*2,WANDER_DIRECTION_TIME = TIME_SECOND_MS*3};
#define		SCALE_MIN		.75
#define		SCALE_BOSS		1.75

class Minion : public Entity
{
protected:
	int m_lastDirectionChange, m_state, m_hitLast;
	bool m_playerTargeted;
public:
	Minion(SDL_Sprite * a_sprite)
		:Entity(a_sprite)
	{
		m_eType = MINION;
		m_state = WANDER;
		m_target.set(m_location);
		m_playerTargeted = false;
		m_lastDirectionChange = m_hitLast = 0;
	}
	void checkState(int a_timePassed, World * a_world);
	void respawn(){heal(getStatNumber(HEALTH_MAX));setLocation(SCREEN_CENTER_X, SCREEN_CENTER_Y);m_state = WANDER;}
	void updateUnique(int a_timePassed, World * a_world){checkState(a_timePassed, a_world);}
	void updateTargPlayer(Entity *a_player){m_target.set(a_player->getLocation());}
	void hitFromPlayer(){m_playerTargeted = true;m_state = CHASE;}
	void wander(int a_timePassed)
	{
		m_lastDirectionChange += a_timePassed;
		if(m_lastDirectionChange > WANDER_DIRECTION_TIME || m_location.equals(m_target))
		{
			int tX, tY;
			tX = m_location.getX()+((rand()%WANDER_MAX)-WANDER_DIST);//gives a + or - 50 change in wander
			tY = m_location.getY()+((rand()%WANDER_MAX)-WANDER_DIST);
			m_target.set(tX, tY);
			m_lastDirectionChange = 0;
		}
	}
	void isPlayerInRange(Entity *a_player, int a_time)
	{
		double distance = getDeltaBetweenLocationAnd(&a_player->getLocation()).getLength();
		if(distance < ENGAGE_RANGE)
			m_state = CHASE;
		else
			m_state = WANDER;
		m_hitLast += a_time;
		if(this->collide(a_player)&&m_hitLast > HIT_DELAY)
		{
			a_player->hit(this->getStatNumber(STRENGTH), BLUNT);
			m_hitLast = 0;
		}
	}
	void scaleToPlayer(Entity * a_player)
	{
		int highest = 0;
		for(int i = 0; i < NUM_STATS; i++)
		{
			if(m_eType == MINION)//scale the minions slightly weeker then player but close
				m_stats[i] = (int)(a_player->getStatNumber((e_stats)i) * SCALE_MIN);
			else if( m_eType == BOSS)
				m_stats[i] = (int)(a_player->getStatNumber((e_stats)i) * SCALE_BOSS);
			if(m_stats[i] <= 0 && (a_player->getStatNumber((e_stats)i) > 0 || (i == STRENGTH || i == INTELLECT)))
				m_stats[i] = 1;
			switch(i)
			{
			case DEFENSE:
			case RESISTANCE_FIRE:
			case RESISTANCE_ICE:
			case RESISTANCE_LIGHTNING:
				if(a_player->getStatNumber((e_stats)i) > highest)
					highest = a_player->getStatNumber((e_stats)i);
			}
		}
		if(m_stats[STRENGTH] < highest)
			m_stats[STRENGTH] = highest;
		if(m_stats[INTELLECT] < highest)
			m_stats[INTELLECT] = highest;
		//in case the players health is low this will set the minion/boss to full
		m_stats[HEALTH_CURRENT] = m_stats[HEALTH_MAX];
	}
};