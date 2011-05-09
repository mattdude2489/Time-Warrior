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
enum e_hud {HUD_WIDTH = SCREEN_WIDTH, HUD_HEIGHT = FRAME_SIZE, HUD_X = 0, HUD_Y = SCREEN_HEIGHT-HUD_HEIGHT};
enum e_exp {POINTS_GIVEN = 1};

class Player : public Entity
{
private:
	Chip *m_gauntlet[NUM_SLOTS], *m_attackInventory[WEAPON*NUM_CHIP_SUBS_PER_TYPE][NUM_CHIP_LEVELS], *m_armorInventory[WEAPON*NUM_CHIP_SUBS_PER_TYPE];
	SPoint m_cameraP;
	int m_statPoints, m_expLvReq;
	double m_experience;
	//If it's loaded from a file, then the Armor and all the chips are set by using New. Thus...get VID OF THEM.
	bool loadedPlayer;
	World * thisWorld;
public:
	void initPlayer(World * newWorld);
	Player():Entity(){initPlayer(NULL);}//OMG DON'T USE THIS DON'T USE THIS OMGWTF DON'T USE THIS
	Player(int a_health, int a_energy, int a_str, int a_int, int a_def, int a_fRes, int a_iRes, int a_lRes, SDL_Sprite * a_sprite, World * newWorld)
		:Entity(a_health, a_energy, a_str, a_int, a_def, a_fRes, a_iRes, a_lRes, a_sprite){initPlayer(newWorld);}
	SPoint * getCamera(){return &m_cameraP;}
	~Player();
	void setLocationUnique(int a_x, int a_y){m_cameraP.set(a_x - SCREEN_CENTER_X, a_y - SCREEN_CENTER_Y);}
	void moveUnique(int a_deltaX, int a_deltaY){m_cameraP.x += a_deltaX;m_cameraP.y += a_deltaY;}
	void drawSlot(e_gauntletSlots a_slot, SDL_Surface * a_screen, int a_x, int a_y){m_gauntlet[a_slot]->drawHUD(a_screen, a_x, a_y);}
	void addToAttackInventory(Chip * a_chip);
	void addToArmorInventory(Chip * a_chip);
	void save();
	void setWorld(World * currWorld) {thisWorld = currWorld;}
	bool loadPlayer();
	void newGame();
	void setGauntletSlot(e_gauntletSlots a_slot);
	void setGauntletSlot(e_gauntletSlots a_slot, Chip * a_chip);
	void setGauntletSlot(e_gauntletSlots a_slot, e_chipSubSubType a_level);
	void activateGauntletAttack(e_gauntletSlots a_slot, int a_targetX, int a_targetY, char a_direction);
	void handleInput(UserInput * ui, World * a_world);
	void setVelocity(double newVelX, double newVelY){m_vel.x = newVelX; m_vel.y = newVelY;}
	void movePlayer(int a_timePassed)
	{
		if(!(m_vel.x == 0 && m_vel.y == 0))
			move((int)(a_timePassed*m_vel.x), (int)(a_timePassed*m_vel.y));
	}
	void handleServerInfo(char * a_in)
	{
		switch(a_in[0])
		{
		case KEY_LEFT:
			setVelocity(-SPEED_PLAYER, m_vel.y);
			break;
		case KEY_RIGHT:
			setVelocity(SPEED_PLAYER, m_vel.y);
			break;
		}
		switch(a_in[1])
		{
		case KEY_UP:
			setVelocity(m_vel.x, -SPEED_PLAYER);
			break;
		case KEY_DOWN:
			setVelocity(m_vel.x, SPEED_PLAYER);
			break;
		}
	}
	void levelUp()
	{
		m_stats[LEVEL]++;
		m_expLvReq += m_stats[LEVEL];
		m_statPoints += POINTS_GIVEN;
		m_stats[HEALTH_CURRENT] = m_stats[HEALTH_MAX];
		m_stats[ENERGY_CURRENT] = m_stats[ENERGY_MAX];
	}
	void gainExperience(double a_amount)
	{
		m_experience += a_amount;
		while(m_experience >= m_expLvReq)
		{
			m_experience -= m_expLvReq;
			levelUp();
		}
	}
	int getExpReq(){return m_expLvReq;}
	double getCurExp(){return m_experience;}
	int getPoints(){return m_statPoints;}
	void incStr()
	{
		m_stats[HEALTH_MAX]++;
		m_stats[HEALTH_CURRENT] = m_stats[HEALTH_MAX];
		m_stats[STRENGTH]++;
		m_stats[DEFENSE]++;
		m_statPoints--;
	}
	void incInt()
	{
		m_stats[ENERGY_MAX]++;
		m_stats[ENERGY_CURRENT] = m_stats[ENERGY_MAX];
		m_stats[ENERGY_REGEN] = (int)(m_stats[ENERGY_MAX] * .05);
		m_stats[INTELLECT]++;
		for(int i = RESISTANCE_FIRE; i < RESISTANCE_FIRE+3; ++i)
			m_stats[i]++;
		m_statPoints--;
	}
};