//Basic Player Class Extensions.
#include "Player.h"
#include "UserInput.h"

void Player::init()
{
	m_eType = PLAYER;
	for(int i = 0; i < NUM_SLOTS; ++i)
		m_gauntlet[i] = NULL;
	for(int i = 0; i < WEAPON; ++i)
	{
		for(int j = 0; j < NUM_CHIP_SUBS_PER_TYPE; ++j)
		{
			for(int k = 0; k < NUM_CHIP_LEVELS; ++k)
				m_attackInventory[i][j][k] = NULL;
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
		m_attackInventory[a_chip->getType()-1][a_chip->getSubType()][a_chip->getSubSubType()] = a_chip;
		break;
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
		m_gauntlet[a_slot] = a_chip;
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
	static char lastKey = KEY_NONE; 
	//This is where the UI goes to get handled by the Player class. Well...it would've been world class, but we dun have one of them yet.
	if(ui->getKeyUD() == KEY_UP)
	{
		//m_sprite->setHIndex(0, 3); //BOO MAGIC NUMBERS.
		m_sprite->setRIndex(0);
		move(0, -1*SPEED_PLAYER);
		lastKey = KEY_UP;
	}
	
	if(ui->getKeyLR() == KEY_RIGHT)
	{
		//m_sprite->setHIndex(1,3);
		m_sprite->setRIndex(1);
		move(SPEED_PLAYER, 0);
		lastKey = KEY_RIGHT;
	}
	
	if(ui->getKeyUD() == KEY_DOWN)
	{
		//m_sprite->setHIndex(2,3);
		m_sprite->setRIndex(2);
		move(0, SPEED_PLAYER);
		lastKey = KEY_DOWN;
	}
	
	if(ui->getKeyLR() == KEY_LEFT)
	{
		//m_sprite->setHIndex(3,3);
		m_sprite->setRIndex(3);
		move(-1*SPEED_PLAYER, 0);
		lastKey = KEY_LEFT;
	}
	switch(ui->getHKeyL())
	{
	case KEY_HOT_ATK2_BAS:
		setGauntletSlot(SLOT_ATK1, BASIC);
		break;
	case KEY_HOT_ATK2_ADV:
		setGauntletSlot(SLOT_ATK1, ADVANCED);
		break;
	case KEY_HOT_ATK2_EXP:
		setGauntletSlot(SLOT_ATK1, EXPERT);
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
	}
	if(ui->getClick() == CLICK_LEFT && ui->getMouseY() < HUD_Y)
		activateGauntletAttack(SLOT_ATK1, m_cameraP.x + ui->getMouseX(), m_cameraP.y + ui->getMouseY(), lastKey);
	if(ui->getClick() == CLICK_RIGHT && ui->getMouseY() < HUD_Y)
		activateGauntletAttack(SLOT_ATK2, m_cameraP.x + ui->getMouseX(), m_cameraP.y + ui->getMouseY(), lastKey);
}