//Basic player class, which extends Entity.h
//Authored by Matt Morrill, created on 3/9/11
//Edited Last by Matt Morrill, created on 3/10/11
#pragma once
#include "Entity.h"
#include "UserInput.h"
#include "Armor.h"
#include "Magic.h"
#include "Weapon.h"

enum e_gauntletSlots {SLOT_ATK1, SLOT_ATK2, SLOT_GEAR_HEAD, SLOT_GEAR_TRUNK, SLOT_GEAR_LIMB_UPPER, SLOT_GEAR_LIMB_LOWER, NUM_SLOTS};

class Player : public Entity
{
	//What goes here?
private:
	//Nada...for now.
	Chip * m_gauntlet[NUM_SLOTS];
public:
	Player();
	void handleInput(UserInput ui);
	//This is Alex's code...copypastaed. Because it works. Quite well in fact.
	Player(int a_def, int a_int, int a_str, int a_health, int a_energy, int a_fRes, int a_iRes, int a_lRes, Sprite * a_sprite)
		:Entity(a_def, a_int, a_str, a_health, a_energy, a_fRes, a_iRes, a_lRes, a_sprite)
	{
		m_eType = PLAYER;
	}
};