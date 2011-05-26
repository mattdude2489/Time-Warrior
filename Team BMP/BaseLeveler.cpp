#include "BaseLeveler.h"

BaseLeveler::BaseLeveler(void)
{
	LS_Init();
	//intitalize the XP_STATS variable Divine
	divineBlock.attack_level = STARTING_LV;
	divineBlock.attack_type = DIVINE_0;
	divineBlock.total_XP = STARTING_XP;
	divineBlock.upgradeable = false;
	/*for(int i = 0; i < MAX_UPGRADE_LV; i++)
	{
		divineBlock.upgrades[i].active = false;
		divineBlock.upgrades[i].upgrade_level = i * 10;
	}
	divineBlock.upgrades[0].active = true;*/
	//Storm
	lightningBlock.attack_level = STARTING_LV;
	lightningBlock.attack_type = STORM;
	lightningBlock.total_XP = STARTING_XP;
	lightningBlock.upgradeable = false;
	/*for(int i = 0; i < MAX_UPGRADE_LV; i++)
	{
		lightningBlock.upgrades[i].active = false;
		lightningBlock.upgrades[i].upgrade_level = i * 10;
	}
	lightningBlock.upgrades[0].active = true;*/
	//fire
	fireBlock.attack_level = STARTING_LV;
	fireBlock.attack_type = FIRE_0;
	fireBlock.total_XP = STARTING_XP;
	fireBlock.upgradeable = false;
	/*for(int i = 0; i < MAX_UPGRADE_LV; i++)
	{
		fireBlock.upgrades[i].active = false;
		fireBlock.upgrades[i].upgrade_level = i * 10;
	}
	fireBlock.upgrades[0].active = true;*/
	//ice
	iceBlock.attack_level = STARTING_LV;
	iceBlock.attack_type = ICE_0;
	iceBlock.total_XP = STARTING_XP;
	iceBlock.upgradeable = false;
	/*for(int i = 0; i < MAX_UPGRADE_LV; i++)
	{
		iceBlock.upgrades[i].active = false;
		iceBlock.upgrades[i].upgrade_level = i * 10;
	}
	iceBlock.upgrades[0].active = true;*/
	//melee
	meleeBlock.attack_level = STARTING_LV;
	meleeBlock.attack_type = MELEE;
	meleeBlock.total_XP = STARTING_XP;
	meleeBlock.upgradeable = false;
	/*for(int i = 0; i < MAX_UPGRADE_LV; i++)
	{
		meleeBlock.upgrades[i].active = false;
		meleeBlock.upgrades[i].upgrade_level = i * 10;
	}
	meleeBlock.upgrades[0].active = true;*/
}

BaseLeveler::~BaseLeveler(void)
{
	delete[] XP_KEY;
}
/*--- Base Functions ---*/
void BaseLeveler::Update()
{
	/*for(int i = 0; i < NUM_BASE_TYPES; i++)
	{
		switch(i)
		{
		case DIVINE_0:
			if((divineBlock.attack_level%10) == 0)
			{
				if(divineBlock.attack_level == 10) { divineBlock.upgrades[0].active = true; }
			}
			if((divineBlock.attack_level%10) == 0)
			{
				if(divineBlock.attack_level == 20) { divineBlock.upgrades[1].active = true; }
			}
			if((divineBlock.attack_level%10) == 0)
			{
				if(divineBlock.attack_level == 30) { divineBlock.upgrades[2].active = true; }
			}
			break;
		case STORM:
			if((lightningBlock.attack_level%10) == 0)
			{
				if(lightningBlock.attack_level == 10) { lightningBlock.upgrades[0].active = true; }
			}
			if((lightningBlock.attack_level%10) == 0)
			{
				if(lightningBlock.attack_level == 20) { lightningBlock.upgrades[1].active = true; }
			}
			if((lightningBlock.attack_level%10) == 0)
			{
				if(lightningBlock.attack_level == 30) { lightningBlock.upgrades[2].active = true; }
			}
			break;
		case FIRE_0:
			if((fireBlock.attack_level%10) == 0)
			{
				if(fireBlock.attack_level == 10) { fireBlock.upgrades[0].active = true; }
			}
			if((fireBlock.attack_level%10) == 0)
			{
				if(fireBlock.attack_level == 20) { fireBlock.upgrades[1].active = true; }
			}
			if((fireBlock.attack_level%10) == 0)
			{
				if(fireBlock.attack_level == 30) { fireBlock.upgrades[2].active = true; }
			}
			break;
		case ICE_0:
			if((iceBlock.attack_level%10) == 0)
			{
				if(iceBlock.attack_level == 10) { iceBlock.upgrades[0].active = true; }
			}
			if((iceBlock.attack_level%10) == 0)
			{
				if(iceBlock.attack_level == 20) { iceBlock.upgrades[1].active = true; }
			}
			if((iceBlock.attack_level%10) == 0)
			{
				if(iceBlock.attack_level == 30) { iceBlock.upgrades[2].active = true; }
			}
			break;
		case MELEE:
			if((meleeBlock.attack_level%10) == 0)
			{
				if(meleeBlock.attack_level == 10) { meleeBlock.upgrades[0].active = true; }
			}
			if((meleeBlock.attack_level%10) == 0)
			{
				if(meleeBlock.attack_level == 20) { meleeBlock.upgrades[1].active = true; }
			}
			if((meleeBlock.attack_level%10) == 0)
			{
				if(meleeBlock.attack_level == 30) { meleeBlock.upgrades[2].active = true; }
			}
			break;
		}//end switch
	}//end for*/
}
/*--- XP Gain Functions ---*/
void BaseLeveler::GainMagicXP(int chipBaseType)
{
	switch(chipBaseType)
	{
	case DIVINE_0:
		divineBlock.total_XP += 1;
		if(isLevelUp((int)DIVINE_0))
			levelUp(DIVINE_0);
		break;
	case STORM:
		lightningBlock.total_XP += 1;
		if(isLevelUp((int)STORM))
			levelUp(STORM);
		break;
	case FIRE_0:
		fireBlock.total_XP+=1;
		if(isLevelUp((int)FIRE_0))
			levelUp(FIRE_0);
		break;
	case ICE_0:
		iceBlock.total_XP += 1;
		if(isLevelUp((int)ICE_0))
			levelUp(ICE_0);
		break;
	}//end switch
}

void BaseLeveler::GainMeleeXP()
{
	meleeBlock.total_XP += 1;
	if(isLevelUp((int)MELEE))
		levelUp(MELEE);
}
/*--- Level checking fuctions ---*/
/*--- Accessor/Mutators ---*/
void BaseLeveler::setAttackLvXP(int type, int level, int experience)
{
	switch(type)
	{
	case DIVINE_0:	divineBlock.attack_level = level;		divineBlock.total_XP = experience;		break;
	case STORM:		lightningBlock.attack_level = level;	lightningBlock.total_XP = experience;	break;
	case FIRE_0:	fireBlock.attack_level = level;			fireBlock.total_XP = experience;		break;
	case ICE_0:		iceBlock.attack_level = level;			iceBlock.total_XP = experience;			break;
	case MELEE:		meleeBlock.attack_level = level;		meleeBlock.total_XP = experience;		break;
	}
	if(isLevelUp(type))
	{
		switch(type)
		{
		case DIVINE_0:	divineBlock.total_XP %= XP_KEY[getAttackLevel(type) + 1].xpRequired;	break;
		case STORM:		lightningBlock.total_XP %= XP_KEY[getAttackLevel(type) + 1].xpRequired;	break;
		case FIRE_0:	fireBlock.total_XP %= XP_KEY[getAttackLevel(type) + 1].xpRequired;		break;
		case ICE_0:		iceBlock.total_XP %= XP_KEY[getAttackLevel(type) + 1].xpRequired;		break;
		case MELEE:		meleeBlock.total_XP %= XP_KEY[getAttackLevel(type) + 1].xpRequired;		break;
		}
	}
}

int BaseLeveler::getAttackXP(int type)
{
	switch(type)
	{
	case DIVINE_0:	return divineBlock.total_XP;	break;
	case STORM:		return lightningBlock.total_XP;	break;
	case FIRE_0:	return fireBlock.total_XP;		break;
	case ICE_0:		return iceBlock.total_XP;		break;
	case MELEE:		return meleeBlock.total_XP;		break;
	}
	return 0;
}

int BaseLeveler::getAttackLevel(int type)
{

	switch(type)
	{
	case DIVINE_0:
		if(divineBlock.upgradeable)
			return divineBlock.attack_level;
		break;
	case STORM:
		if(lightningBlock.upgradeable)
			return lightningBlock.attack_level;
		break;
	case FIRE_0:
		if(fireBlock.upgradeable)
			return fireBlock.attack_level;
		break;
	case ICE_0:
		if(iceBlock.upgradeable)
			return iceBlock.attack_level;
		break;
	case MELEE:
		if(meleeBlock.upgradeable)
			return meleeBlock.attack_level;
		break;
	}
	return 0;
}

bool BaseLeveler::getUpgradeStatus(int type, int upgradeLevel)
{
	/*switch(type)
	{
	case DIVINE_0:	return divineBlock.upgrades[upgradeLevel].active;	 break;
	case STORM:		return lightningBlock.upgrades[upgradeLevel].active; break;
	case FIRE_0:	return fireBlock.upgrades[upgradeLevel].active;		 break;
	case ICE_0:		return iceBlock.upgrades[upgradeLevel].active;		 break;
	case MELEE:		return meleeBlock.upgrades[upgradeLevel].active;	 break;
	}*/
	return false;
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
	case DIVINE_0:	return divineBlock.total_XP >= XP_KEY[divineBlock.attack_level + 1].xpRequired;		  break;
	case STORM:		return lightningBlock.total_XP >= XP_KEY[lightningBlock.attack_level + 1].xpRequired; break;
	case FIRE_0:	return fireBlock.total_XP >= XP_KEY[fireBlock.attack_level + 1].xpRequired;			  break;
	case ICE_0:		return iceBlock.total_XP >= XP_KEY[iceBlock.attack_level + 1].xpRequired;			  break;
	case MELEE:		return meleeBlock.total_XP >= XP_KEY[meleeBlock.attack_level + 1].xpRequired;		  break;
	}//end switch
	return false;
}

bool BaseLeveler::isMaxLevel(int type)
{
	switch(type)
	{
	case DIVINE_0:	return divineBlock.attack_level == MAX_ATK_LVL;		break;
	case STORM:		return lightningBlock.attack_level == MAX_ATK_LVL;	break;
	case FIRE_0:	return fireBlock.attack_level == MAX_ATK_LVL;		break;
	case ICE_0:		return iceBlock.attack_level == MAX_ATK_LVL;		break;
	case MELEE:		return meleeBlock.attack_level == MAX_ATK_LVL;		break;
	}//end switch
	return false;
}