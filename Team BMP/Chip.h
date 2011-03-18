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
		e_chipType m_cType;
		e_chipSubType m_cSubType;
		e_chipSubSubType m_cSubSubType;
		int m_level, m_cost, m_costLv, m_dmg, m_dmgLv;
		bool m_isEquipped;
		Entity * m_owner;
		Location m_target;
	public:
		Chip(e_chipType a_type, e_chipSubType a_subType, e_chipSubSubType a_subSubType)
			:Entity(),m_cType(a_type),m_cSubType(a_subType),m_cSubSubType(a_subSubType),
			m_level(0),m_cost(0),m_costLv(0),m_dmg(0),m_dmgLv(0),m_isEquipped(false)
		{
			m_eType = CHIP;
			m_shouldDraw = false;
		}
		e_chipType getType(){return m_cType;}
		e_chipSubType getSubType(){return m_cSubType;}
		e_chipSubSubType getSubSubType(){return m_cSubSubType;}
		int getLevel(){return m_level;}
		int getCost(){return m_cost;}
		int getDamage(){return m_dmg;}
		bool isEquipped(){return m_isEquipped;}
		void toggleEquip(){m_isEquipped = !m_isEquipped;}
		void setOwner(Entity * a_owner){m_owner = a_owner;}
		void setTarget(int a_x, int a_y){m_target.x = a_x; m_target.y = a_y;}
			//define for each spell
		virtual void levelUpUnique(){}
		void levelUp()
		{
			if(m_cType != ARMOR)
				m_costLv = m_dmgLv = 5;
			m_level++;
			m_cost += m_costLv;
			m_dmg += m_dmgLv;
			levelUpUnique();
		}
		virtual bool shouldApplyEffect(){return false;}//pass single entity as a parameter
		virtual void applyEffect(){}//pass single entity as a parameter
		void activate()//pass list of entities as a parameter
		{
			if(!m_shouldDraw)
			{
				m_shouldDraw = true;
				m_sprite->start();
				m_sprite->setLoopToBegin(true);
				if(m_owner != NULL && m_cType == MAGIC && m_cSubSubType == BASIC)
					setLocation(LOC_SCREEN,m_owner->getLocationScreen().x,m_owner->getLocationScreen().y);
			}
			/*
			for(int i = 0; i < size; ++i)
			{
				if(shouldApplyEffect(i))
					applyEffect(i)
			}
			*/
		}
		void deactivate(){m_shouldDraw = false;}
		virtual char * getName(){return "Chip";}
		virtual char * getDescription(){return "Blank chip.";}
};