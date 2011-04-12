//Armor class
//Author: John Parks
#pragma once
#include "Chip.h"

class Armor : public Chip
{
	protected:
		int m_defense, m_resistance, m_resisType;
	public:
		Armor(e_chipSubType a_subType, e_chipSubSubType a_subSubType)
			:Chip(ARMOR, a_subType, a_subSubType)
		{
			int interval = 33;
			/*
			m_defense & m_resistance range (based on m_cSubSubType)
			bas: 1				- interval
			adv: (interval+1)	- (2*interval)
			exp: (2*interval+1)	- (3*interval)
			*/
			m_defense = (rand()%interval + 1) + (interval*m_cSubSubType);
			m_resistance = (rand()%interval + 1) + (interval*m_cSubSubType);
			//RESISTANCE_FIRE, RESISTANCE_ICE, or RESISTANCE_LIGHTNING
			m_resisType = rand()%3 + RESISTANCE_FIRE;
		}
		void activateUnique()
		{
			m_owner->buffDefenseOrResistance(m_defense, DEFENSE);
			m_owner->buffDefenseOrResistance(m_resistance, (e_stats)m_resisType);
		}
		void deactivateUnique()
		{
			m_owner->debuffDefenseOrResistance(m_defense, DEFENSE);
			m_owner->debuffDefenseOrResistance(m_resistance, (e_stats)m_resisType);
		}
};