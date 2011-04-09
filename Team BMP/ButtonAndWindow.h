#pragma once

#include "srect.h"
#include "UserInput.h"
#include "spoint.h"
#include "trueTextPrinter.h"
#include "Player.h"

#define BUTTONCOLOR			0xffff00
#define WINDOWCOLOR			0xffffff
#define STATBUTTON			0xff0000
#define WINDOWHEIGHT		600
#define WINDOWWIDTH			400
#define WINDOWXY			SPoint(0,0)
#define CHARSIZE			30
#define TEXTSTART			30
#define TEXTYOFF			20
#define	STATOFF				21
#define	STATINC				4

class Button
{
private:
	SRect m_buttonShape;
public:
	Button(){}
	Button(int a_width, int a_height, int a_x, int a_y){setUpButton(a_width, a_height, a_x, a_y);}
	void setUpButton(int a_width, int a_height, int a_x, int a_y)
	{
		m_buttonShape.setWidth(a_width);
		m_buttonShape.setHeight(a_height);
		m_buttonShape.setX(a_x);
		m_buttonShape.setY(a_y);
	}
	void setUpButton(int a_width, int a_height)
	{
		m_buttonShape.setWidth(a_width);
		m_buttonShape.setHeight(a_height);
	}
	void setPos(int a_x, int a_y)
	{
		m_buttonShape.setX(a_x);
		m_buttonShape.setY(a_y);
	}
	bool wasClicked(UserInput * a_input)//returns only if there is a click inside its bounds
	{
		SPoint t_tempPoint(a_input->getMouseX(),a_input->getMouseY()); 
		if(a_input->getClick())
		{
			if(m_buttonShape.contains(t_tempPoint))
				return true;//the click was inside the button;
		}
		return false;//return false in not
	}
	void draw(SDL_Surface * a_screen)
	{
		//this will change when i get a bmp for it to load into as a button
		SDL_FillRect(a_screen, &m_buttonShape, BUTTONCOLOR);
	}
};

class StatWindow
{
private:
	TTtext m_text[NUM_STATS];
	TTtext m_lvlPts[2];
	Player * m_player;
	SRect m_window;
	Button m_addStat[STATINC];
public:
	StatWindow()
	{
		m_window.setHeight(WINDOWHEIGHT);
		m_window.setWidth(WINDOWWIDTH);
		m_window.setPosition(WINDOWXY);
		for(int i = 0; i < STATINC; i++)
		{
			if(i < 2){
				m_addStat[i].setUpButton(TEXTYOFF, TEXTYOFF-2, WINDOWWIDTH/2, STATOFF + (i*STATOFF));
			}
			else{
				m_addStat[i].setUpButton(TEXTYOFF, TEXTYOFF-2, WINDOWWIDTH/2, STATOFF + ((i+1)*STATOFF));
			}
		}
	}
	void InitText(TTF_Font * a_font)
	{
		for(int i = 0; i < NUM_STATS; i++)
			m_text[i].setFont(a_font);
		m_lvlPts[0].setFont(a_font);
		m_lvlPts[1].setFont(a_font);
	}
	Player * getPlayer(){return m_player;}
	void setPlayer(Player * a_player)
	{	
		m_player = a_player;
		putStatsInText();
	}
	void putStatsInText()
	{
		char c_temp[CHARSIZE];
		for(int i = 0; i < NUM_STATS; i++)
		{
			sprintf_s(c_temp, m_player->getStatName(i), m_player->getStatNumber(i));
			m_text[i].setMessage(c_temp);
		}
		sprintf_s(c_temp, "Level: %i", m_player->getLevel());
		m_lvlPts[0].setMessage(c_temp);
		sprintf_s(c_temp, "Points to spend: %i", m_player->getPoints());
		m_lvlPts[1].setMessage(c_temp);
	}
	void handleInput(UserInput * ui)
	{
		for(int i = 0; i <  STATINC; i++)
		{
			if(m_addStat[i].wasClicked(ui))
			{
				switch(i)
				{
				case 0: m_player->incHealth();break;
				case 1: m_player->incEnergy();break;
				case 2: m_player->incStr();break;
				case 3: m_player->incInt();break;
				}
			}
		}
	}
	void draw(SDL_Surface * a_screen)
	{
		int t_x, t_y, t_s = 0;
		t_x = t_y = TEXTSTART;
		SDL_FillRect(a_screen, &m_window, WINDOWCOLOR);
		for(int i = 0; i < NUM_STATS; i++)
		{
			m_text[i].printMessage(a_screen, t_x, t_y);
			if(i == HEALTH_CURRENT || i == ENERGY_CURRENT)
				t_x += m_text[i].getMesageWidth();
			else
			{
				t_x = TEXTSTART;
				t_y += TEXTYOFF;
			}
		}
		for(int i = 0; i < 2; i ++)
		{
			m_lvlPts[i].printMessage(a_screen, i + (i*WINDOWWIDTH/2), 0);
		}
		if(m_player->getPoints()>0)
		{
			for(int i = 0; i < STATINC; i++){
				m_addStat[i].draw(a_screen);
			}
		}
	}
};
