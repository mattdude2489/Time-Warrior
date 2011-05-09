//Magic class
//Author: John Parks
#pragma once
#include "Chip.h"

class Magic : public Chip
{
	protected:
		//stats for combo bonuses
		int m_dmgCombo, m_dmgComboLv;
	public:
		Magic(e_chipSubType a_subType, e_chipSubSubType a_subSubType)
			:Chip(MAGIC, a_subType, a_subSubType),
			m_dmgCombo(0),m_dmgComboLv(0){}
		int getComboBonus(){return m_dmgCombo;}
		void levelUpUnique(){m_dmgCombo += m_dmgComboLv;}
		void activateUnique()
		{
			//adjust the target position
			centerTarget();
			//set the spell's initial position
			switch(m_cSubSubType)
			{
			case BASIC:
			case ADVANCED:
				//Basic & Advanced spells centered on owner
				setLocation(centerAroundOwnerCenterX(), centerAroundOwnerCenterY());
				break;
			case EXPERT:
				//Expert spells @ target
				setLocation(m_target);
				break;
			}
			//heal if using Divine
			if(m_cSubType == DIVINE)
				m_owner->heal(m_owner->getTotalDamageDealt(m_dmg,MAGIC));
		}
		bool shouldApplyEffect(Entity * a_entity)
		{
			//do not apply spell's dmg/effect to certain entities
			owner
			if(a_entity == m_owner)
				return false;
			switch(a_entity->getType())
			{
			case CHIP:
			case NPC:
				return false;
				break;
			//check for collision between spell & entity
			default:
				switch(m_cSubType)
				{
				case DIVINE:
					return collideSimple(a_entity);
					break;
				case LIGHTNING:
				case FIRE:
				case ICE:
					if(a_entity->getType() != m_owner->getType())
						return collideSimple(a_entity);
					else
						return false;
					break;
				default:
					return false;
				}
			}
		}
		void applyEffect(Entity * a_entity)
		{
			//apply effects based on type of spell
			switch(m_cSubType)
			{
			case DIVINE:
				//heal entities of same owner-type
				if(a_entity->getType() == m_owner->getType())
					a_entity->heal(m_owner->getTotalDamageDealt(m_dmg,MAGIC));
			case LIGHTNING:
			case FIRE:
			case ICE:
				//dmg entities of non owner-type
				if(a_entity->getType() != m_owner->getType())
					a_entity->hit(m_owner->getTotalDamageDealt(m_dmg,MAGIC), m_cSubType);
				break;
			}
			//let other entity know who it has been hit by
			if(a_entity->getType()!= m_owner->getType()){
				a_entity->hitFromPlayer();}
			//erase & gain experience from killed entities
			if(a_entity->getStatNumber(HEALTH_CURRENT) <= 0)
			{
				a_entity->setDrawOff();
				m_owner->gainExperience(a_entity->getExperienceFromDefeat(m_owner));
			}
		}
		void updateUniqueTwo(int a_timePassed)
		{
			if(m_flags[FLAG_DRAW])
			{
				switch(m_cSubSubType)
				{
				case BASIC:
					{
						//move Basic spells (projectiles)
						if(moveToTarget((int)(SPEED_MAGIC*a_timePassed)))
						{
							//end spell if it has expired
							if(m_timers[TIMER_GENERAL] >= TIME_EXPIRE)
								deactivate();
						}
					}
					break;
				case ADVANCED:
					//adjust Advanced spells (self-radial) to maintain player-based location
					setLocation(centerAroundOwnerCenterX(), centerAroundOwnerCenterY());
					break;
				}
			}
		}
};
class Divine : public Magic
{
	public:
		Divine(e_chipSubSubType a_subSubType):Magic(DIVINE, a_subSubType){setSprite("Sprites/magic_divine.bmp");}
		char * getName(){
			switch(m_cSubSubType)
			{
			case BASIC:		return "Divine Light";		break;
			case ADVANCED:	return "Divine Barrier";	break;
			case EXPERT:	return "Sanctuary";			break;
			default:		return "Divine";
			}
		}
		char * getDescription(){
			switch(m_cSubSubType)
			{
			case BASIC:		return "Divine projectile.";		break;
			case ADVANCED:	return "Divine explosion.";			break;
			case EXPERT:	return "Divine area-of-effect.";	break;
			default:		return getName();
			}
		}
};
class Lightning : public Magic
{
	public:
		Lightning(e_chipSubSubType a_subSubType):Magic(LIGHTNING, a_subSubType){setSprite("Sprites/magic_lightning.bmp");}
		char * getName(){
			switch(m_cSubSubType)
			{
			case BASIC:		return "Lightning Bolt";	break;
			case ADVANCED:	return "Chain Lightning";	break;
			case EXPERT:	return "Thunderstorm";		break;
			default:		return "Lightning";
			}
		}
		char * getDescription(){
			switch(m_cSubSubType)
			{
			case BASIC:		return "Lightning projectile.";		break;
			case ADVANCED:	return "Lightning explosion.";		break;
			case EXPERT:	return "Lightning area-of-effect.";	break;
			default:		return getName();
			}
		}
};
class Fire : public Magic
{
	public:
		Fire(e_chipSubSubType a_subSubType):Magic(FIRE, a_subSubType){setSprite("Sprites/magic_fire.bmp");}
		char * getName(){
			switch(m_cSubSubType)
			{
			case BASIC:		return "Fireball";		break;
			case ADVANCED:	return "Fire Blast";	break;
			case EXPERT:	return "Armageddon";	break;
			default:		return "Fire";
			}
		}
		char * getDescription(){
			switch(m_cSubSubType)
			{
			case BASIC:		return "Fire projectile.";		break;
			case ADVANCED:	return "Fire explosion.";		break;
			case EXPERT:	return "Fire area-of-effect.";	break;
			default:		return getName();
			}
		}
};
class Ice : public Magic
{
	public:
		Ice(e_chipSubSubType a_subSubType):Magic(ICE, a_subSubType){setSprite("Sprites/magic_ice.bmp");}
		char * getName(){
			switch(m_cSubSubType)
			{
			case BASIC:		return "Ice Shard";	break;
			case ADVANCED:	return "Ice Frost";	break;
			case EXPERT:	return "Blizzard";	break;
			default:		return "Ice";
			}
		}
		char * getDescription(){
			switch(m_cSubSubType)
			{
			case BASIC:		return "Ice projectile.";		break;
			case ADVANCED:	return "Ice explosion.";		break;
			case EXPERT:	return "Ice area-of-effect.";	break;
			default:		return getName();
			}
		}
};