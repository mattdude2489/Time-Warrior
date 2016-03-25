//Armor class
//Author: John Parks
#pragma once
#include "Chip.h"

class Armor : public Chip
{
	private:
		const int m_interval;
	public:
		Armor(e_chipSubType a_subType, e_chipSubSubType a_subSubType)
			:Chip(ARMOR, a_subType, a_subSubType),m_interval(10)
		{
			/*
			range based on m_cSubSubType
			bas: 1					- m_interval
			adv: (m_interval+1)		- (2*m_interval)
			exp: (2*m_interval+1)	- (3*m_interval)
			*/
			//RESISTANCE_FIRE, RESISTANCE_ICE, or RESISTANCE_LIGHTNING
			int m_resisType = rand()%3 + RESISTANCE_FIRE;
			//set bonuses to defense & resistances
			for(int i = DEFENSE; i < RESISTANCE_FIRE+3; ++i)
			{
				if(i == DEFENSE || i == m_resisType)
					m_stats[i] = (rand()%m_interval + 1) + (m_interval*m_cSubSubType);
			}
			setSprite("Sprites/armor.bmp");
		}
		e_stats getResistType()
		{
			for(int i = RESISTANCE_FIRE; i < RESISTANCE_FIRE+3; ++i)
			{
				if(m_stats[i] > 0)
					return (e_stats)i;
			}
			return DEFENSE;
		}
		char * getResistTypeName()
		{
			switch(getResistType())
			{
				case RESISTANCE_FIRE:		return getSubTypeName(FIRE);		break;
				case RESISTANCE_ICE:		return getSubTypeName(ICE);			break;
				case RESISTANCE_LIGHTNING:	return getSubTypeName(LIGHTNING);	break;
				default:					return "Unknown";
			}
		}
		int getValidStatBuff(int a_statBuff)
		{
			while(a_statBuff > (m_interval*(m_cSubSubType+1)))
				a_statBuff -= m_interval;
			if(a_statBuff < m_interval*m_cSubSubType + 1)
				a_statBuff = m_interval*m_cSubSubType + 1;
			return a_statBuff;
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
		void setDefense(int def){m_stats[DEFENSE] = getValidStatBuff(def);}
		//resistType = specific resistance assuming 1st resistance @ 0
		void setResist(int resistType, int resistAmount)
		{
			//set the correct resistance
			if(resistType < 0)
				resistType = 0;
			else if(resistType > 3)
				resistType = 3;
			resistType += RESISTANCE_FIRE;
			//if the amt is not 0
			if(resistAmount)
			{
				//clear any previous resistances
				for(int i = RESISTANCE_FIRE; i < RESISTANCE_FIRE+3; ++i)
					m_stats[i] = 0;
				//set the desired resistance
				m_stats[resistType] = getValidStatBuff(resistAmount);
			}
		}
};