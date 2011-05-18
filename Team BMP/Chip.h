//Chip class
//Author: John Parks
#pragma once
#include "BaseLeveler.h"
#include "Entity.h"
#include "World.h"
#include "Obstacle.h"


enum e_chipType {ARMOR, MAGIC, WEAPON, NUM_CHIP_TYPES};
enum e_chipSubType {HEAD, TRUNK, LIMB_UPPER, LIMB_LOWER, DIVINE, LIGHTNING, FIRE, ICE, BLUNT, RANGE, SLASH, PIERCE, NUM_TOTAL_CHIP_SUBS, NUM_CHIP_SUBS_PER_TYPE = NUM_TOTAL_CHIP_SUBS/NUM_CHIP_TYPES};
enum e_chipSubSubType {BASIC, ADVANCED, EXPERT, NUM_CHIP_LEVELS};

class Chip : public Entity
{
	protected:
		//ID's of type, sub-type, & sub-sub-type
		e_chipType m_cType;
		e_chipSubType m_cSubType;
		e_chipSubSubType m_cSubSubType;
		//whether or not it's equipped
		bool m_isEquipped;
		//flag to whether or not the sprite is on its 1st animation run-through
		bool m_firstIteration;
		//direction owner is facing
		char m_direction;
		//stats for energy cost, dmg, & time since last atk
		int m_cost, m_costLv, m_dmg, m_dmgLv, m_timeSinceLastAttack;
		//sprite to display on HUD
		SDL_Sprite * m_spriteHUD;
		//owner of this Chip
		Entity * m_owner;
		//Chip Leveler
		BaseLeveler * m_tracker;
	public:
		Chip(e_chipType a_type, e_chipSubType a_subType, e_chipSubSubType a_subSubType)
			:Entity(),m_cType(a_type),m_cSubType(a_subType),m_cSubSubType(a_subSubType),
			m_cost(0),m_costLv(0),m_dmg(0),m_dmgLv(0),m_timeSinceLastAttack(0),
			m_isEquipped(false), m_owner(NULL), m_spriteHUD(NULL), m_tracker(NULL)
		    {m_eType = CHIP;m_stats[LEVEL] = 0; m_tracker = new BaseLeveler();}
		~Chip()
		{
			//free memory from allocated sprites
			if(m_sprite && !m_flags[FLAG_NUDE])
				delete m_sprite;
			if(m_spriteHUD)
				delete m_spriteHUD;
			if(m_tracker)
				delete m_tracker;
		}
		//returns Chip's various types
		e_chipType getType(){return m_cType;}
		e_chipSubType getSubType(){return m_cSubType;}
		e_chipSubSubType getSubSubType(){return m_cSubSubType;}
		//returns cost to activate
		int getCost(){return m_cost;}
		//returns amt of dmg it can cause
		int getDamage(){return m_dmg;}
		//returns whether or not it's equipped to anyone
		bool isEquipped(){return m_isEquipped;}
		//toggle whether or not it's equipped
		void equip(){m_isEquipped = true;}
		void unequip(){m_isEquipped = false;}
		//set it's owner
		void setOwner(Entity * a_owner)
		{
			m_owner = a_owner;
			if(m_owner->getType() == PLAYER)
				m_flags[FLAG_OWNER_PLAYER] = true;
		}
		//set the direction to base its location
		void setDirection(char a_dir){m_direction = a_dir;}
		//returns the horizontal center of its owner's sprite
		int getOwnerCenterX()
		{
			if(m_owner)
				return m_owner->getLocation().x + m_owner->getWidthOffsetCenter();
			else
				return 0;
		}
		//returns the vertical center of its owner's sprite
		int getOwnerCenterY()
		{
			if(m_owner)
				return m_owner->getLocation().y + m_owner->getHeightOffsetCenter();
			else
				return 0;
		}
		//returns x location to center around owner
		int centerAroundOwnerCenterX(){return getOwnerCenterX() - m_sprite->getWidthOffsetCenter();}
		//returns y location to center around owner
		int centerAroundOwnerCenterY(){return getOwnerCenterY() - m_sprite->getHeightOffsetCenter();}
		//virtual func to update inherited-class variables
		virtual void levelUpUniqueTwo(){}
		//updates appropriate variables
		void levelUpUnique()
		{
			switch(m_cType)
			{
			case MAGIC:
			case WEAPON:
				//if Magic or Weapon, update dmg & cost increase per level
				m_dmgLv = 5 + m_cSubSubType;
				if(m_cType == MAGIC)
					m_costLv = 5 * (m_cSubSubType + 1);
				else
					m_costLv = 5 * m_cSubSubType;
				break;
			}
			//update cost & dmg
			m_cost += m_costLv;
			m_dmg += m_dmgLv;
			if(m_cSubType == BLUNT)
				m_dmg = (int)((double)m_dmg *.75);
			//update variables unique to inherited classes
			levelUpUniqueTwo();
		}
		//virtual func to specially activate inherited classes
		virtual void activateUnique(){}
		//virtual func to specially de-activate inherited classes
		virtual void deactivateUnique(){}
		//virtual func to specially check for inherited classes
		virtual bool shouldApplyEffect(Entity * a_entity){return false;}
		//virtual func to specially apply effect from inherited classes
		virtual void applyEffect(Entity * a_entity){}
		//adjusts the target to be centered
		void centerTarget(){m_target.subtract(SPoint(m_sprite->getWidthOffsetCenter(), m_sprite->getHeightOffsetCenter()));}
		//activates current Chip
		void activate()
		{
			if(m_owner)
			{
				//init stuff if not already displayed
				if(!m_flags[FLAG_DRAW])
				{
					//apply cost to activate
					m_owner->useEnergy(m_cost);
					//init sprite stuff for non-Armor types
					if(m_cType != ARMOR)
					{
						m_firstIteration = true;
						m_flags[FLAG_DRAW] = true;
						m_sprite->start();
						if(!(m_cType == WEAPON && m_cSubSubType == EXPERT))
							m_sprite->restart(m_cSubSubType);
						else
							m_sprite->restart(m_cSubSubType-1);
						m_sprite->setLoopToBegin(true);
					}
					//handle special activate for inherited classes
					activateUnique();
				}
				else
				{
					//apply secondary cost to guide Basic spells
					if(m_cType == MAGIC && m_cSubSubType == BASIC)
					{
						m_owner->useEnergy(m_cost);
						centerTarget();
					}
				}
			}
		}
		//2nd virtual func to specially update inherited classes
		virtual void updateUniqueTwo(int a_timePassed){}
		//virtual func to specially update inherited classes
		void updateUnique(int a_timePassed, World * a_world)
		{
			//update last-atk timer
			m_timeSinceLastAttack += a_timePassed;
			//chech if should be updated
			if(m_flags[FLAG_DRAW] && m_owner && m_cType != ARMOR)
			{
				//update stuff unique to inherited classes
				updateUniqueTwo(a_timePassed);
				//check grids for entity collision
				bool collisionMade = false, check = false;
				//@ most, check 4 surrounding grids (specified by each of sprite's corners)
				int ul, ur, dl, dr, grid;
				ul = a_world->getLocationGrid(m_location.x, m_location.y);
				ur = a_world->getLocationGrid(m_location.x+m_sprite->getWidth(), m_location.y);
				dl = a_world->getLocationGrid(m_location.x, m_location.y+m_sprite->getHeight());
				dr = a_world->getLocationGrid(m_location.x+m_sprite->getWidth(), m_location.y+m_sprite->getHeight());
				//checks the specified grids
				for(int g = 0; g < 4; ++g)
				{
					//only check the grid if it should & has NOT been checked yet
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
							//check against each entity in grid
							if(shouldApplyEffect(a_world->getEntity(i, grid)))
							{
								//detect collision with Obstacles, but don't apply effect in such case
								if(a_world->getEntity(i, grid)->getType() != OBSTACLE)
								{
									applyEffect(a_world->getEntity(i, grid));
									switch(m_cType)
									{
									case WEAPON:
										switch(m_cSubType)
										{
										case BLUNT:
										case SLASH:
										case PIERCE:
										case RANGE:
											m_tracker->GainMeleeXP();
											break;
										}//end weapon subtype
										break;
									case MAGIC:
										switch(m_cSubType)
										{
										case DIVINE:
											m_tracker->GainMagicXP(0);
											break;
										case LIGHTNING:
											m_tracker->GainMagicXP(1);
											break;
										case FIRE:
											m_tracker->GainMagicXP(2);
											break;
										case ICE:
											m_tracker->GainMagicXP(3);
											break;
										}//end magic subtype
										break;
									}//end type Switch
								}// end collision if
								collisionMade = true;
							}//end shouldApplyEffect
						}//for loop
					}
					check = false;
				}
				//auto-end if projectiles collided or sprite finished iteration
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
		//de-activates current Chip
		void deactivate(){setDrawOff();deactivateUnique();}
		//returns name of Magic spell or Weapon
		virtual char * getName(){return "Chip";}
		//returns description of Magic spell or Weapon
		virtual char * getDescription(){return "Blank chip.";}
		//create new sprite based on filename (file must have certain format/organization)
		void setSprite(char * a_fileName)
		{
			//create new sprites & init transparency
			m_sprite = new SDL_Sprite(a_fileName, FRAME_SIZE, FRAME_SIZE, FRAME_RATE, NUM_ROWS);
			m_spriteHUD = new SDL_Sprite(a_fileName, FRAME_SIZE, FRAME_SIZE, FRAME_RATE, NUM_ROWS);
			m_sprite->setTransparency(COLOR_TRANSPARENT);
			m_spriteHUD->setTransparency(COLOR_TRANSPARENT);
			switch(m_cType)
			{
				//if spell, adjust size based in level
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
				//if weapon, adjust frame size
			case WEAPON:
				if(m_cSubSubType != EXPERT)
				{
					m_sprite->setFrame(FRAME_SIZE, FRAME_SIZE/2);
					m_spriteHUD->setFrame(FRAME_SIZE, FRAME_SIZE/2);
				}
				break;
			}
			//use correct row based on level
			switch(m_cType)
			{
			case ARMOR:
				m_sprite->setRIndex(m_cSubType);
				m_spriteHUD->setRIndex(m_cSubType);
				break;
			case MAGIC:
			case WEAPON:
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
				//dispaly last available frame as "symbol" for HUD sprite
				m_spriteHUD->setCIndex(m_spriteHUD->getMaxFrames()-1);
				break;
			}
		}
		//draws HUD sprite @ specified location
		void drawHUD(SDL_Surface * a_screen, int a_x, int a_y){m_spriteHUD->draw(a_screen, a_x, a_y);}
};