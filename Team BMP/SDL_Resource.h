#ifndef _SDL_RESOURCE_H_
#define _SDL_RESOURCE_H_

#include "Global.h"
#include "sdl/SDL.h"

/*
Resource Container for SDL
Author: Jared Bills
*/
class SDL_Resource {

private:

	/** whether the given resource is active or not */
	bool m_bIsActive;
	/** resource ID */
	Uint32 m_uiResID;

protected:
	
	/** setResID - sets the resource ID to the specified */
	void setResID(const Uint32 & a_kruiID);

public:

	/** Default Constructor */
	SDL_Resource();
	/* Destructor */
	~SDL_Resource();

	/** setActive - sets resource as active or not, as specified */
	void setActive(const bool & a_krbActivate);

	/** isActive - returns true if current resource is active or not */
	const bool & isActive() const;
	/** getResID - returns the resource ID of the resource */
	const Uint32 & getResID() const;

	/** activate - activates the resource to be drawn to the screen or otherwise activated */
	virtual void activate() = 0;

};

#endif
