//Basic Player Class Extensions.
#include "Player.h"
#include "UserInput.h"
#include "ButtonAndWIndow.h"
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
	m_experience = 0;
	m_expLvReq = m_stats[LEVEL]+1;
	for(int i = 0; i < 20; i++)
	{
		playerName[i] = ' ';
	}
	playerName[20] = 0; //I hate the null terminator. I hate it so fucking much.
	setVelocity(0,0);
	thisWorld = newWorld;
	gamePlayed = false;
	/*if(!loadPlayer(0))
		newGame();*/
	m_isStatWindowActive = false;
	m_blankInventory = new SDL_Sprite("Sprites/button1.bmp", FRAME_SIZE, FRAME_SIZE, FRAME_RATE, 2);
	m_blankInventory->setTransparency(COLOR_TRANSPARENT);
}
//draws & formats the display of specified Chip collection
//param:
//	int a_x, a_y		: location of upper-left corner to start drawing
//	e_inventory a_type	: which collection to draw (INVENTORY_ATTACK, INVENTORY_ARMOR, INVENTORY_GAUNTLET)
//	int a_maxColumns	: formats # of columns to display (0 for unlimited columns - a.k.a. as many as it takes)
//	int a_maxNum		: formats # of Chips to draw (0 to draw all)
//	int a_startIndex	: specifies index of which Chip to start drawing process (which to draw 1st)
//returns # of rows it took to draw formatted collection
int Player::drawInventory(SDL_Surface * a_screen, int a_x, int a_y, e_inventory a_type, int a_maxColumns, int a_maxNum, int a_startIndex)
{
	bool valid = false;
	int amt = 0, x = a_x, y = a_y, rows = 0;
	Chip * test = NULL;
	//get size of specified inventory
	switch(a_type)
	{
		case INVENTORY_ATTACK:	amt = WEAPON*NUM_CHIP_SUBS_PER_TYPE*NUM_CHIP_LEVELS;	break;
		case INVENTORY_ARMOR:	amt = WEAPON*NUM_CHIP_SUBS_PER_TYPE;					break;
		default:				amt = NUM_SLOTS;
	}
	//check if the starting index is valid
	if(a_startIndex < 0 || a_startIndex >= amt)
		a_startIndex = 0;
	//draw contents, starting @ specified index
	for(int i = a_startIndex; i < amt; ++i)
	{
		//get Chip from inventory to test
		switch(a_type)
		{
			case INVENTORY_ATTACK:	test = m_attackInventory[i/NUM_CHIP_LEVELS][i%NUM_CHIP_LEVELS];	break;
			case INVENTORY_ARMOR:	test = m_armorInventory[i];										break;
			default:				test = m_gauntlet[i];
		}
		//draw Chip if it exists & can be used, or draw blank
		valid = false;
		if(test)
			valid = test->getStatNumber(LEVEL) > 0;
		if(test && valid)
		{
			test->drawHUD(a_screen, x, y);
			if(test->isEquipped() && a_type != INVENTORY_GAUNTLET)
			{
				m_blankInventory->setRIndex(1);
				m_blankInventory->draw(a_screen, x, y);
			}
		}
		else
		{
			m_blankInventory->setRIndex(0);
			m_blankInventory->draw(a_screen, x, y);
		}
		//calc # of rows it took to draw the inventory with the given format
		rows = ((y-a_y)/FRAME_SIZE) + 1;
		//update position for next iteration
		x += FRAME_SIZE;
		if((x-a_x)/FRAME_SIZE >= a_maxColumns && a_maxColumns > 0)
		{
			x = a_x;
			y += FRAME_SIZE;
		}
		//check if # restriction has been met
		if((i-a_startIndex)+1 >= a_maxNum && a_maxNum > 0)
			break;
	}
	//return # of rows it took to draw the inventory with the given format
	return rows;
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
	if(gamePlayed)
		save(saveFile);
	delete m_blankInventory;
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
void Player::save(int saveToSave)
{
	//File will go like this: PlayerName#(Level)#P#(HP)#(Energy)#(Str)#(Int)#(currentExp)#(ExpRequired)#(StatPoints)#/
	//							A#(SubType)#(SubSubType)#(Level)#(Def)#(ResistFire)#(ResistIce)#(ResistLigtning)#(isEquipped)#/
	//							C#(Type)#(SubType)#(SubSubType)#(Level)#(Cost)#(isEquipped)#/
	//Fire, Ice, Lightning, Bludgeoning, Slashing, Divine, Ranged, Piercing, Armor
	//#'s are spaces, for use of fscanf.

	FILE * outfile;
	if(loadedPlayer)
	{
		outfile = fopen("playerSave.txt", "r+");
		if(outfile == NULL)
			outfile = fopen("playerSave.txt", "w+");
	}
	else
		outfile = fopen("playerSave.txt", "a+");

	fpos_t pos = 0;
	char newArray[10000]; //Please don't kill me.
	int arrIndex = 0;
	for(int i = 0; i < 10000; i++)
	{
		newArray[i] = 10; //PLEASE PLEASE PLEASE DON'T KILL ME.
	}
	//newArray[9999] = 0; //Let's see if this gets rid of that character 152.
	//newArray[10000] = 0; //Null Tahminatah.
	//When here, search through the playerSave until it finds the correct save to save To.
	//once that is found, save the current position in a Position pointer for the FILE stream.
	//Then find the next save in the list (if there is one)
	//and store EVERYTHING AFTER THAT in a char* array. Static, of course.
	//After that, save like normal.
	//Then take everything that's in that char * and PUT IT BACK. This will prevent the saves from overwriting each other.
	if(loadedPlayer)
	{
		char charget;
		//Do what the comment above says.
		while(saveToSave != 0)
		{
			charget = fgetc(outfile);
			if(charget == '#')
				saveToSave--;
		}
		fgetpos(outfile, &pos);
		charget = fgetc(outfile);
		while(charget != '#' && charget != EOF)
			charget = fgetc(outfile); //Get it to the next save...
		if(charget != EOF)
		{
			//charget = fgetc(outfile);
			while(charget != EOF)
			{
				newArray[arrIndex] = charget = fgetc(outfile);
				arrIndex++;
			}
		}
		else
		{
			//Do nothing.
		}
		fsetpos(outfile, &pos); //Reset the cursor to prepare to overwrite.
	}
	//else if(!loadedPlayer)
	//{
	//	//Go to the EOF, and then save.
	//	char charget = fgetc(outfile);
	//	while(charget != EOF)
	//		charget = fgetc(outfile);
	//	fflush(outfile); //To switch the operation to writing.
	//}

	fprintf(outfile, " P %s %i %i %i %i %i %f %i %i / ", playerName , m_stats[LEVEL], m_stats[HEALTH_MAX], m_stats[ENERGY_MAX], m_stats[STRENGTH], m_stats[INTELLECT], m_experience, m_expLvReq, m_statPoints);
	//The Armor
	for(int i = 0; i < WEAPON*NUM_CHIP_SUBS_PER_TYPE; ++i)
	{
		if(m_armorInventory[i])
			fprintf(outfile, "A %i %i %i %i %i %i %i %i / ", m_armorInventory[i]->getSubType(), m_armorInventory[i]->getSubSubType(), m_armorInventory[i]->getStatNumber(LEVEL), m_armorInventory[i]->getStatNumber(DEFENSE), m_armorInventory[i]->getStatNumber(RESISTANCE_FIRE), m_armorInventory[i]->getStatNumber(RESISTANCE_ICE), m_armorInventory[i]->getStatNumber(RESISTANCE_LIGHTNING), m_armorInventory[i]->isEquipped());
	}
	//The Chips/Attack Inventory.
	fprintf(outfile, "C %i %i %i %i %i %i / ", m_gauntlet[SLOT_ATK1]->getType(), m_gauntlet[SLOT_ATK1]->getSubType(), m_gauntlet[SLOT_ATK1]->getSubSubType(), m_gauntlet[SLOT_ATK1]->getStatNumber(LEVEL), m_gauntlet[SLOT_ATK1]->getCost(), m_gauntlet[SLOT_ATK1]->isEquipped());
	for(int i = 0; i < WEAPON*NUM_CHIP_SUBS_PER_TYPE; i++)
	{
		for(int k = 0; k < NUM_CHIP_LEVELS; k++)
		{
			if(m_attackInventory[i][k])
			{
				if(m_attackInventory[i][k] != m_gauntlet[SLOT_ATK1])
					fprintf(outfile, "C %i %i %i %i %i %i / ", m_attackInventory[i][k]->getType(), m_attackInventory[i][k]->getSubType(), m_attackInventory[i][k]->getSubSubType(), m_attackInventory[i][k]->getStatNumber(LEVEL), m_attackInventory[i][k]->getCost(), m_attackInventory[i][k]->isEquipped());
			}
		}
	}
	fprintf(outfile, "#"); //Required for the load screen...I know its annoying. I'm sorry.
	
	if(loadedPlayer) //Only if there is even a SEMBLANCE of a chance that there's something afterwards.
	{				//If it's a newGame, it doesn't matter.
		for(int i = 0; i < 10000; i++)
		{
			if(newArray[i] != 10 && newArray[i] != EOF) //Heh heh...oops? I was an idiot again.
			{
				fprintf(outfile, "%c", newArray[i]);
			}
		}
		//fprintf(outfile, " %s", newArray);
	}
	fclose(outfile);
	//fclose(outfile);
	//if(loadedPlayer) //Checking to see if the player is loaded, thus there might be something in the array.
	//{
	//	if(newArray[0] != 10) //Checking to see if there's something in the array. IF THERE IS, THEN DO THIS.
	//	{
	//		outfile = fopen("playerSave.txt", "a+"); //Append the 
	//		for(int i = 0; i < 10000; i++)
	//		{
	//			if(newArray[i] != 10)
	//				fprintf(outfile, "%c", newArray[i]);
	//		}
	//		fclose(outfile);
	//	}
	//}
}
bool Player::loadPlayer(int saveToLoad)
{
	//File will go like this: P#(HP)#(Energy)#(STR)#(Int)#(currentExp)#(ExpRequired)#(StatPoints)#/
	//							A#(SubType)#(SubSubType)#(Level)#(Def)#(ResistFire)#(ResistIce)#(ResistLigtning)#(isEquipped)#/
	//							C#(Type)#(SubType)#(SubSubType)#(Level)#(Cost)#(isEquipped)#/
	
	FILE * infile;
	infile = fopen("playerSave.txt", "r");
	//check to see if playerSave.txt exists, if it doesn't, break out.
	if(infile == NULL)
		return false;

	this->saveFile = saveToLoad;
	int hpenstrintexpsta; //The various Player stats.
	int chipAndArmorHelper;
	float exp;
	static int gauntletSlotSetter = SLOT_ATK1;
	char charget;
	char name[21];
	while(saveToLoad != 0) //Why can't save be this simple?
	{
		charget = fgetc(infile);
		if(charget == '#')
			saveToLoad--;
	}
	charget = fgetc(infile);
	charget = fgetc(infile);
	while(charget != '#')
	{
		//if it's reading the Player...
		if(charget == 'P')
		{
			//Level (IGNORE)
			charget = fgetc(infile); //Gets rid of the space.
			for(int i = 0; i < 20; i++)
				name[i] = fgetc(infile);
			name[20] = 0; //NULL FUCKING TERMINATOR.
			fscanf_s(infile, "%i", &hpenstrintexpsta);

			//HP (IGNORE)
			fscanf_s(infile, "%i", &hpenstrintexpsta);

			//Energy (IGNORE)
			fscanf_s(infile, "%i", &hpenstrintexpsta);

			//Strength
				//make sure stored value is valid
			fscanf_s(infile, "%i", &hpenstrintexpsta);
			this->setName(name);
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
			//sub-type
			fscanf_s(infile, "%i", &hpenstrintexpsta);
			//sub-sub-type
			fscanf_s(infile, "%i", &chipAndArmorHelper);
			//Set the new armor.
			Armor * gear = new Armor((e_chipSubType)hpenstrintexpsta, (e_chipSubSubType)chipAndArmorHelper);
			gear->setNewed(true);
			//level
			fscanf_s(infile, "%i", &chipAndArmorHelper);
			for(int k = 0; k < chipAndArmorHelper; k++)
				gear->levelUp();
			this->addToArmorInventory(gear);
			//Defense.
			fscanf_s(infile, "%i", &hpenstrintexpsta);
			gear->setDefense(hpenstrintexpsta);
			//Resistences
			for(int i = 0; i < 3; i++)
			{
				fscanf_s(infile, "%i", &hpenstrintexpsta);
				gear->setResist(i, hpenstrintexpsta);
			}
			//equip
			fscanf_s(infile, "%i", &hpenstrintexpsta);
			if(hpenstrintexpsta)
			{
				switch(gear->getSubType())
				{
				case HEAD:			setGauntletSlot(SLOT_ARMOR_HEAD, gear);			break;
				case TRUNK:			setGauntletSlot(SLOT_ARMOR_TRUNK, gear);		break;
				case LIMB_UPPER:	setGauntletSlot(SLOT_ARMOR_LIMB_UPPER, gear);	break;
				case LIMB_LOWER:	setGauntletSlot(SLOT_ARMOR_LIMB_LOWER, gear);	break;
				}
			}
		}
		//If it's reading the Chips...
		else if(charget == 'C' || charget == 'S')
		{
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
			//level
			fscanf_s(infile, "%i", &hpenstrintexpsta);
			for(int i = 0; i < hpenstrintexpsta; i++)
				iHopeThisWorks->levelUp();
			this->addToAttackInventory(iHopeThisWorks);
			thisWorld->add(iHopeThisWorks);
			//cost (IGNORE)
			fscanf_s(infile, "%i", &chipAndArmorHelper);
			//equip
			fscanf_s(infile, "%i", &hpenstrintexpsta);
			if(hpenstrintexpsta)
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
	this->loadedPlayer = true;
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
	Lightning * l1 = new Lightning(BASIC);
	Lightning * l2 = new Lightning(ADVANCED);
	Lightning * l3 = new Lightning(EXPERT);
	Fire * f1 = new Fire(BASIC);
	Fire * f2 = new Fire(ADVANCED);
	Fire * f3 = new Fire(EXPERT);
	Ice * i1 = new Ice(BASIC);
	Ice * i2 = new Ice(ADVANCED);
	Ice * i3 = new Ice(EXPERT);
	Armor * gear = new Armor(HEAD, BASIC);
	b1->setNewed(true);
	b2->setNewed(true);
	b3->setNewed(true);
	s1->setNewed(true);
	s2->setNewed(true);
	s3->setNewed(true);
	d1->setNewed(true);
	d2->setNewed(true);
	d3->setNewed(true);
	l1->setNewed(true);
	l2->setNewed(true);
	l3->setNewed(true);
	f1->setNewed(true);
	f2->setNewed(true);
	f3->setNewed(true);
	i1->setNewed(true);
	i2->setNewed(true);
	i3->setNewed(true);
	gear->setNewed(true);
	this->addToAttackInventory(b1);
	this->addToAttackInventory(b2);
	this->addToAttackInventory(b3);
	this->addToAttackInventory(s1);
	this->addToAttackInventory(s2);
	this->addToAttackInventory(s3);
	this->addToAttackInventory(d1);
	this->addToAttackInventory(d2);
	this->addToAttackInventory(d3);
	this->addToAttackInventory(l1);
	this->addToAttackInventory(l2);
	this->addToAttackInventory(l3);
	this->addToAttackInventory(f1);
	this->addToAttackInventory(f2);
	this->addToAttackInventory(f3);
	this->addToAttackInventory(i1);
	this->addToAttackInventory(i2);
	this->addToAttackInventory(i3);
	this->addToArmorInventory(gear);
	thisWorld->add(b1);
	thisWorld->add(b2);
	thisWorld->add(b3);
	thisWorld->add(s1);
	thisWorld->add(s2);
	thisWorld->add(s3);
	thisWorld->add(d1);
	thisWorld->add(d2);
	thisWorld->add(d3);
	thisWorld->add(l1);
	thisWorld->add(l2);
	thisWorld->add(l3);
	thisWorld->add(f1);
	thisWorld->add(f2);
	thisWorld->add(f3);
	thisWorld->add(i1);
	thisWorld->add(i2);
	thisWorld->add(i3);
	b1->levelUp();
	b2->levelUp();
	b3->levelUp();
	s1->levelUp();
	s2->levelUp();
	s3->levelUp();
	d1->levelUp();
	d2->levelUp();
	d3->levelUp();
	l1->levelUp();
	l2->levelUp();
	l3->levelUp();
	f1->levelUp();
	f2->levelUp();
	f3->levelUp();
	i1->levelUp();
	i2->levelUp();
	i3->levelUp();
	this->setGauntletSlot(SLOT_ATK1, s1);
	this->setGauntletSlot(SLOT_ATK2, f1);
	switch(gear->getSubType())
	{
	case HEAD:			setGauntletSlot(SLOT_ARMOR_HEAD, gear);			break;
	case TRUNK:			setGauntletSlot(SLOT_ARMOR_TRUNK, gear);		break;
	case LIMB_UPPER:	setGauntletSlot(SLOT_ARMOR_LIMB_UPPER, gear);	break;
	case LIMB_LOWER:	setGauntletSlot(SLOT_ARMOR_LIMB_LOWER, gear);	break;
	}
	loadedPlayer = false;
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
	static bool validClick;
	static char lastKey = KEY_DOWN;
	static SPoint mouse;
	static SRect hud(HUD_X, HUD_Y, HUD_WIDTH, HUD_HEIGHT);
	static SRect window(WINDOWXY.x, WINDOWXY.y, WINDOWWIDTH, WINDOWHEIGHT);
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
	case KEY_HOT_ATK1_BAS:	setGauntletSlot(SLOT_ATK1, BASIC);		break;
	case KEY_HOT_ATK1_ADV:	setGauntletSlot(SLOT_ATK1, ADVANCED);	break;
	case KEY_HOT_ATK1_EXP:	setGauntletSlot(SLOT_ATK1, EXPERT);		break;
	case KEY_HOT_ATK1_LEG:	setGauntletSlot(SLOT_ATK1);				break;
	}
	switch(ui->getHKeyR())
	{
	case KEY_HOT_ATK2_BAS:	setGauntletSlot(SLOT_ATK2, BASIC);		break;
	case KEY_HOT_ATK2_ADV:	setGauntletSlot(SLOT_ATK2, ADVANCED);	break;
	case KEY_HOT_ATK2_EXP:	setGauntletSlot(SLOT_ATK2, EXPERT);		break;
	case KEY_HOT_ATK2_LEG:	setGauntletSlot(SLOT_ATK2);				break;
	}
	mouse.set(ui->getMouseX(),  ui->getMouseY());
	validClick = !hud.contains(mouse) && (!m_isStatWindowActive || !window.contains(mouse));
	if(ui->getClick() == CLICK_LEFT && validClick)
			activateGauntletAttack(SLOT_ATK1, m_cameraP.x + ui->getMouseX(), m_cameraP.y + ui->getMouseY(), lastKey, ah);
	if(ui->getClick() == CLICK_RIGHT && validClick)
			activateGauntletAttack(SLOT_ATK2, m_cameraP.x + ui->getMouseX(), m_cameraP.y + ui->getMouseY(), lastKey, ah);
	if(ui->getSpace())
		m_flags[FLAG_ACTIVE] = true;
}