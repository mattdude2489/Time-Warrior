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
			m_target.x -= m_sprite->getWidthOffsetCenter();
			m_target.y -= m_sprite->getHeightOffsetCenter();
			switch(m_cSubSubType)
			{
			case BASIC:
			case ADVANCED:
				setLocation(centerAroundOwnerCenterX(), centerAroundOwnerCenterY());
				break;
			case EXPERT:
				setLocation(m_target.x, m_target.y);
				break;
			}
		}
		bool shouldApplyEffect(Entity * a_entity)
		{
			if(a_entity->getType() == CHIP || a_entity == m_owner)
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
				m_owner->heal(m_dmg);
				if(a_entity->getType() == m_owner->getType())
					a_entity->heal(m_dmg);
				else
					a_entity->hit(m_dmg/2);
				break;
			case LIGHTNING:
			case FIRE:
			case ICE:
				if(a_entity->getType() != m_owner->getType())
					a_entity->hit(m_dmg);
				break;
			}
		}
		void setSprite(char * a_fileName)
		{
			m_sprite = new SDL_Sprite(a_fileName, SPRITE_SIZE, SPRITE_SIZE, SPRITE_SPEED, SPRITE_ROWS);
			m_sprite->setTransparency(COLOR_TRANSPARENT);
			m_spriteHUD = new SDL_Sprite(a_fileName, SPRITE_SIZE, SPRITE_SIZE, SPRITE_SPEED, SPRITE_ROWS);
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
		void switchSign(double & a_num){a_num *= -1;}
		void switchSignIf(double & a_num, bool a_shouldChange)
		{
			if(a_shouldChange)
				switchSign(a_num);
		}
		void updateUniqueTwo(int a_timePassed)
		{
			if(m_shouldDraw)
			{
				switch(m_cSubSubType)
				{
				case BASIC:
					{
						int max = 10;
						double deltaX = m_target.x - m_location.x;
						double deltaY = m_target.y - m_location.y;
						if(deltaX != 0 && deltaY != 0)
						{
							bool switchSignX = deltaX < 0;
							bool switchSignY = deltaY < 0;
							switchSignIf(deltaX, switchSignX);
							switchSignIf(deltaY, switchSignY);
							if(deltaX > max || deltaY > max)
							{
								double slope = deltaY / deltaX;
								if(deltaX > max)
								{
									deltaX = max;
									deltaY = slope * deltaX;
								}
								else if(deltaY > max)
								{
									deltaY = max;
									deltaX = deltaY / slope;
								}
							}
							switchSignIf(deltaX, switchSignX);
							switchSignIf(deltaY, switchSignY);
							move((int)deltaX,(int)deltaY);
						}
						else
						{
							m_timer += a_timePassed;
							if(m_timer >= TIME_EXPIRE)
							{
								m_timer = 0;
								deactivate();
							}
						}
					}
					break;
				case ADVANCED:
					setLocation(centerAroundOwnerCenterX(), centerAroundOwnerCenterY());
					break;
				}
			}
		}
		~Magic()
		{
			if(m_sprite->isSprite())
				delete m_sprite;
			if(m_spriteHUD->isSprite())
				delete m_spriteHUD;
		}
};
class Divine : public Magic
{
	public:
		Divine(e_chipSubSubType a_subSubType):Magic(DIVINE, a_subSubType){setSprite("Sprites/magic_divine.bmp");}
};
class BasicDivine : public Divine
{
	public:
		BasicDivine():Divine(BASIC){}
		char * getName(){return "Divine Heal";}
		char * getDescription(){return "Divine projectile.";}
};
class AdvancedDivine : public Divine
{
	public:
		AdvancedDivine():Divine(ADVANCED){}
		char * getName(){return "Divine Barrier";}
		char * getDescription(){return "Divine explosion.";}
};
class ExpertDivine : public Divine
{
	public:
		ExpertDivine():Divine(EXPERT){}
		char * getName(){return "Sanctuary";}
		char * getDescription(){return "Divine area-of-effect.";}
};
/*class LegendDivine : public Divine
{
	public:
		LegendDivine():Divine(LEGEND){}
		char * getName(){return "Angel";}
		char * getDescription(){return "Divine guardian.";}
};*/
class Lightning : public Magic
{
	public:
		Lightning(e_chipSubSubType a_subSubType):Magic(LIGHTNING, a_subSubType){setSprite("Sprites/magic_lightning.bmp");}
};
class BasicLightning : public Lightning
{
	public:
		BasicLightning():Lightning(BASIC){}
		char * getName(){return "Lightning Bolt";}
		char * getDescription(){return "Lightning projectile.";}
};
class AdvancedLightning : public Lightning
{
	public:
		AdvancedLightning():Lightning(ADVANCED){}
		char * getName(){return "Chain Lightning";}
		char * getDescription(){return "Lightning explosion.";}
};
class ExpertLightning : public Lightning
{
	public:
		ExpertLightning():Lightning(EXPERT){}
		char * getName(){return "Thunderstorm";}
		char * getDescription(){return "Lightning area-of-effect.";}
};
/*class LegendLightning : public Lightning
{
	public:
		LegendLightning():Lightning(LEGEND){}
		char * getName(){return "Thunderbird";}
		char * getDescription(){return "Lightning guardian.";}
};*/
class Fire : public Magic
{
	public:
		Fire(e_chipSubSubType a_subSubType):Magic(FIRE, a_subSubType){setSprite("Sprites/magic_fire.bmp");}
};
class BasicFire : public Fire
{
	public:
		BasicFire():Fire(BASIC){}
		char * getName(){return "Fireball";}
		char * getDescription(){return "Fire projectile.";}
};
class AdvancedFire : public Fire
{
	public:
		AdvancedFire():Fire(ADVANCED){}
		char * getName(){return "Fire Blast";}
		char * getDescription(){return "Fire explosion.";}
};
class ExpertFire : public Fire
{
	public:
		ExpertFire():Fire(EXPERT){}
		char * getName(){return "Armageddon";}
		char * getDescription(){return "Fire area-of-effect.";}
};
/*class LegendFire : public Fire
{
	public:
		LegendFire():Fire(LEGEND){}
		char * getName(){return "Phoenix";}
		char * getDescription(){return "Fire guardian.";}
};*/
class Ice : public Magic
{
	public:
		Ice(e_chipSubSubType a_subSubType):Magic(ICE, a_subSubType){setSprite("Sprites/magic_ice.bmp");}
};
class BasicIce : public Ice
{
	public:
		BasicIce():Ice(BASIC){}
		char * getName(){return "Ice Shard";}
		char * getDescription(){return "Ice projectile.";}
};
class AdvancedIce : public Ice
{
	public:
		AdvancedIce():Ice(ADVANCED){}
		char * getName(){return "Ice Frost";}
		char * getDescription(){return "Ice explosion.";}
};
class ExpertIce : public Ice
{
	public:
		ExpertIce():Ice(EXPERT){}
		char * getName(){return "Blizzard";}
		char * getDescription(){return "Ice area-of-effect.";}
};
/*class LegendIce : public Ice
{
	public:
		LegendIce():Ice(LEGEND){}
		char * getName(){return "Roc";}
		char * getDescription(){return "Ice guardian.";}
};*/