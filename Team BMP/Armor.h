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
			int interval = 10;
			/*
			range based on m_cSubSubType
			bas: 1				- interval
			adv: (interval+1)	- (2*interval)
			exp: (2*interval+1)	- (3*interval)
			*/
			//RESISTANCE_FIRE, RESISTANCE_ICE, or RESISTANCE_LIGHTNING
			int m_resisType = rand()%3 + RESISTANCE_FIRE;
			//set bonuses to defense & resistances
			for(int i = DEFENSE; i < RESISTANCE_FIRE+3; ++i)
			{
				if(i == DEFENSE || i == m_resisType)
					m_stats[i] = (rand()%interval + 1) + (interval*m_cSubSubType);
			}
			setSprite("Sprites/armor.bmp");
		}
		void activateUnique()
		{
			//add to owner's defense & resistances
			for(int i = DEFENSE; i < RESISTANCE_FIRE+3; ++i)
				m_owner->buffDefenseOrResistance(m_stats[i], (e_stats)i);
		}
		void deactivateUnique()
		{
			//subtract from owner's defense & resistances
			for(int i = DEFENSE; i < RESISTANCE_FIRE+3; ++i)
				m_owner->debuffDefenseOrResistance(m_stats[i], (e_stats)i);
		}
		void setDefense(int def){m_stats[DEFENSE] = def;}
		//resistType = specific resistance assuming 1st resistance @ 0
		void setResist(int resistType, int resistAmount)
		{
			//set the correct resistance
			resistType += RESISTANCE_FIRE;
			//if the amt is not 0
			if(resistAmount)
			{
				//clear any previous resistances
				for(int i = RESISTANCE_FIRE; i < RESISTANCE_FIRE+3; ++i)
					m_stats[i] = 0;
			}
			//set the desired resistance
			m_stats[resistType] = resistAmount;
		}
};