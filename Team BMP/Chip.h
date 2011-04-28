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
		int m_cost, m_costLv, m_dmg, m_dmgLv, m_timeSinceLastAttack;
		bool m_isEquipped;
		Entity * m_owner;
		bool m_firstIteration;
		char m_direction;
		SDL_Sprite * m_spriteHUD;
	public:
		Chip(e_chipType a_type, e_chipSubType a_subType, e_chipSubSubType a_subSubType)
			:Entity(),m_cType(a_type),m_cSubType(a_subType),m_cSubSubType(a_subSubType),
			m_cost(0),m_costLv(0),m_dmg(0),m_dmgLv(0),m_timeSinceLastAttack(0),
			m_isEquipped(false), m_owner(NULL){m_eType = CHIP;m_level = 0;m_spriteHUD = NULL;}
		~Chip()
		{
			if(m_sprite && !nude)
				delete m_sprite;
			if(m_spriteHUD)
				delete m_spriteHUD;
		}
		e_chipType getType(){return m_cType;}
		e_chipSubType getSubType(){return m_cSubType;}
		e_chipSubSubType getSubSubType(){return m_cSubSubType;}
		int getLevel(){return m_level;}
		int getCost(){return m_cost;}
		int getDamage(){return m_dmg;}
		bool isEquipped(){return m_isEquipped;}
		void toggleEquip(){m_isEquipped = !m_isEquipped;}
		void setOwner(Entity * a_owner)
		{
			m_owner = a_owner;
			if(m_owner->getType() == PLAYER)
				m_chipOwnerPlayer = true;
		}
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
				m_dmgLv = 5 + m_cSubSubType;
				if(m_cType == MAGIC)
					m_costLv = 5 * (m_cSubSubType + 1);
				else
					m_costLv = 5 * m_cSubSubType;
				break;
			}
			m_level++;
			m_cost += m_costLv;
			m_dmg += m_dmgLv;
			if(m_cSubType == BLUNT)
				m_dmg = (int)((double)m_dmg *.75);
			levelUpUnique();
		}
		virtual void activateUnique(){}
		virtual void deactivateUnique(){}
		virtual bool shouldApplyEffect(Entity * a_entity){return false;}
		virtual void applyEffect(Entity * a_entity){}
		void centerTarget(){m_target.subtract(SPoint(m_sprite->getWidthOffsetCenter(), m_sprite->getHeightOffsetCenter()));}
		void activate()
		{
			if(m_owner)
			{
				if(!m_shouldDraw)
				{
					m_owner->useEnergy(m_cost);
					if(m_cType != ARMOR)
					{
						m_firstIteration = true;
						m_shouldDraw = true;
						m_sprite->start();
						if(m_cType == WEAPON && m_cSubSubType == EXPERT)
							m_sprite->restart(m_cSubSubType-1);
						else
							m_sprite->restart(m_cSubSubType);
						m_sprite->setLoopToBegin(true);
					}
					activateUnique();
				}
				else
				{
					if(m_cType == MAGIC && m_cSubSubType == BASIC)
					{
						m_owner->useEnergy(m_cost);
						centerTarget();
					}
				}
			}
		}
		virtual void updateUniqueTwo(int a_timePassed){}
		void updateUnique(int a_timePassed, World * a_world)
		{
			m_timeSinceLastAttack += a_timePassed;
			if(m_shouldDraw && m_owner && m_cType != ARMOR)
			{
				updateUniqueTwo(a_timePassed);
				bool collisionMade = false, check = false;
				int ul, ur, dl, dr, grid;
				ul = a_world->getLocationGrid(m_location.x, m_location.y);
				ur = a_world->getLocationGrid(m_location.x+m_sprite->getWidth(), m_location.y);
				dl = a_world->getLocationGrid(m_location.x, m_location.y+m_sprite->getHeight());
				dr = a_world->getLocationGrid(m_location.x+m_sprite->getWidth(), m_location.y+m_sprite->getHeight());
				for(int g = 0; g < 4; ++g)
				{
					switch(g)
					{
					case 0:	grid = ul;	check = true;									break;
					case 1:	grid = ur;	check = grid != ul;								break;
					case 2:	grid = dl;	check = grid != ul && grid != ur;				break;
					case 3:	grid = dr;	check = grid != ul && grid != ur && grid != dl;	break;
					}
					if(check)
					{
						for(int i = 0; i < a_world->getGrid(grid)->getNumberOfEntities(); ++i)
						{
							if(shouldApplyEffect(a_world->getEntity(i, grid)))
							{
								if(a_world->getEntity(i, grid)->getType()!= TREE)
									applyEffect(a_world->getEntity(i, grid));
								collisionMade = true;
							}
						}
					}
					check = false;
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
		void deactivate(){m_shouldDraw = false;deactivateUnique();}
		virtual char * getName(){return "Chip";}
		virtual char * getDescription(){return "Blank chip.";}
		void setSprite(char * a_fileName)
		{
			m_sprite = new SDL_Sprite(a_fileName, FRAME_SIZE, FRAME_SIZE, FRAME_RATE, NUM_ROWS);
			m_spriteHUD = new SDL_Sprite(a_fileName, FRAME_SIZE, FRAME_SIZE, FRAME_RATE, NUM_ROWS);
			m_sprite->setTransparency(COLOR_TRANSPARENT);
			m_spriteHUD->setTransparency(COLOR_TRANSPARENT);

			switch(m_cType)
			{
			case MAGIC:
				switch(m_cSubSubType)
				{
				case BASIC:
					m_sprite->stretch(50,50);
					break;
				case ADVANCED:
					m_sprite->stretch(200,200);
					break;
				case EXPERT:
					m_sprite->stretch(300,300);
					break;
				}
				break;
			case WEAPON:
				if(m_cSubSubType != EXPERT)
				{
					m_sprite->setFrame(FRAME_SIZE, FRAME_SIZE/2);
					m_spriteHUD->setFrame(FRAME_SIZE, FRAME_SIZE/2);
				}
				break;
			}
			if(!(m_cType == WEAPON && m_cSubSubType == EXPERT))
			{
				m_sprite->setRIndex(m_cSubSubType);
				m_spriteHUD->setRIndex(m_cSubSubType);
			}
			else
			{
				m_sprite->setRIndex(m_cSubSubType-1);
				m_spriteHUD->setRIndex(m_cSubSubType-1);
			}
			m_spriteHUD->setCIndex(m_spriteHUD->getMaxFrames()-1);
		}
		void drawHUD(SDL_Surface * a_screen, int a_x, int a_y){m_spriteHUD->draw(a_screen, a_x, a_y);}
};