//Basic Player Class Extensions.
#include "Player.h"
#include "UserInput.h"

Player::Player():Entity()
{
	m_eType = PLAYER; //Tells the Entity that it's a PLAYER. YEAH. REMEMBER THEM? THE PLAYERS. YEAH. THEY EXIST TOO.
	//Otherwise, let the usual Entity constructor occur.
	for(int i = 0; i < NUM_SLOTS; ++i)
		m_gauntlet[i] = NULL;
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
void Player::setGauntletSlot(e_gauntletSlots a_slot, e_chipSubSubType a_level){setGauntletSlot(a_slot, m_attackInventory[m_gauntlet[a_slot]->getType()-1][m_gauntlet[a_slot]->getSubType()][a_level]);}
void Player::handleInput(UserInput * ui, World * a_world)
{
	static char lastKey = KEY_NONE; 
	//This is where the UI goes to get handled by the Player class. Well...it would've been world class, but we dun have one of them yet.
	if(ui->getKeyUD() == KEY_UP)
	{
		//m_sprite->setHIndex(0, 3); //BOO MAGIC NUMBERS.
		m_sprite->setRIndex(0);
		move(0, -5);
		lastKey = KEY_UP;
	}
	
	if(ui->getKeyLR() == KEY_RIGHT)
	{
		//m_sprite->setHIndex(1,3);
		m_sprite->setRIndex(1);
		move(5, 0);
		lastKey = KEY_RIGHT;
	}
	
	if(ui->getKeyUD() == KEY_DOWN)
	{
		//m_sprite->setHIndex(2,3);
		m_sprite->setRIndex(2);
		move(0, 5);
		lastKey = KEY_DOWN;
	}
	
	if(ui->getKeyLR() == KEY_LEFT)
	{
		//m_sprite->setHIndex(3,3);
		m_sprite->setRIndex(3);
		move(-5, 0);
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
		activateGauntletAttack(SLOT_ATK1, ui->getMouseX(), ui->getMouseY(), lastKey);
	if(ui->getClick() == CLICK_RIGHT && ui->getMouseY() < HUD_Y)
		activateGauntletAttack(SLOT_ATK2, ui->getMouseX(), ui->getMouseY(), lastKey);
}