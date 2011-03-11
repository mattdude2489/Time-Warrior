//ArmorChip class
//Author: John Parks
#pragma once
#include "Chip.h"

class Armor : public Chip
{
	public:
		Armor(e_cSubType a_subType, e_cSubSubType a_subSubType)
			:Chip(ARMOR, a_subType, a_subSubType){}
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
			case HEAD:
				break;
			case TRUNK:
				break;
			case LIMB_UPPER:
				break;
			case LIMB_LOWER:
				break;
			}
		}
};