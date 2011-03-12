#pragma once
#include "sdl/SDL_ttf.h"
#include "surfaceFunctions.h"


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
		TTF_Init();
		m_font = TTF_OpenFont("arial.ttf",15);
		m_textColor.b = 255;
		m_textColor.r = 0;
		m_textColor.g = 0;
		m_backColor.b = 255;
		m_backColor.r = 255;
		m_backColor.g = 255;
	}
	void setMessage(char * a_message)//sets the text to print, this will need to be done every message
	{
		m_message = TTF_RenderText(m_font, a_message, m_textColor,m_backColor);
	}
	void printMessage(SDL_Surface * a_screen, int a_x, int a_y)//pritns it to the screen with the location passed by x and y
	{
		apply_surface(a_x, a_y, m_message, a_screen);
	}
	~TTtext()
	{
		SDL_FreeSurface(m_message);
		TTF_CloseFont(m_font);
		TTF_Quit();
	}
};