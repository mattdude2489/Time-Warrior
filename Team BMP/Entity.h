#pragma once
#include "Sprite.h"

#define	COLOR_TRANSPARENT		0xff00ff

#define NUM_STATS				9
#define CURRENT_AND_MAX			2
#define STATS_CURRENT			0
#define	STATS_MAX				1

#define NUM_LOCATIONS			2
#define LOC_SCREEN				0
#define LOC_WORLD				1

#define NUM_RESISTANCES			3
#define RESIST_FIRE				0
#define	RESIST_ICE				1
#define	RESIST_LIGHTNING		2

#define CENTER_SCREEN_X			400
#define CENTER_SCREEN_Y			300

#define TIME_TO_REGEN			5000
//each array in this next area the first part is current and second is max
struct Stats
{
	int health[CURRENT_AND_MAX];
	int energy[CURRENT_AND_MAX];
	int strength;
	int intellect;
	int defence;
	int resistances[NUM_RESISTANCES];//RESIST_FIRE/RESIST_ICE/RESIST_LIGHTNING
	int energyregen;
};
struct Location
{
	int x;
	int y;
};
enum e_entityType{PLAYER, MINION, BOSS, CHIP};

class Entity
{
protected:
	Stats m_stats;
	Location m_locations[NUM_LOCATIONS];//0 is screen, 1 is world
	e_entityType m_myType;
	Sprite * m_mySprite;
	int m_timeSinceLastUpdate;
	int m_timeToRegen;
public:
	Entity(){
		m_stats.defence = m_stats.intellect = m_stats.strength = 0;
		for(int i = 0; i < CURRENT_AND_MAX; i++)
			m_stats.energy[i] = m_stats.health[i] = 1;
		for(int i = 0; i < NUM_RESISTANCES; i ++)
			m_stats.resistances[i] = 0;
		m_stats.energyregen = 5;
		m_timeSinceLastUpdate = m_timeToRegen =  0;
	}
	Entity(int a_def, int a_int, int a_str, int a_health, int a_energy, int a_fRes, int a_iRes, int a_lRes, Sprite * a_sprite)
	{
		m_stats.defence = a_def;
		m_stats.intellect = a_int;
		m_stats.strength = a_str;
		for(int i = 0; i < CURRENT_AND_MAX; i++)
		{
			m_stats.energy[i] = a_energy;
			m_stats.health[i] = a_health;
		}
		m_stats.resistances[RESIST_FIRE] = a_fRes;
		m_stats.resistances[RESIST_ICE] = a_iRes;
		m_stats.resistances[RESIST_LIGHTNING] = a_lRes;
		m_mySprite = a_sprite;
		setLocation(LOC_SCREEN, CENTER_SCREEN_X, CENTER_SCREEN_Y);
		m_stats.energyregen = 5;
		m_timeSinceLastUpdate = m_timeToRegen = 0;
	}
	Stats getStats(){return m_stats;}
	Location getLocationScreen(){return m_locations[LOC_SCREEN];}
	Location getLocationWorld(){return m_locations[LOC_WORLD];}
	void move(int a_locIndex, int a_deltaX, int a_deltaY){m_locations[a_locIndex].x += a_deltaX; m_locations[a_locIndex].y += a_deltaY;}
	void setLocation(int a_locIndex, int a_x, int a_y){m_locations[a_locIndex].x = a_x; m_locations[a_locIndex].y = a_y;}
	void update(int a_time)
	{
	//	if(m_stats.energy[CURRENT] != m_stats.energy[MAX])
	//	{
			if(m_timeSinceLastUpdate == 0)
				m_timeSinceLastUpdate = a_time;
			m_timeToRegen += a_time - m_timeSinceLastUpdate;
			if(m_timeToRegen > TIME_TO_REGEN)
			{
				m_stats.energy[STATS_CURRENT] += m_stats.energyregen;
				if(m_stats.energy[STATS_CURRENT] > m_stats.energy[STATS_MAX])
				{
					m_stats.energy[STATS_CURRENT] = m_stats.energy[STATS_MAX];
				}
				m_timeToRegen = 0;
			}
	//	}
		m_mySprite->update(a_time);
	}
	void draw(SDL_Surface * a_screen)
	{
		m_mySprite->draw(a_screen, m_locations[LOC_SCREEN].x,m_locations[LOC_SCREEN].y); 
	}
	void hit(int damage)
	{
		m_stats.health[STATS_CURRENT] -= damage;
		if(m_stats.health[STATS_CURRENT] <0)
		{
			m_stats.health[STATS_CURRENT] = 0;
		}
		m_stats.energy[STATS_CURRENT] -= damage;
		if(m_stats.energy[STATS_CURRENT] <0)
		{
			m_stats.energy[STATS_CURRENT] = 0;
		}
	}
	void heal(int healammount)
	{
		m_stats.health[STATS_CURRENT]+= healammount;
		if(m_stats.health[STATS_CURRENT] > m_stats.health[STATS_MAX])
		{
			m_stats.health[STATS_CURRENT] = m_stats.health[STATS_MAX];
		}
	}
};