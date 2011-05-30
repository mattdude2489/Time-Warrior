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
		bool m_isEquipped, m_firstIteration;
		//direction owner is facing
		char m_direction;
		//stats for energy cost, dmg, & time since last atk
		int m_cost, m_dmg, m_timeSinceLastAttack, m_lastSpriteFrame;
		//sprite to display on HUD
		SDL_Sprite * m_spriteHUD;
		//owner of this Chip
		Entity * m_owner;
		//Chip Leveler
		BaseLeveler m_tracker;
		//flags for sprite's flip(s) & rotation
		bool m_isFlipH, m_isFlipV;
		int m_rotateDeg;
		bool m_isCritical;
	public:
		Chip(e_chipType a_type, e_chipSubType a_subType, e_chipSubSubType a_subSubType)
			:Entity(),m_cType(a_type),m_cSubType(a_subType),m_cSubSubType(a_subSubType),
			m_cost(0),m_dmg(0),m_timeSinceLastAttack(0),
			m_isEquipped(false), m_owner(NULL), m_spriteHUD(NULL), m_isFlipH(false), m_isFlipV(false), m_rotateDeg(0),m_isCritical(false)
		{m_eType = CHIP;}// m_tracker = new BaseLeveler();}
		~Chip()
		{
			//free memory from allocated sprites
			if(m_sprite)
				delete m_sprite;
			if(m_spriteHUD)
				delete m_spriteHUD;
			//if(m_tracker)
			//	delete m_tracker;
		}
		void unlock()
		{
			if(m_cType != ARMOR)
			{
				switch(m_cSubType)
				{
				case DIVINE:	m_tracker.unlock(DIVINE_0);	break;
				case LIGHTNING:	m_tracker.unlock(STORM);	break;
				case FIRE:		m_tracker.unlock(FIRE_0);	break;
				case ICE:		m_tracker.unlock(ICE_0);	break;
				default:		m_tracker.unlock(MELEE);	break;
				}
				resetLevelWithBaseLeveler();
			}
		}
		int getXP()
		{
			if(m_cType != ARMOR)
			{
				switch(m_cSubType)
				{
				case DIVINE:	return m_tracker.getAttackXP(DIVINE_0);	break;
				case LIGHTNING:	return m_tracker.getAttackXP(STORM);	break;
				case FIRE:		return m_tracker.getAttackXP(FIRE_0);	break;
				case ICE:		return m_tracker.getAttackXP(ICE_0);	break;
				default:		return m_tracker.getAttackXP(MELEE);	break;
				}
			}
			return 0;
		}
		void resetLevelWithBaseLeveler()
		{
			if(m_cType != ARMOR)
			{
				switch(m_cSubType)
				{
				case DIVINE:	m_stats[LEVEL] = m_tracker.getAttackLevel(DIVINE_0);	break;
				case LIGHTNING:	m_stats[LEVEL] = m_tracker.getAttackLevel(STORM);		break;
				case FIRE:		m_stats[LEVEL] = m_tracker.getAttackLevel(FIRE_0);		break;
				case ICE:		m_stats[LEVEL] = m_tracker.getAttackLevel(ICE_0);		break;
				default:		m_stats[LEVEL] = m_tracker.getAttackLevel(MELEE);		break;
				}
				//printf("level: %d, xp: %d\n", m_stats[LEVEL], getXP());
				setDamageWithLevel();
			}
		}
		void setDamageWithLevel()
		{
			switch(m_cType)
			{
			case MAGIC:
			case WEAPON:
				//if Magic or Weapon, update dmg & cost increase per level
				m_dmg = 5 * (m_cSubSubType + 1);// + m_stats[LEVEL];
				if(m_cType == MAGIC)
					m_cost = 5 * (m_cSubSubType + 1);// + m_stats[LEVEL];
				else
					m_cost = 5 * m_cSubSubType;// + m_stats[LEVEL];
				break;
			}
			switch(m_cSubType)
			{
			case LIGHTNING:
			case PIERCE:	m_dmg = (int)((double)m_dmg * 1.25);	break;//total dmg (w/critical)	: 2.5
			case FIRE:
			case SLASH:		m_dmg = (int)((double)m_dmg * 2);		break;//total dmg (no effect)	: 2
			case ICE:		m_dmg = (int)((double)m_dmg * .5);		break;//total dmg (w/freeze)	: 2
			case BLUNT:		m_dmg = (int)((double)m_dmg * 1.5);		break;//total dmg (w/knockback)	: 1.5
			case RANGE:		m_dmg = m_dmg * (m_cSubSubType + 1);	break;//total dmg (w/knockback)	: 1.5
			}
		}
		void setLevelAndXP(int level, int experience)
		{
			if(m_cType != ARMOR)
			{
				switch(m_cSubType)
				{
				case DIVINE:	m_tracker.setAttackLvXP(DIVINE_0, level, experience);	break;
				case LIGHTNING:	m_tracker.setAttackLvXP(STORM, level, experience);		break;
				case FIRE:		m_tracker.setAttackLvXP(FIRE_0, level, experience);		break;
				case ICE:		m_tracker.setAttackLvXP(ICE_0, level, experience);		break;
				default:		m_tracker.setAttackLvXP(MELEE, level, experience);		break;
				}
				resetLevelWithBaseLeveler();
			}
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
		void makeSpriteMatchDirection()
		{
			//un-flip sprite if it has been flipped in any way
			if(m_isFlipH)
			{
				m_sprite->flipHorizontal();
				m_isFlipH = false;
			}
			if(m_isFlipV)
			{
				m_sprite->flipVertical();
				m_isFlipV = false;
			}
			//un-rotate sprite if it has been rotated in any way
			switch(m_rotateDeg)
			{
			case 90:	m_sprite->rotate270();	break;
			case 270:	m_sprite->rotate90();	break;
			}
			m_rotateDeg = 0;
			int dir = m_direction;
			if(m_cType == MAGIC || m_cSubType == RANGE)
				dir = getTargetDirection();
			switch(dir)
			{
			case KEY_LEFT:
			case ROW_LEFT:
				m_rotateDeg = 270;
				m_sprite->rotate270();
				break;
			case KEY_RIGHT:
			case ROW_RIGHT:
				if(m_cType == WEAPON)
				{
					m_rotateDeg = 270;
					m_sprite->rotate270();
					m_isFlipH = true;
					m_sprite->flipHorizontal();
				}
				else
				{
					m_rotateDeg = 90;
					m_sprite->rotate90();
				}
				break;
			case KEY_DOWN:
			case ROW_DOWN:
				m_isFlipH = m_isFlipV = true;
				m_sprite->flipHorizontal();
				m_sprite->flipVertical();
				break;
			}
		}
		//returns x location to center around owner
		int centerAroundOwnerCenterX(){return m_owner->getCenter().x - m_sprite->getWidthOffsetCenter();}
		//returns y location to center around owner
		int centerAroundOwnerCenterY(){return m_owner->getCenter().y - m_sprite->getHeightOffsetCenter();}
		//updates appropriate variables
		void levelUpUnique()
		{
			if(m_cType != ARMOR)
			{
				switch(m_cSubType)
				{
				case DIVINE:	m_tracker.levelUp(DIVINE_0);	break;
				case LIGHTNING:	m_tracker.levelUp(STORM);		break;
				case FIRE:		m_tracker.levelUp(FIRE_0);		break;
				case ICE:		m_tracker.levelUp(ICE_0);		break;
				default:		m_tracker.levelUp(MELEE);		break;
				}
				resetLevelWithBaseLeveler();
			}
		}
		//virtual func to specially activate inherited classes
		virtual void activateUnique(){}
		//virtual func to specially de-activate inherited classes
		virtual void deactivateUnique(){}
		//virtual func to specially check for inherited classes
		virtual bool shouldApplyEffect(Entity * a_entity){return false;}
		//virtual func to specially apply effect from inherited classes
		virtual void applyEffect(Entity * a_entity){}
		void applyEffectAndStuffForKill(Entity * a_entity)
		{
			applyEffect(a_entity);
			//erase & gain experience from killed entities
			if(a_entity->getStatNumber(HEALTH_CURRENT) <= 0)
			{
				a_entity->setDrawOff();
				m_owner->gainExperience(a_entity->getExperienceFromDefeat(m_owner));
				if((rand()%100)<20)//20 % chance to get a health pot
					m_owner->receiveHPot();
				if((rand()%100)<10)//10 % chance to get a energy pot
					m_owner->receiveEPot();
			}
		}
		//adjusts the target to be centered
		void centerTarget(){m_target.subtract(SPoint(m_sprite->getWidthOffsetCenter(), m_sprite->getHeightOffsetCenter()));}
		//activates current Chip
		void activate()
		{
			if(m_owner && m_stats[LEVEL] > 0)
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
						m_lastSpriteFrame = -1;
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
					bool should = false;
					switch(m_cType)
					{
					case MAGIC:		should = m_cSubSubType == BASIC;	break;
					case WEAPON:	should = m_cSubType == RANGE;		break;
					}
					if(should)
					{
						m_owner->useEnergy(m_cost);
						centerTarget();
						switch(m_cSubType)
						{
						case RANGE:
							setLocation(centerAroundOwnerCenterX(), centerAroundOwnerCenterY());
						case LIGHTNING:
						case ICE:
							makeSpriteMatchDirection();
							m_sprite->setRIndex(m_cSubSubType);
							break;
						}
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
			//auto-end if not a continuous atk & 1st iteration has finished
			bool notContinuous = false;
			switch(m_cType)
			{
			case MAGIC:		notContinuous = m_cSubSubType != BASIC;	break;
			case WEAPON:	notContinuous = m_cSubType != RANGE;	break;
			}
			if(notContinuous)
			{
				if(!m_firstIteration && m_sprite->getFrame() == 0)
					deactivate();
				else if(m_sprite->getFrame() == m_sprite->getMaxFrames()-1)
					m_firstIteration = false;
			}
			//check if should be updated
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
					if(check && m_lastSpriteFrame != m_sprite->getFrame())
					{
						for(int i = 0; i < a_world->getGrid(grid)->getNumberOfEntities(); ++i)
						{
							//check against each entity in grid
							if(shouldApplyEffect(a_world->getEntity(i, grid)))
							{
								//detect collision with Obstacles, but don't apply effect in such case
								if(a_world->getEntity(i, grid)->getType() != OBSTACLE)
								{
									applyEffectAndStuffForKill(a_world->getEntity(i, grid));
									switch(m_cType)
									{
									case WEAPON:
										switch(m_cSubType)
										{
										case BLUNT:
										case RANGE:
										case SLASH:
										case PIERCE:
											m_tracker.GainMeleeXP();
											break;
										}//end weapon subtype
										break;
									case MAGIC:
										switch(m_cSubType)
										{
										case DIVINE:	m_tracker.GainMagicXP(DIVINE_0);	break;
										case LIGHTNING:	m_tracker.GainMagicXP(STORM);		break;
										case FIRE:		m_tracker.GainMagicXP(FIRE_0);		break;
										case ICE:		m_tracker.GainMagicXP(ICE_0);		break;
										}//end magic subtype
										break;
									}//end type Switch
									resetLevelWithBaseLeveler();
								}// end collision if
								collisionMade = true;
							}//end shouldApplyEffect
						}//for loop
					}
					check = false;
				}
				m_lastSpriteFrame = m_sprite->getFrame();
				//auto-end if projectiles collided
				if(!notContinuous && collisionMade)
					deactivate();
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
			int rows = NUM_ROWS;
			switch(m_cSubType)
			{
			case BLUNT:
			case SLASH:		rows--;
			case RANGE:
			case PIERCE:	rows--;	break;
			}
			m_sprite = new SDL_Sprite(a_fileName, FRAME_SIZE, FRAME_SIZE, FRAME_RATE, rows);
			m_spriteHUD = new SDL_Sprite(a_fileName, FRAME_SIZE, FRAME_SIZE, FRAME_RATE, rows);
			m_sprite->setTransparency(COLOR_TRANSPARENT);
			m_spriteHUD->setTransparency(COLOR_TRANSPARENT);
			switch(m_cType)
			{
				//if spell, adjust size based in level
			case MAGIC:
				switch(m_cSubSubType)
				{
				case BASIC:		m_sprite->stretch(50,50);	break;
				case ADVANCED:	m_sprite->stretch(200,200);	break;
				case EXPERT:	m_sprite->stretch(300,300);	break;
				}
				break;
				//if weapon, adjust frame size
			case WEAPON:
				switch(m_cSubType)
				{
				case BLUNT:
				case SLASH:
					if(m_cSubSubType != EXPERT)
					{
						m_sprite->setFrame(FRAME_SIZE, FRAME_SIZE/2);
						m_spriteHUD->setFrame(FRAME_SIZE, FRAME_SIZE/2);
					}
					break;
				case RANGE:		m_sprite->stretch(50,50);	break;
				case PIERCE:
					m_sprite->setFrame(FRAME_SIZE/2, FRAME_SIZE);
					m_spriteHUD->setFrame(FRAME_SIZE/2, FRAME_SIZE);
					break;
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
				if(!(m_cType == WEAPON && m_cSubSubType == EXPERT) || m_cSubType == RANGE || m_cSubType == PIERCE)
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