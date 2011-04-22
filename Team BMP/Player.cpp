//Basic Player Class Extensions.
#include "Player.h"
#include "UserInput.h"
#include <stdio.h>

void Player::initPlayer()
{
	m_eType = PLAYER;
	for(int i = 0; i < NUM_SLOTS; ++i)
		m_gauntlet[i] = NULL;
	for(int s = 0; s < WEAPON*NUM_CHIP_SUBS_PER_TYPE; ++s)
	{
		for(int l = 0; l < NUM_CHIP_LEVELS; ++l)
			m_attackInventory[s][l] = NULL;
	}
	m_statPoints = 0;
	m_experience = 0;
	m_expLvReq = m_level+1;
	setVelocity(0,0);
	loadedPlayer = loadPlayer();
}
void Player::addToAttackInventory(Chip * a_chip)
{
	switch(a_chip->getType())
	{
	case MAGIC:
	case WEAPON:
		a_chip->setOwner(this);
		m_attackInventory[a_chip->getSubType()-NUM_CHIP_SUBS_PER_TYPE][a_chip->getSubSubType()] = a_chip;
		break;
	}
}
void Player::setGauntletSlot(e_gauntletSlots a_slot)
{
	static int sub1 = m_gauntlet[SLOT_ATK1]->getSubType();
	static int sub2 = m_gauntlet[SLOT_ATK2]->getSubType();
	if(a_slot == SLOT_ATK1 || a_slot == SLOT_ATK2)
	{
		if(a_slot == SLOT_ATK1)
		{
			sub1++;
			if(sub1 > PIERCE)
				sub1 = DIVINE;
			if(m_gauntlet[SLOT_ATK2] != m_attackInventory[sub1-NUM_CHIP_SUBS_PER_TYPE][m_gauntlet[a_slot]->getSubSubType()])
				setGauntletSlot(a_slot, m_attackInventory[sub1-NUM_CHIP_SUBS_PER_TYPE][m_gauntlet[a_slot]->getSubSubType()]);
		}
		else
		{
			sub2++;
			if(sub2 > PIERCE)
				sub2 = DIVINE;
			if(m_gauntlet[SLOT_ATK1] != m_attackInventory[sub2-NUM_CHIP_SUBS_PER_TYPE][m_gauntlet[a_slot]->getSubSubType()])
				setGauntletSlot(a_slot, m_attackInventory[sub2-NUM_CHIP_SUBS_PER_TYPE][m_gauntlet[a_slot]->getSubSubType()]);
		}
	}
}

Player::~Player()
{
	save();
	for(int i = 0; i < NUM_SLOTS; i++)
	{
		if(loadedPlayer == true)
		{
			if(m_gauntlet[i] != NULL && m_gauntlet[i]->getType() == ARMOR)
			{
				delete m_gauntlet[i];
			}
		}
	}
}

void Player::setGauntletSlot(e_gauntletSlots a_slot, e_chipSubSubType a_level)
{
	if(a_slot == SLOT_ATK1 || a_slot == SLOT_ATK2)
	{
		if(a_slot == SLOT_ATK1)
		{
			if(m_gauntlet[SLOT_ATK2] != m_attackInventory[m_gauntlet[a_slot]->getSubType()-NUM_CHIP_SUBS_PER_TYPE][a_level])
				setGauntletSlot(a_slot, m_attackInventory[m_gauntlet[a_slot]->getSubType()-NUM_CHIP_SUBS_PER_TYPE][a_level]);
		}
		else
		{
			if(m_gauntlet[SLOT_ATK1] != m_attackInventory[m_gauntlet[a_slot]->getSubType()-NUM_CHIP_SUBS_PER_TYPE][a_level])
				setGauntletSlot(a_slot, m_attackInventory[m_gauntlet[a_slot]->getSubType()-NUM_CHIP_SUBS_PER_TYPE][a_level]);
		}
	}
}
//Saves the current Player to a .txt file.
//Saves the current experience, experience required for next level, stat points, max Hp, max Energy, str and int.
//SAVES CURRENT INVENTORY, INCLUDING ARMOR AND CHIPS.
void Player::save()
{
	//File will go like this: P#(Level)#(HP)#(Energy)#(STR)#(Int)#(currentExp)#(ExpRequired)#(StatPoints)#/
	//							A#(SubType)#(SubSubType)#(Def)#(ResistFire)#(ResistIce)#(ResistLigtning)#(level)#/
	//							C#(Type)#(SubType)#(SubSubType)#(Dmg)#(Cost)#(Level)#/
	//Fire, Ice, Lightning, Bludgeoning, Slashing, Divine, Ranged, Piercing, Armor
	//#'s are spaces, for use of fscanf.

	FILE * outfile;
	outfile = fopen("playerSave.txt", "w");
	fprintf(outfile, "P %i %i %i %i %i %f %i %i / ", m_level, m_stats[HEALTH_MAX], m_stats[ENERGY_MAX], m_stats[STRENGTH], m_stats[INTELLECT], m_experience, m_expLvReq, m_statPoints);
	//The Armor
	for(int i = SLOT_ARMOR_HEAD; i < NUM_SLOTS; i++)
	{
		if(m_gauntlet[i])
			fprintf(outfile, "A %i %i %i %i %i %i %i / ", m_gauntlet[SLOT_ARMOR_HEAD]->getSubType() ,m_gauntlet[SLOT_ARMOR_HEAD]->getSubSubType() ,m_gauntlet[SLOT_ARMOR_HEAD]->getStatNumber(DEFENSE), m_gauntlet[SLOT_ARMOR_HEAD]->getStatNumber(RESISTANCE_FIRE), m_gauntlet[SLOT_ARMOR_HEAD]->getStatNumber(RESISTANCE_ICE), m_gauntlet[SLOT_ARMOR_HEAD]->getStatNumber(RESISTANCE_LIGHTNING), m_gauntlet[SLOT_ARMOR_HEAD]->getLevel());
	}
	//The Chips/Attack Inventory.
	//Yes, I know magic number are evil, but as these are debug, we're ok to leave them in there for now.
	//once this works, we can change them easily enough.
	for(int i = 0; i < WEAPON*NUM_CHIP_SUBS_PER_TYPE; i++)
	{
		for(int k = 0; k < NUM_CHIP_LEVELS; k++)
		{
			if(m_attackInventory[i][k] != NULL)
				fprintf(outfile, "C %i %i %i %i %i %i / ", m_attackInventory[i][k]->getType(), m_attackInventory[i][k]->getSubType(), m_attackInventory[i][k]->getSubSubType(), m_attackInventory[i][k]->getDamage(), m_attackInventory[i][k]->getCost(), m_attackInventory[i][k]->getLevel());
		}
	}
	fclose(outfile);

}
bool Player::loadPlayer()
{
	//File will go like this: P#(HP)#(Energy)#(STR)#(Int)#(currentExp)#(ExpRequired)#(StatPoints)#/
	//							A#(Def)#(ResistFire)#(ResistIce)#(ResistLigtning)#/
	//							C#(Type)#(SubType)#(SubSubType)#(Dmg)#(Cost)#/
	
	FILE * infile;
	infile = fopen("playerSave.txt", "r");
	//check to see if playerSave.txt exists, if it doesn't, break out.
	if(infile == NULL)
		return false;

	int hpenstrintexpsta; //The various Player stats.
	int chipAndArmorHelper;
	float exp;
	char charget;
	charget = fgetc(infile);
	while(charget != EOF)
	{
		//if it's reading the Player...
		if(charget == 'P')
		{
			//Level
			fscanf_s(infile, "%i", &hpenstrintexpsta);
			this->m_level = hpenstrintexpsta;
			//HP
			fscanf_s(infile, "%i", &hpenstrintexpsta);
			this->m_stats[HEALTH_CURRENT] = hpenstrintexpsta;
			this->m_stats[HEALTH_MAX] = hpenstrintexpsta;
			//Energy
			fscanf_s(infile, "%i", &hpenstrintexpsta);
			this->m_stats[ENERGY_CURRENT] = hpenstrintexpsta;
			this->m_stats[ENERGY_MAX] = hpenstrintexpsta;
			//Strength
			fscanf_s(infile, "%i", &hpenstrintexpsta);
			this->m_stats[STRENGTH] = hpenstrintexpsta;
			//Intelligence
			fscanf_s(infile, "%i", &hpenstrintexpsta);
			this->m_stats[INTELLECT] = hpenstrintexpsta;
			//Current Exp.
			fscanf_s(infile, "%f", &exp);
			this->m_experience = exp;
			//Current Level Requirement
			fscanf_s(infile, "%i", &hpenstrintexpsta);
			this->m_expLvReq = hpenstrintexpsta;
			//Stat Points.
			fscanf_s(infile, "%i", &hpenstrintexpsta);
			this->m_statPoints = hpenstrintexpsta;
		}
		//If it's reading the Armor...
		else if(charget == 'A')
		{
			//Armor goes like this: Subtype, SubSubType, Def, Resist Fire, resist ice, resist light, level.
			fscanf_s(infile, "%i", &hpenstrintexpsta);
			fscanf_s(infile, "%i", &chipAndArmorHelper);
			//Set the new armor.
			Armor * gear = new Armor((e_chipSubType)hpenstrintexpsta, (e_chipSubSubType)chipAndArmorHelper);
			//Defense.
			fscanf_s(infile, "%i", &hpenstrintexpsta);
			gear->setDefense(hpenstrintexpsta);
			//ResistFire & resistIce
			for(int i = 0; i < 3; i++)
			{
				fscanf_s(infile, "%i", &hpenstrintexpsta);
				if(hpenstrintexpsta != 0)
					gear->setResist(i, hpenstrintexpsta);

			}
			fscanf_s(infile, "%i", &chipAndArmorHelper);
			for(int k = 0; k < chipAndArmorHelper; k++)
			{
				gear->levelUp();
			}
			
			gear->setOwner(this);
			//replace the old one...whatever it was...with the new one loaded from a file.
//			m_gauntlet[SLOT_ARMOR_HEAD]->deactivate();
			setGauntletSlot(SLOT_ARMOR_HEAD, gear);
			m_gauntlet[SLOT_ARMOR_HEAD]->activate();
		}
		//If it's reading the Chips...
		else if(charget == 'C')
		{
			
		}
		//To get rid of the slash
		charget = fgetc(infile);
		//To continue or discontinue with the while loop.
		charget = fgetc(infile);
	}
	return true;
}
void Player::setGauntletSlot(e_gauntletSlots a_slot, Chip * a_chip)
{
	bool isValid = false;
	if(a_chip)
	{
		switch(a_slot)
		{
		case SLOT_ATK1:
		case SLOT_ATK2:
			if(a_chip->getLevel() > 0 && a_chip->getType() != ARMOR)
				isValid = true;
			break;
		case SLOT_ARMOR_HEAD:
			if(a_chip->getLevel() > 0 && a_chip->getType() == ARMOR && a_chip->getSubType() == HEAD)
				isValid = true;
			break;
		case SLOT_ARMOR_TRUNK:
			if(a_chip->getLevel() > 0 && a_chip->getType() == ARMOR && a_chip->getSubType() == TRUNK)
				isValid = true;
			break;
		case SLOT_ARMOR_LIMB_UPPER:
			if(a_chip->getLevel() > 0 && a_chip->getType() == ARMOR && a_chip->getSubType() == LIMB_UPPER)
				isValid = true;
			break;
		case SLOT_ARMOR_LIMB_LOWER:
			if(a_chip->getLevel() > 0 && a_chip->getType() == ARMOR && a_chip->getSubType() == LIMB_LOWER)
				isValid = true;
			break;
		}
	}
	if(isValid)
	{
		//debuff stats gained from armor
		if(a_chip->getType() == ARMOR && m_gauntlet[a_slot])
			m_gauntlet[a_slot]->deactivate();
		m_gauntlet[a_slot] = a_chip;
		//buff stats gained from armor
		if(a_chip->getType() == ARMOR)
			m_gauntlet[a_slot]->activate();
	}
}
void Player::activateGauntletAttack(e_gauntletSlots a_slot, int a_targetX, int a_targetY, char a_direction)
{
	if((a_slot == SLOT_ATK1 || a_slot == SLOT_ATK2) && m_gauntlet[a_slot])
	{
		if(m_stats[ENERGY_CURRENT] >= m_gauntlet[a_slot]->getCost())
		{
			m_gauntlet[a_slot]->setTarget(a_targetX, a_targetY);
			m_gauntlet[a_slot]->setDirection(a_direction);
			m_gauntlet[a_slot]->activate();
		}
	}
}
void Player::handleInput(UserInput * ui, World * a_world)
{
	static char lastKey = KEY_DOWN;
	//This is where the UI goes to get handled by the Player class. Well...it would've been world class, but we dun have one of them yet.
	if(ui->getKeyUD() == KEY_UP)
	{
		m_sprite->setRIndex(ROW_UP);
		setVelocity(m_vel.x, -SPEED_PLAYER);
		lastKey = KEY_UP;
		m_activation = false;
	}
	if(ui->getKeyLR() == KEY_RIGHT)
	{
		m_sprite->setRIndex(ROW_RIGHT);
		setVelocity(SPEED_PLAYER, m_vel.y);
		lastKey = KEY_RIGHT;
		m_activation = false;
	}
	if(ui->getKeyUD() == KEY_DOWN)
	{
		m_sprite->setRIndex(ROW_DOWN);
		setVelocity(m_vel.x, SPEED_PLAYER);
		lastKey = KEY_DOWN;
		m_activation = false;
	}
	if(ui->getKeyLR() == KEY_LEFT)
	{
		m_sprite->setRIndex(ROW_LEFT);
		setVelocity(-SPEED_PLAYER, m_vel.y);
		lastKey = KEY_LEFT;
		m_activation = false;
	}
	if(ui->getKeyLR() == KEY_NONE)
		setVelocity(0, m_vel.y);
	if(ui->getKeyUD() == KEY_NONE)
		setVelocity(m_vel.x, 0);

	switch(ui->getHKeyL())
	{
	case KEY_HOT_ATK1_BAS:
		setGauntletSlot(SLOT_ATK1, BASIC);
		break;
	case KEY_HOT_ATK1_ADV:
		setGauntletSlot(SLOT_ATK1, ADVANCED);
		break;
	case KEY_HOT_ATK1_EXP:
		setGauntletSlot(SLOT_ATK1, EXPERT);
		break;
	case KEY_HOT_ATK1_LEG:
		setGauntletSlot(SLOT_ATK1);
		break;
	}
	switch(ui->getHKeyR())
	{
	case KEY_HOT_ATK2_BAS:
		setGauntletSlot(SLOT_ATK2, BASIC);
		break;
	case KEY_HOT_ATK2_ADV:
		setGauntletSlot(SLOT_ATK2, ADVANCED);
		break;
	case KEY_HOT_ATK2_EXP:
		setGauntletSlot(SLOT_ATK2, EXPERT);
		break;
	case KEY_HOT_ATK2_LEG:
		setGauntletSlot(SLOT_ATK2);
		break;
	}
	if(ui->getClick() == CLICK_LEFT && ui->getMouseY() < HUD_Y)
		activateGauntletAttack(SLOT_ATK1, m_cameraP.x + ui->getMouseX(), m_cameraP.y + ui->getMouseY(), lastKey);
	if(ui->getClick() == CLICK_RIGHT && ui->getMouseY() < HUD_Y)
		activateGauntletAttack(SLOT_ATK2, m_cameraP.x + ui->getMouseX(), m_cameraP.y + ui->getMouseY(), lastKey);
	if(ui->getSpace() == true)
		m_activation = true;
}