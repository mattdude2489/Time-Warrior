//Weapon class
//Author: John Parks
#pragma once
#include "Chip.h"

class Weapon : public Chip
{
	public:
		Weapon(e_chipSubType a_subType, e_chipSubSubType a_subSubType)
			:Chip(WEAPON, a_subType, a_subSubType){}
		bool shouldApplyEffect(Entity * a_entity)
		{
			if(a_entity->getType() == CHIP || a_entity == m_owner)
				return false;
			else
			{
				switch(m_cSubSubType)
				{
				case BASIC:
					return false;
					break;
				case ADVANCED:
					return false;
					break;
				case EXPERT:
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
			case BLUNT:
			case RANGE:
			case SLASH:
			case PIERCE:
				if(a_entity->getType() != PLAYER)
					a_entity->hit(m_dmg);
				break;
			}
		}
		~Weapon()
		{
			if(m_sprite->isSprite())
				delete m_sprite;
		}
};
class Slash : public Weapon
{
	public:
		Slash(e_chipSubSubType a_subSubType):Weapon(SLASH, a_subSubType){}
};
class BasicSlash : public Slash
{
	public:
		BasicSlash():Slash(BASIC){setSprite("Sprites/slash.bmp");}
		char * getName(){return "Slash Swing";}
		char * getDescription(){return "Slash attack.";}
		void setSprite(char * a_fileName)
		{
			m_sprite = new SDL_Sprite(a_fileName, 32, 16, 32, 1);
			m_sprite->setTransparency(COLOR_TRANSPARENT);
			//m_sprite->setRIndex(m_cSubSubType);
		}
};