//MagicChip class
//Author: John Parks
#pragma once
#include "Chip.h"

class Magic : public Chip
{
	protected:
		int m_dmgCombo, m_dmgComboLv;
	public:
		Magic(e_cSubType a_subType, e_cSubSubType a_subSubType)
			:Chip(MAGIC, a_subType, a_subSubType),
			m_dmgCombo(0),m_dmgComboLv(0){}
		int getComboBonus(){return m_dmgCombo;}
		void levelUpUnique(){m_dmgCombo += m_dmgComboLv;}
		bool shouldApplyEffect()
		{
			switch(m_subSubType)
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
			switch(m_subType)
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
};
class Divine : public Magic
{
	public:
		Divine(e_cSubSubType a_subSubType):Magic(DIVINE, a_subSubType){}
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
		Lightning(e_cSubSubType a_subSubType):Magic(LIGHTNING, a_subSubType){}
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
		Fire(e_cSubSubType a_subSubType):Magic(FIRE, a_subSubType)
		{
			m_mySprite = new Sprite("Sprites/magic_fire.bmp", 3, 1, 4);
			m_mySprite->setTransparency(COLOR_TRANSPARENT);
			m_mySprite->setHIndex(m_subSubType,3);
			setLocation(LOC_SCREEN, CENTER_SCREEN_X+32, CENTER_SCREEN_Y);
		}
		~Fire(){delete m_mySprite;}
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
		Ice(e_cSubSubType a_subSubType):Magic(ICE, a_subSubType){}
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