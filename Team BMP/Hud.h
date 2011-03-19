#pragma once

#include "Player.h"
#include "srect.h"
#include "trueTextPrinter.h"
#include "ButtonAndWIndow.h"

enum e_bars {HUD_BASE, BAR_BACK, BAR_HEALTH, BAR_ENERGY, NUM_BARS};

#define BAR_HEIGHT			10
#define	BAR_WIDTH			100
#define BAR_X				50
#define BAR_Y				580

#define ONE_HUNDRED_PERCENT	100

#define STATBUTTONSETTINGS	15,15,500,580

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
		colors[BAR_BACK] = COLOR_BACK;
		colors[BAR_HEALTH] = COLOR_HEALTH;
		colors[BAR_ENERGY] = COLOR_ENERGY;
		m_statWin.InitText(m_myFont.getFont());
		m_statButton.setUpButton(STATBUTTONSETTINGS);
		m_showStats = false;
		//m_text.setMessage("testing");
	}
	void handleInput(UserInput * ui)
	{
		if(m_statButton.wasClicked(ui)&&m_showStats == false)
		{
			m_showStats = true;
		}
		else if(m_statButton.wasClicked(ui)&&m_showStats == true)
		{
			m_showStats = false;
		}
	}
	void updateHud(Player * p, UserInput * ui)
	{
		handleInput(ui);
		bars[BAR_HEALTH].setWidth(((double)p->getStats().m_stats[HEALTH_CURRENT]/(double)p->getStats().m_stats[HEALTH_MAX]) * ONE_HUNDRED_PERCENT);
		bars[BAR_ENERGY].setWidth(((double)p->getStats().m_stats[ENERGY_CURRENT]/(double)p->getStats().m_stats[ENERGY_MAX]) * ONE_HUNDRED_PERCENT);
		if(!p->getStats().compare(m_statWin.getStats())){m_statWin.setPlayerStats(p);}
	}
	void draw(SDL_Surface * screen)
	{
		//m_text.printMessage(screen, 0,0);
		for(int i = 0; i < NUM_BARS; i++)
		{
			SDL_FillRect(screen, &bars[i], colors[i]);
		}
		m_statButton.draw(screen);
		if(m_showStats)
		{
			m_statWin.draw(screen);
		}
	}
};