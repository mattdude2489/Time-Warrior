#include "BaseLeveler.h"

BaseLeveler::BaseLeveler(void)
{
	LS_Init();
	//intitalize the XP_STATS variable Divine
	divineBlock.attack_level = STARTING_LV;
	divineBlock.attack_type = DIVINE;
	divineBlock.total_XP = STARTING_XP;
	divineBlock.upgradeable = false;

	//Storm
	lightningBlock.attack_level = STARTING_LV;
	lightningBlock.attack_type = STORM;
	lightningBlock.total_XP = STARTING_XP;
	lightningBlock.upgradeable = false;
	//fire
	fireBlock.attack_level = STARTING_LV;
	fireBlock.attack_type = FIRE;
	fireBlock.total_XP = STARTING_XP;
	fireBlock.upgradeable = false;
	//ice
	iceBlock.attack_level = STARTING_LV;
	iceBlock.attack_type = ICE;
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
	case DIVINE:
		divineBlock.total_XP++;
		if(isLevelUp((int)DIVINE))
			divineBlock.attack_level++;
		break;
	case STORM:
		lightningBlock.total_XP++;
		if(isLevelUp((int)STORM))
			lightningBlock.attack_level++;
		break;
	case FIRE:
		fireBlock.total_XP++;
		if(isLevelUp((int)FIRE))
			fireBlock.attack_level++;
		break;
	case ICE:
		iceBlock.total_XP++;
		if(isLevelUp((int)ICE))
			iceBlock.attack_level++;
		break;
	}//end switch
}

void BaseLeveler::GainMeleeXP()
{
	meleeBlock.total_XP++;
	if(isLevelUp((int)MELEE))
		meleeBlock.attack_level++;
}
/*--- Level checking fuctions ---*/
/*--- Accessor/Mutators ---*/
/*--- Main Functions ---*/
void BaseLeveler::LS_Init()
{
	XP_KEY = new LEVEL_SYSTEM[40];
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
	case DIVINE:
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
	case FIRE:
		if(fireBlock.total_XP >= XP_KEY[fireBlock.attack_level + 1].xpRequired)
			return true;
		else
			return false;
		break;
	case ICE:
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
}