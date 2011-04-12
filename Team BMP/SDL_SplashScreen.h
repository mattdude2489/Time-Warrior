#ifndef _SDL_SPLASHSCREEN_H_
#define _SDL_SPLASHSCREEN_H_

#include "Global.h"
#include "SDL_Resource.h"
#include "SDL_Sprite.h"
#include "sdl\SDL.h"
#include <assert.h>

class SDL_SplashScreen : public SDL_Resource {

protected:

	/** x coordinate to draw the splash screen to, centered for screen surface */
	Sint32 m_iCenterX;
	/** y coordinate to draw the splash screen to, centered for screen surface */
	Sint32 m_iCenterY;
	/** amount of time splash screen will be shown */
	Uint32 m_uiSpeed;
	/** pointer to the screen surface to draw to */
	SDL_Surface * m_pvScreen;
	/** sprite for animation-based splash screen */
	SDL_Sprite * m_pvSprite;

public:

	/** default constructor */
	SDL_SplashScreen();
	/** deconstructor */
	~SDL_SplashScreen();

	/** setSprite - sets the splash sprite to the given reference */
	void setSprite(SDL_Sprite * a_pvSprite);
	/** setSpeed - sets the speed of the splash screen transition to the specified */
	void setSpeed(const Uint32 & a_kruiSpeed);
	/** setScreen - sets the screen to draw to the specified screen surface */
	void setScreen(SDL_Surface * a_pvScreen);
	/** getSprite - returns a constant reference of the sprite the splash screen uses */
	const SDL_Sprite * getSprite() const;
	/** getSpeed - returns amount of time splash screen will be shown */
	const Uint32 & getSpeed() const;
	/** activate - draws splash screen to the screen, and sets as unchanged */
	virtual void activate();

};

#endif
