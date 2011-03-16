#pragma once
#include "Sprite.h"

enum e_stats {STATS_CURRENT, STATS_MAX, NUM_STAT_INSTANCES, NUM_STATS = 11};
enum e_statname	{CHELTH, MHEALTH, CENERGY, MENERGY, STR, INT, DEF, ENERGYGEN, REZFIRE, REZICE, REZLIGHT};
enum e_locations {LOC_SCREEN, LOC_WORLD, NUM_LOCATIONS};
enum e_resistances {RESIST_FIRE, RESIST_ICE, RESIST_LIGHTNING, NUM_RESISTANCES};

#define	COLOR_TRANSPARENT		0xff00ff
#define CENTER_SCREEN_X			400
#define CENTER_SCREEN_Y			300
#define TIME_TO_REGEN			5000


//each array in this next area the first part is current and second is max
struct Stats
{
	int health[NUM_STAT_INSTANCES];
	int energy[NUM_STAT_INSTANCES];
	int strength;
	int intellect;
	int defence;
	int resistances[NUM_RESISTANCES];//RESIST_FIRE/RESIST_ICE/RESIST_LIGHTNING
	int energyregen;

	void copy(Stats a_stats)
	{
		strength = a_stats.strength;
		intellect = a_stats.intellect;
		defence = a_stats.defence;
		energyregen = a_stats.energyregen;
		for(int i = 0; i < NUM_STAT_INSTANCES; i ++)
		{
			health[i] = a_stats.health[i];
			energy[i] = a_stats.energy[i];
		}
		for(int i = 0; i < NUM_RESISTANCES ; i++)
		{
			resistances[i] = a_stats.resistances[i];
		}
	}
	bool compare(Stats a_stats)//return false if not same otherwise return true they are the same
	{
		if(strength != a_stats.strength)				return false;
		if(intellect != a_stats.intellect)				return false;
		if(defence != a_stats.defence)					return false;
		if(energyregen != a_stats.energyregen)			return false;
		for(int i = 0; i < NUM_STAT_INSTANCES; i ++)
		{
			if(health[i] != a_stats.health[i])			return false;
			if(energy[i] != a_stats.energy[i])			return false;
		}
		for(int i = 0; i < NUM_RESISTANCES ; i++)
		{
			if(resistances[i] != a_stats.resistances[i])return false;
		}
		return true;
	}
	int getStatNumber(int stat)//this is ugly i know but i want the stats to stay seperate so it is easier to read and not have to deal with too many arrays
	{
		//returns the stat based on a number
		switch(stat)
		{
		case CHELTH:return health[STATS_CURRENT];
			break;
		case MHEALTH:return health[STATS_MAX];
			break;
		case STR:return strength;
			break;
		case INT:return intellect;
			break;
		case DEF:return defence;
			break;
		case ENERGYGEN:return energyregen;
			break;
		case CENERGY:return energy[STATS_CURRENT];
			break;
		case MENERGY:return energy[STATS_MAX];
			break;
		case REZFIRE:return resistances[RESIST_FIRE];
			break;
		case REZICE:return resistances[RESIST_ICE];
			break;
		case REZLIGHT:return resistances[RESIST_LIGHTNING];
			break;
		}
	}
	char * getName(int i_stat)
	{
		switch(i_stat)//once again sorry for the dirtyness just trying to get code that works
		{
			case CHELTH:return "Health :%i";
				break;
			case MHEALTH:return "/%i";
				break;
			case STR:return "Strength: %i";
				break;
			case INT:return "Intellect: %i";
				break;
			case DEF:return "Defence: %i";
				break;
			case ENERGYGEN:return "Regen: %i";
				break;
			case CENERGY:return "Energy: %i";
				break;
			case MENERGY:return "/%i";
				break;
			case REZFIRE:return "Fire: %i";
				break;
			case REZICE:return "Ice: %i";
				break;
			case REZLIGHT:return "Lightning: %i";
				break;
		}
	}
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
	Location m_locations[NUM_LOCATIONS];//LOC_SCREEN/LOC_WORLD
	e_entityType m_eType;
	Sprite * m_sprite;
	int m_timeSinceLastUpdate;
	int m_timeToRegen;
	bool m_shouldUpdate;
	bool m_shouldDraw;
public:
	Entity(){
		m_stats.defence = m_stats.intellect = m_stats.strength = 0;
		for(int i = 0; i < NUM_STAT_INSTANCES; i++)
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
		for(int i = 0; i < NUM_STAT_INSTANCES; i++)
		{
			m_stats.energy[i] = a_energy;
			m_stats.health[i] = a_health;
		}
		m_stats.resistances[RESIST_FIRE] = a_fRes;
		m_stats.resistances[RESIST_ICE] = a_iRes;
		m_stats.resistances[RESIST_LIGHTNING] = a_lRes;
		m_sprite = a_sprite;
		setLocation(LOC_SCREEN, CENTER_SCREEN_X, CENTER_SCREEN_Y);
		m_stats.energyregen = 5;
		m_timeSinceLastUpdate = m_timeToRegen = 0;
	}
	Stats getStats(){return m_stats;}
	Location getLocationScreen(){return m_locations[LOC_SCREEN];}
	Location getLocationWorld(){return m_locations[LOC_WORLD];}
	void move(int a_locIndex, int a_deltaX, int a_deltaY){m_locations[a_locIndex].x += a_deltaX; m_locations[a_locIndex].y += a_deltaY;}
	void setLocation(int a_locIndex, int a_x, int a_y){m_locations[a_locIndex].x = a_x; m_locations[a_locIndex].y = a_y;}
	virtual void updateUnique(int a_time){}
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
		m_sprite->update(a_time);
		updateUnique(a_time);
	}
	void draw(SDL_Surface * a_screen)
	{
		m_sprite->draw(a_screen, m_locations[LOC_SCREEN].x,m_locations[LOC_SCREEN].y); 
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