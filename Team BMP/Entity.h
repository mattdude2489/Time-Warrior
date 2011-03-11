#pragma once
#include "Sprite.h"

#define NUM_STATS		9
#define NUM_LOCATIONS	2
#define CURRENT2MAX		2
#define NUM_RESISTANCES	3
#define CURRENT			0
#define	MAX				1

#define SCREENLOC		0
#define WORLDLOC		1

#define FIRE			0
#define	ICE				1
#define	LIGHTNING		2

#define CENTERSCREENX	400
#define CENTERSCREENY	300

#define TIMETOREGEN		5000
//each array in this next area the first part is current and second is max
struct Stats
{
	int health[CURRENT2MAX];
	int energy[CURRENT2MAX];
	int strength;
	int intellect;
	int defence;
	int resistances[NUM_RESISTANCES];//fire/ice/lightning
	int energyregen;
};
struct Location
{
	int x;
	int y;
};
enum e_eType{player, minion, boss};

class Entity
{
protected:
	Stats m_stats;
	Location m_locations[NUM_LOCATIONS];//0 is screen, 1 is world
	bool m_isAnimate;
	e_eType m_myType;
	Sprite * m_mySprite;
	int timeSinceLastUpdate;
	int timeToRegen;
public:
	Entity(){
		m_stats.defence = m_stats.intellect = m_stats.strength = 0;
		for(int i = 0; i < CURRENT2MAX; i++)
			m_stats.energy[i] = m_stats.health[i] = 1;
		for(int i = 0; i < NUM_RESISTANCES; i ++)
			m_stats.resistances[i] = 0;
		m_stats.energyregen = 5;
		timeSinceLastUpdate =timeToRegen =  0;
		m_isAnimate = true;
	}
	void setSprite(Sprite * a_sprite){m_mySprite = a_sprite;}
	Entity(int a_def, int a_int, int a_str, int a_health, int a_energy, int a_fRes, int a_iRes, int a_lRes, Sprite * a_sprite)
	{
		m_stats.defence = a_def;
		m_stats.intellect = a_int;
		m_stats.strength = a_str;
		for(int i = 0; i < CURRENT2MAX; i++)
		{
			m_stats.energy[i] = a_energy;
			m_stats.health[i] = a_health;
		}
		m_stats.resistances[FIRE] = a_fRes;
		m_stats.resistances[ICE] = a_iRes;
		m_stats.resistances[LIGHTNING] = a_lRes;
		setSprite(a_sprite);
		m_locations[SCREENLOC].x = CENTERSCREENX;
		m_locations[SCREENLOC].y = CENTERSCREENY;
		m_stats.energyregen = 5;
		timeSinceLastUpdate = timeToRegen = 0;
		m_isAnimate = true;
	}
	Stats getStats(){return m_stats;}
	Location getLocationScreen(){return m_locations[SCREENLOC];}
	Location getLocationWorld(){return m_locations[WORLDLOC];}
	bool isAnimate(){return m_isAnimate;}
	void update(int a_time)
	{
	//	if(m_stats.energy[CURRENT] != m_stats.energy[MAX])
	//	{
			if(timeSinceLastUpdate == 0)
				timeSinceLastUpdate = a_time;
			timeToRegen+= a_time - timeSinceLastUpdate;
			if(timeToRegen > TIMETOREGEN)
			{
				m_stats.energy[CURRENT] += m_stats.energyregen;
				if(m_stats.energy[CURRENT] > m_stats.energy[MAX])
				{
					m_stats.energy[CURRENT] = m_stats.energy[MAX];
				}
				timeToRegen = 0;
			}
	//	}
		m_mySprite->update(a_time);
	}
	void draw(SDL_Surface * a_screen)
	{
		m_mySprite->draw(a_screen, m_locations[SCREENLOC].x,m_locations[SCREENLOC].y); 
	}
	void hit(int damage)
	{
		m_stats.health[CURRENT] -= damage;
		if(m_stats.health[CURRENT] <0)
		{
			m_stats.health[CURRENT] = 0;
		}
		m_stats.energy[CURRENT] -= damage;
		if(m_stats.energy[CURRENT] <0)
		{
			m_stats.energy[CURRENT] = 0;
		}
	}
	void heal(int healammount)
	{
		m_stats.health[CURRENT]+= healammount;
		if(m_stats.health[CURRENT] > m_stats.health[MAX])
		{
			m_stats.health[CURRENT] = m_stats.health[MAX];
		}
	}
};