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
		static bool called = false;
		if(!called)
		{
			called = true;
			switch(a_subType)
			{
			case DIVINE:	m_attack = new Divine(a_subSubType);	break;
			case LIGHTNING:	m_attack = new Lightning(a_subSubType);	break;
			case FIRE:		m_attack = new Fire(a_subSubType);		break;
			case ICE:		m_attack = new Ice(a_subSubType);		break;
			case BLUNT:		m_attack = new Blunt(a_subSubType);		break;
			case RANGE:		m_attack = new Range(a_subSubType);		break;
			case PIERCE:	m_attack = new Pierce(a_subSubType);	break;
			default:		m_attack = new Slash(a_subSubType);		break;
			}
			m_attack->setNewed(true);
			m_attack->setOwner(this);
			m_attack->unlock();
			a_world->add(m_attack);
		}
	}
	void isPlayerInRangeUnique(){m_location = m_start;}
	void updateTargPlayerUnique(Entity * a_player, int a_time)
	{
		m_lastCast += a_time;
		if(this->collideBoundingCircles(a_player, BOSS_ATK_RANGE) && m_lastCast > HIT_DELAY && m_attack)//if in cast range and time to cast cast
		{
			m_attack->setTarget(a_player->getLocation());
			m_attack->activate();
			m_lastCast = 0;
		}
	}
};