#pragma once

#include "srect.h"
#include "UserInput.h"
#include "spoint.h"
#include "trueTextPrinter.h"
#include "Player.h"

#define BUTTONCOLOR			0xffff00
#define WINDOWCOLOR			0xffffff
#define WINDOWHEIGHT		600
#define WINDOWWIDTH			400
#define WINDOWXY			SPoint(0,0)
#define CHARSIZE			30
#define TEXTSTART			30
#define TEXTYOFF			20

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
	Player * m_player;
	SRect m_window;
public:
	StatWindow()
	{
		m_window.setHeight(WINDOWHEIGHT);
		m_window.setWidth(WINDOWWIDTH);
		m_window.setPosition(WINDOWXY);
	}
	void InitText(TTF_Font * a_font)
	{
		for(int i = 0; i < NUM_STATS; i++)
			m_text[i].setFont(a_font);
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
	}
	void draw(SDL_Surface * a_screen)
	{
		int t_x, t_y;
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
	}
};
