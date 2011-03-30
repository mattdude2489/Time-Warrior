//Chip class
//Author: John Parks
#pragma once
#include "Entity.h"
#include "World.h"

enum e_chipType {ARMOR, MAGIC, WEAPON, NUM_CHIP_TYPES};
enum e_chipSubType {HEAD, TRUNK, LIMB_UPPER, LIMB_LOWER, DIVINE, LIGHTNING, FIRE, ICE, BLUNT, RANGE, SLASH, PIERCE, NUM_TOTAL_CHIP_SUBS, NUM_CHIP_SUBS_PER_TYPE = NUM_TOTAL_CHIP_SUBS/NUM_CHIP_TYPES};
enum e_chipSubSubType {BASIC, ADVANCED, EXPERT, NUM_CHIP_LEVELS};

class Chip : public Entity
{
	protected:
		e_chipType m_cType;
		e_chipSubType m_cSubType;
		e_chipSubSubType m_cSubSubType;
		int m_level, m_cost, m_costLv, m_dmg, m_dmgLv;
		bool m_isEquipped;
		Entity * m_owner;
		SPoint m_target;
		bool m_firstIteration;
		char m_direction;
		SDL_Sprite * m_spriteHUD;
	public:
		Chip(e_chipType a_type, e_chipSubType a_subType, e_chipSubSubType a_subSubType)
			:Entity(),m_cType(a_type),m_cSubType(a_subType),m_cSubSubType(a_subSubType),
			m_level(0),m_cost(0),m_costLv(0),m_dmg(0),m_dmgLv(0),
			m_isEquipped(false), m_owner(NULL){m_eType = CHIP;}
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
		void setDirection(char a_dir){m_direction = a_dir;}
		int getOwnerCenterX()
		{
			if(m_owner)
				return m_owner->getLocation().x + m_owner->getWidthOffsetCenter();
			else
				return 0;
		}
		int getOwnerCenterY()
		{
			if(m_owner)
				return m_owner->getLocation().y + m_owner->getHeightOffsetCenter();
			else
				return 0;
		}
		void initHudSprite()
		{
			if(m_spriteHUD->isSprite())
			{
				m_spriteHUD->start();
				m_spriteHUD->update(m_spriteHUD->getMaxFrames()*SPRITE_SPEED);
				m_spriteHUD->update(1);
				m_spriteHUD->stop();
			}
		}
		int centerAroundOwnerCenterX(){return getOwnerCenterX() - m_sprite->getWidthOffsetCenter();}
		int centerAroundOwnerCenterY(){return getOwnerCenterY() - m_sprite->getHeightOffsetCenter();}
			//define for each spell
		virtual void levelUpUnique(){}
		void levelUp()
		{
			switch(m_cType)
			{
			case MAGIC:
			case WEAPON:
				m_dmgLv = 5;
				if(m_cType == MAGIC)
					m_costLv = 5 * (m_cSubSubType + 1);
				else
					m_costLv = 5 * m_cSubSubType;
				break;
			}
			m_level++;
			m_cost += m_costLv;
			m_dmg += m_dmgLv;
			levelUpUnique();
		}
		virtual void activateUnique(){}
		virtual bool shouldApplyEffect(Entity * a_entity){return false;}
		virtual void applyEffect(Entity * a_entity){}
		void activate()
		{
			if(m_owner)
			{
				if(!m_shouldDraw)
				{
					m_owner->useEnergy(m_cost);
					m_firstIteration = true;
					m_shouldDraw = true;
					m_sprite->start();
					m_sprite->setLoopToBegin(true);
					activateUnique();
				}
				else
				{
					if(m_cType == MAGIC && m_cSubSubType == BASIC)
					{
						m_owner->useEnergy(m_cost);
						m_target.x -= m_sprite->getWidthOffsetCenter();
						m_target.y -= m_sprite->getHeightOffsetCenter();
					}
				}
			}
		}
		virtual void updateUniqueTwo(int a_timePassed){}
		void updateUnique(int a_timePassed, World * a_world)
		{
			if(m_shouldDraw && m_owner)
			{
				updateUniqueTwo(a_timePassed);
				bool collisionMade = false;
				for(int g = 0; g < NUM_GRIDS; ++g)
				{
					for(int i = 0; i < a_world->getGrid(g)->getNumberOfEntities(); ++i)
					{
						if(shouldApplyEffect(a_world->getEntity(i, g)))
						{
							applyEffect(a_world->getEntity(i, g));
							collisionMade = true;
						}
					}
				}
				if((((m_cType == MAGIC && m_cSubSubType == BASIC)
					|| (m_cType == WEAPON && m_cSubType == RANGE))
					&& collisionMade) || (!m_firstIteration && m_sprite->getFrame() == 0))
					deactivate();
				else if(m_sprite->getFrame() == m_sprite->getMaxFrames()-1)
				{
					if((m_cType == MAGIC && m_cSubSubType != BASIC)
						|| (m_cType == WEAPON && m_cSubType != RANGE))
						m_firstIteration = false;
				}
			}
		}
		void deactivate(){m_shouldDraw = false;}
		virtual char * getName(){return "Chip";}
		virtual char * getDescription(){return "Blank chip.";}
		virtual void setSprite(char * a_fileName){}
		void drawHUD(SDL_Surface * a_screen, int a_x, int a_y){m_spriteHUD->draw(a_screen, a_x, a_y);}
};