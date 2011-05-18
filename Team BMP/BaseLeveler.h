#pragma once
/*
This class handles XP Gain and leveling for
Characters (15), Melee Attacks (40), Magic Attacks (40)
*/
const int STARTING_XP = 0;
const int STARTING_LV = 1;
const int MAX_ATK_LVL = 40;
const int MAX_CHAR_LV = 15;

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

// hold the type, total XP, and current level of attack
struct XP_STATS
{
	bool upgradeable;
	int attack_type;
	int attack_level;
	int total_XP;
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
	MELEE_3
};

class BaseLeveler
{
public:
	/*--- Constructor ---*/
	BaseLeveler(void);
	~BaseLeveler(void);
	/*--- Base Functions ---*/
	//upgrade function to check if at a upgradeable level 10
	/*--- XP Gain Functions ---*/
	void GainMagicXP(int chipBaseType);
	void GainMeleeXP();
	/*--- Level checking fuctions ---*/
	// check if the level allows for an upgrade
	void upgradeAttack(XP_STATS statBlock){}
	/*--- Accessor/Mutators ---*/
	int getAttackLevel(int type);
	//check if the the attack can be upgraded (every 10 levels)
	void isUpgradeable(XP_STATS statBlock){}
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