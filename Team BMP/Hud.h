#pragma once

#include "Player.h"
#include "srect.h"

#define NUMBERBARS			3
#define BACKBAR				0
#define HEALTHBAR			1
#define	ENERGYBAR			2

#define BACKCOLOR			0x0000ff
#define HEALTHCOLOR			0xff0000
#define	ENERGYCOLOR			0x00ff00

#define BHEIGHT				50
#define	BWIDTH				100
#define BX					50
#define BY					500

#define CURRENT				0
#define	MAX					1
#define ONEHUNDREDPERCENT	100

class Hud
{
private:
	SRect bars[NUMBERBARS];
	Uint32 colors[NUMBERBARS];

public:
	Hud()
	{
		for(int i = 0; i < NUMBERBARS; i++)
		{
			//set all to be same then modify
			bars[i].setHeight(BHEIGHT);
			bars[i].setWidth(BWIDTH);
			bars[i].setX(BX);
			bars[i].setY(BY);
		}
		bars[BACKBAR].setHeight(2*BHEIGHT);
		bars[ENERGYBAR].setY(BY+BHEIGHT);
		colors[BACKBAR] = BACKCOLOR;
		colors[HEALTHBAR] = HEALTHCOLOR;
		colors[ENERGYBAR] = ENERGYCOLOR;
	}
	void updateHud(Player p)
	{
		bars[HEALTHBAR].setWidth( ((double)p.getStats().health[CURRENT]/(double)p.getStats().health[MAX]) * ONEHUNDREDPERCENT);
		bars[ENERGYBAR].setWidth( ((double)p.getStats().energy[CURRENT]/(double)p.getStats().energy[MAX]) * ONEHUNDREDPERCENT);
	}
	void draw(SDL_Surface * screen)
	{
		for(int i = 0; i < NUMBERBARS; i++)
		{
			SDL_FillRect(screen, &bars[i], colors[i]);
		}
	}
};