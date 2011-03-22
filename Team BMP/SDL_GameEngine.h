#ifndef _SDL_GAMEENGINE_H_
#define _SDL_GAMEENGINE_H_

//==========================
//Global Definitions
//==========================
#include "Global.h"

//==========================
//SDL
//==========================
#include "sdl/sdl.h"

//==========================
//SDL libraries
//==========================
#include "srect.h"
#include "spoint.h"

//==========================
//Engine libraries
//==========================
#include "userinput.h"
#include "State.h"
#include "EngineOwnedStates.h"
#include "SDL_Resource.h"
#include "Chip.h"
#include "Entity.h"
#include "Player.h"

//==========================
//Misc libraries
//==========================
#include "templatevector.h"
#include <assert.h>

/*
Game Engine for SDL
Author: Jared Bills
*/
class SDL_GameEngine {

private:

	/** is game running? */
	bool m_bIsRunning;
	/** has game changed considerably? */
	bool m_bChange;
	/** screen width */
	Uint16 m_usScreenWidth;
	/** screen height */
	Uint16 m_usScreenHeight;
	/** MS delay */
	Uint32 m_uiMS;
	/** list of loaded resources */
	TemplateVector<SDL_Resource *> m_vResources;
	/** screen rect */
	SDL_Rect m_vScreenRect;
	/** user input */
	UserInput m_vUI;
	/** screen surface */
	SDL_Surface * m_pvScreen;
	/** current state of the engine */
	State<SDL_GameEngine> * m_pvCurrentState;
	
public:

	/** Default Constructor */
	SDL_GameEngine();
	/** Constructor - sets screen surface and screen resolution the engine will use */
	SDL_GameEngine(SDL_Surface * a_pvScreen, const Uint16 & a_krusScreenWidth, const Uint16 & a_krusScreenHeight);
	/** Destructor */
	~SDL_GameEngine();

	/** loadAll - loads all resources from a file according to the given resource ID */
	bool loadAll(const Uint32 & a_uiID);
	/** unloadAll (1) - removes all resources from memory */
	void unloadAll();
	/** unloadAll (2) - removes all resources with the specified resource type from memory */
	void unloadAll(const Uint32 & a_uiID);
	/** setScreenSurface - sets the screen surface the engine will use */
	void setScreenSurface(SDL_Surface * a_pvScreen);
	/** setScreenDimensions - sets the screen width and height to the specified */
	void setScreenDimensions(const Uint16 & a_krusScreenWidth, const Uint16 & a_krusScreenHeight);
	/** changeState - changes the game state to the next */
	void nextState();
	/** prevState - changes the game state to the last */
	void prevState();
	/** quitGame - calls for the game to quit */
	void quitGame();
	/** isGameRunning - returns true if game is currently running */
	const bool & isGameRunning() const;
	/** hasChanged - returns true if game has changed */
	const bool & hasChanged() const;

	/** getScreenSurface - returns the surface being drawn to */
	SDL_Surface * getScreenSurface() const;
	/** getMSDelay - returns the time in ms since the last frame updated */
	const Uint32 & getMSDelay() const;
	/** getScreenWidth - returns the screen's width */
	const Uint16 & getScreenWidth() const;
	/** getScreenHeight - returns the screen's height */
	const Uint16 & getScreenHeight() const;
	/** getScreenRect - returns a rect the size of the screen */
	const SDL_Rect & getScreenRect() const;
	/** getRes - returns resource with given resource ID and index */
	const SDL_Resource * getRes(const Uint32 & a_uiID, const Uint32 & a_uiIndex) const;
	/** getResAmount (1) - returns the amount of overall resources loaded into memory */
	const Uint32 getResAmount() const;
	/** getResAmount (2) - returns the amount of resources of a specified resource type */
	const Uint32 getResAmount(const Uint32 & a_uiID) const;
	
	/** init - sets up all engine pre-requisites and prepares the engine to begin running */
	bool init();
	/** update - updates the game */
	void update(const Uint32 & a_kruiMS);
	/** draw - draws to the screen */
	void draw();

};

#endif
