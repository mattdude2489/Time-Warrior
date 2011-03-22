#pragma once
#include "SDL_Sprite.h"
#include <stdio.h>

enum e_stats {HEALTH_CURRENT, HEALTH_MAX, ENERGY_CURRENT, ENERGY_MAX, ENERGY_REGEN, STRENGTH, INTELLECT, DEFENSE, RESISTANCE_FIRE, RESISTANCE_ICE, RESISTANCE_LIGHTNING, NUM_STATS};
enum e_locations {LOC_SCREEN, LOC_WORLD, NUM_LOCATIONS};
enum e_entityType{CHIP, PLAYER, DUMMY, MINION, BOSS, OBSTACLE};
enum e_colors {COLOR_HEALTH = 0xff0000, COLOR_ENERGY = 0x00ff00, COLOR_BACK = 0x0000ff, COLOR_BASE = 0xffffff, COLOR_TRANSPARENT = 0xff00ff};
enum e_screen {SCREEN_WIDTH = 800, SCREEN_HEIGHT = 600, SCREEN_CENTER_X = SCREEN_WIDTH/2, SCREEN_CENTER_Y = SCREEN_HEIGHT/2, SCREEN_BPP = 32};
enum e_time {TIME_SECOND_MS = 1000, TIME_REGEN = TIME_SECOND_MS, TIME_EXPIRE = TIME_SECOND_MS*5};
enum e_sprite {SPRITE_SIZE = 32, SPRITE_SPEED = TIME_SECOND_MS/30, SPRITE_ROWS = 4};

//each array in this next area the first part is current and second is max
struct Stats
{
	int m_stats[NUM_STATS];

	void copy(Stats a_stats)
	{
		for(int i = 0; i < NUM_STATS; ++i)
			m_stats[i] = a_stats.m_stats[i];
	}
	bool compare(Stats a_stats)//return false if not same otherwise return true they are the same
	{
		for(int i = 0; i < NUM_STATS; ++i)
		{
			if(m_stats[i] != a_stats.m_stats[i])
				return false;
		}
		return true;
	}
	int getStatNumber(int stat)//this is ugly i know but i want the stats to stay seperate so it is easier to read and not have to deal with too many arrays
	{
		//returns the stat based on a number
		if(stat < 0 || stat > NUM_STATS)
			return 0;
		else
			return m_stats[stat];
	}
	char * getName(int i_stat)
	{
		switch(i_stat)//once again sorry for the dirtyness just trying to get code that works
		{
			case HEALTH_CURRENT:return "Health :%i";
				break;
			case HEALTH_MAX:return "/%i";
				break;
			case ENERGY_CURRENT:return "Energy: %i";
				break;
			case ENERGY_MAX:return "/%i";
				break;
			case ENERGY_REGEN:return "\tRegen: %i";
				break;
			case STRENGTH:return "Strength: %i";
				break;
			case INTELLECT:return "Intellect: %i";
				break;
			case DEFENSE:return "Defense: %i";
				break;
			case RESISTANCE_FIRE:return "\tFire: %i";
				break;
			case RESISTANCE_ICE:return "\tIce: %i";
				break;
			case RESISTANCE_LIGHTNING:return "\tLightning: %i";
				break;
			default:return "Invalid";
		}
	}
};
struct Location{int x, y;};

class Entity
{
protected:
	Stats m_stats;
	Location m_locations[NUM_LOCATIONS];//LOC_SCREEN/LOC_WORLD
	e_entityType m_eType;
	SDL_Sprite * m_sprite;
	int m_timeToRegen;
	bool m_shouldDraw;
	SDL_Rect m_hb;
public:
	void init(int a_def, int a_int, int a_str, int a_health, int a_energy, int a_fRes, int a_iRes, int a_lRes)
	{
		m_stats.m_stats[DEFENSE] = a_def;
		m_stats.m_stats[INTELLECT] = a_int;
		m_stats.m_stats[STRENGTH] = a_str;
		m_stats.m_stats[HEALTH_CURRENT] = m_stats.m_stats[HEALTH_MAX] = a_health;
		m_stats.m_stats[ENERGY_CURRENT] = m_stats.m_stats[ENERGY_MAX] = a_energy;
		m_stats.m_stats[RESISTANCE_FIRE] = a_fRes;
		m_stats.m_stats[RESISTANCE_ICE] = a_iRes;
		m_stats.m_stats[RESISTANCE_LIGHTNING] = a_lRes;
		m_stats.m_stats[ENERGY_REGEN] = 1;
		m_timeToRegen =  0;
		setLocation(LOC_SCREEN, SCREEN_CENTER_X, SCREEN_CENTER_Y);
		m_shouldDraw = false;
	}
	Entity(){init(0, 0, 0, 1, 1, 0, 0, 0);}
	Entity(int a_def, int a_int, int a_str, int a_health, int a_energy, int a_fRes, int a_iRes, int a_lRes, SDL_Sprite * a_sprite)
	{
		init(a_def, a_int, a_str, a_health, a_energy, a_fRes, a_iRes, a_lRes);
		m_shouldDraw = true;
		m_sprite = a_sprite;
		m_sprite->start();
		m_hb.x = SCREEN_CENTER_X;
		m_hb.y = SCREEN_CENTER_Y;
		m_hb.w = m_sprite->getWidth();
		m_hb.h = 5;
		m_eType = DUMMY;
	}
	Stats getStats(){return m_stats;}
	void setType(e_entityType type) {m_eType = type;}
	int getType() {return (int)m_eType;}
	bool getVisible() {return m_shouldDraw;}
	Location getLocationScreen(){return m_locations[LOC_SCREEN];}
	Location getLocationWorld(){return m_locations[LOC_WORLD];}
	int getWidthOffsetCenter(){return m_sprite->getWidthOffsetCenter();}
	int getHeightOffsetCenter(){return m_sprite->getHeightOffsetCenter();}
	void move(int a_locIndex, int a_deltaX, int a_deltaY){m_locations[a_locIndex].x += a_deltaX; m_locations[a_locIndex].y += a_deltaY;}
	void setLocation(int a_locIndex, int a_x, int a_y){m_locations[a_locIndex].x = a_x; m_locations[a_locIndex].y = a_y;}
	virtual void updateUnique(int a_timePassed){}
	void update(int a_timePassed)
	{
		m_timeToRegen += a_timePassed;
		if(m_timeToRegen >= TIME_SECOND_MS)
		{
			m_timeToRegen = 0;
			m_stats.m_stats[ENERGY_CURRENT] += m_stats.m_stats[ENERGY_REGEN];
			if(m_stats.m_stats[ENERGY_CURRENT] > m_stats.m_stats[ENERGY_MAX])
				m_stats.m_stats[ENERGY_CURRENT] = m_stats.m_stats[ENERGY_MAX];
			if(m_eType == DUMMY)
				heal(m_stats.m_stats[ENERGY_REGEN]);
		}
		m_hb.x = m_locations[LOC_SCREEN].x;
		m_hb.y = m_locations[LOC_SCREEN].y;
		m_hb.w = (((double)m_stats.getStatNumber(HEALTH_CURRENT)/(double)m_stats.getStatNumber(HEALTH_MAX))*(double)m_sprite->getWidth());
		m_sprite->update(a_timePassed);
		updateUnique(a_timePassed);
	}
	void draw(SDL_Surface * a_screen)
	{
		if(m_shouldDraw)
		{
			if(m_eType != CHIP)
				SDL_FillRect(a_screen, &m_hb, COLOR_HEALTH);
			m_sprite->draw(a_screen, m_locations[LOC_SCREEN].x,m_locations[LOC_SCREEN].y); 
		}
	}
	void hit(int damage)
	{
		m_stats.m_stats[HEALTH_CURRENT] -= damage;
		if(m_stats.m_stats[HEALTH_CURRENT] <0)
			m_stats.m_stats[HEALTH_CURRENT] = 0;
	}
	void useEnergy(int a_energy)
	{
		m_stats.m_stats[ENERGY_CURRENT] -= a_energy;
		if(m_stats.m_stats[ENERGY_CURRENT] <0)
			m_stats.m_stats[ENERGY_CURRENT] = 0;
	}
	void heal(int healamount)
	{
		m_stats.m_stats[HEALTH_CURRENT]+= healamount;
		if(m_stats.m_stats[HEALTH_CURRENT] > m_stats.m_stats[HEALTH_MAX])
			m_stats.m_stats[HEALTH_CURRENT] = m_stats.m_stats[HEALTH_MAX];
	}
	//Says if there is a collision between two entities.
	bool collideSimple(Entity * otherEntity)
	{
		if(m_shouldDraw && otherEntity->getVisible())
			return m_sprite->rectCollide(m_locations[LOC_SCREEN].x, m_locations[LOC_SCREEN].y, *otherEntity->m_sprite, otherEntity->getLocationScreen().x, otherEntity->getLocationScreen().y);
		else
			return false;
	}
	bool collide(Entity * otherEntity)
	{
		//If one of them is the chip; get rid of it. Right now. Seriously, just don't do it.
		//If they are two players, you need not care. If it's two minions, need not care.
		if(m_eType == CHIP || otherEntity->m_eType == CHIP || m_eType == otherEntity->m_eType)
			return false;
		else
			return collideSimple(otherEntity);
	}
};