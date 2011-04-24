//Armor class
//Author: John Parks
#pragma once
#include "Chip.h"

class Armor : public Chip
{
	protected:
		int m_resisType;
	public:
		Armor(e_chipSubType a_subType, e_chipSubSubType a_subSubType)
			:Chip(ARMOR, a_subType, a_subSubType)
		{
			int interval = 33;
			/*
			range based on m_cSubSubType
			bas: 1				- interval
			adv: (interval+1)	- (2*interval)
			exp: (2*interval+1)	- (3*interval)
			*/
			//RESISTANCE_FIRE, RESISTANCE_ICE, or RESISTANCE_LIGHTNING
			m_resisType = rand()%3 + RESISTANCE_FIRE;
			m_stats[DEFENSE] = (rand()%interval + 1) + (interval*m_cSubSubType);
			m_stats[m_resisType] = (rand()%interval + 1) + (interval*m_cSubSubType);
		}
		void activateUnique()
		{
			m_owner->buffDefenseOrResistance(m_stats[DEFENSE], DEFENSE);
			for(int i = RESISTANCE_FIRE; i < RESISTANCE_FIRE+3; ++i)
				m_owner->buffDefenseOrResistance(m_stats[i], (e_stats)i);
		}
		void deactivateUnique()
		{
			m_owner->debuffDefenseOrResistance(m_stats[DEFENSE], DEFENSE);
			for(int i = RESISTANCE_FIRE; i < RESISTANCE_FIRE+3; ++i)
				m_owner->debuffDefenseOrResistance(m_stats[i], (e_stats)i);
		}
		void setDefense(int def)
		{
			m_stats[DEFENSE] = def;
		}
		void setResist(int resistType, int resistAmount)
		{
			resistType += RESISTANCE_FIRE;
			for(int i = RESISTANCE_FIRE; i < RESISTANCE_FIRE+3; ++i)
				m_stats[i] = 0;
			m_stats[resistType] = resistAmount;
		}
};