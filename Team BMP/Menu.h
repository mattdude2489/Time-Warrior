#ifndef _MENU_H_
#define _MENU_H_

#include "Global.h"
#include "SDL_Resource.h"
#include "templatevector.h"
#include "sdl\SDL.h"

class Menu_Button;

class Menu : public SDL_Resource {
protected:
	Uint32 m_uiMenuID;
	TemplateVector<Menu_Button *> m_buttons;
public:
	Menu();
	~Menu();
	virtual void activate() = 0;
};

#endif
