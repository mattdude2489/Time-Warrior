#ifndef _SDL_GUI_H_
#define _SDL_GUI_H_

#include "Global.h"
#include "srect.h"
#include "sdl/SDL.h"
#include "sdl/SDL_ttf.h"
#include "function.h"

#include <assert.h>

/*
* Class: SDL GUI Object
* Author: Jared Bills
* Info: Abstract GUI Object class (using SDL) managed by passed-in screen surface info.
*/
class SDL_GUI_Object {
protected:
	/** whether the object needs to have resources freed or not */
	bool m_freeResources;
	/** object dimensions */
	SRect m_hitbox;
	/** screen dimensions */
	SRect m_screenbox;
	/** screen reference */
	SDL_Surface * m_pScreen;
public:
	/** default constructor */
	inline SDL_GUI_Object() : m_pScreen(NULL) { m_screenbox.x = 0; m_screenbox.y = 0; m_screenbox.w = 0; m_screenbox.h = 0; m_hitbox.x = 0; m_hitbox.y = 0; m_hitbox.w = 0; m_hitbox.h = 0; }
	/** destructor */
	inline ~SDL_GUI_Object() {}
	/** constructor (1) - sets the screen to the specified */
	inline SDL_GUI_Object(SDL_Surface * & a_pScreen) : m_freeResources(false), m_pScreen(a_pScreen) { assert(m_pScreen != NULL); m_screenbox.x = 0; m_screenbox.y = 0; m_screenbox.w = m_pScreen->w; m_screenbox.h = m_pScreen->h; m_hitbox.x = 0; m_hitbox.y = 0; m_hitbox.w = 0; m_hitbox.h = 0; }
	/** constructor (2) - sets the screen and object dimensions to the specified */
	inline SDL_GUI_Object(SDL_Surface * & a_pScreen, const Uint32 & a_xDim, const Uint32 & a_yDim) : m_freeResources(false), m_pScreen(a_pScreen) { assert(m_pScreen != NULL && a_xDim && a_yDim); m_screenbox.x = 0; m_screenbox.y = 0; m_screenbox.w = m_pScreen->w; m_screenbox.h = m_pScreen->h; m_hitbox.x = 0; m_hitbox.y = 0; m_hitbox.w = a_xDim - MIN(GUI_DIM_OFF, a_xDim); m_hitbox.h = a_yDim - MIN(GUI_DIM_OFF, a_yDim); }
	/** constructor (3) - sets the screen and object coords and dimensions to the specified */
	inline SDL_GUI_Object(SDL_Surface * & a_pScreen, const Sint32 & a_xPos, const Sint32 & a_yPos, const Uint32 & a_xDim, const Uint32 & a_yDim) : m_freeResources(false), m_pScreen(a_pScreen) { assert(m_pScreen != NULL && a_xDim && a_yDim); m_screenbox.x = 0; m_screenbox.y = 0; m_screenbox.w = m_pScreen->w; m_screenbox.h = m_pScreen->h; m_hitbox.x = a_xPos + GUI_POS_OFF; m_hitbox.y = a_yPos + GUI_POS_OFF; m_hitbox.w = a_xDim - MIN(GUI_DIM_OFF, a_xDim); m_hitbox.h = a_yDim - MIN(GUI_DIM_OFF, a_yDim); }

	/** setScreen - sets the screen to the specified */
	inline void setScreen(SDL_Surface * & a_pScreen) { assert(a_pScreen != NULL); m_pScreen = a_pScreen; m_screenbox.w = m_pScreen->w; m_screenbox.h = m_pScreen->h; }
	/** setGUIPos - sets the object position with GUI offsets */
	inline void setGUIPos(const Sint32 & a_x, const Sint32 & a_y) { m_hitbox.x = a_x + GUI_POS_OFF; m_hitbox.y = a_y + GUI_POS_OFF; }
	/** setGUIDimensions - sets the object dimensions with GUI offsets */
	inline void setGUIDimensions(const Uint32 & a_x, const Uint32 & a_y) { m_hitbox.w = a_x - MIN(GUI_DIM_OFF, a_x); m_hitbox.h = a_y - MIN(GUI_DIM_OFF, a_y); }
	/** setGUI - sets the object position and dimensions with GUI offsets */
	inline void setGUI(const Sint32 & a_x, const Sint32 & a_y, const Uint32 & a_w, const Uint32 & a_h) { setGUIPos(a_x, a_y); setGUIDimensions(a_w, a_h); }
	/** forceGUIPos - directly sets the object position to the specified */
	inline void forceGUIPos(const Sint32 & a_x, const Sint32 & a_y) { m_hitbox.x = a_x; m_hitbox.y = a_y; }
	/** forceGUIDimensions - directly sets the object dimensions to the specified */
	inline void forceGUIDimensions(const Uint32 & a_x, const Uint32 & a_y) { m_hitbox.w = a_x; m_hitbox.h = a_y; }
	/** forceGUI - directly sets the object position and dimensions to the specified */
	inline void forceGUI(const Sint32 & a_x, const Sint32 & a_y, const Uint32 & a_w, const Uint32 & a_h) { forceGUIPos(a_x, a_y); forceGUIDimensions(a_w, a_h); }

	/** getGUI - retuns the GUI object's dimensions */
	inline const SRect & getGUI() const { return m_hitbox; }
	/** mouseCollide - returns true if specified mouse coordinate lies within object dimensions */
	inline const bool mouseCollide(const Uint32 & a_mX, const Uint32 & a_mY) { assert(a_mX < m_screenbox.w && a_mY < m_screenbox.h); return ( ( (signed) a_mX < m_hitbox.w && (signed) a_mX >= m_hitbox.x ) && ( (signed) a_mY < m_hitbox.h && (signed) a_mY >= m_hitbox.y ) ); }
	/** mouseOnEdge - returns true if the mouse lies on the edge of the object boundries */
	inline const bool mouseOnEdge(const Uint32 & a_mX, const Uint32 & a_mY) { assert(a_mX < m_screenbox.w && a_mY < m_screenbox.h); return ( ( ( (signed) a_mX == m_hitbox.x || (signed) a_mX == m_hitbox.x - 1 ) || ( (signed) a_mX == m_hitbox.w - 1 || (signed) a_mX == m_hitbox.w ) ) || ( ( (signed) a_mY == m_hitbox.y || (signed) a_mY == m_hitbox.y - 1 ) || ( (signed) a_mY == m_hitbox.h - 1 || (signed) a_mY == m_hitbox.h ) ) ); }
	
	/** onClick() - when the specified mouse clicks on the object */
	virtual void onClick(const Uint32 & a_mX, const Uint32 & a_mY) = 0;
	/** onRollOver() - when the specified mouse rolls onto the object */
	virtual void onRollOver(const Uint32 & a_mX, const Uint32 & a_mY) = 0;
	/** onHover() - when the specified mouse hovers over the object */
	virtual void onHover(const Uint32 & a_mX, const Uint32 & a_mY) = 0;
	/** onRollOff() - when the specified mouse rolls off of the object */
	virtual void onRollOff(const Uint32 & a_mX, const Uint32 & a_mY) = 0;
	/** onInactive() - when the specified mouse does not hover over the object */
	virtual void onInactive(const Uint32 & a_mX, const Uint32 & a_mY) = 0;
	/** draw() - draws the object to the screen */
	virtual void draw() = 0;
};

/*
* Class: SDL GUI Button
* Author: Jared Bills
* Info: Activates functionality based on a click, and contains graphic.
*/
class SDL_GUI_Button : public SDL_GUI_Object {
protected:
	/** function the button will call when clicked on (with c-string param) */
	Function m_action;
	/** button's graphic */
	SDL_Surface * m_pGraphic;
public:
	/** default constructor */
	inline SDL_GUI_Button() : m_pGraphic(NULL) {}
	/** destructor */
	inline ~SDL_GUI_Button() { if(m_pGraphic != NULL && m_freeResources) SDL_FreeSurface(m_pGraphic); }
	/** constructor (1) - loads a graphic from the specified file (also sets the GUI dimensions) */
	SDL_GUI_Button(const char * a_filename);
	/** constructor (2) - loads a graphic from a specified surface (also sets the GUI dimensions) */
	SDL_GUI_Button(SDL_Surface * & a_pSurface);
	
	/** loadGraphic (1) - loads a graphic from the specified file (also sets the GUI dimensions) */
	const bool loadGraphic(const char * a_filename);
	/** loadGraphic (2) - loads a graphic from a specified surface (also sets the GUI dimensions) */
	void loadGraphic(SDL_Surface * & a_pSurface);
	/** setClickAction - sets the button's clicking action function */
	inline void setClickAction(const Function & a_action) { assert(a_action.isValid()); m_action = a_action; }

	/** getGraphic - returns the button's graphic */
	inline const SDL_Surface * getGraphic() const { return m_pGraphic; }
	/** getClickAction - returns the button's clicking action function */
	inline const Function & getClickAction() const { return m_action; }

	/** onClick() - when the specified mouse clicks on the object */
	virtual void onClick(const Uint32 & a_mX, const Uint32 & a_mY);
	/** onRollOver() - when the specified mouse rolls onto the object */
	virtual void onRollOver(const Uint32 & a_mX, const Uint32 & a_mY);
	/** onHover() - when the specified mouse hovers over the object */
	virtual void onHover(const Uint32 & a_mX, const Uint32 & a_mY);
	/** onRollOff() - when the specified mouse rolls off of the object */
	virtual void onRollOff(const Uint32 & a_mX, const Uint32 & a_mY);
	/** onInactive() - when the specified mouse does not hover over the object */
	virtual void onInactive(const Uint32 & a_mX, const Uint32 & a_mY);
	/** draw() - draws the object to the screen */
	virtual void draw();
};

#endif
