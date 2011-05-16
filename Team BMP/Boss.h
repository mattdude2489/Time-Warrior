#pragma once

#include "Minion.h"
#include "Magic.h"
#include "Weapon.h"

class Boss : public Minion
{
private:
	Chip * m_attack;
	SPoint m_start;
	int m_lastCast;
public:
	Boss(SDL_Sprite * a_sprite)
		:Minion(a_sprite)
	{
		m_eType = BOSS;
		m_state = GUARD;
		m_lastCast = 0;
		m_attack = NULL;
	}
	void setBossLoc(SPoint a_point)
	{
		setLocation(a_point);
		m_start = a_point;
	}
	void setChip(e_chipSubType a_subType, e_chipSubSubType a_subSubType, World * a_world)
	{
		if(m_attack)
			delete m_attack;
		switch(a_subType)
		{
		case DIVINE:	m_attack = new Divine(a_subSubType);	break;
		case LIGHTNING:	m_attack = new Lightning(a_subSubType);	break;
		case FIRE:		m_attack = new Fire(a_subSubType);		break;
		case ICE:		m_attack = new Ice(a_subSubType);		break;
		case BLUNT:		m_attack = new Blunt(a_subSubType);		break;
		//case RANGE:		m_attack = new Range(a_subSubType);		break;
		case SLASH:		m_attack = new Slash(a_subSubType);		break;
		//case PIERCE:	m_attack = new Pierce(a_subSubType);	break;
		}
		if(m_attack)
		{
			m_attack->setNewed(true);
			m_attack->setOwner(this);
			m_attack->levelUp();
			a_world->add(m_attack);
		}
	}
	void isPlayerInRange(Entity *a_player, int a_time)
	{
		double distance = getDeltaBetweenLocationAnd(&a_player->getLocation()).getLength();
		if(distance < BOSS_ENGAGE)
			m_state = CHASE;
		else
		{
			//if the player runs too far away the boss will reset to its original position and guard
			m_location = m_start;
			m_state = GUARD;
		}
		//updates the time till they can attack and cast again
		m_hitLast += a_time;
		m_lastCast += a_time;
		if(this->collide(a_player)&&m_hitLast > HIT_DELAY)//if coliding and the time to hit is up hit
		{
			a_player->hit(this->getStatNumber(STRENGTH), BLUNT);
			m_hitLast = 0;
		}
		if(distance < BOSS_ATK_RANGE && m_lastCast > HIT_DELAY && m_attack)//if in cast range and time to cast cast
		{
			m_attack->setTarget(a_player->getLocation());
			m_attack->activate();
			m_lastCast = 0;
		}
	}
};