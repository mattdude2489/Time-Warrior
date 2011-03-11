//WeaponChip class
//Author: John Parks
#pragma once
#include "Chip.h"

class Weapon : public Chip
{
	public:
		Weapon(e_cSubType a_subType, e_cSubSubType a_subSubType)
			:Chip(WEAPON, a_subType, a_subSubType){}
		bool shouldApplyEffect()
		{
			switch(m_subSubType)
			{
			case BASIC:
				return false;
				break;
			case ADVANCED:
				return false;
				break;
			case EXPERT:
				return false;
				break;
			default:
				return false;
			}
		}
		void applyEffect()
		{
			switch(m_subType)
			{
			case BLUNT:
				//apply secondary effect
				break;
			case RANGE:
				//apply secondary effect
				break;
			case SLASH:
				//apply secondary effect
				break;
			case PIERCE:
				//apply secondary effect
				break;
			}
		}
};