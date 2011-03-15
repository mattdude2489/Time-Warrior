#pragma once

#include "Player.h"
#include "srect.h"
#include "trueTextPrinter.h"

enum e_bars {BAR_BACK, BAR_HEALTH, BAR_ENERGY, NUM_BARS};

#define COLOR_BACK			0x0000ff
#define COLOR_HEALTH		0xff0000
#define	COLOR_ENERGY		0x00ff00

#define B_HEIGHT			10
#define	B_WIDTH				100
#define B_X					50
#define B_Y					580

#define ONE_HUNDRED_PERCENT	100

class Hud
{
private:
	SRect bars[NUM_BARS];
	Uint32 colors[NUM_BARS];
	TTtext m_text;
public:
	Hud()
	{
		for(int i = 0; i < NUM_BARS; i++)
		{
			//set all to be same then modify
			bars[i].setHeight(B_HEIGHT);
			bars[i].setWidth(B_WIDTH);
			bars[i].setX(B_X);
			bars[i].setY(B_Y);
		}
		bars[BAR_BACK].setHeight(2*B_HEIGHT);
		bars[BAR_ENERGY].setY(B_Y+B_HEIGHT);
		colors[BAR_BACK] = COLOR_BACK;
		colors[BAR_HEALTH] = COLOR_HEALTH;
		colors[BAR_ENERGY] = COLOR_ENERGY;
		m_text.setMessage("testing");
	}
	void updateHud(Player p)
	{
		bars[BAR_HEALTH].setWidth( ((double)p.getStats().health[STATS_CURRENT]/(double)p.getStats().health[STATS_MAX]) * ONE_HUNDRED_PERCENT);
		bars[BAR_ENERGY].setWidth( ((double)p.getStats().energy[STATS_CURRENT]/(double)p.getStats().energy[STATS_MAX]) * ONE_HUNDRED_PERCENT);
	}
	void draw(SDL_Surface * screen)
	{
		m_text.printMessage(screen, 0,0);
		for(int i = 0; i < NUM_BARS; i++)
		{
			SDL_FillRect(screen, &bars[i], colors[i]);
		}
	}
};