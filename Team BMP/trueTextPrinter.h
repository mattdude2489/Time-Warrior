#pragma once
#include "sdl/SDL_ttf.h"
#include "surfaceFunctions.h"
#define FONTSIZE		10

class MyFont
{
private:
	TTF_Font * m_font;
public:
	MyFont()
	{
		TTF_Init();
	}
	MyFont(int fontsize)
	{
		TTF_Init();
		m_font = TTF_OpenFont("arial.ttf", fontsize);
	}
	void setFont(int fontsize)
	{
		m_font = TTF_OpenFont("arial.ttf", fontsize);
	}
	void changeSizeTo(int fontsize)
	{
		m_font = TTF_OpenFont("arial.ttf", fontsize);
	}
	TTF_Font * getFont(){return m_font;}
	~MyFont()
	{
		TTF_CloseFont(m_font);
		TTF_Quit();
	}
};

class TTtext
{
private:
	SDL_Surface * m_message;
	TTF_Font * m_font;
	SDL_Color m_textColor;
	SDL_Color m_backColor;

public:
	TTtext()//inits everything to print text
	{
		m_textColor.b = 255;
		m_textColor.r = 0;
		m_textColor.g = 0;
		m_backColor.b = 255;
		m_backColor.r = 255;
		m_backColor.g = 255;
		m_message = NULL;
	}
	//Sets the text color, but not the background.
	void setTextColor(Uint32 color)
	{
		//m_textColor = SDL_Color(color);
		m_textColor.r = color >> 16;
		m_textColor.g = color >> 8;
		m_textColor.b = color;
	}
	//Sets background color of the text.
	void setBackColor(Uint32 color)
	{
		m_backColor.r = color >> 16;
		m_backColor.g = color >> 8;
		m_backColor.b = color;
	}
	void setMessage(char * a_message)//sets the text to print, this will need to be done every message
	{
		m_message = TTF_RenderText_Blended(m_font, a_message, m_textColor);
	}

	void printMessage(SDL_Surface * a_screen, int a_x, int a_y)//pritns it to the screen with the location passed by x and y
	{
		apply_surface(a_x, a_y, m_message, a_screen);
	}
	bool messageAvailable() {if(m_message != NULL) return true; else return false;}
	int getMesageWidth(){return m_message->w;}
	void setFont(TTF_Font * a_font){m_font = a_font;}
	void setFontSize(int a_size)
	{
		
	}
	~TTtext()
	{
		if(m_message != NULL)
			SDL_FreeSurface(m_message);
	}
};