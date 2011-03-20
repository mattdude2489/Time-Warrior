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
		if(m_stats.m_stats[ENERGY_CURRENT] >= m_gauntlet[a_slot]->getCost())
		{
			useEnergy(m_gauntlet[a_slot]->getCost());
			m_gauntlet[a_slot]->setTarget(a_targetX, a_targetY);
			m_gauntlet[a_slot]->setDirection(a_direction);
			m_gauntlet[a_slot]->activate();
		}
	}
}
void Player::handleInput(UserInput * ui, World * a_world)
{
	static char lastKey = 'n'; 
	//This is where the UI goes to get handled by the Player class. Well...it would've been world class, but we dun have one of them yet.
	if(ui->getKeyUD() == 'w')
	{
		//m_sprite->setHIndex(0, 3); //BOO MAGIC NUMBERS.
		m_sprite->setRIndex(0);
		move(LOC_SCREEN, 0, -5);
		lastKey = 'w';
	}
	
	if(ui->getKeyLR() == 'd')
	{
		//m_sprite->setHIndex(1,3);
		m_sprite->setRIndex(1);
		move(LOC_SCREEN, 5, 0);
		lastKey = 'd';
	}
	
	if(ui->getKeyUD() == 's')
	{
		//m_sprite->setHIndex(2,3);
		m_sprite->setRIndex(2);
		move(LOC_SCREEN, 0, 5);
		lastKey = 's';
	}
	
	if(ui->getKeyLR() == 'a')
	{
		//m_sprite->setHIndex(3,3);
		m_sprite->setRIndex(3);
		move(LOC_SCREEN, -5, 0);
		lastKey = 'a';
	}
	if(ui->getClick() == CLICK_LEFT && ui->getMouseY() < HUD_Y)
		activateGauntletAttack(SLOT_ATK1, ui->getMouseX(), ui->getMouseY(), lastKey);
	if(ui->getClick() == CLICK_RIGHT && ui->getMouseY() < HUD_Y)
		activateGauntletAttack(SLOT_ATK2, ui->getMouseX(), ui->getMouseY(), lastKey);
}