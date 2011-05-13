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
		m_armorInventory[s] = NULL;
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
void Player::drawInventory(SDL_Surface * a_screen, int a_x, int a_y, int a_columns)
{
	int x = a_x, y = a_y;
	for(int i = 0; i < WEAPON*NUM_CHIP_SUBS_PER_TYPE; ++i)
	{
		if(m_armorInventory[i])
			m_armorInventory[i]->drawHUD(a_screen, x, y);
		x += FRAME_SIZE;
		if((x-a_x)/FRAME_SIZE > a_columns)
		{
			x = a_x;
			y += FRAME_SIZE;
		}
	}
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
void Player::addToArmorInventory(Chip * a_chip)
{
	bool slotFound = false;
	if(a_chip->getType() == ARMOR)
	{
		for(int i = 0; i < WEAPON*NUM_CHIP_SUBS_PER_TYPE; ++i)
		{
			if(!m_armorInventory[i] && !slotFound)
			{
				a_chip->setOwner(this);
				m_armorInventory[i] = a_chip;
				slotFound = true;
			}
		}
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
	for(int i = 0; i < WEAPON*NUM_CHIP_SUBS_PER_TYPE; ++i)
	{
		if(m_armorInventory[i])
		{
			if(m_armorInventory[i]->getFlag(FLAG_NUDE))
				delete m_armorInventory[i];
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
	//							C#(Type)#(SubType)#(SubSubType)#(isEquipped)#(Cost)#(Level)#/
	//Fire, Ice, Lightning, Bludgeoning, Slashing, Divine, Ranged, Piercing, Armor
	//#'s are spaces, for use of fscanf.

	FILE * outfile;
	outfile = fopen("playerSave.txt", "w");
	fprintf(outfile, "P %i %i %i %i %i %f %i %i / ", m_stats[LEVEL], m_stats[HEALTH_MAX], m_stats[ENERGY_MAX], m_stats[STRENGTH], m_stats[INTELLECT], m_experience, m_expLvReq, m_statPoints);
	//The Armor
	for(int i = 0; i < WEAPON*NUM_CHIP_SUBS_PER_TYPE; ++i)
	{
		if(m_armorInventory[i])
			fprintf(outfile, "A %i %i %i %i %i %i %i / ", m_armorInventory[i]->getSubType() ,m_armorInventory[i]->getSubSubType() ,m_armorInventory[i]->getStatNumber(DEFENSE), m_armorInventory[i]->getStatNumber(RESISTANCE_FIRE), m_armorInventory[i]->getStatNumber(RESISTANCE_ICE), m_armorInventory[i]->getStatNumber(RESISTANCE_LIGHTNING), m_armorInventory[i]->getStatNumber(LEVEL));
	}
	//The Chips/Attack Inventory.
	fprintf(outfile, "C %i %i %i %i %i %i / ", m_gauntlet[SLOT_ATK1]->getType(), m_gauntlet[SLOT_ATK1]->getSubType(), m_gauntlet[SLOT_ATK1]->getSubSubType(), m_gauntlet[SLOT_ATK1]->isEquipped(), m_gauntlet[SLOT_ATK1]->getCost(), m_gauntlet[SLOT_ATK1]->getStatNumber(LEVEL));
	for(int i = 0; i < WEAPON*NUM_CHIP_SUBS_PER_TYPE; i++)
	{
		for(int k = 0; k < NUM_CHIP_LEVELS; k++)
		{
			if(m_attackInventory[i][k])
			{
				if(m_attackInventory[i][k] != m_gauntlet[SLOT_ATK1])
					fprintf(outfile, "C %i %i %i %i %i %i / ", m_attackInventory[i][k]->getType(), m_attackInventory[i][k]->getSubType(), m_attackInventory[i][k]->getSubSubType(), m_attackInventory[i][k]->isEquipped(), m_attackInventory[i][k]->getCost(), m_attackInventory[i][k]->getStatNumber(LEVEL));
			}
		}
	}
	fclose(outfile);

}
bool Player::loadPlayer()
{
	//File will go like this: P#(HP)#(Energy)#(STR)#(Int)#(currentExp)#(ExpRequired)#(StatPoints)#/
	//							A#(Def)#(ResistFire)#(ResistIce)#(ResistLigtning)#/
	//							C#(Type)#(SubType)#(SubSubType)#(isEquipped)#(Cost)#(Level)#/
	
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
			//Level (IGNORE)
			fscanf_s(infile, "%i", &hpenstrintexpsta);

			//HP (IGNORE)
			fscanf_s(infile, "%i", &hpenstrintexpsta);

			//Energy (IGNORE)
			fscanf_s(infile, "%i", &hpenstrintexpsta);

			//Strength
				//make sure stored value is valid
			fscanf_s(infile, "%i", &hpenstrintexpsta);
			this->m_stats[STRENGTH] = hpenstrintexpsta;
			if(this->m_stats[STRENGTH] < 0)
				this->m_stats[STRENGTH] = 0;
				//update related stats
			this->m_stats[HEALTH_MAX] += this->m_stats[STRENGTH];
			this->m_stats[DEFENSE] += this->m_stats[STRENGTH];

			//Intelligence
				//make sure stored value is valid
			fscanf_s(infile, "%i", &hpenstrintexpsta);
			this->m_stats[INTELLECT] = hpenstrintexpsta;
			if(this->m_stats[INTELLECT] < 0)
				this->m_stats[INTELLECT] = 0;
				//update related stats
			this->m_stats[ENERGY_MAX] += this->m_stats[INTELLECT];
			m_stats[ENERGY_REGEN] = (int)(m_stats[ENERGY_MAX] * .05);
			for(int i = RESISTANCE_FIRE; i < RESISTANCE_FIRE+3; ++i)
				m_stats[i] += this->m_stats[INTELLECT];

			//Current Exp.
				//make sure stored value is valid
			fscanf_s(infile, "%f", &exp);
			this->m_experience = exp;
			if(this->m_experience < 0)
				this->m_experience = 0;

			//Current Level Requirement (IGNORE)
			fscanf_s(infile, "%i", &hpenstrintexpsta);

			//Stat Points.
				//make sure stored value is valid
			fscanf_s(infile, "%i", &hpenstrintexpsta);
			this->m_statPoints = hpenstrintexpsta;
			if(this->m_statPoints < 0)
				this->m_statPoints = 0;
				//make sure level is valid
					//lv = default level (1) + strength + intellect + leftover stat points
			int lvShould = this->m_stats[LEVEL] + this->m_stats[STRENGTH] + this->m_stats[INTELLECT] + this->m_statPoints;
			int ptsBeforeLeveling = this->m_statPoints;
			for(int i = this->m_stats[LEVEL]; i < lvShould; ++i)
				this->levelUp();
				//make sure exp is valid
			this->m_statPoints = ptsBeforeLeveling;
			if(this->m_experience >= this->m_expLvReq)
			{
				int expToScaleDown = (int)this->m_experience;
				this->m_experience -= expToScaleDown;
				this->m_experience += expToScaleDown % this->m_expLvReq;
			}
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
				gear->levelUp();
			this->addToArmorInventory(gear);
			//replace the old one...whatever it was...with the new one loaded from a file.
			switch(gear->getSubType())
			{
			case HEAD:			setGauntletSlot(SLOT_ARMOR_HEAD, gear);			break;
			case TRUNK:			setGauntletSlot(SLOT_ARMOR_TRUNK, gear);		break;
			case LIMB_UPPER:	setGauntletSlot(SLOT_ARMOR_LIMB_UPPER, gear);	break;
			case LIMB_LOWER:	setGauntletSlot(SLOT_ARMOR_LIMB_LOWER, gear);	break;
			}
		}
		//If it's reading the Chips...
		else if(charget == 'C' || charget == 'S')
		{
			//C#(Type)#(SubType)#(SubSubType)#(isEquipped)#(Cost)#(Level)#/
			//Minus the #'s.
			//For example C 1 4 0 is Type MAGIC, Subtype DIVINE, SubSubType BASIC.

			//type
			fscanf_s(infile, "%i", &hpenstrintexpsta);
			//sub-type
			fscanf_s(infile, "%i", &chipAndArmorHelper);
			Chip * iHopeThisWorks;
			switch(hpenstrintexpsta)
			{
			case ARMOR:
				break;
			case MAGIC:
				//sub-sub-type
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
				//sub-sub-type
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
			//equip
			fscanf_s(infile, "%i", &hpenstrintexpsta);
			bool equip = hpenstrintexpsta;
			//cost (IGNORE)
			fscanf_s(infile, "%i", &chipAndArmorHelper);
			//level
			fscanf_s(infile, "%i", &hpenstrintexpsta);
			for(int i = 0; i < hpenstrintexpsta; i++)
				iHopeThisWorks->levelUp();
			this->addToAttackInventory(iHopeThisWorks);
			thisWorld->add(iHopeThisWorks);
			if(equip)
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
	this->addToArmorInventory(gear);
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
	switch(gear->getSubType())
	{
	case HEAD:			setGauntletSlot(SLOT_ARMOR_HEAD, gear);			break;
	case TRUNK:			setGauntletSlot(SLOT_ARMOR_TRUNK, gear);		break;
	case LIMB_UPPER:	setGauntletSlot(SLOT_ARMOR_LIMB_UPPER, gear);	break;
	case LIMB_LOWER:	setGauntletSlot(SLOT_ARMOR_LIMB_LOWER, gear);	break;
	}
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
		//take out current Chip
		if(m_gauntlet[a_slot])
		{
			m_gauntlet[a_slot]->unequip();
			m_gauntlet[a_slot]->deactivate();
		}
		//put in selected Chip
		m_gauntlet[a_slot] = a_chip;
		m_gauntlet[a_slot]->equip();
		//buff stats gained from armor
		if(a_chip->getType() == ARMOR)
			m_gauntlet[a_slot]->activate();
	}
}
void Player::activateGauntletAttack(e_gauntletSlots a_slot, int a_targetX, int a_targetY, char a_direction, AudioHandler * ah)
{
	if((a_slot == SLOT_ATK1 || a_slot == SLOT_ATK2) && m_gauntlet[a_slot])
	{
		if(m_stats[ENERGY_CURRENT] >= m_gauntlet[a_slot]->getCost())
		{
			m_gauntlet[a_slot]->setTarget(a_targetX, a_targetY);
			m_gauntlet[a_slot]->setDirection(a_direction);
			m_gauntlet[a_slot]->activate();
			switch(m_gauntlet[a_slot]->getSubType())
			{
			case DIVINE:	ah->playEffect(E_DIVINE);		break;
			case LIGHTNING:	ah->playEffect(E_LIGHTNING);	break;
			case FIRE:		ah->playEffect(E_FIRE);			break;
			case ICE:		ah->playEffect(E_ICE);			break;
			case BLUNT:		ah->playEffect(E_BLUNT);		break;
			case SLASH:		ah->playEffect(E_SLASH);		break;
			}
		}
	}
}
void Player::handleInput(UserInput * ui, World * a_world, AudioHandler *ah)
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
		activateGauntletAttack(SLOT_ATK1, m_cameraP.x + ui->getMouseX(), m_cameraP.y + ui->getMouseY(), lastKey, ah);

	if(ui->getClick() == CLICK_RIGHT && ui->getMouseY() < HUD_Y)
		activateGauntletAttack(SLOT_ATK2, m_cameraP.x + ui->getMouseX(), m_cameraP.y + ui->getMouseY(), lastKey, ah);
	if(ui->getSpace() == true)
		m_flags[FLAG_ACTIVE] = true;
}