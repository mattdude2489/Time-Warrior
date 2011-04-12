#include "SDL_SplashScreen.h"

/** default constructor */
SDL_SplashScreen::SDL_SplashScreen() : m_iCenterX(0), m_iCenterY(0), m_uiSpeed(0), m_pvScreen(NULL), m_pvSprite(NULL) {
	setResID(RESOURCE_SPLASH);
}

/** deconstructor */
SDL_SplashScreen::~SDL_SplashScreen() {
	//if sprite exists
	if(m_pvSprite != NULL) {
		//destroy sprite resources
		m_pvSprite->destroy();
		//delete the sprite
		delete [] m_pvSprite;
		//nullify the sprite
		m_pvSprite = NULL;
	}
}



/** setSprite - sets the splash sprite to the given reference */
void SDL_SplashScreen::setSprite(SDL_Sprite * a_pvSprite) {
	assert(a_pvSprite != NULL);
	m_pvSprite = a_pvSprite;
	setChanged(true);
}

/** setSpeed - sets the speed of the splash screen transition to the specified */
void SDL_SplashScreen::setSpeed(const Uint32 & a_kruiSpeed) {
	m_uiSpeed = a_kruiSpeed;
}

/** setScreen - sets the screen to draw to the specified screen surface */
void SDL_SplashScreen::setScreen(SDL_Surface * a_pvScreen) {
	assert(a_pvScreen != NULL && m_pvSprite != NULL);
	m_pvScreen = a_pvScreen;
	m_iCenterX = (m_pvScreen->w >> 1) - (m_pvSprite->getWidth() >> 1);
	m_iCenterY = (m_pvScreen->h >> 1) - (m_pvSprite->getHeight() >> 1);
	setChanged(true);
}

/** getSprite - returns a constant reference of the sprite the splash screen uses */
const SDL_Sprite * SDL_SplashScreen::getSprite() const {
	return m_pvSprite;
}

/** getSpeed - returns amount of time splash screen will be shown */
const Uint32 & SDL_SplashScreen::getSpeed() const {
	return m_uiSpeed;
}

/** draws splash screen to the screen, and sets as unchanged */
void SDL_SplashScreen::activate() {
	//draw the splash screen to the screen
	m_pvSprite->draw(m_pvScreen, m_iCenterX, m_iCenterY);
	setChanged(false);
}
