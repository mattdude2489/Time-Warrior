//Chip class
//Author: John Parks
#pragma once
#include "Entity.h"

enum e_chipType {ARMOR, MAGIC, WEAPON};
enum e_chipSubType {HEAD, TRUNK, LIMB_UPPER, LIMB_LOWER, DIVINE, LIGHTNING, FIRE, ICE, BLUNT, RANGE, SLASH, PIERCE};
enum e_chipSubSubType {BASIC, ADVANCED, EXPERT, LEGEND};

class Chip : public Entity
{
	protected:
		e_chipType m_type;
		e_chipSubType m_subType;
		e_chipSubSubType m_subSubType;
		int m_level, m_cost, m_costLv, m_dmg, m_dmgLv;
		bool m_equip;
	public:
		Chip(e_chipType a_type, e_chipSubType a_subType, e_chipSubSubType a_subSubType)
			:m_type(a_type),m_subType(a_subType),m_subSubType(a_subSubType),
			m_level(0),m_cost(0),m_costLv(0),m_dmg(0),m_dmgLv(0),m_equip(false){m_myType = CHIP;}
		e_chipType getType(){return m_type;}
		e_chipSubType getSubType(){return m_subType;}
		e_chipSubSubType getSubSubType(){return m_subSubType;}
		int getLevel(){return m_level;}
		int getCost(){return m_cost;}
		int getDamage(){return m_dmg;}
		bool isEquipped(){return m_equip;}
		void toggleEquip(){m_equip = !m_equip;}
			//define for each spell
		virtual void levelUpUnique(){}
		void levelUp()
		{
			m_level++;
			m_cost += m_costLv;
			m_dmg += m_dmgLv;
			levelUpUnique();
		}
		virtual bool shouldApplyEffect(){return false;}//pass single entity as a parameter
		virtual void applyEffect(){}//pass single entity as a parameter
		void activate()//pass list of entities as a parameter
		{
			/*
			for(int i = 0; i < size; ++i)
			{
				if(shouldApplyEffect(i))
					applyEffect(i)
			}
			*/
		}
		virtual char * getName(){return "Chip";}
		virtual char * getDescription(){return "Blank chip.";}
};