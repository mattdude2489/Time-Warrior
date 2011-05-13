#pragma once

#include "srect.h"
#include "UserInput.h"
#include "spoint.h"
#include "trueTextPrinter.h"
#include "Player.h"
#include "surfaceFunctions.h"

#define BUTTONCOLOR			0xffff00
#define WINDOWCOLOR			0xffffff
#define STATBUTTON			0xff0000
//only used by Player to check atk-click
#define WINDOWHEIGHT		SCREEN_HEIGHT
#define WINDOWWIDTH			(SCREEN_WIDTH/2)
//used to position window & its contents
#define WINDOWXY			SPoint(0,0)
#define CHARSIZE			30
#define TEXTSTART			FRAME_SIZE
#define TEXTYOFF			FRAME_SIZE
#define	STATOFF				4
#define	STATINC				2


class Button
{
private:
	SRect m_buttonShape;
	SDL_Sprite * m_image;
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
	void setUpButton(int a_width, int a_height){m_buttonShape.setWidth(a_width);m_buttonShape.setHeight(a_height);}
	void setImage(char * a_fileName)
	{
		m_image = new SDL_Sprite(a_fileName, FRAME_SIZE, FRAME_SIZE, FRAME_RATE, 1);
		m_image->setTransparency(COLOR_TRANSPARENT);
		m_image->setLoopToBegin(true);
		m_buttonShape.setHeight(m_image->getHeight());
		m_buttonShape.setWidth(m_image->getWidth());
	}
	void setPos(int a_x, int a_y){m_buttonShape.setX(a_x);m_buttonShape.setY(a_y);}
	void update(int a_time){m_image->start();m_image->update(a_time);}
	void stopAni(){m_image->stop();m_image->restart();}
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
	void draw(SDL_Surface * a_screen){m_image->draw(a_screen, m_buttonShape.getX(), m_buttonShape.getY());}
	~Button(){delete m_image;}
};

class StatWindow
{
private:
	TTtext m_text[NUM_STATS];
	TTtext m_lvlPts;
	Player * m_player;
	SDL_Surface * m_window;
	Button m_addStat[STATINC];
public:
	StatWindow()
	{
		m_window = load_image("Sprites/SideBar.bmp");
		for(int i = 0; i < STATINC; i++)
		{
			m_addStat[i].setPos(WINDOWXY.x + (m_window->w/2), WINDOWXY.y + ((i+STATOFF)*TEXTYOFF));
			m_addStat[i].setImage("Sprites/button1.bmp");
		}
	}
	void InitText(TTF_Font * a_font)
	{
		for(int i = 0; i < NUM_STATS; i++)
			m_text[i].setFont(a_font);
		m_lvlPts.setFont(a_font);
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
			sprintf_s(c_temp, m_player->getStatName((e_stats)i), m_player->getStatNumber((e_stats)i));
			m_text[i].setMessage(c_temp);
		}
		sprintf_s(c_temp, "Points to spend: %i", m_player->getPoints());
		m_lvlPts.setMessage(c_temp);
	}
	void handleInput(UserInput * ui)
	{
		for(int i = 0; i <  STATINC; i++)
		{
			if(m_addStat[i].wasClicked(ui) && m_player->getPoints() > 0)
			{
				switch(i)
				{
				case 0:	m_player->incStr();	break;
				case 1:	m_player->incInt();	break;
				}
			}
		}
	}
	void draw(SDL_Surface * a_screen)
	{
		int t_x = WINDOWXY.x, t_y = WINDOWXY.y;
		apply_surface(t_x,t_y,m_window, a_screen);
		t_x += TEXTSTART;
		for(int i = 0; i < NUM_STATS; i++)
		{
			m_text[i].printMessage(a_screen, t_x, t_y);
			if(i == HEALTH_CURRENT || i == ENERGY_CURRENT)
				t_x += m_text[i].getMesageWidth();
			else
			{
				t_x = WINDOWXY.x + TEXTSTART;
				t_y += TEXTYOFF;
			}
		}
		m_lvlPts.printMessage(a_screen, WINDOWXY.x + m_window->w/2, WINDOWXY.y);
		if(m_player->getPoints() > 0)
		{
			for(int i = 0; i < STATINC; i++)
				m_addStat[i].draw(a_screen);
		}
		m_player->drawInventory(a_screen, WINDOWXY.x, t_y, m_window->w/FRAME_SIZE, MAGIC);
		t_y += FRAME_SIZE*3;
		m_player->drawInventory(a_screen, WINDOWXY.x, t_y, m_window->w/FRAME_SIZE, ARMOR);
		t_y += FRAME_SIZE*2;
		m_player->drawGauntlet(a_screen, WINDOWXY.x, t_y, m_window->w/FRAME_SIZE);
	}
	~StatWindow(){}
};
