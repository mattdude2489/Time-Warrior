#include "SDL_Resource.h"

/** Default Constructor */
SDL_Resource::SDL_Resource() : m_uiResID(RESOURCE_UNKNOWN) {
}

/** Destructor */
SDL_Resource::~SDL_Resource() {
}




/** setResID - sets the resource ID to the specified */
void SDL_Resource::setResID(const Uint32 & a_kruiID) {
	m_uiResID = a_kruiID;
}

/** setChanged - sets the resource to be significantly changed or not */
void SDL_Resource::setChanged(const bool & a_krbChange) {
	m_bHasChanged = a_krbChange;
}




/** setActive - sets resource as active or not, as specified */
void SDL_Resource::setActive(const bool & a_krbActivate) {
	m_bIsActive = a_krbActivate;
}




/** isActive - returns true if current resource is active or not */
const bool & SDL_Resource::isActive() const {
	return m_bIsActive;
}

/** hasChanged - returns true if current resource has significantly changed since last drawn */
const bool & SDL_Resource::hasChanged() const {
	return m_bHasChanged;
}

/** getResID - returns the resource ID of the resource */
const Uint32 & SDL_Resource::getResID() const {
	return m_uiResID;
}