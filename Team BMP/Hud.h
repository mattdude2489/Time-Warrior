#pragma once

#include "Player.h"
#include "srect.h"
#include "trueTextPrinter.h"
#include "ButtonAndWIndow.h"

#define ONE_HUNDRED_PERCENT	100

enum e_bars	{HUD_BASE, BAR_BACK, BAR_HEALTH, BAR_ENERGY, BAR_EXP,  NUM_BARS, NUM_STAT_BARS = NUM_BARS-2};
enum e_bar	{BAR_WIDTH = ONE_HUNDRED_PERCENT, BAR_HEIGHT = HUD_HEIGHT/NUM_STAT_BARS, BAR_X = HUD_X, BAR_Y = HUD_Y};

class Hud
{
private:
	SRect bars[NUM_BARS];
	Uint32 colors[NUM_BARS];
	SDL_Sprite pots;
	MyFont m_myFont[2];
	TTtext m_text[NUM_STAT_BARS];
	TTtext m_potText[NUM_POTS];
	StatWindow m_statWin;
	bool m_showStats;
	Button m_statButton;
public:
	Hud()
	{
		
		for(int i = 0; i < NUM_BARS; i++)
		{
			//set all to be same then modify
			if(i != HUD_BASE)
			{
				bars[i].setHeight(BAR_HEIGHT);
				bars[i].setWidth(BAR_WIDTH);
				bars[i].setX(BAR_X);
				bars[i].setY(BAR_Y);
			}
			else
			{
				bars[i].setHeight(HUD_HEIGHT);
				bars[i].setWidth(HUD_WIDTH);
				bars[i].setX(HUD_X);
				bars[i].setY(HUD_Y);
			}
		}
		bars[BAR_BACK].setHeight(NUM_STAT_BARS*BAR_HEIGHT);
		bars[BAR_ENERGY].setY(BAR_Y+BAR_HEIGHT);
		bars[BAR_EXP].setY(BAR_Y+(2*BAR_HEIGHT));
		colors[HUD_BASE] = COLOR_BASE;
		colors[BAR_BACK] = COLOR_BACK;
		colors[BAR_HEALTH] = COLOR_HEALTH;
		colors[BAR_ENERGY] = COLOR_ENERGY;
		colors[BAR_EXP] = COLOR_EXP;
		m_myFont[0].setFont(FONTSIZE);
		m_myFont[1].setFont(18);
		m_statWin.InitText(m_myFont[1].getFont());
		m_statButton.setUpButton(FRAME_SIZE, FRAME_SIZE, ((HUD_X+HUD_WIDTH)-FRAME_SIZE), HUD_Y);
		m_statButton.setImage("Sprites/button.bmp");
		m_showStats = false;
		for(int i = 0; i < NUM_STAT_BARS; i++)
		{
			m_text[i].setFont(m_myFont[0].getFont());
			m_text[i].setTextColor(0xffffffff);
		}
		for(int i = 0; i < NUM_POTS; i++)
		{
			m_potText[i].setFont(m_myFont[0].getFont());
			m_potText[i].setTextColor(0xffffffff);
		}
		m_text[0].setMessage("Health");
		m_text[1].setMessage("Energy");
		m_text[2].setMessage("Experience");
		m_potText[POT_HEALTH].setMessage("0");
		m_potText[POT_ENERGY].setMessage("0");
		pots.setSprite("Sprites/Icons.bmp", FRAME_SIZE, FRAME_SIZE, FRAME_RATE, NUM_ICON_ROWS);
		pots.setTransparency(COLOR_TRANSPARENT);
	}
	void handleInput(UserInput * ui)
	{
		if((m_statButton.wasClicked(ui) && !m_showStats) )
			m_showStats = true;
		else if((m_statButton.wasClicked(ui) && m_showStats) )
			m_showStats = false;
		if(m_showStats)
			m_statWin.handleInput(ui);
	}
	void updateHud(Player * p, UserInput * ui, int a_time)
	{
		handleInput(ui);
		bars[BAR_HEALTH].setWidth((int)((double)p->getStatNumber(HEALTH_CURRENT)/(double)p->getStatNumber(HEALTH_MAX) * ONE_HUNDRED_PERCENT));
		bars[BAR_ENERGY].setWidth((int)((double)p->getStatNumber(ENERGY_CURRENT)/(double)p->getStatNumber(ENERGY_MAX) * ONE_HUNDRED_PERCENT));
		bars[BAR_EXP].setWidth((int)(p->getCurExp()/(double)p->getExpReq() * ONE_HUNDRED_PERCENT));
		m_statWin.setPlayer(p);
		p->setWindowStatus(m_showStats);
		char temp[3];
		sprintf_s(temp, "%i",  p->getPots(POT_HEALTH));
		m_potText[POT_HEALTH].setMessage(temp);
		sprintf_s(temp, "%i",  p->getPots(POT_ENERGY));
		m_potText[POT_ENERGY].setMessage(temp);
		if(p->getPoints()>0)
			m_statButton.update(a_time);
		else
			m_statButton.stopAni();
	}
	void draw(SDL_Surface * screen)
	{
		for(int i = 0; i < NUM_BARS; i++)
			SDL_FillRect(screen, &bars[i], colors[i]);
		for(int i = 0; i < NUM_STAT_BARS; i++)
			m_text[i].printMessage(screen, bars[BAR_BACK].getWidth(), bars[BAR_BACK].getY()+(i*BAR_HEIGHT)-1);
		int x = m_statButton.getX()-(FRAME_SIZE*SLOT_ARMOR_HEAD);
		m_statWin.getPlayer()->drawInventory(screen, x, HUD_Y, INVENTORY_GAUNTLET, 0, (m_statButton.getX()-x)/FRAME_SIZE, SLOT_ATK1, true);
		m_statButton.draw(screen);
		//drawing the pots on screen
		pots.setRIndex(ICON_ROW_POTS);
		pots.setCIndex(POT_HEALTH);
		pots.draw(screen, (SCREEN_WIDTH/2)-FRAME_SIZE, SCREEN_HEIGHT-FRAME_SIZE);
		pots.setCIndex(POT_ENERGY);
		pots.draw(screen, (SCREEN_WIDTH/2), SCREEN_HEIGHT-FRAME_SIZE);
		for(int i = 0; i < NUM_POTS; i ++)
		{//will print the text at 1/2 screen then minus frame width for firest then exactly at mid
			m_potText[i].printMessage(screen, (SCREEN_WIDTH/2)-(FRAME_SIZE*(1-i)), SCREEN_HEIGHT-FRAME_SIZE);
		}
		if(m_showStats)
			m_statWin.draw(screen);
		
	}
};