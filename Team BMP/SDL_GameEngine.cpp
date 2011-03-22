#include "SDL_GameEngine.h"

/** Default Constructor */
SDL_GameEngine::SDL_GameEngine() : m_bIsRunning(false), m_bChange(false), m_usScreenWidth(0), m_usScreenHeight(0), m_uiMS(0), m_pvScreen(NULL), m_pvCurrentState(NULL) {
	m_vScreenRect.x = 0;
	m_vScreenRect.y = 0;
	m_vScreenRect.w = m_usScreenWidth;
	m_vScreenRect.h = m_usScreenHeight;
}

/** Constructor - sets screen surface and screen resolution the engine will use */
SDL_GameEngine::SDL_GameEngine(SDL_Surface * a_pvScreen, const Uint16 & a_krusScreenWidth, const Uint16 & a_krusScreenHeight) : m_bIsRunning(false), m_bChange(true), m_usScreenWidth(a_krusScreenWidth), m_usScreenHeight(a_krusScreenHeight), m_uiMS(0), m_pvScreen(a_pvScreen), m_pvCurrentState(NULL) {
	m_vScreenRect.x = 0;
	m_vScreenRect.y = 0;
	m_vScreenRect.w = m_usScreenWidth;
	m_vScreenRect.h = m_usScreenHeight;
}

/** Destructor */
SDL_GameEngine::~SDL_GameEngine() {
	unloadAll();
}




/** loadAll - loads all resources from a file according to the given resource ID */
bool SDL_GameEngine::loadAll(const Uint32 & a_uiID) {
	//TODO: load resources from files
	return true;
}

/** unloadAll (1) - removes all resources from memory */
void SDL_GameEngine::unloadAll() {
	//go through all resources
	for(Uint32 uiI = 0; uiI < (unsigned)m_vResources.size(); ++uiI)
		//release all resources from memory
		delete m_vResources.get(uiI);
	//release the list of resources
	m_vResources.release();
}

/** unloadAll (2) - removes all resources with the specified resource type from memory */
void SDL_GameEngine::unloadAll(const Uint32 & a_uiID) {
	//go through all resources
	for(Uint32 uiI = 0; uiI < (unsigned)m_vResources.size(); ++uiI) {
		//grab the current resource
		SDL_Resource * pvCur = m_vResources.get(uiI);
		//if it is of a specified resource type
		if(pvCur->getResID() == a_uiID) {
			//delete the resource
			delete pvCur;
			//and remove it from the resource list
			m_vResources.remove(uiI);
		}
	}
}

/** setScreenSurface - sets the screen surface the engine will use */
void SDL_GameEngine::setScreenSurface(SDL_Surface * a_pvScreen) {
	m_pvScreen = a_pvScreen;
	m_bChange = true;
}

/** setScreenDimensions - sets the screen width and height to the specified */
void SDL_GameEngine::setScreenDimensions(const Uint16 & a_krusScreenWidth, const Uint16 & a_krusScreenHeight) {
	m_usScreenWidth = a_krusScreenWidth;
	m_usScreenHeight = a_krusScreenHeight;
	m_vScreenRect.w = a_krusScreenWidth;
	m_vScreenRect.h = a_krusScreenHeight;
	m_bChange = true;
}

/** changeState - changes the game state to the given */
void SDL_GameEngine::nextState() {
	assert(m_pvCurrentState && m_pvCurrentState->getNextState());
	m_pvCurrentState->exit(this);
	m_pvCurrentState = m_pvCurrentState->getNextState();
	m_pvCurrentState->enter(this);
	m_bChange = true;
}

/** prevState - changes the game state to the last */
void SDL_GameEngine::prevState() {
	assert(m_pvCurrentState && m_pvCurrentState->getNextState());
	m_pvCurrentState->exit(this);
	m_pvCurrentState = m_pvCurrentState->getPrevState();
	m_pvCurrentState->enter(this);
	m_bChange = true;
}

/** quitGame - calls for the game to quit */
void SDL_GameEngine::quitGame() {
	m_bIsRunning = false;
}




/** isGameRunning - returns true if game is currently running */
const bool & SDL_GameEngine::isGameRunning() const {
	return m_bIsRunning;
}

/** hasChanged - returns true if game has changed */
const bool & SDL_GameEngine::hasChanged() const {
	return m_bChange;
}

/** getScreenSurface - returns the surface being drawn to */
SDL_Surface * SDL_GameEngine::getScreenSurface() const {
	return m_pvScreen;
}

/** getMSDelay - returns the time in ms since the last frame updated */
const Uint32 & SDL_GameEngine::getMSDelay() const {
	return m_uiMS;
}

/** getScreenWidth - returns the screen's width */
const Uint16 & SDL_GameEngine::getScreenWidth() const {
	return m_usScreenWidth;
}

/** getScreenHeight - returns the screen's height */
const Uint16 & SDL_GameEngine::getScreenHeight() const {
	return m_usScreenHeight;
}

/** getScreenRect - returns a rect the size of the screen */
const SDL_Rect & SDL_GameEngine::getScreenRect() const {
	return m_vScreenRect;
}

/** getRes - returns resource with given resource type and index */
const SDL_Resource * SDL_GameEngine::getRes(const Uint32 & a_uiID, const Uint32 & a_uiIndex) const {
	//hold variable ready for the index of resources found with the given type
	Uint32 uiIndex = 0;
	//go through all resources
	for(Uint32 uiI = 0; uiI < (unsigned)m_vResources.size(); ++uiI) {
		//get the current resource
		SDL_Resource * pvCur = m_vResources.get(uiI);
		//if the current res ID matches the specified ID
		if(pvCur->getResID() == a_uiID) {
			//and res ID index matches specified index
			if(uiIndex++ == a_uiIndex)
				//return the resource
				return pvCur;
		}
	}
	//return NULL if resource not found
	return NULL;
}

/** getResAmount (1) - returns the amount of overall resources loaded into memory */
const Uint32 SDL_GameEngine::getResAmount() const {
	return m_vResources.size();
}

/** getResAmount (2) - returns the amount of resources of a specified resource type */
const Uint32 SDL_GameEngine::getResAmount(const Uint32 & a_uiID) const {
	//hold variable ready for the amount of given resources for the specified type of resource
	Uint32 uiAmount = 0;
	//go through all resources
	for(Uint32 uiI = 0; uiI < (unsigned)m_vResources.size(); ++uiI) {
		//if res ID matches specified ID
		if(m_vResources.get(uiI)->getResID() == a_uiID)
			//increment the resource amount for that ID type
			++uiAmount;
	}
	//return the specified resource amount
	return uiAmount;
}




/** init - sets up all engine pre-requisites and prepares the engine to begin running */
bool SDL_GameEngine::init() {
	assert(m_pvScreen != NULL && m_usScreenWidth && m_usScreenHeight);
	m_pvCurrentState = Splash_State::instance();
	//TODO: init everything else
	m_pvCurrentState->enter(this);
	return true;
}

/** update - updates the game */
void SDL_GameEngine::update(const Uint32 & a_kruiMS) {
	//record change in time
	m_uiMS = a_kruiMS;
	//execute current state
	m_pvCurrentState->execute(this);
}

/** draw - draws to the screen */
void SDL_GameEngine::draw() {
	//if game has changed
	if(m_bChange) {
		//for all resources
		for(Uint32 uiI = 0; uiI < (unsigned)m_vResources.size(); ++uiI) {
			//get current resource
			SDL_Resource * pvCurRes = m_vResources.get(uiI);
			//if active
			if(pvCurRes->isActive()) {
				//check resource type
				switch(pvCurRes->getResID()) {
				//if unknown
				case RESOURCE_UNKNOWN:
					break;
				//if surface
				case RESOURCE_SURFACE:
					break;
				//if sprite
				case RESOURCE_SPRITE:
					break;
				//if splash screen
				case RESOURCE_SPLASH:
					break;
				//else
				default:
					break;
				}
			}
		}
		//flip the buffer
		SDL_Flip(m_pvScreen);
		//reset change to false
		m_bChange = false;
	}
}