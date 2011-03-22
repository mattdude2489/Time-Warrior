#ifndef _ENGINEOWNEDSTATES_H_
#define _ENGINEOWNEDSTATES_H_

#include "Global.h"
#include "sdl\sdl.h"
#include "State.h"

class SDL_GameEngine;
class SDL_Resource;
class SDL_SplashScreen;
class Menu;

class Splash_State : public State<SDL_GameEngine> {

private:
	
	/** timer to keep track of how many miliseconds pass while the splash screen is displaying */
	Uint32 m_uiTimer;
	/** speed in ms for which to transition between splash screens */
	Uint32 m_uiSpeed;
	/** index to currently displayed splash screen */
	Uint32 m_uiCurrentSplash;
	/** amount of splash screens to go through */
	Uint32 m_uiSplashAmount;
	/** pointer to the current splash screen */
	SDL_SplashScreen * m_pvSplashScreen;

public:
	
	/** default constructor */
	Splash_State();
	/** deconstructor */
	~Splash_State();

	/** instance - returns singleton instance of splash state */
	static Splash_State * instance();
	/** enter - executes when the splash state is entered */
	virtual void enter(SDL_GameEngine * a_pvEngine);
	/** execute - executes during normal operation of splash state */
	virtual void execute(SDL_GameEngine * a_pvEngine);
	/** exit - executes when the splash state is left */
	virtual void exit(SDL_GameEngine * a_pvEngine);

};

class Menu_State : public State<SDL_GameEngine> {

private:
	
	/** index to currently displayed menu screen */
	Uint32 m_uiCurrentMenu;
	/** amount of menu screens available */
	Uint32 m_uiMenuAmount;
	/** pointer to currently active menu */
	Menu * m_pvMenu;

public:
	
	/** default constructor */
	Menu_State();
	/** deconstructor */
	~Menu_State();

	/** instance - returns singleton instance of menu state */
	static Menu_State * instance();
	/** enter - executes when the menu state is entered */
	virtual void enter(SDL_GameEngine * a_pvEngine);
	/** execute - executes during normal operation of menu state */
	virtual void execute(SDL_GameEngine * a_pvEngine);
	/** exit - executes when the menu state is left */
	virtual void exit(SDL_GameEngine * a_pvEngine);

};

class Game_State : public State<SDL_GameEngine> {

private:

	/** pointer to the engine */
	SDL_GameEngine * m_pvEngine;
	/** pointer to the current state of gameplay */
	State<Game_State> * m_pvGameState;

public:

	/** default constructor */
	Game_State();
	/** deconstructor */
	~Game_State();
	
	/** instance - returns singleton instance of game state */
	static Game_State * instance();
	/** enter - executes when the game state is entered */
	virtual void enter(SDL_GameEngine * a_pvEngine);
	/** execute - executes during normal operation of game state */
	virtual void execute(SDL_GameEngine * a_pvEngine);
	/** exit - executes when the game state is left */
	virtual void exit(SDL_GameEngine * a_pvEngine);

	/** getEngine - returns a reference to the game engine */
	SDL_GameEngine * getEngine() const;
	/** changeState - changes the state of the game to the specified */
	void changeState(State<Game_State> * a_pvState);
	/** revertState - reverts the state of the game back to the last one */
	void revertState();

};

#endif
