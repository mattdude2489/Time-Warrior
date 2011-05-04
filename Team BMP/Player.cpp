//Basic Player Class Extensions.
#include "Player.h"
#include "UserInput.h"
#include <stdio.h>

void Player::initPlayer(World * newWorld)
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
	m_expLvReq = m_stats[LEVEL]+1;
	setVelocity(0,0);
	thisWorld = newWorld;
	if(!loadPlayer())
		newGame();
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
	for(int i = 0; i < NUM_SLOTS; i++) //Deletes the Armor from memory.
	{
		//if(m_gauntlet[i] != NULL && m_gauntlet[i]->getType() == ARMOR)
		if(m_gauntlet[i] != NULL) 
		{
			if(m_gauntlet[i]->getFlag(FLAG_NUDE) && m_gauntlet[i]->getType() == ARMOR)
				delete m_gauntlet[i];
		}
	}
	//for(int i = 0; i < WEAPON*NUM_CHIP_SUBS_PER_TYPE; i++) //Deletes the chips from memory.
	//{
	//	for(int k = 0; k < NUM_CHIP_LEVELS; k++)
	//	{
	//		if(m_attackInventory[i][k] != NULL)
	//			if(m_attackInventory[i][k]->getFlag(FLAG_NUDE))
	//				delete m_attackInventory[i][k];
	//	}
	//}
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
	fprintf(outfile, "P %i %i %i %i %i %f %i %i / ", m_stats[LEVEL], m_stats[HEALTH_MAX], m_stats[ENERGY_MAX], m_stats[STRENGTH], m_stats[INTELLECT], m_experience, m_expLvReq, m_statPoints);
	//The Armor
	for(int i = SLOT_ARMOR_HEAD; i < NUM_SLOTS; i++)
	{
		if(m_gauntlet[i])
			fprintf(outfile, "A %i %i %i %i %i %i %i / ", m_gauntlet[i]->getSubType() ,m_gauntlet[i]->getSubSubType() ,m_gauntlet[i]->getStatNumber(DEFENSE), m_gauntlet[i]->getStatNumber(RESISTANCE_FIRE), m_gauntlet[i]->getStatNumber(RESISTANCE_ICE), m_gauntlet[i]->getStatNumber(RESISTANCE_LIGHTNING), m_gauntlet[i]->getStatNumber(LEVEL));
	}
	//The Chips/Attack Inventory.
	//Yes, I know magic number are evil, but as these are debug, we're ok to leave them in there for now.
	//once this works, we can change them easily enough.
	//Before the double for loops, put the stuff into here.
	bool slotChipsIn = false;
	fprintf(outfile, "S %i %i %i %i %i %i / ", m_gauntlet[slotChipsIn]->getType(), m_gauntlet[slotChipsIn]->getSubType(), m_gauntlet[slotChipsIn]->getSubSubType(), m_gauntlet[slotChipsIn]->getDamage(), m_gauntlet[slotChipsIn]->getCost(), m_gauntlet[slotChipsIn]->getStatNumber(LEVEL));
	slotChipsIn = true;
	fprintf(outfile, "S %i %i %i %i %i %i / ", m_gauntlet[slotChipsIn]->getType(), m_gauntlet[slotChipsIn]->getSubType(), m_gauntlet[slotChipsIn]->getSubSubType(), m_gauntlet[slotChipsIn]->getDamage(), m_gauntlet[slotChipsIn]->getCost(), m_gauntlet[slotChipsIn]->getStatNumber(LEVEL));
	for(int i = 0; i < WEAPON*NUM_CHIP_SUBS_PER_TYPE; i++)
	{
		for(int k = 0; k < NUM_CHIP_LEVELS; k++)
		{
			if(m_attackInventory[i][k] != NULL)
			{
				if(!((m_attackInventory[i][k]->getType() == m_gauntlet[0]->getType() && m_attackInventory[i][k]->getSubType() == m_gauntlet[0]->getSubType() && m_attackInventory[i][k]->getSubSubType() == m_gauntlet[0]->getSubSubType())
					|| (m_attackInventory[i][k]->getType() == m_gauntlet[1]->getType() && m_attackInventory[i][k]->getSubType() == m_gauntlet[1]->getSubType() && m_attackInventory[i][k]->getSubSubType() == m_gauntlet[1]->getSubSubType())))
				{
					fprintf(outfile, "C %i %i %i %i %i %i / ", m_attackInventory[i][k]->getType(), m_attackInventory[i][k]->getSubType(), m_attackInventory[i][k]->getSubSubType(), m_attackInventory[i][k]->getDamage(), m_attackInventory[i][k]->getCost(), m_attackInventory[i][k]->getStatNumber(LEVEL));
				}
			}
		}
	}
	fclose(outfile);

}
bool Player::loadPlayer()
{
	//File will go like this: P#(HP)#(Energy)#(STR)#(Int)#(currentExp)#(ExpRequired)#(StatPoints)#/
	//							A#(Def)#(ResistFire)#(ResistIce)#(ResistLigtning)#/
	//							C#(Type)#(SubType)#(SubSubType)#(Dmg)#(Cost)#(Level)#/
	
	FILE * infile;
	infile = fopen("playerSave.txt", "r");
	//check to see if playerSave.txt exists, if it doesn't, break out.
	if(infile == NULL)
		return false;

	int hpenstrintexpsta; //The various Player stats.
	int chipAndArmorHelper;
	float exp;
	static int gauntletSlotSetter = 0;
	char charget;
	charget = fgetc(infile);
	while(charget != EOF)
	{
		//if it's reading the Player...
		if(charget == 'P')
		{
			//Level
			fscanf_s(infile, "%i", &hpenstrintexpsta);
			this->m_stats[LEVEL] = hpenstrintexpsta;
			//HP
			fscanf_s(infile, "%i", &hpenstrintexpsta);
			//Energy
			fscanf_s(infile, "%i", &hpenstrintexpsta);
			//Strength
			fscanf_s(infile, "%i", &hpenstrintexpsta);
			this->m_stats[STRENGTH] = hpenstrintexpsta;
			if(this->m_stats[STRENGTH] < 0)
				this->m_stats[STRENGTH] = 0;
			this->m_stats[HEALTH_MAX] += this->m_stats[STRENGTH];
			this->m_stats[HEALTH_CURRENT] = this->m_stats[HEALTH_MAX];
			this->m_stats[DEFENSE] += this->m_stats[STRENGTH];
			//Intelligence
			fscanf_s(infile, "%i", &hpenstrintexpsta);
			this->m_stats[INTELLECT] = hpenstrintexpsta;
			if(this->m_stats[INTELLECT] < 0)
				this->m_stats[INTELLECT] = 0;
			this->m_stats[ENERGY_MAX] += this->m_stats[INTELLECT];
			this->m_stats[ENERGY_CURRENT] = this->m_stats[ENERGY_MAX];
			m_stats[ENERGY_REGEN] = (int)(m_stats[ENERGY_MAX] * .05);
			for(int i = RESISTANCE_FIRE; i < RESISTANCE_FIRE+3; ++i)
				m_stats[i] += this->m_stats[INTELLECT];
			//Current Exp.
			fscanf_s(infile, "%f", &exp);
			this->m_experience = exp;
			if(this->m_experience < 0)
				this->m_experience = 0;
			//Current Level Requirement
			fscanf_s(infile, "%i", &hpenstrintexpsta);
			this->m_expLvReq = hpenstrintexpsta;
			if(this->m_expLvReq < 0)
				this->m_expLvReq = 0;
			//Stat Points.
			fscanf_s(infile, "%i", &hpenstrintexpsta);
			this->m_statPoints = hpenstrintexpsta;
			if(this->m_statPoints < 0)
				this->m_statPoints = 0;
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
				gear->setResist(i, hpenstrintexpsta);
			}
			fscanf_s(infile, "%i", &chipAndArmorHelper);
			for(int k = 0; k < chipAndArmorHelper; k++)
			{
				gear->levelUp();
			}
			
			gear->setOwner(this);
			//replace the old one...whatever it was...with the new one loaded from a file.
			setGauntletSlot(SLOT_ARMOR_HEAD, gear);
		}
		//If it's reading the Chips...
		else if(charget == 'C' || charget == 'S')
		{
			//C#(Type)#(SubType)#(SubSubType)#(Dmg)#(Cost)#(Level)#/
			//Minus the #'s.
			//For example C 1 4 0 is Type MAGIC, Subtype DIVINE, SubSubType BASIC.
			fscanf_s(infile, "%i", &hpenstrintexpsta);
			fscanf_s(infile, "%i", &chipAndArmorHelper);
			Chip * iHopeThisWorks;
			switch(hpenstrintexpsta)
			{
			case ARMOR:
				break;
			case MAGIC:
				fscanf_s(infile, "%i", &hpenstrintexpsta);
				switch(chipAndArmorHelper)
				{
				case DIVINE:	iHopeThisWorks = new Divine((e_chipSubSubType)hpenstrintexpsta);	break;
				case LIGHTNING:	iHopeThisWorks = new Lightning((e_chipSubSubType)hpenstrintexpsta);	break;
				case FIRE:		iHopeThisWorks = new Fire((e_chipSubSubType)hpenstrintexpsta);		break;
				case ICE:		iHopeThisWorks = new Ice((e_chipSubSubType)hpenstrintexpsta);		break;
				}
				break;
			case WEAPON:
				fscanf_s(infile, "%i", &hpenstrintexpsta);
				switch(chipAndArmorHelper)
				{
				case BLUNT:		iHopeThisWorks = new Blunt((e_chipSubSubType)hpenstrintexpsta);		break;
				//case RANGE:		iHopeThisWorks = new Ranged((e_chipSubSubType)hpenstrintexpsta);	break;
				case SLASH:		iHopeThisWorks = new Slash((e_chipSubSubType)hpenstrintexpsta);		break;
				//case PIERCE:	iHopeThisWorks = new Pierce((e_chipSubSubType)hpenstrintexpsta);	break;
				}
			}
			iHopeThisWorks->setNewed(true);
			fscanf_s(infile, "%i", &hpenstrintexpsta);
			fscanf_s(infile, "%i", &chipAndArmorHelper);
			fscanf_s(infile, "%i", &hpenstrintexpsta);
			//To get it to where the level is.
			for(int i = 0; i < hpenstrintexpsta; i++)
			{
				iHopeThisWorks->levelUp();
			}
			this->addToAttackInventory(iHopeThisWorks);
			thisWorld->add(iHopeThisWorks);
			if(charget == 'S')
			{
				this->setGauntletSlot((e_gauntletSlots)gauntletSlotSetter, iHopeThisWorks);
				gauntletSlotSetter++;
			}
		}
		//To get rid of the slash
		charget = fgetc(infile);
		//To continue or discontinue with the while loop.
		charget = fgetc(infile);
	}
	return true;
}
void Player::newGame()
{
	Blunt * b1 = new Blunt(BASIC);
	Blunt * b2 = new Blunt(ADVANCED);
	Blunt * b3 = new Blunt(EXPERT);
	Slash * s1 = new Slash(BASIC);
	Slash * s2 = new Slash(ADVANCED);
	Slash * s3 = new Slash(EXPERT);
	Divine * d1 = new Divine(BASIC);
	Divine * d2 = new Divine(ADVANCED);
	Divine * d3 = new Divine(EXPERT);
	Fire * f1 = new Fire(BASIC);
	Fire * f2 = new Fire(ADVANCED);
	Fire * f3 = new Fire(EXPERT);
	Armor * gear = new Armor(HEAD, BASIC);
	b1->levelUp();
	b2->levelUp();
	b3->levelUp();
	s1->levelUp();
	s2->levelUp();
	s3->levelUp();
	d1->levelUp();
	d2->levelUp();
	d3->levelUp();
	f1->levelUp();
	f2->levelUp();
	f3->levelUp();
	gear->levelUp();
	this->addToAttackInventory(b1);
	this->addToAttackInventory(b2);
	this->addToAttackInventory(b3);
	this->addToAttackInventory(s1);
	this->addToAttackInventory(s2);
	this->addToAttackInventory(s3);
	this->addToAttackInventory(d1);
	this->addToAttackInventory(d2);
	this->addToAttackInventory(d3);
	this->addToAttackInventory(f1);
	this->addToAttackInventory(f2);
	this->addToAttackInventory(f3);
	gear->setOwner(this);
	b1->setNewed(true);
	b2->setNewed(true);
	b3->setNewed(true);
	s1->setNewed(true);
	s2->setNewed(true);
	s3->setNewed(true);
	d1->setNewed(true);
	d2->setNewed(true);
	d3->setNewed(true);
	f1->setNewed(true);
	f2->setNewed(true);
	f3->setNewed(true);
	gear->setNewed(true);
	thisWorld->add(b1);
	thisWorld->add(b2);
	thisWorld->add(b3);
	thisWorld->add(s1);
	thisWorld->add(s2);
	thisWorld->add(s3);
	thisWorld->add(d1);
	thisWorld->add(d2);
	thisWorld->add(d3);
	thisWorld->add(f1);
	thisWorld->add(f2);
	thisWorld->add(f3);
	this->setGauntletSlot(SLOT_ATK1, s1);
	this->setGauntletSlot(SLOT_ATK2, f1);
	this->setGauntletSlot(SLOT_ARMOR_HEAD, gear);
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
			if(a_chip->getStatNumber(LEVEL) > 0 && a_chip->getType() != ARMOR)
				isValid = true;
			break;
		case SLOT_ARMOR_HEAD:
			if(a_chip->getStatNumber(LEVEL) > 0 && a_chip->getType() == ARMOR && a_chip->getSubType() == HEAD)
				isValid = true;
			break;
		case SLOT_ARMOR_TRUNK:
			if(a_chip->getStatNumber(LEVEL) > 0 && a_chip->getType() == ARMOR && a_chip->getSubType() == TRUNK)
				isValid = true;
			break;
		case SLOT_ARMOR_LIMB_UPPER:
			if(a_chip->getStatNumber(LEVEL) > 0 && a_chip->getType() == ARMOR && a_chip->getSubType() == LIMB_UPPER)
				isValid = true;
			break;
		case SLOT_ARMOR_LIMB_LOWER:
			if(a_chip->getStatNumber(LEVEL) > 0 && a_chip->getType() == ARMOR && a_chip->getSubType() == LIMB_LOWER)
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
		m_gauntlet[a_slot]->toggleEquip();
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
		m_flags[FLAG_ACTIVE] = false;
	}
	if(ui->getKeyLR() == KEY_RIGHT)
	{
		m_sprite->setRIndex(ROW_RIGHT);
		setVelocity(SPEED_PLAYER, m_vel.y);
		lastKey = KEY_RIGHT;
		m_flags[FLAG_ACTIVE] = false;
	}
	if(ui->getKeyUD() == KEY_DOWN)
	{
		m_sprite->setRIndex(ROW_DOWN);
		setVelocity(m_vel.x, SPEED_PLAYER);
		lastKey = KEY_DOWN;
		m_flags[FLAG_ACTIVE] = false;
	}
	if(ui->getKeyLR() == KEY_LEFT)
	{
		m_sprite->setRIndex(ROW_LEFT);
		setVelocity(-SPEED_PLAYER, m_vel.y);
		lastKey = KEY_LEFT;
		m_flags[FLAG_ACTIVE] = false;
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
		m_flags[FLAG_ACTIVE] = true;
}