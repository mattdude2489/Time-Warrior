//Basic player class, which extends Entity.h
//Authored by Matt Morrill, created on 3/9/11
//Edited Last by Matt Morrill, created on 3/9/11
#pragma once
#include "Entity.h"
#include "UserInput.h"


class Player : public Entity
{
	//What goes here?
private:
	//Nada...for now.
public:
	Player();
	void handleInput(UserInput ui);
	//This is Alex's code...copypastaed. Because it works. Quite well in fact.
	Player(int a_def, int a_int, int a_str, int a_health, int a_energy, int a_fRes, int a_iRes, int a_lRes, Sprite * a_sprite)
	{
		m_myType = player;
		m_stats.defence = a_def;
		m_stats.intellect = a_int;
		m_stats.strength = a_str;
		for(int i = 0; i < CURRENT2MAX; i++)
		{
			m_stats.energy[i] = a_energy;
			m_stats.health[i] = a_health;
		}
		m_stats.resistances[FIRE] = a_fRes;
		m_stats.resistances[ICE] = a_iRes;
		m_stats.resistances[LIGHTNING] = a_lRes;
		m_mySprite = a_sprite;
		m_locations[SCREENLOC].x = CENTERSCREENX;
		m_locations[SCREENLOC].y = CENTERSCREENY;
	}
};