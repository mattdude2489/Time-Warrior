#pragma once

#include "Player.h"
#include "srect.h"
#include "trueTextPrinter.h"
#include "ButtonAndWIndow.h"

enum e_bars {HUD_BASE, BAR_BACK, BAR_HEALTH, BAR_ENERGY, NUM_BARS};

#define COLOR_BACK			0x0000ff
#define COLOR_HEALTH		0xff0000
#define	COLOR_ENERGY		0x00ff00
#define COLOR_BASE			0xffffff

#define B_HEIGHT			10
#define	B_WIDTH				100
#define B_X					50
#define B_Y					580
#define BASEX				0
#define BASEY				550
#define BASEWIDTH			800
#define BASEHEIGHT			50

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
				bars[i].setHeight(B_HEIGHT);
				bars[i].setWidth(B_WIDTH);
				bars[i].setX(B_X);
				bars[i].setY(B_Y);
			}
			else
			{
				bars[i].setHeight(BASEHEIGHT);
				bars[i].setWidth(BASEWIDTH);
				bars[i].setX(BASEX);
				bars[i].setY(BASEY);
			}
		}
		bars[BAR_BACK].setHeight(2*B_HEIGHT);
		bars[BAR_ENERGY].setY(B_Y+B_HEIGHT);
		colors[BAR_BACK] = COLOR_BACK;
		colors[BAR_HEALTH] = COLOR_HEALTH;
		colors[BAR_ENERGY] = COLOR_ENERGY;
		m_statWin.InitText(m_myFont.getFont());
		m_statButton.setUpButton(STATBUTTONSETTINGS);
		m_showStats = false;
		//m_text.setMessage("testing");
	}
	void handleInput(UserInput ui)
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
	void updateHud(Player p, UserInput ui)
	{
		handleInput(ui);
		bars[BAR_HEALTH].setWidth( (int)((double)p.getStats().m_stats[HEALTH_CURRENT]/(double)p.getStats().m_stats[HEALTH_MAX]) * ONE_HUNDRED_PERCENT);
		bars[BAR_ENERGY].setWidth( (int)((double)p.getStats().m_stats[ENERGY_CURRENT]/(double)p.getStats().m_stats[ENERGY_MAX]) * ONE_HUNDRED_PERCENT);
		if(!p.getStats().compare(m_statWin.getStats()))
		{
			m_statWin.setPlayerStats(p);
		}
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