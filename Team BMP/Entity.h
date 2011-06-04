#pragma once
#include "SDL_Sprite.h"
#include "spoint.h"
#include "UserInput.h"
#include "srect.h"

class World;
struct Tile;
enum e_chipType;
enum e_chipSubType;
enum e_obstacleType;

enum e_stats {LEVEL, HEALTH_CURRENT, HEALTH_MAX, ENERGY_CURRENT, ENERGY_MAX, ENERGY_REGEN , STRENGTH, INTELLECT, DEFENSE, RESISTANCE_FIRE, RESISTANCE_ICE, RESISTANCE_LIGHTNING, NUM_STATS};
enum e_entityType{DUMMY, CHIP, PLAYER, NPC, MINION, BOSS, OBSTACLE};
enum e_colors {COLOR_HEALTH = 0xff0000, COLOR_ENERGY = 0x00ff00, COLOR_BACK = 0x0000ff, COLOR_BASE = 0x808080, COLOR_TRANSPARENT = 0xff00ff, COLOR_EXP = 0x00ffff};
enum e_screen {SCREEN_WIDTH = 800, SCREEN_HEIGHT = 600, SCREEN_CENTER_X = SCREEN_WIDTH/2, SCREEN_CENTER_Y = SCREEN_HEIGHT/2, SCREEN_BPP = 32};
enum e_time {TIME_SECOND_MS = 1000, TIME_REGEN = TIME_SECOND_MS, TIME_INACTIVE = TIME_SECOND_MS/5, TIME_EXPIRE = TIME_SECOND_MS*5, TIME_WANDER = TIME_SECOND_MS*3};
enum e_timer {TIMER_GENERAL, TIMER_REGEN, NUM_TIMERS};
enum e_pots{POT_HEALTH, POT_ENERGY, NUM_POTS};
enum e_flags {FLAG_DRAW, FLAG_ACTIVE, FLAG_OWNER_PLAYER, FLAG_NUDE, FLAG_STOPPED, NUM_FLAGS};
enum e_frame {FRAME_SIZE = 32, FRAME_RATE = 100, DRAGON_FRAME = 65};
enum e_rows {ROW_UP, ROW_RIGHT, ROW_DOWN, ROW_LEFT, NUM_ROWS};
enum e_effect {KNOCKBACK, FREEZE, NUM_EFFECTS};
enum e_material {MTRL_DEFAULT, MTRL_WOOD, MTRL_FIRE, MTRL_EARTH, MTRL_METAL, MTRL_WATER, MTRL_AIR, MTRL_RUBBER, MTRL_LIGHT, MTRL_DARK};
enum e_barrier {BARRIER_NONE, BARRIER_RED, BARRIER_GREEN, BARRIER_YELLOW};
#define	SPEED_PLAYER	.1
#define	SPEED_MAGIC		(SPEED_PLAYER*2)
#define	SPEED_RANGE		(SPEED_PLAYER*3)
#define	SPEED_MINION	SPEED_PLAYER

struct v2D {double x, y;};//PLEASE DONT HATE ME
struct effect {bool active; SPoint target; int timer, timeLimit, dmg;};

class Entity
{
protected:
	bool m_flags[NUM_FLAGS];
	int m_stats[NUM_STATS], m_timers[NUM_TIMERS], m_pots[NUM_POTS], m_statPoints, m_index;
	e_entityType m_eType;
	e_material m_mtrl;
	SPoint m_location, m_prevLoc, *m_camera, m_target, m_lastWLoc;
	SDL_Sprite * m_sprite;
	SDL_Rect m_hb;
	v2D m_vel; //The velocity. - ONLY for player movement
	effect m_effects[NUM_EFFECTS];
public:
	Entity(){init();}
	Entity(SDL_Sprite * a_sprite){init(a_sprite);}
	Entity(int a_health, int a_energy, int a_str, int a_int, int a_def, int a_fRes, int a_iRes, int a_lRes, SDL_Sprite * a_sprite)
	{
		init(a_health, a_energy, a_str, a_int, a_def, a_fRes, a_iRes, a_lRes, a_sprite);
	}
	virtual~Entity()
	{
	}
	void init(){init(true, true, false, false, false, false, false, false);}
	void init(SDL_Sprite * a_sprite){init();initSprite(a_sprite);}
	void init(int a_health, int a_energy, int a_str, int a_int, int a_def, int a_fRes, int a_iRes, int a_lRes)
	{
		m_stats[HEALTH_CURRENT] = m_stats[HEALTH_MAX] = a_health;
		m_stats[ENERGY_CURRENT] = m_stats[ENERGY_MAX] = a_energy;
		m_stats[ENERGY_REGEN] = (int)(m_stats[ENERGY_MAX] * .05);
		m_stats[STRENGTH] = a_str;
		m_stats[INTELLECT] = a_int;
		m_stats[DEFENSE] = a_def;
		m_stats[RESISTANCE_FIRE] = a_fRes;
		m_stats[RESISTANCE_ICE] = a_iRes;
		m_stats[RESISTANCE_LIGHTNING] = a_lRes;
		m_stats[LEVEL] = 1;
		for(int i = 0; i < NUM_TIMERS; ++i)
			m_timers[i] = 0;
		for(int i = 0; i < NUM_FLAGS; ++i)
			m_flags[i] = false;
		m_camera = NULL;
		setLocation(SCREEN_CENTER_X, SCREEN_CENTER_Y);
		m_prevLoc = m_location;
		m_sprite = NULL;
		for(int i = 0; i < NUM_EFFECTS; ++i)
			m_effects[i].active = false;
		m_lastWLoc.set(0,0);//using the 00 to say there is none yet
		m_eType = DUMMY;
		m_mtrl = MTRL_DEFAULT;
		m_statPoints = 0;
		for(int i = 0; i < NUM_POTS; ++i)
			m_pots[i] = 0;
	}
	void initSprite(SDL_Sprite * a_sprite)
	{
		m_flags[FLAG_DRAW] = true;
		m_sprite = a_sprite;
		m_sprite->setTransparency(COLOR_TRANSPARENT);
		m_sprite->restart(ROW_DOWN);
		m_sprite->start();
	}
	void init(int a_health, int a_energy, int a_str, int a_int, int a_def, int a_fRes, int a_iRes, int a_lRes, SDL_Sprite * a_sprite)
	{
		init(a_health, a_energy, a_str, a_int, a_def, a_fRes, a_iRes, a_lRes);
		initSprite(a_sprite);
	}
	void setMaterial(e_material a_mtrl){m_mtrl = a_mtrl;}
	void reallocateResistancesAccordingToMaterial();
	void setCamera(SPoint * a_camera){m_camera = a_camera;}
	void setLocation(SPoint newLoc){setLocation(newLoc.x, newLoc.y);}
	void update(int a_timePassed, World * a_world);
	void setLocation(int a_x, int a_y){setLocationUnique(a_x, a_y);m_location.set(a_x, a_y);}
	void setNewed(bool newed){m_flags[FLAG_NUDE] = newed;}
	void move(int a_deltaX, int a_deltaY)
	{
		moveUnique(a_deltaX, a_deltaY);
		m_location.x += a_deltaX;
		m_location.y += a_deltaY;
		m_timers[TIMER_GENERAL] = 0;
		m_sprite->start();
	}
	void move(SPoint a_point){move(a_point.x,a_point.y);}
	void draw(SDL_Surface * a_screen)
	{
		if(m_flags[FLAG_DRAW] && m_camera)
		{
			m_hb.x = getLocationScreen().x;
			m_hb.y = getLocationScreen().y;
			if(m_eType != CHIP && getStatNumber(HEALTH_CURRENT) < getStatNumber(HEALTH_MAX))
				SDL_FillRect(a_screen, &m_hb, COLOR_HEALTH);
			switch(m_eType)
			{
			case MINION:
			case BOSS:
				{
				int d = getTargetDirection();
				if(d != NUM_ROWS)
					m_sprite->setRIndex(d);
				}
				break;
			case OBSTACLE:
				m_sprite->setRIndex(m_index);
				break;
			}
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
	void hit(int a_amount, e_chipSubType a_type){hit(getTotalDamageTaken(a_amount, a_type));}
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
	virtual void handleServerInfo(char * a_in){}
	virtual void isPlayerInRange(Entity *a_player, int a_time){}
	void setTarget(int a_x, int a_y){m_target.set(a_x, a_y);}
	void setTarget(SPoint a_point){m_target.set(a_point);}
	bool getFlag(e_flags a_flag){return m_flags[a_flag];}
	void setDrawOff(){m_flags[FLAG_DRAW] = false;}
	virtual SPoint * getCamera(){return m_camera;}
	SPoint getDeltaBetweenLocationAnd(SPoint * a_point)
	{
		//calculate the delta (difference) between the target & current location
		return a_point->difference(m_location);
	}
	//@return true if delta is 0 (location @ target), false if not
	bool moveTo(SPoint * a_target, int a_maxDistance)
	{
		//calculate the delta between the target & current location
		SPoint delta = getDeltaBetweenLocationAnd(a_target);
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
	bool moveToTarget(int a_maxDistance){return moveTo(&m_target, a_maxDistance);};
	int getTargetDirection()
	{
		/*
		Sprite-Direction Diagram: based on angle between target and location
		+---+
		|\U/|
		|L R|
		|/D\|
		+---+
		*/
		if(!m_location.equals(m_target))
		{
			int angleDeg = m_location.getAngleDeg(m_target, false);
			//printf("angle: %d\n", angleDeg);
			if(angleDeg < 45)
				return ROW_RIGHT;
			else if(angleDeg < 135)
				return ROW_UP;
			else if(angleDeg < 225)
				return ROW_LEFT;
			else if(angleDeg < 315)
				return ROW_DOWN;
			else
				return ROW_RIGHT;
		}
		else
			return NUM_ROWS;
	}
	virtual void setLocationUnique(int a_x, int a_y){}
	virtual void moveUnique(int a_deltaX, int a_deltaY){}
	virtual void updateUnique(int a_timePassed, World * a_world){}
	virtual void drawUnique(SDL_Surface *a_screen){}
	virtual void movePlayer(int a_timePassed){}
	virtual int getKeyLevel(){return 0;}
	virtual void setKeyLevel(int a_level){}
	virtual int getDIndex(){return 0;}
	//Due to the CPU intensive nature of the pixel Collision, this should rarely be used.
	/*bool epicCollide(SDL_Sprite * a_sprite, int a_x, int a_y)
	{
		if(m_flags[FLAG_DRAW] && a_sprite->isSprite())
			return m_sprite->pixelCollide(m_location.x, m_location.y, *a_sprite, a_x, a_y);//caused run-time crash
		else
			return false;
	}
	bool epicCollide(Entity * a_entity)
	{
		if(a_entity->getFlag(FLAG_DRAW))
			return epicCollide(a_entity->m_sprite, a_entity->getLocation().x, a_entity->getLocation().y);
		else
			return false;
	}*/
	bool collideSimple(SDL_Sprite * a_sprite, int a_x, int a_y)
	{
		if(m_flags[FLAG_DRAW] && a_sprite->isSprite())
			return m_sprite->rectCollide(m_location.x, m_location.y, *a_sprite, a_x, a_y);
		else
			return false;
	}
	//Says if there is a collision between two entities.
	bool collideSimple(Entity * a_entity)
	{
		if(a_entity->getFlag(FLAG_DRAW))
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
	bool collideBoundingCircles(Entity * a_entity, int buffer)
	{
		if(a_entity->getFlag(FLAG_DRAW) && m_flags[FLAG_DRAW])
			return a_entity->getCenter().difference(getCenter()).getLength() <= (getRadius() + a_entity->getRadius() + buffer);
		else
			return false;
	}
	bool collideBoundingCircles(Entity * a_entity){return collideBoundingCircles(a_entity,0);}
	char * getStatName(e_stats a_stat)
	{
		switch(a_stat)//once again sorry for the dirtyness just trying to get code that works
		{
			case LEVEL:					return "Level: %i";				break;
			case HEALTH_CURRENT:		return "Health: %i";			break;
			case HEALTH_MAX:			return "/%i";					break;
			case ENERGY_CURRENT:		return "Energy: %i";			break;
			case ENERGY_MAX:			return "/%i";					break;
			case ENERGY_REGEN:			return "Energy Regen: %i";		break;
			case STRENGTH:				return "Strength: %i";			break;
			case INTELLECT:				return "Intellect: %i";			break;
			case DEFENSE:				return "Defense: %i";			break;
			case RESISTANCE_FIRE:		return "Resist Fire: %i";		break;
			case RESISTANCE_ICE:		return "Resist Ice: %i";		break;
			case RESISTANCE_LIGHTNING:	return "Resist Lightning: %i";	break;
			default:					return "Invalid";
		}
	}
	int getType(){return (int)m_eType;}
	int getWidthOffsetCenter(){return m_sprite->getWidthOffsetCenter();}
	int getHeightOffsetCenter(){return m_sprite->getHeightOffsetCenter();}
	double getRadius(){return SPoint(getWidthOffsetCenter(),getHeightOffsetCenter()).getLength();}
	SPoint getCenter(){return m_location.sum(SPoint(getWidthOffsetCenter(),getHeightOffsetCenter()));}
	int getStatNumber(e_stats a_stat){return m_stats[a_stat];}
	int getPoints(){return m_statPoints;}
	SPoint getLocation(){return m_location;}
	SPoint getPreviousLocation() {return m_prevLoc;}
	SPoint getLocationScreen(){return m_location.difference(*m_camera);}
	SDL_Sprite * getSprite() {return m_sprite;}
	int getTotalDamageTaken(int a_amount, e_chipSubType a_type);
	int getTotalDamageDealt(int a_amount, e_chipType a_type);
	virtual void gainExperience(double a_amount){}
	double getExperienceFromDefeat(Entity * a_defeater){return m_stats[LEVEL] * ((double)m_stats[LEVEL] / a_defeater->getStatNumber(LEVEL));}
	virtual void hitFromPlayer(){}
	void buffDefenseOrResistance(int a_amount, e_stats a_type)
	{
		switch(a_type)
		{
		case DEFENSE:
		case RESISTANCE_FIRE:
		case RESISTANCE_ICE:
		case RESISTANCE_LIGHTNING:
			m_stats[a_type] += a_amount;
			break;
		}
	}
	void debuffDefenseOrResistance(int a_amount, e_stats a_type)
	{
		switch(a_type)
		{
		case DEFENSE:
		case RESISTANCE_FIRE:
		case RESISTANCE_ICE:
		case RESISTANCE_LIGHTNING:
			m_stats[a_type] -= a_amount;
			if(m_stats[a_type] < 0)
				m_stats[a_type] = 0;
			break;
		}
	}
	bool colideWithTile(Tile * a_tile);
	//e_effect KNOCKBACK, maxDistance, direction
	//e_effect FREEZE, dmg, time(numIntervals, sizeIntervalMS)
	void activateEffect(e_effect a_effect, int a_maxDistanceOrDmg, SPoint * a_directionOrTime)
	{
		if(a_effect == KNOCKBACK && !m_effects[KNOCKBACK].active)
		{
			m_effects[KNOCKBACK].active = true;
			m_effects[KNOCKBACK].timer = 0;
			m_effects[KNOCKBACK].timeLimit = (int)(a_maxDistanceOrDmg / SPEED_MAGIC);
			m_effects[KNOCKBACK].target = *a_directionOrTime;

			//TODO: put this code in func (used here & moveTo(SPoint*))
			m_effects[KNOCKBACK].target.setX((int)(((double)m_effects[KNOCKBACK].target.x/m_effects[KNOCKBACK].target.getLength()) * a_maxDistanceOrDmg));
			m_effects[KNOCKBACK].target.setY((int)(((double)m_effects[KNOCKBACK].target.y/m_effects[KNOCKBACK].target.getLength()) * a_maxDistanceOrDmg));

			m_effects[KNOCKBACK].target.add(m_location);
		}
		if(a_effect == FREEZE)
		{
			m_effects[FREEZE].active = true;
			m_effects[FREEZE].timer = 0;
			//# of intervals completed
			m_effects[FREEZE].target.x = 0;
			//# of intervals to complete
			m_effects[FREEZE].target.y = a_directionOrTime->x;
			//time limit of each interval
			m_effects[FREEZE].timeLimit = a_directionOrTime->y;
			//dmg to do each interval
			m_effects[FREEZE].dmg = a_maxDistanceOrDmg;
			if(m_effects[FREEZE].dmg < 1)
				m_effects[FREEZE].dmg = 1;
		}
	}
	void useEffects(int a_timePassed);
	bool isLastWSet(){return !m_lastWLoc.isZero();}
	void setLastW(){m_lastWLoc.set(m_location.getX(), m_location.getY()+(FRAME_SIZE));}
	void setCurrentLocToLast(World * a_world);
	virtual bool isObstacle(e_obstacleType a_type){return false;}
	virtual void setStopped(){m_flags[FLAG_STOPPED] = true;};
	virtual int getIndex(){return 0;}
	virtual void levelUpUnique(){};
	virtual bool colideWithDoor(Entity * a_player){return false;}
	void levelUp()
	{
		m_statPoints++;
		m_stats[LEVEL]++;
		levelUpUnique();
		m_stats[HEALTH_CURRENT] = m_stats[HEALTH_MAX];
		m_stats[ENERGY_CURRENT] = m_stats[ENERGY_MAX];
	}
	void decreasePoints()
	{
		m_statPoints--;
		if(m_statPoints < 0)
			m_statPoints = 0;
	}
	void incStr()
	{
		m_stats[HEALTH_MAX]++;
		m_stats[HEALTH_CURRENT] = m_stats[HEALTH_MAX];
		m_stats[STRENGTH]++;
		m_stats[DEFENSE]++;
		decreasePoints();
	}
	void incInt()
	{
		m_stats[ENERGY_MAX]++;
		m_stats[ENERGY_CURRENT] = m_stats[ENERGY_MAX];
		m_stats[ENERGY_REGEN] = (int)(m_stats[ENERGY_MAX] * .05);
		m_stats[INTELLECT]++;
		for(int i = RESISTANCE_FIRE; i < RESISTANCE_FIRE+3; ++i)
			m_stats[i]++;
		decreasePoints();
	}
	void usePot(e_pots a_pot)
	{
		switch(a_pot)
		{
		case POT_HEALTH:
		case POT_ENERGY:
			if(m_pots[a_pot] > 0)
			{
				m_pots[a_pot]--;
				if(a_pot == POT_HEALTH)
					heal(m_stats[HEALTH_MAX]/4);
				else
				{
					m_stats[ENERGY_CURRENT]+= (m_stats[ENERGY_MAX]/4);
					if(m_stats[ENERGY_CURRENT]>m_stats[ENERGY_MAX])
						m_stats[ENERGY_CURRENT]=m_stats[ENERGY_MAX];
				}
			}
			break;
		}
	}
	void receivePot(e_pots a_pot){m_pots[a_pot]++;}
	int getPots(e_pots a_pot){return m_pots[a_pot];}
	virtual void unlockEarnedAttacks(){}
};