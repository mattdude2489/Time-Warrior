#include "SDL_GUI.h"
#include "SDL_Sprite.h"

/** constructor (1) - loads a graphic from the specified file (also sets the GUI dimensions) */
SDL_GUI_Button::SDL_GUI_Button(const char * a_filename) {
	SDL_Surface * temp = SDL_LoadBMP(a_filename);
	m_pGraphic = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);
	assert(m_pGraphic != NULL);
	forceGUIDimensions(m_pGraphic->w, m_pGraphic->h);
	m_freeResources = true;
}

/** constructor (2) - loads a graphic from a specified surface (also sets the GUI dimensions) */
SDL_GUI_Button::SDL_GUI_Button(SDL_Surface * & a_pSurface) {
	assert(a_pSurface != NULL);
	m_pGraphic = a_pSurface;
	forceGUIDimensions(a_pSurface->w, a_pSurface->h);
}



/** loadGraphic (1) - loads a graphic from the specified file (also sets the GUI dimensions) */
const bool SDL_GUI_Button::loadGraphic(const char * a_filename) {
	bool successful;
	if(m_pGraphic != NULL && m_freeResources)
		SDL_FreeSurface(m_pGraphic);
	SDL_Surface * temp = SDL_LoadBMP(a_filename);
	m_pGraphic = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);
	if(successful = (m_pGraphic != NULL)) {
		forceGUIDimensions(m_pGraphic->w, m_pGraphic->h);
		m_freeResources = true;
	}
	return successful;
}

/** loadGraphic (2) - loads a graphic from a specified surface (also sets the GUI dimensions) */
void SDL_GUI_Button::loadGraphic(SDL_Surface * & a_pSurface) {
	assert(a_pSurface != NULL);
	if(m_pGraphic != NULL && m_freeResources)
		SDL_FreeSurface(m_pGraphic);
	m_pGraphic = a_pSurface;
	forceGUIDimensions(a_pSurface->w, a_pSurface->h);
}



/** onClick() - when the specified mouse clicks on the object */
void SDL_GUI_Button::onClick(const Uint32 & a_mX, const Uint32 & a_mY) {
	 assert(m_action.isValid());
	 m_action.callFunc();
}
/** onHover() - when the specified mouse hovers over the object */
void SDL_GUI_Button::onHover(const Uint32 & a_mX, const Uint32 & a_mY) {}

/** onRollOver() - when the specified mouse rolls onto the object */
void SDL_GUI_Button::onRollOver(const Uint32 & a_mX, const Uint32 & a_mY) {
	for(Sint32 y = 0; y < m_pGraphic->h; ++y)
		for(Sint32 x = 0; x < m_pGraphic->w; ++x)
			SDL_Sprite::setPixel(m_pGraphic, x, y, SDL_Sprite::getPixel(m_pGraphic, x, y) + HIGHLIGHT_COL_OFF);
}

/** onInactive() - when the specified mouse does not hover over the object */
void SDL_GUI_Button::onInactive(const Uint32 & a_mX, const Uint32 & a_mY) {}

/** onRollOff() - when the specified mouse rolls off of the object */
void SDL_GUI_Button::onRollOff(const Uint32 & a_mX, const Uint32 & a_mY) {
	for(Sint32 y = 0; y < m_pGraphic->h; ++y)
		for(Sint32 x = 0; x < m_pGraphic->w; ++x)
			SDL_Sprite::setPixel(m_pGraphic, x, y, SDL_Sprite::getPixel(m_pGraphic, x, y) - HIGHLIGHT_COL_OFF);
}

/** draw() - draws the object to the screen */
void SDL_GUI_Button::draw() {
	//record border rect
	SDL_Rect borderRect = { m_hitbox.x - PADDING_PX - BORDER_PX, m_hitbox.y - PADDING_PX - BORDER_PX, m_hitbox.w + ((PADDING_PX + BORDER_PX) << 1), m_hitbox.h + ((PADDING_PX + BORDER_PX) << 1) };
	//record background rect
	SDL_Rect paddingRect = { m_hitbox.x - PADDING_PX, m_hitbox.y - PADDING_PX, m_hitbox.w + (PADDING_PX << 1), m_hitbox.h + (PADDING_PX << 1) };
	//fill the border with the border color
	SDL_FillRect(m_pScreen, &borderRect, BORDER_COL);
	//fill the background with the background color
	SDL_FillRect(m_pScreen, &paddingRect, BACKGROUND_COL);
	//create src rect for surface
	SDL_Rect src = { 0, 0, m_hitbox.w, m_hitbox.h };
	//create dst rect with button's position on the screen
	SDL_Rect dst = { m_hitbox.x, m_hitbox.y, 0, 0 };
	//draw the graphic on the screen
	SDL_BlitSurface(m_pGraphic, &src, m_pScreen, &dst);
}
