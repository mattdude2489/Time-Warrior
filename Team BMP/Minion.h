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
	virtual void updateTargPlayerUnique(Entity * a_player, int a_time){}
	void updateTargPlayer(Entity * a_player, int a_time)
	{
		m_target.set(a_player->getLocation());
		m_hitLast += a_time;
		if(this->collideBoundingCircles(a_player) && m_hitLast > HIT_DELAY)
		{
			a_player->hit(this->getStatNumber(STRENGTH), BLUNT);
			m_hitLast = 0;
		}
		updateTargPlayerUnique(a_player, a_time);
	}
	void hitFromPlayer(){m_playerTargeted = true;m_state = CHASE;}
	void wander(int a_timePassed)
	{
		m_lastDirectionChange += a_timePassed;
		if(m_lastDirectionChange > WANDER_DIRECTION_TIME || m_location.equals(m_target)||m_flags[FLAG_STOPPED])
		{
			int tX, tY;
			tX = m_location.getX()+((rand()%WANDER_MAX)-WANDER_DIST);//gives a + or - 50 change in wander
			tY = m_location.getY()+((rand()%WANDER_MAX)-WANDER_DIST);
			m_target.set(tX, tY);
			m_lastDirectionChange = 0;
			m_flags[FLAG_STOPPED] = false;
		}
	}
	virtual void isPlayerInRangeUnique(){}
	void isPlayerInRange(Entity *a_player)
	{
		int range = 0;
		switch(m_eType)
		{
		case MINION:	range = ENGAGE_RANGE;	break;
		case BOSS:		range = BOSS_ENGAGE;	break;
		}
		if(collideBoundingCircles(a_player, range))
			m_state = CHASE;
		else
		{
			switch(m_eType)
			{
			case MINION:	m_state = WANDER;	break;
			case BOSS:		m_state = GUARD;	isPlayerInRangeUnique();	break;
			}
		}
	}
	void scaleToPlayer(Entity * a_player)
	{
		int highest, lowest;
		double scaler = 0;
		switch(m_eType)
		{
		case MINION:	scaler = SCALE_MIN;		break;
		case BOSS:		scaler = SCALE_BOSS;	break;
		}
		highest = (int)(a_player->getStatNumber(LEVEL) * scaler);
		//add whatever base health/energy the player started with
		m_stats[HEALTH_MAX] += a_player->getStatNumber(HEALTH_MAX) - a_player->getStatNumber(STRENGTH);
		m_stats[ENERGY_MAX] += a_player->getStatNumber(ENERGY_MAX) - a_player->getStatNumber(INTELLECT);
		m_stats[HEALTH_CURRENT] = m_stats[HEALTH_MAX];
		m_stats[ENERGY_CURRENT] = m_stats[ENERGY_MAX];
		//level up to the highest allowed level
		for(int i = m_stats[LEVEL]; i < highest || m_stats[LEVEL] <= 0; ++i)
			levelUp();
		//increase personal stats
		for(int i = 0; m_statPoints > 0; ++i)
		{
			if(i%2)
				incStr();
			else
				incInt();
		}
		//check to make sure dmg is always done
		while(m_stats[STRENGTH] < a_player->getStatNumber(DEFENSE) || m_stats[STRENGTH] <= 0)
			incStr();
		lowest = a_player->getStatNumber(RESISTANCE_FIRE);
		for(int i = RESISTANCE_FIRE+1; i < RESISTANCE_FIRE+3; ++i)
		{
			if(lowest > a_player->getStatNumber((e_stats)i))
				lowest = a_player->getStatNumber((e_stats)i);
		}
		while(m_stats[INTELLECT] < lowest || m_stats[INTELLECT] <= 0)
			incInt();
		m_statPoints = 0;
		//alter resists based on material/substance
		reallocateResistancesAccordingToMaterial();
	}
};