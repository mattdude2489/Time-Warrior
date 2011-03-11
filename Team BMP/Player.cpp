//Basic Player Class Extensions.
#include "Player.h"
#include "UserInput.h"

Player::Player()
{
	m_myType = player; //Tells the Entity that it's a PLAYER. YEAH. REMEMBER THEM? THE PLAYERS. YEAH. THEY EXIST TOO.
	//Otherwise, let the usual Entity constructor occur.
}

void Player::handleInput(UserInput ui)
{
	//This is where the UI goes to get handled by the Player class. Well...it would've been world class, but we dun have one of them yet.
	if(ui.getKeyUD() == 'w')
	{
		m_mySprite->setHIndex(0, 3); //BOO MAGIC NUMBERS.
		m_locations[0].y -= 5;
	}
	
	if(ui.getKeyLR() == 'd')
	{
		m_mySprite->setHIndex(1,3);
		m_locations[0].x += 5;
	}
	
	if(ui.getKeyUD() == 's')
	{
		m_mySprite->setHIndex(2,3);
		m_locations[0].y += 5;
	}
	
	if(ui.getKeyLR() == 'a')
	{
		m_mySprite->setHIndex(3,3);
		m_locations[0].x -= 5;
	}
}