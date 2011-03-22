#ifndef _MENU_BUTTON_H_
#define _MENU_BUTTON_H_

#include "Global.h"
#include "sdl\SDL.h"
#include "srect.h"

class Menu_Object {
protected:
	SRect m_vMenuSpace;
public:
	onClick() = 0;
	onHover() = 0;
};

#endif
