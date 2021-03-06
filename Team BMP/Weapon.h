//Weapon class
//Author: John Parks
#pragma once
#include "Chip.h"
#include "audioHandler.h"

class Weapon : public Chip
{
	public:
		Weapon(e_chipSubType a_subType, e_chipSubSubType a_subSubType)
			:Chip(WEAPON, a_subType, a_subSubType){}
		//set the weapon's position based on whatever direction its owner is facing
		void setLocationUsingDirection()
		{
			if(m_owner)
			{
				//set the weapon's position
				switch(m_cSubSubType)
				{
				case BASIC:
				case ADVANCED:
				case EXPERT:
					if(m_cSubSubType != EXPERT || m_cSubType == PIERCE)
					{
						//Basic & Advanced weapons based on owner's direction
						switch(m_direction)
						{
						case ROW_UP:
						case KEY_UP:
							setLocation(centerAroundOwnerCenterX(), m_owner->getCenter().y - m_sprite->getHeight());
							break;
						case ROW_LEFT:
						case KEY_LEFT:
							setLocation(m_owner->getCenter().x - m_sprite->getWidth(), centerAroundOwnerCenterY());
							break;
						case ROW_DOWN:
						case KEY_DOWN:
							setLocation(centerAroundOwnerCenterX(), m_owner->getCenter().y);
							break;
						case ROW_RIGHT:
						case KEY_RIGHT:
							setLocation(m_owner->getCenter().x, centerAroundOwnerCenterY());
							break;
						}
					}
					else
						setLocation(centerAroundOwnerCenterX(), centerAroundOwnerCenterY());
					break;
				}
			}
		}
		void activateUnique()
		{
			if(m_cSubType == RANGE)
			{
				//adjust the target position
				centerTarget();
				setLocation(centerAroundOwnerCenterX(), centerAroundOwnerCenterY());
			}
			makeSpriteMatchDirection();
			//due to different row sizes, sprite's row has to be re-set
			if(m_cSubSubType != EXPERT || m_cSubType == RANGE || m_cSubType == PIERCE)
				m_sprite->setRIndex(m_cSubSubType);
			else
				m_sprite->setRIndex(m_cSubSubType-1);
			//set weapon's position based on facing direction
			if(m_cSubType != RANGE)
				setLocationUsingDirection();
			m_isCritical = false;
			if(m_cSubType == PIERCE)
				m_isCritical = isChanceSuccessful((m_cSubSubType+1) * 25);//Bas: 25%, Adv: 50%, Exp: 75%
		}
		bool shouldApplyEffect(Entity * a_entity)
		{
			//do not apply weapon's dmg/effect to certain entities
			if(a_entity == m_owner)
				return false;
			switch(a_entity->getType())
			{
			case CHIP:
			case NPC:
				return false;
				break;
			//check for collision between weapon & entity
			default:
				switch(m_cSubSubType)
				{
				case BASIC:
				case ADVANCED:
				case EXPERT:
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
			//apply effects based on type of weapon
			switch(m_cSubType)
			{
			case BLUNT:
				//activate knockback effect
				if(a_entity->getType() != m_owner->getType())
				{
					//apply knockback distance based on last successful hit & weapon level
					int limit = TIME_SECOND_MS*(m_cSubSubType+1);
					if(m_timeSinceLastAttack > limit)
						m_timeSinceLastAttack = limit;
					int maxDistance = FRAME_SIZE;
					maxDistance += maxDistance * (m_timeSinceLastAttack / TIME_SECOND_MS);
					a_entity->activateEffect(KNOCKBACK, maxDistance, &m_owner->getDeltaBetweenLocationAnd(&a_entity->getLocation()));
					m_timeSinceLastAttack = 0;
				}
			case RANGE:
			case SLASH:
			case PIERCE://if critical, do additional hit
				if(m_isCritical && a_entity->getType() != m_owner->getType())
					a_entity->hit(m_owner->getTotalDamageDealt(m_dmg,WEAPON), m_cSubType);
				//dmg entities of non owner-type
				if(a_entity->getType() != m_owner->getType())
					a_entity->hit(m_owner->getTotalDamageDealt(m_dmg,WEAPON), m_cSubType);
				break;
			}
		}
		void updateUniqueTwo(int a_timePassed)
		{
			if(m_flags[FLAG_DRAW])
			{
				switch(m_cSubType)
				{
				case BLUNT:
				case SLASH:
				case PIERCE:
					//adjust weapon's location based on player's direction
					setLocationUsingDirection();
					break;
				case RANGE:
					//move projectiles
					if(moveToTarget((int)(SPEED_RANGE*a_timePassed)))
					{
						//end projectile if it has expired
						if(m_timers[TIMER_GENERAL] >= TIME_INACTIVE)
							deactivate();
					}
					break;
				}
			}
		}
};
class Blunt : public Weapon
{
	public:
		Blunt(e_chipSubSubType a_subSubType):Weapon(BLUNT, a_subSubType){}
		char * getName(){
			switch(m_cSubSubType)
			{
			case BASIC:		return "Blunt Bash";		break;
			case ADVANCED:	return "X-Bash";			break;
			case EXPERT:	return "Hurricane Bash";	break;
			default:		return "Blunt";
			}
		}
		char * getDescription(){
			switch(m_cSubSubType)
			{
			case BASIC:		return "Blunt attack.";				break;
			case ADVANCED:	return "Double Blunt attack.";		break;
			case EXPERT:	return "360 Degree Blunt attack.";	break;
			default:		return getName();
			}
		}
};
class Range : public Weapon
{
	public:
		Range(e_chipSubSubType a_subSubType):Weapon(RANGE, a_subSubType){}
		char * getName(){
			switch(m_cSubSubType)
			{
			case BASIC:		return "Ranged Shot";	break;
			case ADVANCED:	return "Double Shot";	break;
			case EXPERT:	return "Tri-Shot";		break;
			default:		return "Range";
			}
		}
		char * getDescription(){
			switch(m_cSubSubType)
			{
			case BASIC:		return "Ranged attack.";		break;
			case ADVANCED:	return "Double Ranged attack.";	break;
			case EXPERT:	return "Triple Ranged attack.";	break;
			default:		return getName();
			}
		}
};
class Slash : public Weapon
{
	public:
		Slash(e_chipSubSubType a_subSubType):Weapon(SLASH, a_subSubType){}
		char * getName(){
			switch(m_cSubSubType)
			{
			case BASIC:		return "Slash Strike";		break;
			case ADVANCED:	return "X-Strike";			break;
			case EXPERT:	return "Hurricane Strike";	break;
			default:		return "Slash";
			}
		}
		char * getDescription(){
			switch(m_cSubSubType)
			{
			case BASIC:		return "Slash attack.";				break;
			case ADVANCED:	return "Double Slash attack.";		break;
			case EXPERT:	return "360 Degree Slash attack.";	break;
			default:		return getName();
			}
		}
};
class Pierce : public Weapon
{
	public:
		Pierce(e_chipSubSubType a_subSubType):Weapon(PIERCE, a_subSubType){}
		char * getName(){
			switch(m_cSubSubType)
			{
			case BASIC:		return "Pierce Stab";	break;
			case ADVANCED:	return "Running Stab";	break;
			case EXPERT:	return "Stabby-Stabby";	break;
			default:		return "Pierce";
			}
		}
		char * getDescription(){
			switch(m_cSubSubType)
			{
			case BASIC:		return "Pierce attack.";				break;
			case ADVANCED:	return "Charged Pierce attack.";		break;
			case EXPERT:	return "Super-Charged Pierce attack.";	break;
			default:		return getName();
			}
		}
};