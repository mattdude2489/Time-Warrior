#pragma once
#include "Sprite.h"

#define NUM_STATS		9
#define NUM_LOCATIONS	2
#define CURRENT2MAX		2
#define NUM_RESISTANCES	3

#define SCREENLOC		0
#define WORLDLOC		1

#define FIRE			0
#define	ICE				1
#define	LIGHTNING		2

#define CENTERSCREENX	400
#define CENTERSCREENY	300
//each array in this next area the first part is current and second is max
struct Stats
{
	int health[CURRENT2MAX];
	int energy[CURRENT2MAX];
	int strength;
	int intellect;
	int defence;
	int resistances[NUM_RESISTANCES];//fire/ice/lightning
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
	e_eType m_myType;
	Sprite * m_mySprite;
public:
	Entity(){
		m_stats.defence = m_stats.intellect = m_stats.strength = 0;
		for(int i = 0; i < CURRENT2MAX; i++)
			m_stats.energy[i] = m_stats.health[i] = 1;
		for(int i = 0; i < NUM_RESISTANCES; i ++)
			m_stats.resistances[i] = 0;
	}
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
		m_mySprite = a_sprite;
		m_locations[SCREENLOC].x = CENTERSCREENX;
		m_locations[SCREENLOC].y = CENTERSCREENY;
	}
	Stats getStats(){return m_stats;}
	Location getLocationScreen(){return m_locations[SCREENLOC];}
	Location getLocationWorld(){return m_locations[WORLDLOC];}
	void update(int a_time)
	{
		m_mySprite->update(a_time);
	}
	void draw(SDL_Surface * a_screen)
	{
		m_mySprite->draw(a_screen, m_locations[SCREENLOC].x,m_locations[SCREENLOC].y); 
	}
};