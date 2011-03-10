//WeaponChip class
//Author: John Parks
#pragma once
#include "Chip.h"

class WeaponChip : public Chip
{
	public:
		WeaponChip(e_subType a_subType, e_subSubType a_subSubType)
			:Chip(Weapon, a_subType, a_subSubType){}
		bool shouldApplyEffect()
		{
			switch(m_subSubType)
			{
			case Basic:
				return false;
				break;
			case Advanced:
				return false;
				break;
			case Expert:
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
			case Blunt:
				//apply secondary effect
				break;
			case Range:
				//apply secondary effect
				break;
			case Slash:
				//apply secondary effect
				break;
			case Pierce:
				//apply secondary effect
				break;
			}
		}
};