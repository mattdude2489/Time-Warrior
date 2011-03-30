#pragma once

#include "Player.h"
#include "srect.h"
#include "trueTextPrinter.h"
#include "ButtonAndWIndow.h"

#define ONE_HUNDRED_PERCENT	100

enum e_bar {BAR_WIDTH = ONE_HUNDRED_PERCENT, BAR_HEIGHT = HUD_HEIGHT/2, BAR_X = HUD_X, BAR_Y = HUD_Y};
enum e_bars {HUD_BASE, BAR_BACK, BAR_HEALTH, BAR_ENERGY, NUM_BARS};

#define STAT_BUTTON_SETTINGS	HUD_HEIGHT,HUD_HEIGHT,((HUD_X+HUD_WIDTH)-HUD_HEIGHT),HUD_Y

class Hud
{
private:
	SRect bars[NUM_BARS];
	Uint32 colors[NUM_BARS];
	MyFont m_myFont;
	//TTtext m_text;
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
		bars[BAR_BACK].setHeight(2*BAR_HEIGHT);
		bars[BAR_ENERGY].setY(BAR_Y+BAR_HEIGHT);
		colors[HUD_BASE] = COLOR_BASE;
		colors[BAR_BACK] = COLOR_BACK;
		colors[BAR_HEALTH] = COLOR_HEALTH;
		colors[BAR_ENERGY] = COLOR_ENERGY;
		m_statWin.InitText(m_myFont.getFont());
		m_statButton.setUpButton(STAT_BUTTON_SETTINGS);
		m_showStats = false;
		//m_text.setMessage("testing");
	}
	void handleInput(UserInput * ui)
	{
		if(m_statButton.wasClicked(ui) && !m_showStats)
			m_showStats = true;
		else if(m_statButton.wasClicked(ui) && m_showStats)
			m_showStats = false;
	}
	void updateHud(Player * p, UserInput * ui)
	{
		handleInput(ui);
		bars[BAR_HEALTH].setWidth((double)p->getStatNumber(HEALTH_CURRENT)/(double)p->getStatNumber(HEALTH_MAX) * ONE_HUNDRED_PERCENT);
		bars[BAR_ENERGY].setWidth((double)p->getStatNumber(ENERGY_CURRENT)/(double)p->getStatNumber(ENERGY_MAX) * ONE_HUNDRED_PERCENT);
		m_statWin.setPlayer(p);
	}
	void draw(SDL_Surface * screen)
	{
		//m_text.printMessage(screen, 0,0);
		for(int i = 0; i < NUM_BARS; i++)
			SDL_FillRect(screen, &bars[i], colors[i]);
		m_statButton.draw(screen);
		for(int i = 0; i < 2; ++i)
			m_statWin.getPlayer()->drawSlot((e_gauntletSlots)i, screen, (HUD_X + HUD_WIDTH) - ((2-(i-1)) * HUD_HEIGHT), HUD_Y);
		if(m_showStats)
			m_statWin.draw(screen);
	}
};