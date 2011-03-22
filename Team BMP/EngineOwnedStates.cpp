#include "EngineOwnedStates.h"
#include "SDL_GameEngine.h"
#include "SDL_SplashScreen.h"
#include "Menu.h"
#include "GameOwnedStates.h"
#include <assert.h>

//=================================================
//	SPLASH STATE
//=================================================

/** default constructor */
Splash_State::Splash_State() : m_uiTimer(0), m_uiSpeed(0), m_uiCurrentSplash(0), m_uiSplashAmount(0) {
	m_pvNextState = Menu_State::instance();
}

/** deconstructor */
Splash_State::~Splash_State() {
}

/** instance - returns singleton instance of splash state */
Splash_State * Splash_State::instance() {
	//create static instance of the splash state
	static Splash_State instance;
	//return its address
	return &instance;
}

/** enter - executes when the splash state is entered */
void Splash_State::enter(SDL_GameEngine * a_pvEngine) {
	//load all splash screen resources
	a_pvEngine->loadAll(RESOURCE_SPLASH);
	//prepare splash state variables
	m_uiTimer = 0;
	m_uiCurrentSplash = 0;
	m_uiSplashAmount = a_pvEngine->getResAmount(RESOURCE_SPLASH);
	m_pvSplashScreen = (SDL_SplashScreen *)a_pvEngine->getRes(RESOURCE_SPLASH, m_uiCurrentSplash);
	assert(m_pvSplashScreen != NULL);
	m_uiSpeed = m_pvSplashScreen->getSpeed();
	m_pvSplashScreen->setActive(true);
}

/** execute - executes during normal operation of splash state */
void Splash_State::execute(SDL_GameEngine * a_pvEngine) {
	//record boolean for changing the splash screen
	bool bSplashChange = false;

	//change the splash screen if the timer exceeds the speed of the transition
	bSplashChange |= ( (m_uiTimer+=a_pvEngine->getMSDelay()) >= m_uiSpeed );
	
	//if splash screen changes
	if(bSplashChange) {
		//set splash screen to inactive
		m_pvSplashScreen->setActive(false);
		//if more splash screens need to be drawn
		if(m_uiCurrentSplash < m_uiSplashAmount) {
			//set timer to 0
			m_uiTimer = 0;
			//set splash screen to the next one
			m_pvSplashScreen = (SDL_SplashScreen *)a_pvEngine->getRes(RESOURCE_SPLASH, ++m_uiCurrentSplash);
			//set the speed of the new splash screen
			m_uiSpeed = m_pvSplashScreen->getSpeed();
			//set the new splash screen as active
			m_pvSplashScreen->setActive(true);
		}
		//else if no more need to be drawn
		else
			//go to the next state
			a_pvEngine->nextState();
	}
}

/** exit - executes when the splash state is left */
void Splash_State::exit(SDL_GameEngine * a_pvEngine) {
	//unload all splash screen resources
	a_pvEngine->unloadAll(RESOURCE_SPLASH);
}



//=================================================
//	MENU SCREEN
//=================================================
/** default constructor */
Menu_State::Menu_State() : m_uiCurrentMenu(0), m_uiMenuAmount(0), m_pvMenu(NULL) {
	m_pvPrevState = Splash_State::instance();
	m_pvNextState = Game_State::instance();
}

/** deconstructor */
Menu_State::~Menu_State() {
}

/** instance - returns singleton instance of menu state */
Menu_State * Menu_State::instance() {
	static Menu_State instance;
	return &instance;
}

/** enter - executes when the menu state is entered */
void Menu_State::enter(SDL_GameEngine * a_pvEngine) {
	//load all splash screen resources
	a_pvEngine->loadAll(RESOURCE_MENU);
	//set all menu state variables
	m_uiCurrentMenu = 0;
	m_uiMenuAmount = a_pvEngine->getResAmount(RESOURCE_MENU);
	m_pvMenu = (Menu *)a_pvEngine->getRes(RESOURCE_MENU, m_uiCurrentMenu);
	assert(m_pvMenu != NULL);
	m_pvMenu->setActive(true);
}

/** execute - executes during normal operation of menu state */
void Menu_State::execute(SDL_GameEngine * a_pvEngine) {
	bool bStateChange = false;
	//TODO: read input

	//TODO: check if mouse position hovers over menu button, and clicks it, and take appropriate action

	//if state needs to be changed
	if(bStateChange)
		//change it to the next state
		a_pvEngine->nextState();
}

/** exit - executes when the menu state is left */
void Menu_State::exit(SDL_GameEngine * a_pvEngine) {
	//unload all menu resources
	a_pvEngine->unloadAll(RESOURCE_MENU);
}



//=================================================
//	GAME STATE
//=================================================
/** default constructor */
Game_State::Game_State() : m_pvEngine(NULL), m_pvGameState(Load_State::instance()) {
	m_pvPrevState = Menu_State::instance();
	m_pvNextState = Menu_State::instance();
}

/** deconstructor */
Game_State::~Game_State() {
}

/** instance - returns singleton instance of game state */
Game_State * Game_State::instance() {
	static Game_State instance;
	return &instance;
}

/** enter - executes when the game state is entered */
void Game_State::enter(SDL_GameEngine * a_pvEngine) {
	m_pvEngine = a_pvEngine;
	m_pvGameState->enter(this);
}

/** execute - executes during normal operation of game state */
void Game_State::execute(SDL_GameEngine * a_pvEngine) {
	m_pvEngine = a_pvEngine;
	m_pvGameState->execute(this);
}

/** exit - executes when the game state is left */
void Game_State::exit(SDL_GameEngine * a_pvEngine) {
	m_pvEngine = a_pvEngine;
	m_pvGameState->exit(this);
}

/** getEngine - returns a reference to the game engine */
SDL_GameEngine * Game_State::getEngine() const {
	assert(m_pvEngine != NULL);
	return m_pvEngine;
}

/** changeState - changes the state of the game to the specified */
void Game_State::changeState(State<Game_State> * a_pvState) {
	assert(m_pvGameState && a_pvState);
	m_pvGameState->setNextState(a_pvState);
	a_pvState->setPrevState(m_pvGameState);
	m_pvGameState->exit(this);
	m_pvGameState = a_pvState;
	m_pvGameState->enter(this);
}

/** revertState - reverts the state of the game back to the last one */
void Game_State::revertState() {
	assert(m_pvGameState && m_pvGameState->getPrevState());
	State<Game_State> * pvPrevState = m_pvGameState->getPrevState();
	pvPrevState->setNextState(m_pvGameState);
	m_pvGameState->exit(this);
	m_pvGameState = pvPrevState;
	m_pvGameState->enter(this);
}
