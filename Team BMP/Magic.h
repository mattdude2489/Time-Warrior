//Magic class
//Author: John Parks
#pragma once
#include "Chip.h"

class Magic : public Chip
{
	protected:
		int m_dmgCombo, m_dmgComboLv;
		bool m_isSpriteInit;
	public:
		Magic(e_chipSubType a_subType, e_chipSubSubType a_subSubType)
			:Chip(MAGIC, a_subType, a_subSubType),
			m_dmgCombo(0),m_dmgComboLv(0),m_isSpriteInit(false){}
		int getComboBonus(){return m_dmgCombo;}
		void levelUpUnique(){m_dmgCombo += m_dmgComboLv;}
		bool shouldApplyEffect()
		{
			switch(m_cSubSubType)
			{
			case BASIC:
				//calc if collision
				return false;
				break;
			case ADVANCED:
				//calc if within mini-radius
				return false;
				break;
			case EXPERT:
				//calc if within greater radius
				return false;
				break;
			default:
				return false;
			}
		}
		void applyEffect()
		{
			switch(m_cSubType)
			{
			case DIVINE:
				//apply elemental effect
				break;
			case LIGHTNING:
				//apply elemental effect
				break;
			case FIRE:
				//apply elemental effect
				break;
			case ICE:
				//apply elemental effect
				break;
			}
		}
		void setSprite(const char * a_fileName)
		{
			const int maxFrames = 3;
			m_sprite = new Sprite(a_fileName, maxFrames, 1, LEGEND+1);
			m_sprite->setTransparency(COLOR_TRANSPARENT);
			m_sprite->setHIndex(m_cSubSubType, maxFrames);
			m_isSpriteInit = true;
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
			case LEGEND:
				break;
			}
		}
		void switchSign(double & a_num){a_num *= -1;}
		void switchSignIf(double & a_num, bool a_shouldChange)
		{
			if(a_shouldChange)
				switchSign(a_num);
		}
		void updateUnique(int a_time)
		{
			if(m_isLaunched)
			{
				int max = 10;
				double deltaX = m_target.x - m_locations[LOC_SCREEN].x;
				double deltaY = m_target.y - m_locations[LOC_SCREEN].y;
				if(deltaX == 0 && deltaY == 0)
					m_isLaunched = m_shouldDraw = false;
				else
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
				move(LOC_SCREEN,(int)deltaX,(int)deltaY);
				}
			}
		}
		~Magic()
		{
			if(m_isSpriteInit)
				delete m_sprite;
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
class LegendDivine : public Divine
{
	public:
		LegendDivine():Divine(LEGEND){}
		char * getName(){return "Angel";}
		char * getDescription(){return "Divine guardian.";}
};
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
class LegendLightning : public Lightning
{
	public:
		LegendLightning():Lightning(LEGEND){}
		char * getName(){return "Thunderbird";}
		char * getDescription(){return "Lightning guardian.";}
};
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
class LegendFire : public Fire
{
	public:
		LegendFire():Fire(LEGEND){}
		char * getName(){return "Phoenix";}
		char * getDescription(){return "Fire guardian.";}
};
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
class LegendIce : public Ice
{
	public:
		LegendIce():Ice(LEGEND){}
		char * getName(){return "Roc";}
		char * getDescription(){return "Ice guardian.";}
};