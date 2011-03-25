//Basic player class, which extends Entity.h
//Authored by Matt Morrill, created on 3/9/11
//Edited Last by Matt Morrill, created on 3/10/11
#pragma once
#include "Entity.h"
#include "UserInput.h"
#include "Armor.h"
#include "Magic.h"
#include "Weapon.h"
#include "World.h"

enum e_gauntletSlots {SLOT_ATK1, SLOT_ATK2, SLOT_ARMOR_HEAD, SLOT_ARMOR_TRUNK, SLOT_ARMOR_LIMB_UPPER, SLOT_ARMOR_LIMB_LOWER, NUM_SLOTS};
enum e_hud {HUD_WIDTH = SCREEN_WIDTH, HUD_HEIGHT = SPRITE_SIZE, HUD_X = 0, HUD_Y = SCREEN_HEIGHT-HUD_HEIGHT};

class Player : public Entity
{
private:
	Chip * m_gauntlet[NUM_SLOTS];
	Chip * m_attackInventory[WEAPON][NUM_CHIP_SUBS_PER_TYPE][NUM_CHIP_LEVELS];
	SPoint m_cameraP;
public:
	Player();
	void activateGauntletAttack(e_gauntletSlots a_slot, int a_targetX, int a_targetY, char a_direction);
	SPoint * getCamera(){return &m_cameraP;}
	void moveUnique(int a_deltaX, int a_deltaY)
	{
		m_cameraP.x += a_deltaX;
		m_cameraP.y += a_deltaY;
	}
	void handleInput(UserInput * ui, World * a_world);
	void setLocationUnique(int a_x, int a_y)
	{
		m_cameraP.x = a_x - SCREEN_CENTER_X;
		m_cameraP.x = a_y - SCREEN_CENTER_Y;
	}
	void setGauntletSlot(e_gauntletSlots a_slot, Chip * a_chip)
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
	void setGauntletSlot(e_gauntletSlots a_slot, e_chipSubSubType a_level);
	void addToAttackInventory(Chip * a_chip)
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
	void drawSlot(e_gauntletSlots a_slot, SDL_Surface * a_screen, int a_x, int a_y){m_gauntlet[a_slot]->drawHUD(a_screen, a_x, a_y);}
	//This is Alex's code...copypastaed. Because it works. Quite well in fact.
	Player(int a_def, int a_int, int a_str, int a_health, int a_energy, int a_fRes, int a_iRes, int a_lRes, SDL_Sprite* a_sprite)
		:Entity(a_def, a_int, a_str, a_health, a_energy, a_fRes, a_iRes, a_lRes, a_sprite)
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
};