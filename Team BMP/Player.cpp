//Basic Player Class Extensions.
#include "Player.h"
#include "UserInput.h"

void Player::initPlayer()
{
	m_eType = PLAYER;
	for(int i = 0; i < NUM_SLOTS; ++i)
		m_gauntlet[i] = NULL;
	for(int j = 0; j < WEAPON*NUM_CHIP_SUBS_PER_TYPE; ++j)
	{
		for(int k = 0; k < NUM_CHIP_LEVELS; ++k)
			m_attackInventory[j][k] = NULL;
	}
	m_lvl = 1;
	m_statPoints = 0;
	setVelocity(0,0);
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
	/*for(int i = 0; i < WEAPON; ++i)
	{
		for(int j = 0; j < NUM_CHIP_SUBS_PER_TYPE; ++j)
		{
			for(int k = 0; k < NUM_CHIP_LEVELS; ++k)
				printf("%d, ", m_attackInventory[i][j][k]);
		}
		printf("\n");
	}
	printf("\n\n");*/
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