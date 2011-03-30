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
	Chip *m_gauntlet[NUM_SLOTS], *m_attackInventory[WEAPON][NUM_CHIP_SUBS_PER_TYPE][NUM_CHIP_LEVELS];
	SPoint m_cameraP;
public:
	void init();
	Player():Entity(){init();}
	Player(int a_def, int a_int, int a_str, int a_health, int a_energy, int a_fRes, int a_iRes, int a_lRes, SDL_Sprite* a_sprite)
		:Entity(a_def, a_int, a_str, a_health, a_energy, a_fRes, a_iRes, a_lRes, a_sprite){init();}
	SPoint * getCamera(){return &m_cameraP;}
	void setLocationUnique(int a_x, int a_y){m_cameraP.set(a_x - SCREEN_CENTER_X, a_y - SCREEN_CENTER_Y);}
	void moveUnique(int a_deltaX, int a_deltaY){m_cameraP.x += a_deltaX;m_cameraP.y += a_deltaY;}
	void drawSlot(e_gauntletSlots a_slot, SDL_Surface * a_screen, int a_x, int a_y){m_gauntlet[a_slot]->drawHUD(a_screen, a_x, a_y);}
	void addToAttackInventory(Chip * a_chip);
	void setGauntletSlot(e_gauntletSlots a_slot, Chip * a_chip);
	void setGauntletSlot(e_gauntletSlots a_slot, e_chipSubSubType a_level){setGauntletSlot(a_slot, m_attackInventory[m_gauntlet[a_slot]->getType()-1][m_gauntlet[a_slot]->getSubType()][a_level]);}
	void activateGauntletAttack(e_gauntletSlots a_slot, int a_targetX, int a_targetY, char a_direction);
	void handleInput(UserInput * ui, World * a_world);
};