#include "BaseLeveler.h"

BaseLeveler::BaseLeveler(void)
{
	LS_Init();
	//intitalize the XP_STATS variable Divine
	divineBlock.attack_level = STARTING_LV;
	divineBlock.attack_type = DIVINE_0;
	divineBlock.total_XP = STARTING_XP;
	divineBlock.upgradeable = false;

	//Storm
	lightningBlock.attack_level = STARTING_LV;
	lightningBlock.attack_type = STORM;
	lightningBlock.total_XP = STARTING_XP;
	lightningBlock.upgradeable = false;
	//fire
	fireBlock.attack_level = STARTING_LV;
	fireBlock.attack_type = FIRE_0;
	fireBlock.total_XP = STARTING_XP;
	fireBlock.upgradeable = false;
	//ice
	iceBlock.attack_level = STARTING_LV;
	iceBlock.attack_type = ICE_0;
	iceBlock.total_XP = STARTING_XP;
	iceBlock.upgradeable = false;
	//melee
	meleeBlock.attack_level = STARTING_LV;
	meleeBlock.attack_type = MELEE;
	meleeBlock.total_XP = STARTING_XP;
	meleeBlock.upgradeable = false;
}

BaseLeveler::~BaseLeveler(void)
{
	delete[] XP_KEY;
}
/*--- Base Functions ---*/
/*--- XP Gain Functions ---*/
void BaseLeveler::GainMagicXP(int chipBaseType)
{
	switch(chipBaseType)
	{
	case DIVINE_0:
		divineBlock.total_XP += 1;
		if(isLevelUp((int)DIVINE_0))
			divineBlock.attack_level += 1;
		break;
	case STORM:
		lightningBlock.total_XP += 1;
		if(isLevelUp((int)STORM))
			lightningBlock.attack_level += 1;
		break;
	case FIRE_0:
		fireBlock.total_XP+=1;
		if(isLevelUp((int)FIRE_0))
			fireBlock.attack_level += 1;
		break;
	case ICE_0:
		iceBlock.total_XP += 1;
		if(isLevelUp((int)ICE_0))
			iceBlock.attack_level += 1;
		break;
	}//end switch
}

void BaseLeveler::GainMeleeXP()
{
	meleeBlock.total_XP += 1;
	if(isLevelUp((int)MELEE))
		meleeBlock.attack_level += 1;
}
/*--- Level checking fuctions ---*/
/*--- Accessor/Mutators ---*/
int BaseLeveler::getAttackLevel(int type)
{
	switch(type)
	{
	case DIVINE_0: return divineBlock.attack_level; break;
	case STORM: return lightningBlock.attack_level; break;
	case FIRE_0: return fireBlock.attack_level; break;
	case ICE_0: return iceBlock.attack_level; break;
	case MELEE: return meleeBlock.attack_level; break;
	}
}
/*--- Main Functions ---*/
void BaseLeveler::LS_Init()
{
	XP_KEY = new LEVEL_SYSTEM[MAX_ATK_LVL];
	for(int i = 0; i < MAX_ATK_LVL; i++)
	{
		if(i == 0)
		{
			XP_KEY[0].level = STARTING_LV;
			XP_KEY[0].xpRequired = STARTING_XP;
		}
		else
		{
			XP_KEY[i].level = (i + 1);
			XP_KEY[i].xpRequired += XP_KEY[i].level;
		}//end else
	}// end for
}

bool BaseLeveler::isLevelUp(int type)
{
	switch(type)
	{
	case DIVINE_0:
		if(divineBlock.total_XP >= XP_KEY[divineBlock.attack_level + 1].xpRequired)
			return true;
		else
			return false;
		break;
	case STORM:
		if(lightningBlock.total_XP >= XP_KEY[lightningBlock.attack_level + 1].xpRequired)
			return true;
		else
			return false;
		break;
	case FIRE_0:
		if(fireBlock.total_XP >= XP_KEY[fireBlock.attack_level + 1].xpRequired)
			return true;
		else
			return false;
		break;
	case ICE_0:
		if(iceBlock.total_XP >= XP_KEY[iceBlock.attack_level + 1].xpRequired)
			return true;
		else
			return false;
		break;
	case MELEE:
		if(meleeBlock.total_XP >= XP_KEY[meleeBlock.attack_level + 1].xpRequired)
			return true;
		else
			return false;
		break;
	}//end switch
	return false;
}

bool BaseLeveler::isMaxLevel(int type)
{
	switch(type)
	{
	case DIVINE_0:
		if(divineBlock.attack_level == MAX_ATK_LVL)
			return true;
		break;
	case STORM:
		if(lightningBlock.attack_level == MAX_ATK_LVL)
			return true;
		break;
	case FIRE_0:
		if(fireBlock.attack_level == MAX_ATK_LVL)
			return true;
		break;
	case ICE_0:
		if(iceBlock.attack_level == MAX_ATK_LVL)
			return true;
		break;
	case MELEE:
		if(meleeBlock.attack_level == MAX_ATK_LVL)
			return true;
		break;
	}//end switch
	return false;
}