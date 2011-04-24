//Armor class
//Author: John Parks
#pragma once
#include "Chip.h"

class Armor : public Chip
{
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
			int m_resisType = rand()%3 + RESISTANCE_FIRE;
			for(int i = DEFENSE; i < NUM_STATS; ++i)
			{
				if(i == DEFENSE || i == m_resisType)
					m_stats[i] = (rand()%interval + 1) + (interval*m_cSubSubType);
			}
		}
		void activateUnique()
		{
			for(int i = DEFENSE; i < NUM_STATS; ++i)
				m_owner->buffDefenseOrResistance(m_stats[i], (e_stats)i);
		}
		void deactivateUnique()
		{
			for(int i = DEFENSE; i < NUM_STATS; ++i)
				m_owner->debuffDefenseOrResistance(m_stats[i], (e_stats)i);
		}
		void setDefense(int def){m_stats[DEFENSE] = def;}
		void setResist(int resistType, int resistAmount)
		{
			//set the correct resistance
			resistType += RESISTANCE_FIRE;
			//if the amt is not 0
			if(resistAmount)
			{
				//clear any previous resistances
				for(int i = RESISTANCE_FIRE; i < NUM_STATS; ++i)
					m_stats[i] = 0;
			}
			//set the desired resistance
			m_stats[resistType] = resistAmount;
		}
};