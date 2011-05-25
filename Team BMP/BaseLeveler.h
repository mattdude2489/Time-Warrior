#pragma once
/*
This class handles XP Gain and leveling for
Characters (15), Melee Attacks (40), Magic Attacks (40)
*/
const int STARTING_XP = 0;
const int STARTING_LV = 1;
const int MAX_ATK_LVL = 30;
const int MAX_CHAR_LV = 15;
const int NUM_BASE_TYPES = 5;
const int MAX_UPGRADE_LV = 4;

/*XP Gain Formula | f(n){n > 0} = f(n-1) + n
f(1) = 0 + 1 = 1
f(2) = 1 + 2 = 3
f(3) = 3 + 3 = 6
*/

//contains the level and XP required to reach it
struct LEVEL_SYSTEM
{
	int level;
	int xpRequired;
};
// upgrades
struct CHIP_UPGRADE
{
	int upgrade_level;
	bool active;
};

// hold the type, total XP, and current level of attack
struct XP_STATS
{
	bool upgradeable;
	int attack_type;
	int attack_level;
	int total_XP;
	//each should only be concerned with itself,
	//unlocking other levels will be handled by Player/Chip
	//CHIP_UPGRADE upgrades[MAX_UPGRADE_LV];
};

// the base attack types
enum BASE_TYPE
{
	DIVINE_0 = 0,
	STORM,
	FIRE_0,
	ICE_0,
	MELEE,
	DIVINE_1,
	STORM_1,
	FIRE_1,
	ICE_1,
	MELEE_1,
	DIVINE_2,
	STORM_2,
	FIRE_2,
	ICE_2,
	MELEE_2,
	DIVINE_3,
	STORM_3,
	FIRE_3,
	ICE_3,
	MELEE_3,
};

class BaseLeveler
{
public:
	/*--- Constructor ---*/
	BaseLeveler(void);
	~BaseLeveler(void);
	/*--- Base Functions ---*/
	//upgrade function to check if at a upgradeable level 10 and if chip ha\s gained a level
	void Update();
	/*--- XP Gain Functions ---*/
	void GainMagicXP(int chipBaseType);
	void GainMeleeXP();
	/*--- Level checking fuctions ---*/
	// check if the level allows for an upgrade
	void upgradeAttack(XP_STATS statBlock){}
	/*--- Accessor/Mutators ---*/
	void setAttackLvXP(int type, int level, int experience);
	int getAttackLevel(int type);
	int getAttackXP(int type);
	bool getUpgradeStatus(int type, int upgradeLevel);
	//check if the the attack can be upgraded (every 10 levels)
	void isUpgradeable(XP_STATS statBlock){}
	void unlock(int type)
	{
		switch(type)
		{
		case DIVINE_0:	divineBlock.upgradeable = true;		break;
		case STORM:		lightningBlock.upgradeable= true;	break;
		case FIRE_0:	fireBlock.upgradeable = true;		break;
		case ICE_0:		iceBlock.upgradeable= true;			break;
		case MELEE:		meleeBlock.upgradeable = true;		break;
		}
	}
private:
	/*--- Main Functions ---*/
	void LS_Init();
	//check the current level and XP if, XP
	bool isLevelUp(int type);
	bool isMaxLevel(int type);
	/*--- Outside Variables ---*/
	/*--- Class Variables ---*/
	// This goes up to L40
	LEVEL_SYSTEM *XP_KEY;
	XP_STATS divineBlock;
	XP_STATS lightningBlock;
	XP_STATS fireBlock;
	XP_STATS iceBlock;
	XP_STATS meleeBlock;
};