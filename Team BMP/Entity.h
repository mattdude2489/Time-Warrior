#pragma once
#include "SDL_Sprite.h"
#include "spoint.h"
#include "UserInput.h"

class World;
struct Tile;

enum e_stats {HEALTH_CURRENT, HEALTH_MAX, ENERGY_CURRENT, ENERGY_MAX, ENERGY_REGEN, STRENGTH, INTELLECT, DEFENSE, RESISTANCE_FIRE, RESISTANCE_ICE, RESISTANCE_LIGHTNING, NUM_STATS};
enum e_entityType{CHIP, PLAYER, DUMMY, MINION, BOSS, OBSTACLE, NPC};
enum e_colors {COLOR_HEALTH = 0xff0000, COLOR_ENERGY = 0x00ff00, COLOR_BACK = 0x0000ff, COLOR_BASE = 0x808080, COLOR_TRANSPARENT = 0xff00ff};
enum e_screen {SCREEN_WIDTH = 800, SCREEN_HEIGHT = 600, SCREEN_CENTER_X = SCREEN_WIDTH/2, SCREEN_CENTER_Y = SCREEN_HEIGHT/2, SCREEN_BPP = 32};
enum e_time {TIME_SECOND_MS = 1000, TIME_REGEN = TIME_SECOND_MS, TIME_INACTIVE = TIME_SECOND_MS/5, TIME_EXPIRE = TIME_SECOND_MS*5, TIME_WANDER = TIME_SECOND_MS*3};
enum e_frame {FRAME_SIZE = 32, FRAME_RATE = 20};
enum e_rows {ROW_UP, ROW_RIGHT, ROW_DOWN, ROW_LEFT, NUM_ROWS};
enum e_speed {SPEED_PLAYER = 5, SPEED_MAGIC = SPEED_PLAYER*2, SPEED_MINION = SPEED_PLAYER};

struct v2D
{
	float x;
	float y;
};

class Entity
{
protected:
	int m_stats[NUM_STATS], m_timeToRegen, m_timer;
	e_entityType m_eType;
	bool m_shouldDraw;
	bool activation; //This bool is pretty much there entirely for NPC dialogue at the moment.
	SPoint m_location, m_prevLoc, *m_camera, m_target;
	SDL_Sprite * m_sprite;
	SDL_Rect m_hb;
	v2D m_vel; //The velocity.
public:
	Entity(){init(0, 0, 0, 1, 1, 0, 0, 0);}
	Entity(int a_def, int a_int, int a_str, int a_health, int a_energy, int a_fRes, int a_iRes, int a_lRes, SDL_Sprite * a_sprite)
	{
		init(a_def, a_int, a_str, a_health, a_energy, a_fRes, a_iRes, a_lRes, a_sprite);

	}
	void init(int a_def, int a_int, int a_str, int a_health, int a_energy, int a_fRes, int a_iRes, int a_lRes, SDL_Sprite * a_sprite)
	{
		init(a_def, a_int, a_str, a_health, a_energy, a_fRes, a_iRes, a_lRes);
		m_eType = DUMMY;
		m_shouldDraw = true;
		m_sprite = a_sprite;
		m_sprite->setTransparency(COLOR_TRANSPARENT);
		m_sprite->restart(ROW_DOWN);
		m_sprite->start();
		m_vel.x = 0;
		m_vel.y = 0;
	}
	void init(int a_def, int a_int, int a_str, int a_health, int a_energy, int a_fRes, int a_iRes, int a_lRes)
	{
		m_stats[DEFENSE] = a_def;
		m_stats[INTELLECT] = a_int;
		m_stats[STRENGTH] = a_str;
		m_stats[HEALTH_CURRENT] = m_stats[HEALTH_MAX] = a_health;
		m_stats[ENERGY_CURRENT] = m_stats[ENERGY_MAX] = a_energy;
		m_stats[RESISTANCE_FIRE] = a_fRes;
		m_stats[RESISTANCE_ICE] = a_iRes;
		m_stats[RESISTANCE_LIGHTNING] = a_lRes;
		m_stats[ENERGY_REGEN] = 1;
		m_timeToRegen = m_timer = 0;
		m_shouldDraw = false;
		m_camera = NULL;
		setLocation(SCREEN_CENTER_X, SCREEN_CENTER_Y);
		m_prevLoc = m_location;
		activation = false;
	}
	void setCamera(SPoint * a_camera){m_camera = a_camera;}
	void setLocation(SPoint newLoc){setLocation(newLoc.x, newLoc.y);}
	void update(int a_timePassed, World * a_world);
	void setLocation(int a_x, int a_y)
	{
		setLocationUnique(a_x, a_y);
		m_location.x = a_x;
		m_location.y = a_y;
	}
	void move(int a_deltaX, int a_deltaY)
	{
		moveUnique(a_deltaX, a_deltaY);
		m_location.x += a_deltaX;
		m_location.y += a_deltaY;
		m_timer = 0;
//		m_sprite->start();
	}
	void move(SPoint a_point){move(a_point.x,a_point.y);}
	void draw(SDL_Surface * a_screen)
	{
		if(m_shouldDraw && m_camera)
		{
			m_hb.x = getLocationScreen().x;
			m_hb.y = getLocationScreen().y;
			if(m_eType != CHIP && getStatNumber(HEALTH_CURRENT) < getStatNumber(HEALTH_MAX))
				SDL_FillRect(a_screen, &m_hb, COLOR_HEALTH);
			m_sprite->draw(a_screen, getLocationScreen().x, getLocationScreen().y);
			drawUnique(a_screen);
		}
	}
	void hit(int a_amount)
	{
		m_stats[HEALTH_CURRENT] -= a_amount;
		if(m_stats[HEALTH_CURRENT] < 0)
			m_stats[HEALTH_CURRENT] = 0;
	}
	void useEnergy(int a_amount)
	{
		m_stats[ENERGY_CURRENT] -= a_amount;
		if(m_stats[ENERGY_CURRENT] < 0)
			m_stats[ENERGY_CURRENT] = 0;
	}
	void heal(int a_amount)
	{
		m_stats[HEALTH_CURRENT] += a_amount;
		if(m_stats[HEALTH_CURRENT] > m_stats[HEALTH_MAX])
			m_stats[HEALTH_CURRENT] = m_stats[HEALTH_MAX];
	}
	void regen(int a_amount)
	{
		m_stats[ENERGY_CURRENT] += a_amount;
		if(m_stats[ENERGY_CURRENT] > m_stats[ENERGY_MAX])
			m_stats[ENERGY_CURRENT] = m_stats[ENERGY_MAX];
	}
	void handleServerInfo(char * a_in)
	{
		switch(a_in[0])
		{
		case KEY_LEFT:
			move(-1*SPEED_PLAYER, 0);
			break;
		case KEY_RIGHT:
			move(SPEED_PLAYER, 0);
			break;
		}
		switch(a_in[1])
		{
		case KEY_UP:
			move(0,-1*SPEED_PLAYER);	
			break;
		case KEY_DOWN:
			move(0,SPEED_PLAYER);
			break;
		}
	}
	void setTarget(int a_x, int a_y){m_target.set(a_x, a_y);}
	void setTarget(SPoint a_point){m_target.set(a_point);}
	bool getActivation() {return activation;}
	SPoint getDeltaBetweenLocationAnd(SPoint * a_point)
	{
		//calculate the delta (difference) between the target & current location
		return a_point->difference(m_location);
	}
	//@return true if delta is 0 (location @ target), false if not
	bool moveToTarget(int a_maxDistance)
	{
		//calculate the delta between the target & current location
		SPoint delta = getDeltaBetweenLocationAnd(&m_target);
		if(!delta.isZero())
		{
			//calculate the length
			double length = delta.getLength();
			//if the length is greater than the allowed distance
			if(length > a_maxDistance)
			{
				//normalize the vector (divide by its length), & set to max distance
				delta.setX((int)(((double)delta.x/length) * a_maxDistance));
				delta.setY((int)(((double)delta.y/length) * a_maxDistance));
			}
			//move the allowed distance
			move(delta);
			return false;
		}
		else
			return true;
	}
	void faceTargetDirection()
	{
		/*
		Sprite-Direction Diagram: based on how player's sprite faces when moved
		+-+-+-+
		|L|U|R|
		+-+-+-+
		|L| |R|
		+-+-+-+
		|L|D|D|
		+-+-+-+
		*/
		if(m_target.x < m_location.x)
				m_sprite->setRIndex(ROW_LEFT);
		else
		{
			if(m_target.x > m_location.x)
			{
				if(m_target.y > m_location.y)
					m_sprite->setRIndex(ROW_DOWN);
				else
					m_sprite->setRIndex(ROW_RIGHT);
			}
			else
			{
				if(m_target.y < m_location.y)
					m_sprite->setRIndex(ROW_UP);
				else if(m_target.y > m_location.y)
					m_sprite->setRIndex(ROW_DOWN);
			}
		}
	}
	virtual void setLocationUnique(int a_x, int a_y){}
	virtual void moveUnique(int a_deltaX, int a_deltaY){}
	virtual void updateUnique(int a_timePassed, World * a_world){}
	virtual void drawUnique(SDL_Surface *a_screen){}
	bool getVisible() {return m_shouldDraw;}

	//Due to the CPU intensive nature of the pixel Collision, this should rarely be used.
	bool epicCollide(SDL_Sprite * a_sprite, int a_x, int a_y)
	{
		if(m_shouldDraw && a_sprite->isSprite())
			return m_sprite->pixelCollide(m_location.x, m_location.y, *a_sprite, a_x, a_y);
		else
			return false;
	}
	bool epicCollide(Entity * a_entity)
	{
		if(a_entity->getVisible())
		{
			return epicCollide(a_entity->m_sprite, a_entity->getLocation().x, a_entity->getLocation().y);
		}
		else
			return false;
	}

	bool collideSimple(SDL_Sprite * a_sprite, int a_x, int a_y)
	{
		if(m_shouldDraw && a_sprite->isSprite())
			return m_sprite->rectCollide(m_location.x, m_location.y, *a_sprite, a_x, a_y);
		else
			return false;
	}
	//Says if there is a collision between two entities.
	bool collideSimple(Entity * a_entity)
	{
		if(a_entity->getVisible())
			return collideSimple(a_entity->m_sprite, a_entity->getLocation().x, a_entity->getLocation().y);
		else
			return false;
	}
	bool collide(Entity * a_entity)
	{
		//If one of them is the chip; get rid of it. Right now. Seriously, just don't do it.
		//If they are two players, you need not care. If it's two minions, need not care.
		if(m_eType == CHIP || a_entity->m_eType == CHIP || m_eType == a_entity->m_eType)
			return false;
		else
			return collideSimple(a_entity);
	}
	char * getStatName(int a_stat)
	{
		switch(a_stat)//once again sorry for the dirtyness just trying to get code that works
		{
			case HEALTH_CURRENT:		return "Health :%i";		break;
			case HEALTH_MAX:			return "/%i";				break;
			case ENERGY_CURRENT:		return "Energy: %i";		break;
			case ENERGY_MAX:			return "/%i";				break;
			case ENERGY_REGEN:			return "\tRegen: %i";		break;
			case STRENGTH:				return "Strength: %i";		break;
			case INTELLECT:				return "Intellect: %i";		break;
			case DEFENSE:				return "Defense: %i";		break;
			case RESISTANCE_FIRE:		return "\tFire: %i";		break;
			case RESISTANCE_ICE:		return "\tIce: %i";			break;
			case RESISTANCE_LIGHTNING:	return "\tLightning: %i";	break;
			default:					return "Invalid";
		}
	}
	int getType() {return (int)m_eType;}
	int getWidthOffsetCenter(){return m_sprite->getWidthOffsetCenter();}
	int getHeightOffsetCenter(){return m_sprite->getHeightOffsetCenter();}
	int getStatNumber(int a_stat)
	{
		//returns the stat based on a number
		if(a_stat < 0 || a_stat > NUM_STATS)
			return 0;
		else
			return m_stats[a_stat];
	}
	SPoint getLocation(){return m_location;}
	SPoint getPreviousLocation() {return m_prevLoc;}
	SPoint getLocationScreen(){return m_location.difference(*m_camera);}
	SDL_Sprite * getSprite() {return m_sprite;}
	v2D getVelocity() {return m_vel;}
	void setVelocity(float newVelX, float newVelY) 
	{
		m_vel.x = newVelX; m_vel.y = newVelY;
		if(m_vel.x != 0 || m_vel.y != 0)
			m_sprite->start();
		else
			m_sprite->stop();
	}
	void setVelocity(v2D newVel) {m_vel = newVel;}
};