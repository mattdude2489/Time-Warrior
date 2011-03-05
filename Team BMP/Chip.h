enum e_type {Armor, Magic, Weapon};
enum e_subType {Head, Trunk, UpperLimb, LowerLimb, Divine, Lightning, Fire, Ice, Blunt, Range, Slash, Pierce};
enum e_subSubType {Basic, Advanced, Expert};

class Chip
{
	protected:
		e_type m_type;
		e_subType m_subType;
		e_subSubType m_subSubType;
		int m_level, m_cost, m_costLv;
		bool m_equip;
	public:
		Chip(e_type a_type, e_subType a_subType, e_subSubType a_subSubType)
			:m_type(a_type),m_subType(a_subType),m_subSubType(a_subSubType),
			m_level(0),m_cost(0),m_costLv(0),m_equip(false){}
		e_type getType(){return m_type;}
		e_subType getSubType(){return m_subType;}
		e_subSubType getSubSubType(){return m_subSubType;}
		int getLevel(){return m_level;}
		int getCost(){return m_cost;}
		bool isEquipped(){return m_equip;}
		void toggleEquip(){m_equip = !m_equip;}
			//define for each spell
		virtual void levelUpUnique(){}
		void levelUp()
		{
			m_level++;
			m_cost += m_costLv;
			levelUpUnique();
		}
		virtual void applyEffect(){}
		virtual char * getName(){return "Chip";}
		virtual char * getDescription(){return "Blank chip.";}
};
class MagicChip : public Chip
{
	protected:
		int m_dmg, m_dmgCombo;		//base dmg done w/out combo bonus, bonus dmg to be added if in combo
		int m_LvDmg, m_LvDmgCombo;	//amt added per level
	public:
		MagicChip(e_subType a_subType, e_subSubType a_subSubType)
			:Chip(Magic, a_subType, a_subSubType),
			m_dmg(0),m_dmgCombo(0),m_LvDmg(0),m_LvDmgCombo(0){}
		int getBaseDmg(){return m_dmg;}
		int getComboBonus(){return m_dmgCombo;}
		void levelUpUnique()
		{
			m_dmg += m_LvDmg;
			m_dmgCombo += m_LvDmgCombo;
		}
		//Legend (Summon): Thunderbird, Phoenix, Rain Bird
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
		char * getDescription(){return "Heal projectile.";}
};
class AdvancedDivine : public DivineMagic
{
	public:
		AdvancedDivine():DivineMagic(Advanced){}
		char * getName(){return "Divine Barrier";}
		char * getDescription(){return "Heal explosion.";}
};
class ExpertDivine : public DivineMagic
{
	public:
		ExpertDivine():DivineMagic(Expert){}
		char * getName(){return "Sanctuary";}
		char * getDescription(){return "Heal area-of-effect.";}
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