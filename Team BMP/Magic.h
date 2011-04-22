//Magic class
//Author: John Parks
#pragma once
#include "Chip.h"

class Magic : public Chip
{
	protected:
		int m_dmgCombo, m_dmgComboLv;
	public:
		Magic(e_chipSubType a_subType, e_chipSubSubType a_subSubType)
			:Chip(MAGIC, a_subType, a_subSubType),
			m_dmgCombo(0),m_dmgComboLv(0){}
		int getComboBonus(){return m_dmgCombo;}
		void levelUpUnique(){m_dmgCombo += m_dmgComboLv;}
		void activateUnique()
		{
			centerTarget();
			switch(m_cSubSubType)
			{
			case BASIC:
			case ADVANCED:
				setLocation(centerAroundOwnerCenterX(), centerAroundOwnerCenterY());
				break;
			case EXPERT:
				setLocation(m_target);
				break;
			}
			if(m_cSubType == DIVINE)
				m_owner->heal(m_owner->getTotalDamageDealt(m_dmg,MAGIC));
		}
		bool shouldApplyEffect(Entity * a_entity)
		{
			if(a_entity->getType() == CHIP || a_entity->getType()== NPC || a_entity == m_owner)
				return false;
			else
			{
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
			switch(m_cSubType)
			{
			case DIVINE:
				if(a_entity->getType() == m_owner->getType())
					a_entity->heal(m_owner->getTotalDamageDealt(m_dmg,MAGIC));
				else
					a_entity->hit(m_owner->getTotalDamageDealt(m_dmg/2,MAGIC), m_cSubType);
				break;
			case LIGHTNING:
			case FIRE:
			case ICE:
				if(a_entity->getType() != m_owner->getType())
					a_entity->hit(m_owner->getTotalDamageDealt(m_dmg,MAGIC), m_cSubType);
				break;
			}
			if(a_entity->getType()!= m_owner->getType()){
				a_entity->hitFromPlayer();}
			if(a_entity->getStatNumber(HEALTH_CURRENT) <= 0)
				m_owner->gainExperience(a_entity->getExperienceFromDefeat(m_owner));
		}
		void setSprite(char * a_fileName)
		{
			m_sprite = new SDL_Sprite(a_fileName, FRAME_SIZE, FRAME_SIZE, FRAME_RATE, NUM_ROWS);
			m_sprite->setTransparency(COLOR_TRANSPARENT);
			m_spriteHUD = new SDL_Sprite(a_fileName, FRAME_SIZE, FRAME_SIZE, FRAME_RATE, NUM_ROWS);
			m_spriteHUD->setTransparency(COLOR_TRANSPARENT);
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
			m_sprite->setRIndex(m_cSubSubType);
			m_spriteHUD->setRIndex(m_cSubSubType);
			initHudSprite();
		}
		void updateUniqueTwo(int a_timePassed)
		{
			if(m_shouldDraw)
			{
				switch(m_cSubSubType)
				{
				case BASIC:
					{
						if(moveToTarget((int)(SPEED_MAGIC*a_timePassed)))
						{
							if(m_timer >= TIME_EXPIRE)
								deactivate();
						}
					}
					break;
				case ADVANCED:
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
			case BASIC:
				return "Divine Light";
				break;
			case ADVANCED:
				return "Divine Barrier";
				break;
			case EXPERT:
				return "Sanctuary";
				break;
			default:
				return "Divine";
			}
		}
		char * getDescription(){
			switch(m_cSubSubType)
			{
			case BASIC:
				return "Divine projectile.";
				break;
			case ADVANCED:
				return "Divine explosion.";
				break;
			case EXPERT:
				return "Divine area-of-effect.";
				break;
			default:
				return getName();
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
			case BASIC:
				return "Lightning Bolt";
				break;
			case ADVANCED:
				return "Chain Lightning";
				break;
			case EXPERT:
				return "Thunderstorm";
				break;
			default:
				return "Lightning";
			}
		}
		char * getDescription(){
			switch(m_cSubSubType)
			{
			case BASIC:
				return "Lightning projectile.";
				break;
			case ADVANCED:
				return "Lightning explosion.";
				break;
			case EXPERT:
				return "Lightning area-of-effect.";
				break;
			default:
				return getName();
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
			case BASIC:
				return "Fireball";
				break;
			case ADVANCED:
				return "Fire Blast";
				break;
			case EXPERT:
				return "Armageddon";
				break;
			default:
				return "Fire";
			}
		}
		char * getDescription(){
			switch(m_cSubSubType)
			{
			case BASIC:
				return "Fire projectile.";
				break;
			case ADVANCED:
				return "Fire explosion.";
				break;
			case EXPERT:
				return "Fire area-of-effect.";
				break;
			default:
				return getName();
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
			case BASIC:
				return "Ice Shard";
				break;
			case ADVANCED:
				return "Ice Frost";
				break;
			case EXPERT:
				return "Blizzard";
				break;
			default:
				return "Ice";
			}
		}
		char * getDescription(){
			switch(m_cSubSubType)
			{
			case BASIC:
				return "Ice projectile.";
				break;
			case ADVANCED:
				return "Ice explosion.";
				break;
			case EXPERT:
				return "Ice area-of-effect.";
				break;
			default:
				return getName();
			}
		}
};