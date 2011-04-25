#pragma once
#include "SDL_Sprite.h"
#include "spoint.h"
#include "UserInput.h"
#include "srect.h"

class World;
struct Tile;
enum e_chipType;
enum e_chipSubType;

enum e_stats {HEALTH_CURRENT, HEALTH_MAX, ENERGY_CURRENT, ENERGY_MAX, ENERGY_REGEN , STRENGTH, INTELLECT, DEFENSE, RESISTANCE_FIRE, RESISTANCE_ICE, RESISTANCE_LIGHTNING, NUM_STATS};
enum e_entityType{DUMMY, CHIP, PLAYER, NPC, MINION, BOSS, OBSTACLE};
enum e_colors {COLOR_HEALTH = 0xff0000, COLOR_ENERGY = 0x00ff00, COLOR_BACK = 0x0000ff, COLOR_BASE = 0x808080, COLOR_TRANSPARENT = 0xff00ff, COLOR_EXP = 0x00ffff};
enum e_screen {SCREEN_WIDTH = 800, SCREEN_HEIGHT = 600, SCREEN_CENTER_X = SCREEN_WIDTH/2, SCREEN_CENTER_Y = SCREEN_HEIGHT/2, SCREEN_BPP = 32};
enum e_time {TIME_SECOND_MS = 1000, TIME_REGEN = TIME_SECOND_MS, TIME_INACTIVE = TIME_SECOND_MS/5, TIME_EXPIRE = TIME_SECOND_MS*5, TIME_WANDER = TIME_SECOND_MS*3};
enum e_frame {FRAME_SIZE = 32, FRAME_RATE = TIME_SECOND_MS/30};
enum e_rows {ROW_UP, ROW_RIGHT, ROW_DOWN, ROW_LEFT, NUM_ROWS};
enum e_effect {KNOCKBACK, NUM_EFFECTS};

#define	SPEED_PLAYER	.1
#define	SPEED_MAGIC		(SPEED_PLAYER*2)
#define	SPEED_MINION	SPEED_PLAYER

struct v2D {double x, y;};//PLEASE DONT HATE ME
struct effect {bool active; SPoint target; int timer, timeLimit;};

class Entity
{
protected:
	int m_stats[NUM_STATS], m_timeToRegen, m_timer, m_level;
	e_entityType m_eType;
	bool m_shouldDraw, m_activation, m_chipOwnerPlayer; //This bool is pretty much there entirely for NPC dialogue at the moment.
	SPoint m_location, m_prevLoc, *m_camera, m_target, m_lastWLoc;
	SDL_Sprite * m_sprite;
	SDL_Rect m_hb;
	v2D m_vel; //The velocity. - ONLY for player movement
	//I don't apologize for your vulgarity, or my own.
	bool nude; //Lolz. Keeps track of whether or not it needs to delete it's own Sprite pointer when this is destroyed.
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
		if(nude)
			delete m_sprite;
	}
	void init(){init(1, 1, 0, 0, 0, 0, 0, 0);}
	void init(SDL_Sprite * a_sprite){init();initSprite(a_sprite);}
	void init(int a_health, int a_energy, int a_str, int a_int, int a_def, int a_fRes, int a_iRes, int a_lRes)
	{
		m_stats[HEALTH_CURRENT] = m_stats[HEALTH_MAX] = a_health;
		m_stats[ENERGY_CURRENT] = m_stats[ENERGY_MAX] = a_energy;
		m_stats[ENERGY_REGEN] = 5;
		m_stats[STRENGTH] = a_str;
		m_stats[INTELLECT] = a_int;
		m_stats[DEFENSE] = a_def;
		m_stats[RESISTANCE_FIRE] = a_fRes;
		m_stats[RESISTANCE_ICE] = a_iRes;
		m_stats[RESISTANCE_LIGHTNING] = a_lRes;
		m_timeToRegen = m_timer = 0;
		m_level = 1;
		m_shouldDraw = m_activation = nude = m_chipOwnerPlayer = false;
		m_camera = NULL;
		setLocation(SCREEN_CENTER_X, SCREEN_CENTER_Y);
		m_prevLoc = m_location;
		m_sprite = NULL;
		for(int i = 0; i < NUM_EFFECTS; ++i)
			m_effects[i].active = false;
		m_lastWLoc.set(0,0);//using the 00 to say there is none yet
	}
	void initSprite(SDL_Sprite * a_sprite)
	{
		m_eType = DUMMY;
		m_shouldDraw = true;
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
	void setCamera(SPoint * a_camera){m_camera = a_camera;}
	void setLocation(SPoint newLoc){setLocation(newLoc.x, newLoc.y);}
	void update(int a_timePassed, World * a_world);
	void setLocation(int a_x, int a_y)
	{
		setLocationUnique(a_x, a_y);
		m_location.x = a_x;
		m_location.y = a_y;
	}
	void setNewed(bool newed)
	{
		nude = newed; //Set the newed variable to whatever it needs to be.
	}
	void move(int a_deltaX, int a_deltaY)
	{
		moveUnique(a_deltaX, a_deltaY);
		m_location.x += a_deltaX;
		m_location.y += a_deltaY;
		m_timer = 0;
		m_sprite->start();
	}
	void move(SPoint a_point){move(a_point.x,a_point.y);}
	virtual void draw(SDL_Surface * a_screen)
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
	bool getActivation() {return m_activation;}
	void setDrawOff(){m_shouldDraw = false;}
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
	bool getNewed() {return nude;}
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
	virtual void movePlayer(int a_timePassed){}
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
			case HEALTH_CURRENT:		return "Health:%i";			break;
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
	int getTotalDamageTaken(int a_amount, e_chipSubType a_type);
	int getTotalDamageDealt(int a_amount, e_chipType a_type);
	int getLevel(){return m_level;}
	virtual void gainExperience(double a_amount){}
	double getExperienceFromDefeat(Entity * a_defeater){return m_level * ((double)m_level / a_defeater->getLevel());}
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
	void activateEffect(e_effect a_effect, int a_maxDistance, SPoint * a_direction)
	{
		if(a_effect == KNOCKBACK && !m_effects[KNOCKBACK].active)
		{
			m_effects[KNOCKBACK].active = true;
			m_effects[KNOCKBACK].timer = 0;
			m_effects[KNOCKBACK].timeLimit = (int)(a_maxDistance / SPEED_MAGIC);
			m_effects[KNOCKBACK].target = *a_direction;

			//TODO: put this code in func (used here & moveTo(SPoint*))
			m_effects[KNOCKBACK].target.setX((int)(((double)m_effects[KNOCKBACK].target.x/m_effects[KNOCKBACK].target.getLength()) * a_maxDistance));
			m_effects[KNOCKBACK].target.setY((int)(((double)m_effects[KNOCKBACK].target.y/m_effects[KNOCKBACK].target.getLength()) * a_maxDistance));

			m_effects[KNOCKBACK].target.add(m_location);
		}
	}
	void useEffects(int a_timePassed)
	{
		if(m_effects[KNOCKBACK].active)
		{
			m_effects[KNOCKBACK].timer += a_timePassed;
			if(moveTo(&m_effects[KNOCKBACK].target, (int)(SPEED_MAGIC*a_timePassed))
			|| m_effects[KNOCKBACK].timer > m_effects[KNOCKBACK].timeLimit)
				m_effects[KNOCKBACK].active = false;
		}
	}
	bool isChipOwnerPlayer(){return m_chipOwnerPlayer;}
	bool isLastWSet(){return !m_lastWLoc.isZero();}
	void setLastW()
	{
		m_lastWLoc.setX(m_location.getX());
		//set with offset so when you go back it doesnt send you into the dungeon again
		m_lastWLoc.setY(m_location.getY()+(2*FRAME_SIZE));
	}
	void setCurrentLocToLast(World * a_world);
	virtual bool isPortal(){return false;}
	virtual bool isDungeon(){return false;}
	virtual int getIndex(){return 0;}
};