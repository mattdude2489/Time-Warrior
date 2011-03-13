//WeaponChip class
//Author: John Parks
#pragma once
#include "Chip.h"

class Weapon : public Chip
{
	public:
		Weapon(e_chipSubType a_subType, e_chipSubSubType a_subSubType)
			:Chip(WEAPON, a_subType, a_subSubType){}
		bool shouldApplyEffect()
		{
			switch(m_cSubSubType)
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
			switch(m_cSubType)
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