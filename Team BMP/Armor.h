//ArmorChip class
//Author: John Parks
#pragma once
#include "Chip.h"

class ArmorChip : public Chip
{
	public:
		ArmorChip(e_subType a_subType, e_subSubType a_subSubType)
			:Chip(Armor, a_subType, a_subSubType){}
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
			case Head:
				break;
			case Trunk:
				break;
			case UpperLimb:
				break;
			case LowerLimb:
				break;
			}
		}
};