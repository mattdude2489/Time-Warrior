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

enum e_inventory		{INVENTORY_ATTACK, INVENTORY_ARMOR, INVENTORY_GAUNTLET, NUM_INVENTORY};
enum e_gauntletSlots	{SLOT_ATK1, SLOT_ATK2, SLOT_ARMOR_HEAD, SLOT_ARMOR_TRUNK, SLOT_ARMOR_LIMB_UPPER, SLOT_ARMOR_LIMB_LOWER, NUM_SLOTS};
enum e_hud				{HUD_WIDTH = SCREEN_WIDTH, HUD_HEIGHT = FRAME_SIZE, HUD_X = 0, HUD_Y = SCREEN_HEIGHT-HUD_HEIGHT};
enum e_req				{REQ_PLYR_ACCESS_ADV = 2, REQ_PLYR_ACCESS_EXP = REQ_PLYR_ACCESS_ADV+1, REQ_CHIP_BAS_TO_ADV = 10, REQ_CHIP_ADV_TO_EXP = REQ_CHIP_BAS_TO_ADV, REQ_CHIP_BAS_TO_EXP = REQ_CHIP_BAS_TO_ADV+REQ_CHIP_ADV_TO_EXP};

class Player : public Entity
{
private:
	Chip *m_gauntlet[NUM_SLOTS], *m_attackInventory[WEAPON*NUM_CHIP_SUBS_PER_TYPE][NUM_CHIP_LEVELS], *m_armorInventory[WEAPON*NUM_CHIP_SUBS_PER_TYPE];
	SPoint m_cameraP;
	int m_expLvReq, saveFile, barrierKey;//barrier key will be a number just to make life easy
	double m_experience;
	//If it's loaded from a file, then the Armor and all the chips are set by using New. Thus...get VID OF THEM.
	bool loadedPlayer, m_isStatWindowActive, gamePlayed, deleted;
	bool * eventsSeen;
	World * thisWorld;
	SDL_Sprite * m_blankInventory;
	char playerName[21];

public:
	Player():Entity(){initPlayer(NULL);}//OMG DON'T USE THIS DON'T USE THIS OMGWTF DON'T USE THIS
	Player(int a_health, int a_energy, int a_str, int a_int, int a_def, int a_fRes, int a_iRes, int a_lRes, SDL_Sprite * a_sprite, World * newWorld)
		:Entity(a_health, a_energy, a_str, a_int, a_def, a_fRes, a_iRes, a_lRes, a_sprite){initPlayer(newWorld);}
	~Player();
	bool loadPlayer(int saveToLoad);
	bool getGamePlayed() {return gamePlayed;}
	bool getDeleted() {return deleted;}
	char * getName() {return playerName;}
	int getExpReq(){return m_expLvReq;}
	int getKeyLevel(){return barrierKey;}
	int drawInventory(SDL_Surface * a_screen, int a_x, int a_y, e_inventory a_type, int a_maxColumns, int a_maxNum, int a_startIndex, bool a_isMiniUI);
	double getCurExp(){return m_experience;}
	SPoint * getCamera(){return &m_cameraP;}
	Chip * getHUDClickedChip(SPoint a_click, int a_x, int a_y, e_inventory a_type, int a_maxColumns, int a_maxNum, int a_startIndex);
	void initPlayer(World * newWorld);
	void setWindowStatus(bool a_isActive){m_isStatWindowActive = a_isActive;}
	void setLocationUnique(int a_x, int a_y){m_cameraP.set(a_x - SCREEN_CENTER_X, a_y - SCREEN_CENTER_Y);}
	void moveUnique(int a_deltaX, int a_deltaY){m_cameraP.x += a_deltaX;m_cameraP.y += a_deltaY;}
	void addToAttackInventory(Chip * a_chip);
	bool addToArmorInventory(Chip * a_chip);
	void save(int saveToSave);
	void setWorld(World * currWorld) {thisWorld = currWorld;}
	//Set the gamePlayed variable, which tracks if the game has been played.
	void setGamePlayed(bool game) {gamePlayed = game;}
	void newGame();
	bool getLoaded() { return loadedPlayer; }
	void setName(char newName[21]) {for(int i = 0; i < 21; i++) {playerName[i] = newName[i];}}
	void setGauntletArmor(Chip * a_chip)
	{
		switch(a_chip->getSubType())
		{
		case HEAD:			setGauntletSlot(SLOT_ARMOR_HEAD, a_chip);		break;
		case TRUNK:			setGauntletSlot(SLOT_ARMOR_TRUNK, a_chip);		break;
		case LIMB_UPPER:	setGauntletSlot(SLOT_ARMOR_LIMB_UPPER, a_chip);	break;
		case LIMB_LOWER:	setGauntletSlot(SLOT_ARMOR_LIMB_LOWER, a_chip);	break;
		}
	}
	void removeGauntletArmor(e_chipSubType a_type)
	{
		e_gauntletSlots a_slot = SLOT_ARMOR_HEAD;
		switch(a_type)
		{
		case HEAD:
		case TRUNK:
		case LIMB_UPPER:
		case LIMB_LOWER:
			switch(a_type)
			{
			case HEAD:			a_slot = SLOT_ARMOR_HEAD;		break;
			case TRUNK:			a_slot = SLOT_ARMOR_TRUNK;		break;
			case LIMB_UPPER:	a_slot = SLOT_ARMOR_LIMB_UPPER;	break;
			case LIMB_LOWER:	a_slot = SLOT_ARMOR_LIMB_LOWER;	break;
			}
			if(m_gauntlet[a_slot])
			{
				m_gauntlet[a_slot]->unequip();
				m_gauntlet[a_slot] = NULL;
			}
		}
	}
	void setGauntletSlot(e_gauntletSlots a_slot, Chip * a_chip);
	void setGauntletSlot(e_gauntletSlots a_slot, e_chipSubSubType a_level);
	void activateGauntletAttack(e_gauntletSlots a_slot, int a_targetX, int a_targetY, char a_direction, AudioHandler * ah);
	void handleInput(UserInput * ui, World * a_world, AudioHandler *ah);
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
		case KEY_LEFT:	setVelocity(-SPEED_PLAYER, m_vel.y);	break;
		case KEY_RIGHT:	setVelocity(SPEED_PLAYER, m_vel.y);		break;
		}
		switch(a_in[1])
		{
		case KEY_UP:	setVelocity(m_vel.x, -SPEED_PLAYER);	break;
		case KEY_DOWN:	setVelocity(m_vel.x, SPEED_PLAYER);		break;
		}
	}
	void lockModdedAttacks()
	{
		for(int i = 0; i < WEAPON*NUM_CHIP_SUBS_PER_TYPE; ++i)
		{
			if(m_stats[LEVEL] < REQ_PLYR_ACCESS_ADV || m_attackInventory[i][BASIC]->getStatNumber(LEVEL) < REQ_CHIP_BAS_TO_ADV)
				m_attackInventory[i][ADVANCED]->resetAndLock();
			if(m_stats[LEVEL] < REQ_PLYR_ACCESS_EXP || (m_attackInventory[i][BASIC]->getStatNumber(LEVEL) < REQ_CHIP_BAS_TO_EXP && m_attackInventory[i][ADVANCED]->getStatNumber(LEVEL) < REQ_CHIP_ADV_TO_EXP))
				m_attackInventory[i][EXPERT]->resetAndLock();
		}
	}
	void unlockEarnedAttacks()
	{
		for(int i = 0; i < WEAPON*NUM_CHIP_SUBS_PER_TYPE; ++i)
		{
			if(m_stats[LEVEL] >= REQ_PLYR_ACCESS_ADV && m_attackInventory[i][BASIC]->getStatNumber(LEVEL) >= REQ_CHIP_BAS_TO_ADV)
				m_attackInventory[i][ADVANCED]->unlock();
			if(m_stats[LEVEL] >= REQ_PLYR_ACCESS_EXP && (m_attackInventory[i][BASIC]->getStatNumber(LEVEL) >= REQ_CHIP_BAS_TO_EXP || m_attackInventory[i][ADVANCED]->getStatNumber(LEVEL) >= REQ_CHIP_ADV_TO_EXP))
				m_attackInventory[i][EXPERT]->unlock();
		}
	}
	void levelUpUnique(){m_expLvReq += m_stats[LEVEL];}
	void gainExperience(double a_amount)
	{
		m_experience += a_amount;
		while(m_experience >= m_expLvReq)
		{
			m_experience -= m_expLvReq;
			levelUp();
		}
	}
	void destroyPlayer();
	void setKeyLevel(int a_level){barrierKey = a_level;}
	void sellArmor(Chip * a_chip)
	{
		if(a_chip->getType() == ARMOR)
		{
			a_chip->setOwner(this);
			a_chip->sell();
			for(int i = 0; i < WEAPON*NUM_CHIP_SUBS_PER_TYPE; ++i)
			{
				if(m_armorInventory[i] == a_chip)
					m_armorInventory[i] = NULL;
			}
			if(a_chip->getFlag(FLAG_NUDE))
				delete a_chip;
		}
	}
	void pickUpArmor()
	{
		e_chipSubSubType lv;
		if(m_stats[LEVEL] <= 10)//1-10, only get BASIC armor
			lv = BASIC;
		else if(m_stats[LEVEL] <= 20)//11-20, get BASIC or ADVANCED armor
			lv = e_chipSubSubType(rand()%EXPERT);
		else//21+, get BASIC, ADVANCED, or EXPERT armor
			lv = e_chipSubSubType(rand()%NUM_CHIP_LEVELS);
		Armor * gear = new Armor(e_chipSubType(rand()%NUM_CHIP_SUBS_PER_TYPE), lv);
		gear->setNewed(true);
		if(!this->addToArmorInventory(gear))
			sellArmor(gear);
	}
	bool eventSeen(int currentWorld, int currentEvent)
	{
		if (eventsSeen[currentEvent])
			return true;
		else
			return false;
	}
};