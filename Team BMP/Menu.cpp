#include "Menu.h"

Menu::Menu() : m_uiMenuID(0) {
}

Menu::~Menu() {
	m_buttons.release();
}
