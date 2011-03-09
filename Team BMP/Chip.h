//Chip class
//Author: John Parks

enum e_type {Armor, Magic, Weapon};
enum e_subType {Head, Trunk, UpperLimb, LowerLimb, Divine, Lightning, Fire, Ice, Blunt, Range, Slash, Pierce};
enum e_subSubType {Basic, Advanced, Expert, Legend};

class Chip
{
	protected:
		e_type m_type;
		e_subType m_subType;
		e_subSubType m_subSubType;
		int m_level, m_cost, m_costLv, m_dmg, m_dmgLv;
		bool m_equip;
	public:
		Chip(e_type a_type, e_subType a_subType, e_subSubType a_subSubType)
			:m_type(a_type),m_subType(a_subType),m_subSubType(a_subSubType),
			m_level(0),m_cost(0),m_costLv(0),m_dmg(0),m_dmgLv(0),m_equip(false){}
		e_type getType(){return m_type;}
		e_subType getSubType(){return m_subType;}
		e_subSubType getSubSubType(){return m_subSubType;}
		int getLevel(){return m_level;}
		int getCost(){return m_cost;}
		int getDamage(){return m_dmg;}
		bool isEquipped(){return m_equip;}
		void toggleEquip(){m_equip = !m_equip;}
			//define for each spell
		virtual void levelUpUnique(){}
		void levelUp()
		{
			m_level++;
			m_cost += m_costLv;
			m_dmg += m_dmgLv;
			levelUpUnique();
		}
		virtual bool shouldApplyEffect(){return false;}//pass single entity as a parameter
		virtual void applyEffect(){}//pass single entity as a parameter
		void activate()//pass list of entities as a parameter
		{
			/*
			for(int i = 0; i < size; ++i)
			{
				if(shouldApplyEffect(i))
					applyEffect(i)
			}
			*/
		}
		virtual char * getName(){return "Chip";}
		virtual char * getDescription(){return "Blank chip.";}
};
class ArmorChip : public Chip
{
	public:
		ArmorChip(e_subType a_subType, e_subSubType a_subSubType)
			:Chip(Armor, a_subType, a_subSubType){}
		bool shouldApplyEffect()
		{
			switch(m_subSubType)
			{
			case Basic:
				return false;
				break;
			case Advanced:
				return false;
				break;
			case Expert:
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
			case Head:
				break;
			case Trunk:
				break;
			case UpperLimb:
				break;
			case LowerLimb:
				break;
			}
		}
};
class MagicChip : public Chip
{
	protected:
		int m_dmgCombo, m_dmgComboLv;
	public:
		MagicChip(e_subType a_subType, e_subSubType a_subSubType)
			:Chip(Magic, a_subType, a_subSubType),
			m_dmgCombo(0),m_dmgComboLv(0){}
		int getComboBonus(){return m_dmgCombo;}
		void levelUpUnique(){m_dmgCombo += m_dmgComboLv;}
		bool shouldApplyEffect()
		{
			switch(m_subSubType)
			{
			case Basic:
				//calc if collision
				return false;
				break;
			case Advanced:
				//calc if within mini-radius
				return false;
				break;
			case Expert:
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
			case Divine:
				//apply elemental effect
				break;
			case Lightning:
				//apply elemental effect
				break;
			case Fire:
				//apply elemental effect
				break;
			case Ice:
				//apply elemental effect
				break;
			}
		}
};
class WeaponChip : public Chip
{
	public:
		WeaponChip(e_subType a_subType, e_subSubType a_subSubType)
			:Chip(Weapon, a_subType, a_subSubType){}
		bool shouldApplyEffect()
		{
			switch(m_subSubType)
			{
			case Basic:
				return false;
				break;
			case Advanced:
				return false;
				break;
			case Expert:
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
			case Blunt:
				//apply secondary effect
				break;
			case Range:
				//apply secondary effect
				break;
			case Slash:
				//apply secondary effect
				break;
			case Pierce:
				//apply secondary effect
				break;
			}
		}
};
class DivineMagic : public MagicChip
{
	public:
		DivineMagic(e_subSubType a_subSubType):MagicChip(Divine, a_subSubType){}
};
class BasicDivine : public DivineMagic
{
	public:
		BasicDivine():DivineMagic(Basic){}
		char * getName(){return "Divine Heal";}
		char * getDescription(){return "Divine projectile.";}
};
class AdvancedDivine : public DivineMagic
{
	public:
		AdvancedDivine():DivineMagic(Advanced){}
		char * getName(){return "Divine Barrier";}
		char * getDescription(){return "Divine explosion.";}
};
class ExpertDivine : public DivineMagic
{
	public:
		ExpertDivine():DivineMagic(Expert){}
		char * getName(){return "Sanctuary";}
		char * getDescription(){return "Divine area-of-effect.";}
};
class LegendDivine : public DivineMagic
{
	public:
		LegendDivine():DivineMagic(Legend){}
		char * getName(){return "Angel";}
		char * getDescription(){return "Divine guardian.";}
};
class LightningMagic : public MagicChip
{
	public:
		LightningMagic(e_subSubType a_subSubType):MagicChip(Lightning, a_subSubType){}
};
class BasicLightning : public LightningMagic
{
	public:
		BasicLightning():LightningMagic(Basic){}
		char * getName(){return "Lightning Bolt";}
		char * getDescription(){return "Lightning projectile.";}
};
class AdvancedLightning : public LightningMagic
{
	public:
		AdvancedLightning():LightningMagic(Advanced){}
		char * getName(){return "Chain Lightning";}
		char * getDescription(){return "Lightning explosion.";}
};
class ExpertLightning : public LightningMagic
{
	public:
		ExpertLightning():LightningMagic(Expert){}
		char * getName(){return "Thunderstorm";}
		char * getDescription(){return "Lightning area-of-effect.";}
};
class LegendLightning : public LightningMagic
{
	public:
		LegendLightning():LightningMagic(Legend){}
		char * getName(){return "Thunderbird";}
		char * getDescription(){return "Lightning guardian.";}
};
class FireMagic : public MagicChip
{
	public:
		FireMagic(e_subSubType a_subSubType):MagicChip(Fire, a_subSubType){}
};
class BasicFire : public FireMagic
{
	public:
		BasicFire():FireMagic(Basic){}
		char * getName(){return "Fireball";}
		char * getDescription(){return "Fire projectile.";}
};
class AdvancedFire : public FireMagic
{
	public:
		AdvancedFire():FireMagic(Advanced){}
		char * getName(){return "Fire Blast";}
		char * getDescription(){return "Fire explosion.";}
};
class ExpertFire : public FireMagic
{
	public:
		ExpertFire():FireMagic(Expert){}
		char * getName(){return "Armageddon";}
		char * getDescription(){return "Fire area-of-effect.";}
};
class LegendFire : public FireMagic
{
	public:
		LegendFire():FireMagic(Legend){}
		char * getName(){return "Phoenix";}
		char * getDescription(){return "Fire guardian.";}
};
class IceMagic : public MagicChip
{
	public:
		IceMagic(e_subSubType a_subSubType):MagicChip(Ice, a_subSubType){}
};
class BasicIce : public IceMagic
{
	public:
		BasicIce():IceMagic(Basic){}
		char * getName(){return "Ice Shard";}
		char * getDescription(){return "Ice projectile.";}
};
class AdvancedIce : public IceMagic
{
	public:
		AdvancedIce():IceMagic(Advanced){}
		char * getName(){return "Ice Frost";}
		char * getDescription(){return "Ice explosion.";}
};
class ExpertIce : public IceMagic
{
	public:
		ExpertIce():IceMagic(Expert){}
		char * getName(){return "Blizzard";}
		char * getDescription(){return "Ice area-of-effect.";}
};
class LegendIce : public IceMagic
{
	public:
		LegendIce():IceMagic(Legend){}
		char * getName(){return "Roc";}
		char * getDescription(){return "Ice guardian.";}
};