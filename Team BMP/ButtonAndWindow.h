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
	int getX(){return m_buttonShape.x;}
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
	TTtext m_text[NUM_STATS],  m_lvlPts, m_inventory[NUM_INVENTORY], m_playerName, m_chipInfo;
	Player * m_player;
	SDL_Surface * m_window;
	Button m_addStat[STATINC];
	int m_yStartInvAttack, m_yStartInvArmor;
	bool m_drawInfo;
public:
	StatWindow()
	{
		m_window = load_image("Sprites/SideBar.bmp");
		for(int i = 0; i < STATINC; i++)
		{
			m_addStat[i].setPos(WINDOWXY.x + (m_window->w/2), WINDOWXY.y + ((i+STATOFF)*TEXTYOFF));
			m_addStat[i].setImage("Sprites/button1.bmp");
		}
		m_yStartInvAttack = m_yStartInvArmor = 0;
		m_drawInfo = false;
	}
	void InitText(TTF_Font * a_font)
	{
		for(int i = 0; i < NUM_STATS; i++)
			m_text[i].setFont(a_font);
		m_lvlPts.setFont(a_font);
		m_playerName.setFont(a_font);
		for(int i = 0; i < NUM_INVENTORY; i++)
			m_inventory[i].setFont(a_font);
		m_chipInfo.setFont(a_font);
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
		sprintf_s(c_temp, "Unlocked Attacks:");
		m_inventory[INVENTORY_ATTACK].setMessage(c_temp);
		sprintf_s(c_temp, "Armor Inventory:");
		m_inventory[INVENTORY_ARMOR].setMessage(c_temp);
		sprintf_s(c_temp, "Gauntlet Configuration:");
		m_inventory[INVENTORY_GAUNTLET].setMessage(c_temp);
		sprintf_s(c_temp, "Name: %s", m_player->getName(), 21);
		m_playerName.setMessage(c_temp);
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
		SPoint t_tempPoint(ui->getMouseX(),ui->getMouseY()); 
		Chip * clicked = NULL;
		bool cl = ui->getClick() == CLICK_LEFT, cr = ui->getClick() == CLICK_RIGHT;
		if(cl || cr)
		{
			clicked = m_player->getHUDClickedChip(t_tempPoint, WINDOWXY.x, m_yStartInvAttack, INVENTORY_ATTACK, m_window->w/FRAME_SIZE, 0, 0);
			if(!clicked)
				clicked = m_player->getHUDClickedChip(t_tempPoint, WINDOWXY.x, m_yStartInvArmor, INVENTORY_ARMOR, m_window->w/FRAME_SIZE, 0, 0);
			if(clicked)
			{
				if(clicked->getType() == ARMOR)
				{
					if(cl)
					{
						if(clicked->isEquipped())
							m_player->removeGauntletArmor(clicked->getSubType());
						else
							m_player->setGauntletArmor(clicked);
					}
					else if(cr)
					{
						if(clicked->isEquipped())
							m_player->removeGauntletArmor(clicked->getSubType());
						m_player->sellArmor(clicked);
					}
				}
				else
				{
					if(cl)
						m_player->setGauntletSlot(SLOT_ATK1, clicked);
					else if(cr)
						m_player->setGauntletSlot(SLOT_ATK2, clicked);
				}
			}
		}
		clicked =  m_player->getHUDClickedChip(t_tempPoint, WINDOWXY.x, m_yStartInvAttack, INVENTORY_ATTACK, m_window->w/FRAME_SIZE, 0, 0);
		if(!clicked)
				clicked = m_player->getHUDClickedChip(t_tempPoint, WINDOWXY.x, m_yStartInvArmor, INVENTORY_ARMOR, m_window->w/FRAME_SIZE, 0, 0);
		if(clicked)//not really clicking
		{
			char temp[32];
			switch(clicked->getType())
			{
			case MAGIC:		sprintf(temp, "Magic: %02i", clicked->getDamage());				break;
			case WEAPON:	sprintf(temp, "Weapon: %02i", clicked->getDamage());			break;
			case ARMOR:		sprintf(temp, "Armor: %02i", clicked->getStatNumber(DEFENSE));	break;
			}
			m_chipInfo.setMessageBackground(temp);
			m_chipInfo.setLoc(ui->getMouseX(), ui->getMouseY());
			m_drawInfo = true;
			
		}
		else
			m_drawInfo = false;
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
		t_x = WINDOWXY.x + m_window->w/4;
		m_lvlPts.printMessage(a_screen, WINDOWXY.x + m_window->w/2, WINDOWXY.y);
		m_playerName.printMessage(a_screen, WINDOWXY.x + m_window->w/2, WINDOWXY.y + TEXTYOFF);
		if(m_player->getPoints() > 0)
		{
			for(int i = 0; i < STATINC; i++)
				m_addStat[i].draw(a_screen);
		}
		for(int i = 0; i < NUM_INVENTORY; ++i)
		{
			m_inventory[i].printMessage(a_screen, t_x, t_y);
			t_y += TEXTYOFF;
			switch((e_inventory)i)
			{
			case INVENTORY_ATTACK:	m_yStartInvAttack = t_y;	break;
			case INVENTORY_ARMOR:	m_yStartInvArmor = t_y;		break;
			}
			t_y += m_player->drawInventory(a_screen, WINDOWXY.x, t_y, (e_inventory)i, m_window->w/FRAME_SIZE, 0, 0) * FRAME_SIZE;
		}
		if(m_drawInfo)
		{
			m_chipInfo.printMessage(a_screen);
		}
	}
	~StatWindow(){}
};
