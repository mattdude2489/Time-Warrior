#pragma once

#include "Minion.h"
#include "Magic.h"
#include "Weapon.h"

#define NUM_MAX_BOSS_ATKS	3

class Boss : public Minion
{
private:
	Chip * m_attack[NUM_MAX_BOSS_ATKS];
	SPoint m_start;
	int m_lastCast, m_numAtks;
public:
	Boss(SDL_Sprite * a_sprite)
		:Minion(a_sprite)
	{
		m_eType = BOSS;
		m_state = GUARD;
		m_lastCast = m_numAtks = 0;
		for(int i = 0; i < NUM_MAX_BOSS_ATKS; ++i)
			m_attack[i] = NULL;
	}
	void setBossLoc(SPoint a_point)
	{
		setLocation(a_point);
		m_start = a_point;
	}
	void setChip(e_chipSubType a_subType, e_chipSubSubType a_subSubType, World * a_world)
	{
		if(m_numAtks < NUM_MAX_BOSS_ATKS)
		{
			if(!m_attack[m_numAtks])
			{
				switch(a_subType)
				{
				case DIVINE:	m_attack[m_numAtks] = new Divine(a_subSubType);		break;
				case LIGHTNING:	m_attack[m_numAtks] = new Lightning(a_subSubType);	break;
				case FIRE:		m_attack[m_numAtks] = new Fire(a_subSubType);		break;
				case ICE:		m_attack[m_numAtks] = new Ice(a_subSubType);		break;
				case BLUNT:		m_attack[m_numAtks] = new Blunt(a_subSubType);		break;
				case RANGE:		m_attack[m_numAtks] = new Range(a_subSubType);		break;
				case PIERCE:	m_attack[m_numAtks] = new Pierce(a_subSubType);		break;
				default:		m_attack[m_numAtks] = new Slash(a_subSubType);		break;
				}
				m_attack[m_numAtks]->setNewed(true);
				m_attack[m_numAtks]->setOwner(this);
				m_attack[m_numAtks]->unlock();
				a_world->add(m_attack[m_numAtks]);
				m_numAtks++;
			}
		}
	}
	void isPlayerInRangeUnique(){m_location = m_start;}
	void updateTargPlayerUnique(Entity * a_player, int a_time, AudioHandler * ah)
	{
		m_lastCast += a_time;
		if(this->collideBoundingCircles(a_player, BOSS_ENGAGE) && m_lastCast > HIT_DELAY && m_numAtks > 0)//if in cast range and time to cast cast
		{
			int atk = rand()%m_numAtks;
			if(m_attack[atk])
			{
				m_attack[atk]->setTarget(a_player->getLocation());
				m_attack[atk]->activate(ah);
				m_lastCast = 0;
			}
		}
	}
	void stopAttack()
	{
		for(int i = 0; i < NUM_MAX_BOSS_ATKS; ++i)
		{
			if(m_attack[i])
				m_attack[i]->deactivate();
		}
	}
};