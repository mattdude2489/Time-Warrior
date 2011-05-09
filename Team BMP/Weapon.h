//Weapon class
//Author: John Parks
#pragma once
#include "Chip.h"
#include "audioHandler.h"

class Weapon : public Chip
{
	protected:
		//flags for sprite's flip(s) & rotation
		bool m_isFlipH, m_isFlipV;
		int m_rotateDeg;
	public:
		Weapon(e_chipSubType a_subType, e_chipSubSubType a_subSubType)
			:Chip(WEAPON, a_subType, a_subSubType),
			m_isFlipH(false),m_isFlipV(false),m_rotateDeg(0){}
		//set the weapon's position based on whatever direction its owner is facing
		void setLocationUsingDirection()
		{
			//set the weapon's position
			switch(m_cSubSubType)
			{
			case BASIC:
			case ADVANCED:
				//Basic & Advanced weapons based on owner's direction
				switch(m_direction)
				{
				case KEY_UP:
					setLocation(centerAroundOwnerCenterX(), getOwnerCenterY() - m_sprite->getHeight());
					break;
				case KEY_LEFT:
					setLocation(getOwnerCenterX() - m_sprite->getWidth(), centerAroundOwnerCenterY());
					break;
				case KEY_DOWN:
					setLocation(centerAroundOwnerCenterX(), getOwnerCenterY());
					break;
				case KEY_RIGHT:
					setLocation(getOwnerCenterX(), centerAroundOwnerCenterY());
					break;
				}
				break;
			default:
				//Expert weapons centered on owner
				setLocation(centerAroundOwnerCenterX(), centerAroundOwnerCenterY());
			}
		}
		void activateUnique()
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
			for(int i = 0; i < (360/90) - m_rotateDeg / 90; ++i)
				m_sprite->rotate90();
			m_rotateDeg = 0;
			//adjust the flip/rotation to match the facing direction
			switch(m_cSubSubType)
			{
			case BASIC:
			case ADVANCED:
			case EXPERT:
				switch(m_direction)
				{
				case KEY_LEFT:
				case KEY_RIGHT:
					m_rotateDeg = 270;
					m_sprite->rotate270();
					if(m_direction == KEY_RIGHT)
					{
						m_isFlipH = true;
						m_sprite->flipHorizontal();
					}
					break;
				case KEY_DOWN:
					m_isFlipH = m_isFlipV = true;
					m_sprite->flipHorizontal();
					m_sprite->flipVertical();
					break;
				}
				//due to different row sizes, sprite's row has to be re-set
				if(m_cSubSubType != EXPERT)
					m_sprite->setRIndex(m_cSubSubType);
				else
					m_sprite->setRIndex(m_cSubSubType-1);
				//set weapon's position based on facing direction
				setLocationUsingDirection();
				break;
			}
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
				//knockback entities of non owner-type
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
			case PIERCE:
				//dmg entities of non owner-type
				if(a_entity->getType() != m_owner->getType())
					a_entity->hit(m_owner->getTotalDamageDealt(m_dmg,WEAPON), m_cSubType);
				break;
			}
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
				case ADVANCED:
				case EXPERT:
					//adjust weapon's location based on player's direction
					setLocationUsingDirection();
					break;
				}
			}
		}
};
class Slash : public Weapon
{
	public:
		Slash(e_chipSubSubType a_subSubType):Weapon(SLASH, a_subSubType){setSprite("Sprites/weapon_slash.bmp");}
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
class Blunt : public Weapon
{
	public:
		Blunt(e_chipSubSubType a_subSubType):Weapon(BLUNT, a_subSubType){setSprite("Sprites/weapon_blunt.bmp");}
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