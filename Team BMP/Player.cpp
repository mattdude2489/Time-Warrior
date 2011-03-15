//Basic Player Class Extensions.
#include "Player.h"
#include "UserInput.h"

Player::Player():Entity()
{
	m_eType = PLAYER; //Tells the Entity that it's a PLAYER. YEAH. REMEMBER THEM? THE PLAYERS. YEAH. THEY EXIST TOO.
	//Otherwise, let the usual Entity constructor occur.
}

void Player::handleInput(UserInput ui)
{
	//This is where the UI goes to get handled by the Player class. Well...it would've been world class, but we dun have one of them yet.
	if(ui.getKeyUD() == 'w')
	{
		m_sprite->setHIndex(0, 3); //BOO MAGIC NUMBERS.
		move(LOC_SCREEN, 0, -5);
	}
	
	if(ui.getKeyLR() == 'd')
	{
		m_sprite->setHIndex(1,3);
		move(LOC_SCREEN, 5, 0);
	}
	
	if(ui.getKeyUD() == 's')
	{
		m_sprite->setHIndex(2,3);
		move(LOC_SCREEN, 0, 5);
	}
	
	if(ui.getKeyLR() == 'a')
	{
		m_sprite->setHIndex(3,3);
		move(LOC_SCREEN, -5, 0);
	}
	if(ui.getClick() == CLICK_LEFT)
	{
		m_gauntlet[SLOT_ATK1]->setTarget(ui.getMouseX(), ui.getMouseY());
		m_gauntlet[SLOT_ATK1]->activate();
	}
	if(ui.getClick() == CLICK_RIGHT)
	{
		m_gauntlet[SLOT_ATK2]->setTarget(ui.getMouseX(), ui.getMouseY());
		m_gauntlet[SLOT_ATK2]->activate();
	}
}