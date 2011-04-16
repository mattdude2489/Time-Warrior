#pragma once

#include "Minion.h"



class Boss : public Minion
{
private:
	Chip * m_attack;
	SPoint m_start;
	int m_lastCast;

public:
	Boss(){}
	Boss(int a_health, int a_energy, int a_str, int a_int, int a_def, int a_fRes, int a_iRes, int a_lRes, SDL_Sprite * a_sprite)
	{
		bossInit(a_health, a_energy, a_str, a_int, a_def, a_fRes, a_iRes, a_lRes, a_sprite);
	}
	void bossInit(int a_health, int a_energy, int a_str, int a_int, int a_def, int a_fRes, int a_iRes, int a_lRes, SDL_Sprite * a_sprite)
	{
		init(a_health, a_energy, a_str, a_int, a_def, a_fRes, a_iRes, a_lRes, a_sprite);
		m_eType = BOSS;
		m_lastDirectionChange = 0;
		m_hitLast = 0;
		m_state = GUARD;
		m_target.set(m_location);
		m_playerTargeted = false;
		m_lastCast = 0;
	}
	void setBossLoc(SPoint a_point)
	{
		setLocation(a_point);
		m_start = a_point;
	}
	void setChip(Chip * a_chip)
	{
		m_attack = new Chip(a_chip->getType(), a_chip->getSubType(),a_chip->getSubSubType() );
		m_attack->setOwner(this);
		m_attack->levelUp();
	}
	void isPlayerInRange(Entity *a_player, int a_time)
	{
		double distance = getDeltaBetweenLocationAnd(&a_player->getLocation()).getLength();
		if(distance < BOSS_ENGAGE)
		{
			m_state = CHASE;
		}
		else
		{
			m_state = GUARD;
		}
		m_hitLast += a_time;
		m_lastCast += a_time;
		if(this->collide(a_player)&&m_hitLast > HIT_DELAY)
		{
			a_player->hit(this->getStatNumber(STRENGTH), BLUNT);
			m_hitLast = 0;
		}
		/*if(distance < BOSS_ATK_RANGE && m_lastCast > HIT_DELAY)
		{
			m_attack->setTarget(a_player->getLocation());
			m_attack->activate();

			m_lastCast = 0;
		}*/
	}
	~Boss()
	{
		delete m_attack;
	}
};