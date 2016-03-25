//Chip class
//Author: John Parks
#pragma once
#include "BaseLeveler.h"
#include "Entity.h"
#include "World.h"
#include "Obstacle.h"

enum e_chipType			{ARMOR, MAGIC, WEAPON, NUM_CHIP_TYPES};
enum e_chipSubType		{HEAD, TRUNK, LIMB_UPPER, LIMB_LOWER, DIVINE, LIGHTNING, FIRE, ICE, BLUNT, RANGE, SLASH, PIERCE, NUM_TOTAL_CHIP_SUBS, NUM_CHIP_SUBS_PER_TYPE = NUM_TOTAL_CHIP_SUBS/NUM_CHIP_TYPES};
enum e_chipSubSubType	{BASIC, ADVANCED, EXPERT, NUM_CHIP_LEVELS};

class Chip : public Entity
{
	protected:
		//ID's of type, sub-type, & sub-sub-type
		e_chipType m_cType;
		e_chipSubType m_cSubType;
		e_chipSubSubType m_cSubSubType;
		//whether or not it's equipped/crit, flags for sprite stuff
		bool m_isEquipped, m_isCritical, m_isFlipH, m_isFlipV, m_firstIteration, m_onlyAnimateOnce;
		//direction owner is facing
		char m_direction;
		//stats for energy-cost/dmg/time, sprite info
		int m_cost, m_dmg, m_timeSinceLastAttack, m_lastSpriteFrame, m_rotateDeg, m_stretchFactor;
		//sprite to display on HUD
		SDL_Sprite * m_spriteHUD;
		//input rect for HUD sprite
		SRect m_spriteRectHUD;
		//owner of this Chip
		Entity * m_owner;
		//Chip Leveler
		BaseLeveler m_tracker;
	public:
		Chip(e_chipType a_type, e_chipSubType a_subType, e_chipSubSubType a_subSubType)
			:Entity(),m_cType(a_type),m_cSubType(a_subType),m_cSubSubType(a_subSubType),
			m_isEquipped(false),m_isFlipH(false),m_isFlipV(false),m_isCritical(false),m_onlyAnimateOnce(false),
			m_cost(0),m_dmg(0),m_timeSinceLastAttack(0), m_rotateDeg(0), m_stretchFactor(1),
			m_owner(NULL), m_spriteHUD(NULL)
		{
			m_eType = CHIP;
			resetLevelWithBaseLeveler();
			switch(m_cType)
			{
			case MAGIC:		m_onlyAnimateOnce = m_cSubSubType != BASIC;	break;
			case WEAPON:	m_onlyAnimateOnce = m_cSubType != RANGE;	break;
			}
		}
		~Chip()
		{
			destroyChip();
		}
		void destroyChip()
		{
			//free memory from allocated sprites
			if(m_sprite)
				delete m_sprite;
			if(m_spriteHUD)
				delete m_spriteHUD;
		}
		bool isChanceSuccessful(int a_percentage)
		{
			//only calc crit if a_percentage is not an extreme
			if(a_percentage > 0 && a_percentage < 100)
			{
				//calc random percentage
				int num = rand() % 100 + 1;
				//if the percentage or remainder easily divides 100 (2, 4, 5, 10, 20, 25, 50), check again
				if(!(100 % a_percentage) || !(100 % (100 - a_percentage)))
				{
					if(a_percentage < 50)
						return !(num % (100 / a_percentage));
					else
						return !(!(num % (100 / (100 - a_percentage))));
				}
				//else just check if the num is within range
				else
					return num <= a_percentage;
			}
			else if(a_percentage >= 100)
				return true;
			else
				return false;
		}
		virtual e_stats getResistType(){return DEFENSE;}
		virtual char * getResistTypeName(){return "Unknown";}
		void unlock()
		{
			switch(m_cType)
			{
			case MAGIC:
			case WEAPON:
				switch(m_cSubType)
				{
				case DIVINE:	m_tracker.unlock(DIVINE_0);	break;
				case LIGHTNING:	m_tracker.unlock(STORM);	break;
				case FIRE:		m_tracker.unlock(FIRE_0);	break;
				case ICE:		m_tracker.unlock(ICE_0);	break;
				default:		m_tracker.unlock(MELEE);	break;
				}
				resetLevelWithBaseLeveler();
				break;
			}
		}
		void lock()
		{
			switch(m_cType)
			{
			case MAGIC:
			case WEAPON:
				switch(m_cSubType)
				{
				case DIVINE:	m_tracker.lock(DIVINE_0);	break;
				case LIGHTNING:	m_tracker.lock(STORM);		break;
				case FIRE:		m_tracker.lock(FIRE_0);		break;
				case ICE:		m_tracker.lock(ICE_0);		break;
				default:		m_tracker.lock(MELEE);		break;
				}
				resetLevelWithBaseLeveler();
				break;
			}
		}
		int getXP()
		{
			switch(m_cType)
			{
			case MAGIC:
			case WEAPON:
				switch(m_cSubType)
				{
				case DIVINE:	return m_tracker.getAttackXP(DIVINE_0);	break;
				case LIGHTNING:	return m_tracker.getAttackXP(STORM);	break;
				case FIRE:		return m_tracker.getAttackXP(FIRE_0);	break;
				case ICE:		return m_tracker.getAttackXP(ICE_0);	break;
				default:		return m_tracker.getAttackXP(MELEE);	break;
				}
				break;
			}
			return 0;
		}
		void resetLevelWithBaseLeveler()
		{
			switch(m_cType)
			{
			case MAGIC:
			case WEAPON:
				switch(m_cSubType)
				{
				case DIVINE:	m_stats[LEVEL] = m_tracker.getAttackLevel(DIVINE_0);	break;
				case LIGHTNING:	m_stats[LEVEL] = m_tracker.getAttackLevel(STORM);		break;
				case FIRE:		m_stats[LEVEL] = m_tracker.getAttackLevel(FIRE_0);		break;
				case ICE:		m_stats[LEVEL] = m_tracker.getAttackLevel(ICE_0);		break;
				default:		m_stats[LEVEL] = m_tracker.getAttackLevel(MELEE);		break;
				}
				setDamageWithLevel();
				break;
			}
		}
		void setDamageWithLevel()
		{
			switch(m_cType)
			{
			case MAGIC:
			case WEAPON:
				//if Magic or Weapon, update dmg & cost increase per level
				m_dmg = 5 * (m_cSubSubType + 1) + m_stats[LEVEL]/2;
				if(m_cType == MAGIC)
					m_cost = 5 * (m_cSubSubType + 1) + m_stats[LEVEL]/2;
				else if(m_cSubSubType != BASIC)
					m_cost = 5 * m_cSubSubType + m_stats[LEVEL]/2;
				break;
			}
			switch(m_cSubType)
			{
			case LIGHTNING:
			case PIERCE:	m_dmg = (int)((double)m_dmg * .75);		break;//total % (w/crit)	: 150
			case ICE:		m_dmg = (int)((double)m_dmg * .25);		break;//total % (w/freeze)	: 100
			case BLUNT:		m_dmg = (int)((double)m_dmg * .75);		break;//total % (w/knckbck)	: 75
			default:												break;//total %				: 100
			}
		}
		void resetAndLock(){setLevelAndXP(STARTING_LV, STARTING_XP, false);}
		void setLevelAndXP(int level, int experience, bool a_unlock)
		{
			setLevelAndXP(level, experience);
			if(a_unlock)
				unlock();
			else
				lock();
		}
		void setLevelAndXP(int level, int experience)
		{
			switch(m_cType)
			{
			case MAGIC:
			case WEAPON:
				switch(m_cSubType)
				{
				case DIVINE:	m_tracker.setAttackLvXP(DIVINE_0, level, experience);	break;
				case LIGHTNING:	m_tracker.setAttackLvXP(STORM, level, experience);		break;
				case FIRE:		m_tracker.setAttackLvXP(FIRE_0, level, experience);		break;
				case ICE:		m_tracker.setAttackLvXP(ICE_0, level, experience);		break;
				default:		m_tracker.setAttackLvXP(MELEE, level, experience);		break;
				}
				resetLevelWithBaseLeveler();
				break;
			}
		}
		//returns Chip's various types
		e_chipType getType(){return m_cType;}
		e_chipSubType getSubType(){return m_cSubType;}
		e_chipSubSubType getSubSubType(){return m_cSubSubType;}
		char * getTypeName(e_chipType a_cType)
		{
			switch(a_cType)
			{
			case ARMOR:		return "Armor";		break;
			case MAGIC:		return "Magic";		break;
			case WEAPON:	return "Weapon";	break;
			default:		return "Unknown";
			}
		}
		char * getTypeName(){return getTypeName(m_cType);}
		char * getSubTypeName(e_chipSubType a_cSubType)
		{
			switch(a_cSubType)
			{
			case HEAD:			return "Head";		break;
			case TRUNK:			return "Torso";		break;
			case LIMB_UPPER:	return "Arm";		break;
			case LIMB_LOWER:	return "Leg";		break;
			case DIVINE:		return "Divine";	break;
			case LIGHTNING:		return "Electric";	break;
			case FIRE:			return "Fire";		break;
			case ICE:			return "Ice";		break;
			case BLUNT:			return "Blunt";		break;
			case RANGE:			return "Range";		break;
			case SLASH:			return "Slash";		break;
			case PIERCE:		return "Pierce";	break;
			default:			return "Unknown";
			}
		}
		char * getSubTypeName(){return getSubTypeName(m_cSubType);}
		//returns cost to activate
		int getCost(){return m_cost;}
		//returns amt of dmg it can cause
		int getDamage(){return m_dmg;}
		//returns whether or not it's equipped to anyone
		bool isEquipped(){return m_isEquipped;}
		//toggle whether or not it's equipped
		void equip(){
			m_isEquipped = true;
			//buff stats gained from armor
			if(m_cType == ARMOR)
				activate(NULL);
		}
		void unequip(){m_isEquipped = false;deactivate();}
		//set it's owner
		void setOwner(Entity * a_owner)
		{
			m_owner = a_owner;
			if(m_owner->getType() == PLAYER)
				m_flags[FLAG_OWNER_PLAYER] = true;
			switch(m_cType)
			{
			case MAGIC:
				switch(m_cSubType)
				{
				case DIVINE:	setSprite("Sprites/magic_divine.bmp");		break;
				case LIGHTNING:	setSprite("Sprites/magic_lightning.bmp");	break;
				case FIRE:		setSprite("Sprites/magic_fire.bmp");		break;
				case ICE:		setSprite("Sprites/magic_ice.bmp");			break;
				}
				break;
			case WEAPON:
				switch(m_cSubType)
				{
				case BLUNT:		setSprite("Sprites/weapon_blunt.bmp");		break;
				case RANGE:		setSprite("Sprites/weapon_range.bmp");		break;
				case SLASH:		setSprite("Sprites/weapon_slash.bmp");		break;
				case PIERCE:	setSprite("Sprites/weapon_pierce.bmp");		break;
				}
				break;
			}
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
			//let other entity know who it has been hit by
			if(m_flags[FLAG_OWNER_PLAYER])
				a_entity->hitFromPlayer();
			//erase & gain experience from killed entities
			if(a_entity->getStatNumber(HEALTH_CURRENT) <= 0 && m_owner)
			{
				if(a_entity->getType() != PLAYER)
				{
					a_entity->setDrawOff();
					a_entity->stopAttack();
				}
				m_owner->gainExperience(a_entity->getExperienceFromDefeat(m_owner));
				m_owner->unlockEarnedAttacks();
				if(m_owner->getPots(POT_HEALTH) < MAX_POTS)
				{
					if(isChanceSuccessful(20))//20 % chance to get a health pot
						m_owner->receivePot(POT_HEALTH);
				}
				if(m_owner->getPots(POT_ENERGY) < MAX_POTS)
				{
					if(isChanceSuccessful(10))//10 % chance to get a energy pot
						m_owner->receivePot(POT_ENERGY);
				}
				//chance for armor drop
				int chance = 0;
				switch(a_entity->getType())
				{
				case MINION:	chance = 5;		break;
				case BOSS:		chance = 20;	break;
				}
				if(isChanceSuccessful(chance))
					m_owner->pickUpArmor();
			}
		}
		//plays sound effect
		void playSoundEffect(AudioHandler * ah)
		{
			switch(m_cSubType)
			{
			case DIVINE:	ah->playEffect(E_DIVINE);		break;
			case LIGHTNING:	ah->playEffect(E_LIGHTNING);	break;
			case FIRE:		ah->playEffect(E_FIRE);			break;
			case ICE:		ah->playEffect(E_ICE);			break;
			case BLUNT:		ah->playEffect(E_BLUNT);		break;
			case RANGE:		ah->playEffect(E_BOW);			break;
			case SLASH:		ah->playEffect(E_SLASH);		break;
			case PIERCE:	ah->playEffect(E_PIERCE);		break;
			}
		}
		//activates current Chip
		void activate(AudioHandler * ah)
		{
			if(m_owner && m_stats[LEVEL] > 0)
			{
				//init stuff if not already displayed
				if(!m_flags[FLAG_DRAW])
				{
					//apply cost to activate
					m_owner->useEnergy(m_cost);
					//init sprite stuff for attacks
					switch(m_cType)
					{
					case MAGIC:
					case WEAPON:
						m_firstIteration = true;
						m_lastSpriteFrame = -1;
						m_flags[FLAG_DRAW] = true;
						m_sprite->start();
						if(!(m_cType == WEAPON && m_cSubSubType == EXPERT))
							m_sprite->restart(m_cSubSubType);
						else
							m_sprite->restart(m_cSubSubType-1);
						m_sprite->setLoopToBegin(true);
						
						//add sound
						playSoundEffect(ah);
						break;
					}
					//handle special activate for inherited classes
					activateUnique();
				}
				else
				{
					//apply secondary cost to guide/re-shoot projectiles
					bool should = false;
					switch(m_cType)
					{
					case MAGIC:		should = m_cSubSubType == BASIC;	break;
					case WEAPON:	should = m_cSubType == RANGE;		break;
					}
					if(should)
					{
						m_owner->useEnergy(m_cost);
						playSoundEffect(ah);
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
		void updateUnique(int a_timePassed, World * a_world, AudioHandler * ah)
		{
			//update last-atk timer
			m_timeSinceLastAttack += a_timePassed;
			//auto-end if not a continuous atk & 1st iteration has finished
			
			if(m_onlyAnimateOnce)
			{
				if(!m_firstIteration && m_sprite->getFrame() == 0)
					deactivate();
				else if(m_sprite->getFrame() == m_sprite->getMaxFrames()-1)
					m_firstIteration = false;
			}
			//check if should be updated
			if(m_flags[FLAG_DRAW] && m_owner && (m_cType == MAGIC || m_cType == WEAPON))
			{
				//update stuff unique to inherited classes
				updateUniqueTwo(a_timePassed);
				//check grids for entity collision
				bool collisionMade = false, check = false;
				//checks the specified grids
				for(int g = 0; g < NUM_GRIDS; ++g)
				{
					//only check the grid if it should & has NOT been checked yet
					if(a_world->isGridVisible(g) && m_lastSpriteFrame != m_sprite->getFrame() && (!m_onlyAnimateOnce || (m_onlyAnimateOnce && m_sprite->getFrame() == m_sprite->getMaxFrames()-1)))
					{
						for(int i = 0; i < a_world->getGrid(g)->getNumberOfEntities(); ++i)
						{
							//check against each entity in grid
							if(shouldApplyEffect(a_world->getEntity(i, g)) )
							{
								//detect collision with Obstacles, but don't apply effect in such case
								if(a_world->getEntity(i, g)->getType() != OBSTACLE)
								{
									applyEffectAndStuffForKill(a_world->getEntity(i, g));
									switch(m_cType)
									{
									case MAGIC:
										switch(m_cSubType)
										{
										case DIVINE:	m_tracker.GainMagicXP(DIVINE_0);	break;
										case LIGHTNING:	m_tracker.GainMagicXP(STORM);		break;
										case FIRE:		m_tracker.GainMagicXP(FIRE_0);		break;
										case ICE:		m_tracker.GainMagicXP(ICE_0);		break;
										}//end magic subtype
										break;
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
									}//end type Switch
									resetLevelWithBaseLeveler();
									m_owner->unlockEarnedAttacks();
								}// end collision if
								collisionMade = true;
							}//end shouldApplyEffect
						}//for loop
					}
					check = false;
				}
				m_lastSpriteFrame = m_sprite->getFrame();
				//auto-end if projectiles collided
				if(!m_onlyAnimateOnce && collisionMade)
					deactivate();
			}
		}
		//de-activates current Chip
		void deactivate()
		{
			setDrawOff();
			this->deactivateUnique();
		}
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
			m_spriteHUD = new SDL_Sprite("Sprites/Icons.bmp", FRAME_SIZE, FRAME_SIZE, FRAME_RATE, NUM_ICON_ROWS);
			m_sprite->setTransparency(COLOR_TRANSPARENT);
			m_spriteHUD->setTransparency(COLOR_TRANSPARENT);
			//adjust size to be appropriate to owner
			if(m_owner)
			{
				int dim;
				if(m_owner->getHeightOffsetCenter()*2 > FRAME_SIZE)
					dim = m_owner->getHeightOffsetCenter()*2;
				else
					dim = m_owner->getWidthOffsetCenter()*2;
				if(dim % FRAME_SIZE == 0)
					m_stretchFactor = dim / FRAME_SIZE;
				else
					m_stretchFactor = (dim / FRAME_SIZE) + 1;
			}
			float fullSize = float(m_stretchFactor * 100);
			switch(m_cType)
			{
				//if spell, adjust size based in level
			case MAGIC:
				switch(m_cSubSubType)
				{
				case BASIC:		m_sprite->stretch(fullSize/3,fullSize/3);	break;
				case ADVANCED:	m_sprite->stretch(fullSize*2,fullSize*2);	break;
				case EXPERT:	m_sprite->stretch(fullSize*3,fullSize*3);	break;
				}
				break;
				//if weapon, adjust frame size
			case WEAPON:
				m_sprite->stretch(fullSize,fullSize);
				switch(m_cSubType)
				{
				case BLUNT:
				case SLASH:
					switch(m_cSubSubType)
					{
					case BASIC:
					case ADVANCED:
						m_sprite->setFrame(FRAME_SIZE*m_stretchFactor, (FRAME_SIZE/2)*m_stretchFactor);
						//m_spriteHUD->setFrame(FRAME_SIZE, FRAME_SIZE/2);
						break;
					}
					break;
				case RANGE:		m_sprite->stretch(fullSize/2,fullSize/2);	break;
				case PIERCE:
					m_sprite->setFrame((FRAME_SIZE/2)*m_stretchFactor, FRAME_SIZE*m_stretchFactor);
					//m_spriteHUD->setFrame(FRAME_SIZE/2, FRAME_SIZE);
					break;
				}
				break;
			}
			//use correct row based on level
			switch(m_cType)
			{
			case ARMOR:
				m_sprite->setRIndex(m_cSubType);
				m_spriteRectHUD.setDimension(SPoint(FRAME_SIZE,FRAME_SIZE));
				break;
			case MAGIC:
			case WEAPON:
				if(!(m_cType == WEAPON && m_cSubSubType == EXPERT) || m_cSubType == RANGE || m_cSubType == PIERCE)
				{
					m_sprite->setRIndex(m_cSubSubType);
				}
				else
				{
					m_sprite->setRIndex(m_cSubSubType-1);
				}
				//set up input rect for HUD sprite
				m_spriteRectHUD.setDimension(SPoint(FRAME_SIZE,FRAME_SIZE));
				break;
			}
		}
		bool isWithinSpriteRect(int a_rectLocX, int a_rectLocY, SPoint a_point){
			m_spriteRectHUD.setPosition(SPoint(a_rectLocX,a_rectLocY));
			return m_spriteRectHUD.contains(a_point);
		}
		//draws HUD sprite @ specified location
		void drawHUD(SDL_Surface * a_screen, int a_x, int a_y, e_iconBG a_iconBG)
		{
			//draw background
			if(a_iconBG != ICON_BG_NONE)
			{
				m_spriteHUD->setRIndex(ICON_ROW_BACKGROUND);
				m_spriteHUD->setCIndex(a_iconBG);
				m_spriteHUD->draw(a_screen, a_x, a_y);
			}

			//draw icon
			switch(m_cType)
			{
			case ARMOR:
				m_spriteHUD->setRIndex(ICON_ROW_HEAD + (m_cSubType - HEAD));
				m_spriteHUD->setCIndex(m_owner->getSpriteNum());
				break;
			case MAGIC:
				m_spriteHUD->setRIndex(ICON_ROW_DIVINE + (m_cSubType - DIVINE));
				m_spriteHUD->setCIndex(m_cSubSubType);
				break;
			case WEAPON:
				m_spriteHUD->setRIndex(ICON_ROW_BLUNT + (m_cSubType - BLUNT));
				m_spriteHUD->setCIndex(m_cSubSubType);
				break;
			}
			m_spriteHUD->draw(a_screen, a_x, a_y);
		}
		void drawHUD(SDL_Surface * a_screen, int a_x, int a_y)
		{
			drawHUD(a_screen,a_x,a_y,ICON_BG_NONE);
		}
		int getWidthOffsetCenterHUD(){return m_spriteHUD->getWidthOffsetCenter();}
		int getHeightOffsetCenterHUD(){return m_spriteHUD->getHeightOffsetCenter();}
		void sell()
		{
			if(m_cType == ARMOR)
			{
				int resist = 0;
				for(int i = RESISTANCE_FIRE; i < RESISTANCE_FIRE+3; ++i)
				{
					if(m_stats[i] > resist)
						resist = m_stats[i];
				}
				//sell for health pot if buffs are equal or def is more
				if(m_stats[DEFENSE] >= resist)
				{
					if(m_owner->getPots(POT_HEALTH) < MAX_POTS)
						m_owner->receivePot(POT_HEALTH);
				}
				//sell for energy pot if buffs are equal or resist is more
				if(resist >= m_stats[DEFENSE])
				{
					if(m_owner->getPots(POT_ENERGY) < MAX_POTS)
						m_owner->receivePot(POT_ENERGY);
				}
			}
		}
};