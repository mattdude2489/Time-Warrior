#include "SDL_Sprite.h"

/** Default Constructor */
SDL_Sprite::SDL_Sprite() : m_bExternalSurfaceManagement(false), m_bRunning(false), m_bLoopsToBeginning(false), m_uiTimer(0), m_uiSpeed(0), m_uiWidth(0), m_uiHeight(0), m_uiRIndex(0), m_uiCIndex(0), m_uiMaxRows(0), m_iIndexIterator(1), m_prguiMaxFrames(NULL), m_pvSurface(NULL) {
}

/** Constructor (1) - load in sprite from file with properties */
SDL_Sprite::SDL_Sprite(char * a_szFileName, const Uint32 & a_kruiFrameWidth, const Uint32 & a_kruiFrameHeight, const Uint32 & a_kruiAnimSpeed, const Uint32 & a_kruiRows) : m_bExternalSurfaceManagement(false), m_bRunning(false), m_bLoopsToBeginning(false), m_uiTimer(0), m_uiSpeed(a_kruiAnimSpeed), m_uiWidth(a_kruiFrameWidth), m_uiHeight(a_kruiFrameHeight), m_uiRIndex(0), m_uiCIndex(0), m_uiMaxRows(a_kruiRows), m_iIndexIterator(1), m_prguiMaxFrames(NULL), m_pvSurface(NULL) {
	//create temp surface to hold loaded bitmap data
	SDL_Surface * pvTemp = SDL_LoadBMP(a_szFileName);
	//set sprite's surface to display format for temp surface
    m_pvSurface = SDL_DisplayFormat(pvTemp);
	//free the temp surface
    SDL_FreeSurface(pvTemp);
	//if sprite's surface is not valid
	if(m_pvSurface == NULL) {
		//reset settings back to default
		m_uiWidth = 0;
		m_uiHeight = 0;
		m_uiSpeed = 0;
		m_uiMaxRows = 0;
	}
	//else if sprite's surface is valid
	else {
		//create new max frame data
		m_prguiMaxFrames = new Uint32 [m_uiMaxRows];
		//record max frames as surface width divided by frame width
		Uint32 uiMaxFrames = m_pvSurface->w/m_uiWidth;
		//for all rows
		for(Uint32 uiI = 0; uiI < m_uiMaxRows; ++uiI)
			//set max frames
			m_prguiMaxFrames[uiI] = uiMaxFrames;
	}
}

/** Constructor (2) - load in sprite from copied surface with properties */
SDL_Sprite::SDL_Sprite(SDL_Surface * a_pvSurface, const Uint32 & a_kruiFrameWidth, const Uint32 & a_kruiFrameHeight, const Uint32 & a_kruiAnimSpeed, const Uint32 & a_kruiRows) : m_bExternalSurfaceManagement(false), m_bRunning(false), m_bLoopsToBeginning(false), m_uiTimer(0), m_uiSpeed(a_kruiAnimSpeed), m_uiWidth(a_kruiFrameWidth), m_uiHeight(a_kruiFrameHeight), m_uiRIndex(0), m_uiCIndex(0), m_uiMaxRows(a_kruiRows), m_iIndexIterator(1), m_prguiMaxFrames(NULL), m_pvSurface(NULL) {
	//if surface is valid
	if(a_pvSurface != NULL) {
		//gather the surface's pixel format
		SDL_PixelFormat * pvF = a_pvSurface->format;
		//store color key flag
		Uint32 uiColorKeyFlag = (a_pvSurface->flags & SDL_SRCCOLORKEY);
		// create a new surface for sprite
		m_pvSurface = SDL_CreateRGBSurfaceFrom(
			a_pvSurface->pixels, a_pvSurface->w, a_pvSurface->h, pvF->BitsPerPixel,
			a_pvSurface->pitch, pvF->Rmask, pvF->Gmask, pvF->Bmask,
			(uiColorKeyFlag?0:pvF->Amask)
			);
		//if color key is flagged
		if(uiColorKeyFlag)
			//copy color key from surface to sprite's surface
			SDL_SetColorKey(m_pvSurface, SDL_RLEACCEL|SDL_SRCCOLORKEY, pvF->colorkey);
	}
	//if sprite's surface is not valid
	if(m_pvSurface == NULL) {
		//reset settings back to default
		m_uiWidth = 0;
		m_uiHeight = 0;
		m_uiSpeed = 0;
		m_uiMaxRows = 0;
	}
	//else if sprite's surface is valid
	else {
		//create new max frame data
		m_prguiMaxFrames = new Uint32 [m_uiMaxRows];
		//record max frames as surface width divided by frame width
		Uint32 uiMaxFrames = m_pvSurface->w/m_uiWidth;
		//for all rows
		for(Uint32 uiI = 0; uiI < m_uiMaxRows; ++uiI)
			//set max frames
			m_prguiMaxFrames[uiI] = uiMaxFrames;
	}
}

/** Destructor */
SDL_Sprite::~SDL_Sprite() {
	//Free the sprite's resources
	destroy();
}




/** setSprite (1) - load in new sprite from file with properties */
void SDL_Sprite::setSprite(char * a_szFileName, const Uint32 & a_kruiFrameWidth, const Uint32 & a_kruiFrameHeight, const Uint32 & a_kruiAnimSpeed, const Uint32 & a_kruiRows) {
	//if max frame data still exists
	if(m_prguiMaxFrames != NULL) {
		//free the data
		delete [] m_prguiMaxFrames;
		//and nullify the data pointer
		m_prguiMaxFrames = NULL;
	}
	//create temp surface to hold loaded bitmap data
	SDL_Surface * pvTemp = SDL_LoadBMP(a_szFileName);
	//if sprite's surface still exists
	if(m_pvSurface != NULL && !m_bExternalSurfaceManagement)
		//free the surface
		SDL_FreeSurface(m_pvSurface);
	//set sprite's surface to display format for temp surface
    m_pvSurface = SDL_DisplayFormat(pvTemp);
	//free the temp surface
    SDL_FreeSurface(pvTemp);
	//if sprite's surface is not valid
    if(m_pvSurface == NULL) {
		//reset settings back to default
        m_uiWidth = 0;
        m_uiHeight = 0;
        m_uiSpeed = 0;
		m_uiMaxRows = 0;
    }
	//else if sprite's surface is valid
	else {
		//set settings to the specified
		m_uiWidth = a_kruiFrameWidth;
		m_uiHeight = a_kruiFrameHeight;
        m_uiSpeed = a_kruiAnimSpeed;
		m_uiMaxRows = a_kruiRows;
		//create new max frame data
		m_prguiMaxFrames = new Uint32 [m_uiMaxRows];
		//record max frames as surface width divided by frame width
		Uint32 uiMaxFrames = m_pvSurface->w/m_uiWidth;
		//for all rows
		for(Uint32 uiI = 0; uiI < m_uiMaxRows; ++uiI)
			//set max frames
			m_prguiMaxFrames[uiI] = uiMaxFrames;
    }
	//set surface as not externally managed
	m_bExternalSurfaceManagement = false;
	//set sprite to not running
	m_bRunning = false;
	//set sprite to not loop to beginning
	m_bLoopsToBeginning = false;
	//restart the sprite animation
	restart();
}

/** setSprite (2) - load in new sprite from copied surface with properties */
void SDL_Sprite::setSprite(SDL_Surface * a_pvSurface, const Uint32 & a_kruiFrameWidth, const Uint32 & a_kruiFrameHeight, const Uint32 & a_kruiAnimSpeed, const Uint32 & a_kruiRows) {
	//if max frame data still exists
	if(m_prguiMaxFrames != NULL) {
		//free the data
		delete [] m_prguiMaxFrames;
		//and nullify the data pointer
		m_prguiMaxFrames = NULL;
	}
	//if surface is valid
	if(a_pvSurface != NULL) {
		//gather the surface's pixel format
		SDL_PixelFormat * pvF = a_pvSurface->format;
		//store color key flag
		Uint32 uiColorKeyFlag = (a_pvSurface->flags & SDL_SRCCOLORKEY);
		//if sprite's surface still exists
		if(m_pvSurface != NULL && !m_bExternalSurfaceManagement)
			//free the surface
			SDL_FreeSurface(m_pvSurface);
		// create a new surface for sprite
		m_pvSurface = SDL_CreateRGBSurfaceFrom(
			a_pvSurface->pixels, a_pvSurface->w, a_pvSurface->h, pvF->BitsPerPixel,
			a_pvSurface->pitch, pvF->Rmask, pvF->Gmask, pvF->Bmask,
			(uiColorKeyFlag?0:pvF->Amask)
			);
		//if color key is flagged
		if(uiColorKeyFlag)
			//copy color key from surface to sprite's surface
			SDL_SetColorKey(m_pvSurface, SDL_RLEACCEL|SDL_SRCCOLORKEY, pvF->colorkey);
	}
	//if sprite's surface is not valid
    if(m_pvSurface == NULL) {
		//reset settings back to default
        m_uiWidth = 0;
        m_uiHeight = 0;
        m_uiSpeed = 0;
		m_uiMaxRows = 0;
    }
	//else if sprite's surface is valid
	else {
		//set settings to the specified
		m_uiWidth = a_kruiFrameWidth;
		m_uiHeight = a_kruiFrameHeight;
        m_uiSpeed = a_kruiAnimSpeed;
		m_uiMaxRows = a_kruiRows;
		//create new max frame data
		m_prguiMaxFrames = new Uint32 [m_uiMaxRows];
		//record max frames as surface width divided by frame width
		Uint32 uiMaxFrames = m_pvSurface->w/m_uiWidth;
		//for all rows
		for(Uint32 uiI = 0; uiI < m_uiMaxRows; ++uiI)
			//set max frames
			m_prguiMaxFrames[uiI] = uiMaxFrames;
    }
	//set surface as not externally managed
	m_bExternalSurfaceManagement = false;
	//set sprite to not running
	m_bRunning = false;
	//set sprite to not loop to beginning
	m_bLoopsToBeginning = false;
	//restart the sprite animation
	restart();
}

/** start - starts the sprite animation */
void SDL_Sprite::start() {
	m_bRunning = true;
}

/** stop - stops the sprite animation */
void SDL_Sprite::stop() {
	m_bRunning = false;
}

/** restart (1) - restarts the sprite animation */
void SDL_Sprite::restart() {
	m_uiTimer = 0;
	m_uiRIndex = 0;
	m_uiCIndex = 0;
	m_iIndexIterator = 1;
}

/** restart (2) - restarts the sprite animation at a given row */
const bool SDL_Sprite::restart(const Uint32 & a_kruiRIndex) {
	bool bValid = a_kruiRIndex < m_uiMaxRows;
	m_uiTimer = 0;
	m_uiRIndex = (bValid?a_kruiRIndex:0);
	m_uiCIndex = 0;
	m_iIndexIterator = 1;
	return bValid;
}

/** update - updates the sprite based on the change in time */
void SDL_Sprite::update(const Uint32 & a_kruiMSPassed) {
	//if sprite is running
	if(m_bRunning)
	{
		//update timer with ms passed
		m_uiTimer += a_kruiMSPassed;
		//if time passed is greater than speed of frame transition
		if(m_uiTimer > m_uiSpeed)
		{
			//decrement timer by speed
			m_uiTimer -= m_uiSpeed;
			//iterate to the next frame
			m_uiCIndex += m_iIndexIterator;
			//record the max frames for the current row
			Uint32 maxFrames = m_prguiMaxFrames[m_uiRIndex];
			//if current frame index is out of bounds
			if(!(m_uiCIndex < maxFrames)) {
				//if this frame is before the first frame
				if(m_uiCIndex & (SIGNED_32)) {
					//if sprite doesn't loop to the beginning
					if(!m_bLoopsToBeginning) {
						//set the frame index to the first frame
						m_uiCIndex = 0;
						//change the sprite to animate forward
						m_iIndexIterator = -m_iIndexIterator;
					}
					//else if sprite loops to the beginning
					else
						//set the frame index to the last frame
						m_uiCIndex = maxFrames - 1;
				}
				//else if this frame is after the last frame
				else {
					//if sprite doesn't loop to the beginning
					if(!m_bLoopsToBeginning) {
						//set the frame index to the last frame
						m_uiCIndex = maxFrames - 1;
						//change the sprite to animate backwards
						m_iIndexIterator = -m_iIndexIterator;
					}
					//else if sprite loops to the beginning
					else
						//set the frame index to the first frame
						m_uiCIndex = 0;
				}
			}
		}
	}
}

/** draw - draws the sprite to the screen at given x and y coords */
void SDL_Sprite::draw(SDL_Surface * a_pvScreenSurface, const Sint32 & a_kriX, const Sint32& a_kriY) {
	//create a temp source rect the size of the sprite frame to read from the sprite surface
	SDL_Rect vSrcRect = { m_uiCIndex*m_uiWidth, m_uiRIndex*m_uiHeight, m_uiWidth, m_uiHeight };
	//create a temp destination rect with the location for which to draw to the screen with
	SDL_Rect vDstRect = { a_kriX, a_kriY, 0, 0 };
	//blit the sprite surface onto the screen surface, using the source and destination rects
	SDL_BlitSurface(m_pvSurface, &vSrcRect, a_pvScreenSurface, &vDstRect);
}

/** destroy - frees sprite's resources */
void SDL_Sprite::destroy() {
	//if sprite's surface exists
	if(m_pvSurface != NULL && !m_bExternalSurfaceManagement) {
		//free the surface
		SDL_FreeSurface(m_pvSurface);
		//and nullify the surface pointer
		m_pvSurface = NULL;
	}
	//if max frame data still exists
	if(m_prguiMaxFrames != NULL) {
		//free the data
		delete [] m_prguiMaxFrames;
		//and nullify the data pointer
		m_prguiMaxFrames = NULL;
	}
}

/** setLoopToBegin - sets animation to loop from beginning or not */
void SDL_Sprite::setLoopToBegin(const bool & a_krbLoop) {
	m_bLoopsToBeginning = a_krbLoop;
}

void SDL_Sprite::setMaxRows(const Uint32 & a_kruiRows) {
	//set new amount of max rows
	m_uiMaxRows = a_kruiRows;
	//if max frame data exists
	if(m_prguiMaxFrames != NULL)
		//free the data
		delete [] m_prguiMaxFrames;
	//create new max frame data
	m_prguiMaxFrames = new Uint32 [m_uiMaxRows];
	//set the max frames to sprite width divided by frame width
	setMaxFrames(m_pvSurface->w/m_uiWidth);
}

/** setFrame - sets the frame dimensions to the specified */
void SDL_Sprite::setFrame(const Uint32 & a_kruiWidth, const Uint32 & a_kruiHeight) {
	//set frame width
	m_uiWidth = a_kruiWidth;
	//set frame height
	m_uiHeight = a_kruiHeight;
	//if surface exists
	if(m_pvSurface)
		//set max frames to surface width divided by frame width
		setMaxFrames(m_pvSurface->w/m_uiWidth);
}

/** setMaxFrames (1) - set maximum number of frames for all rows */
void SDL_Sprite::setMaxFrames(const Uint32 & a_kruiMaxFrames) {
	//for all rows
	for(Uint32 uiI = 0; uiI < m_uiMaxRows; ++uiI)
		//set max frames
		m_prguiMaxFrames[uiI] = a_kruiMaxFrames;
}

/** setMaxFrames (2) - set maximum number of frames for specific row */
const bool SDL_Sprite::setMaxFrames(const Uint32 & a_kruiRIndex, const Uint32 & a_kruiMaxFrames) {
	//record boolean for out of bounds check
	bool bValid = a_kruiRIndex < m_uiMaxRows;
	//if valid
	if(bValid)
		//set max frames for the specified row index
		m_prguiMaxFrames[a_kruiRIndex] = a_kruiMaxFrames;
	//return validity check
	return bValid;
}

/** setRIndex - set sprite to use new animation row */
void SDL_Sprite::setRIndex(const Uint32 & a_kruiRIndex) {
	//set row index to specified
	m_uiRIndex = a_kruiRIndex;
}

/** setTransparency (1) - sets color keycode to correlate to transparency for sprite */
void SDL_Sprite::setTransparency(const Uint32 & a_kruiColorKey) {
	//set the color key for transparency on the sprite's surface
	SDL_SetColorKey(m_pvSurface, SDL_SRCCOLORKEY, a_kruiColorKey);
}

/** setTransparency (2) - sets color keycode to correlate to transparency for sprite */
void SDL_Sprite::setTransparency(const Uint8 & a_krbyRed, const Uint8 & a_krbyGreen, const Uint8 & a_krbyBlue) {
	//set the color key for transparency on the sprite's surface
	SDL_SetColorKey(m_pvSurface, SDL_SRCCOLORKEY, SDL_MapRGB(m_pvSurface->format, a_krbyRed, a_krbyGreen, a_krbyBlue));
}

/** setSpeed - sets the animation speed of the sprite */
void SDL_Sprite::setSpeed(const Uint32 & a_kruiAnimSpeed) {
	m_uiSpeed = a_kruiAnimSpeed;
}

/** setIndexIterator - change direction and skip amount for frame iteration during animation */
void SDL_Sprite::setFrameIterator(const Sint32 & a_kriFrameIterator) {
	m_iIndexIterator = a_kriFrameIterator;
}

/** setSurface - sets the sprite's surface to a new one, but needs to know if surface will be managed outside sprite class */
void SDL_Sprite::setSurface(SDL_Surface * a_pvSurface, const bool & a_krbExternalManagement) {
	//if sprite's surface still exists
	if(m_pvSurface != NULL)
		//free any current sprite resources along with the surface
		destroy();
	//make sprite's surface equal to the new surface
	m_pvSurface = a_pvSurface;
	//set external surface management to specified
	m_bExternalSurfaceManagement = a_krbExternalManagement;
}

/** running - returns true if sprite is started */
const bool & SDL_Sprite::running() const {
	//return boolean for whether sprite is running
	return m_bRunning;
}

/** looping - returns true if sprite is looping to beginning */
const bool & SDL_Sprite::looping() const {
	//return boolean for whether sprite is looping
	return m_bLoopsToBeginning;
}

/** isSprite - returns true if sprite is valid */
const bool SDL_Sprite::isSprite() const {
	//sprite is valid if has valid surface with width and height, and valid frame width and height
	return m_pvSurface && m_uiWidth > 0 && m_uiHeight > 0 && m_pvSurface->w > 0 && m_pvSurface->h > 0;
}

/** getSpeed - returns the animation speed */
const Uint32 & SDL_Sprite::getSpeed() const {
	//return animation speed
	return m_uiSpeed;
}

/** getRow - returns row index */
const Uint32 & SDL_Sprite::getRow() const {
	//return row index
	return m_uiRIndex;
}

/** getFrame - returns frame index */
const Uint32 & SDL_Sprite::getFrame() const {
	//return frame index
	return m_uiCIndex;
}

/** getWidth - returns width of frame */
const Uint32 & SDL_Sprite::getWidth() const {
	//return frame width
	return m_uiWidth;
}

/** getHeight - return height of frame */
const Uint32 & SDL_Sprite::getHeight() const {
	//return frame height
	return m_uiHeight;
}
/** getSpriteWidth - return width of sprite's surface */
const Uint32 SDL_Sprite::getSpriteWidth() const {
	//return surface width
	return m_pvSurface->w;
}

/** getSpriteHeight - return height of sprite's surface */
const Uint32 SDL_Sprite::getSpriteHeight() const {
	//return surface height
	return m_pvSurface->h;
}

/** getMaxRows - return number of rows */
const Uint32 & SDL_Sprite::getMaxRows() const {
	//return max rows
	return m_uiMaxRows;
}

/** getMaxFrames (1) - return number of frames at a row with highest number of frames */
const Uint32 SDL_Sprite::getMaxFrames() const {
	//record max frames of first row as best thus far
	Uint32 uiMaxFrames = m_prguiMaxFrames[0];
	//go through all rows
	for(Uint32 uiI = 1; uiI < m_uiMaxRows; ++uiI) {
		//if current row has more max frames than last best
		if(m_prguiMaxFrames[uiI] > uiMaxFrames)
			//store current row's max frames as best
			uiMaxFrames = m_prguiMaxFrames[uiI];
	}
	//return the best case for highest number of max frames
	return uiMaxFrames;
}

/** getMaxFrames (2) - return number of frames at a specified row */
const Uint32 SDL_Sprite::getMaxFrames(const Uint32 & a_kruiRIndex) const {
	//record max frames as error code first off
	Uint32 uiMaxFrames = UINT32_ERROR_OOB;
	//if row index is not out of bounds
	if(a_kruiRIndex < m_uiMaxRows)
		//set max frames at specified row index
		uiMaxFrames = m_prguiMaxFrames[a_kruiRIndex];
	//return max frames
	return uiMaxFrames;
}

/** equals - returns true if the sprite's surface is the same as this one's in memory */
const bool SDL_Sprite::equals(const SDL_Sprite & a_krvCompare) const {
	//return true if sprite's surface in memory is the same as this one's
	return m_pvSurface == a_krvCompare.getSurface();
}

/** getSurface - returns the sprite's surface */
SDL_Surface * SDL_Sprite::getSurface() const {
	//return the pointer to the surface
	return m_pvSurface;
}




/** isTransparentPixel - returns true if the requested pixel is transparent on the sprite */
const bool SDL_Sprite::isTransparentPixel(const Sint32 & a_kriX, const Sint32 & a_kriY) const {
	//record pixel color as specified
	Uint32 uiPixelColor = getPixel(a_kriX, a_kriY);
	//return true if pixel color is equal to color key
	return (uiPixelColor == m_pvSurface->format->colorkey);
}

/** setTransparentPixel - sets a requested pixel's color to be the keycode color for transparency */
void SDL_Sprite::setTransparentPixel(const Sint32 & a_kriX, const Sint32 & a_kriY) {
	//sets the given pixel to transparent
	setPixel(a_kriX, a_kriY, m_pvSurface->format->colorkey);
}

/** getPixel - returns color data for requested pixel */
Uint32 SDL_Sprite::getPixel(const Sint32 & a_kriX, const Sint32 & a_kriY) const {
	//return the requested pixel from the sprite's surface
	return getPixel(m_pvSurface, a_kriX, a_kriY);
}

/** setPixel (1) - sets a requested pixel's color data */
void SDL_Sprite::setPixel(const Sint32 & a_kriX, const Sint32 & a_kriY, const Uint32 & a_kruiColor) {
	//set the requested pixel on the sprite's surface
	setPixel(m_pvSurface, a_kriX, a_kriY, a_kruiColor);
}

/** setPixel (2) - sets a requested pixel's color data */
void SDL_Sprite::setPixel(const Sint32 & a_kriX, const Sint32 & a_kriY, const Uint8 & a_kruiRed, const Uint8 & a_kruiGreen, const Uint8 & a_kruiBlue) {
	//create a variable for the pixel's color
	Uint32 uiColor = 0;
	//set the blue component
	uiColor += (a_kruiBlue << 16);
	//set the green component
	uiColor += (a_kruiGreen << 8);
	//set the red component
	uiColor += a_kruiRed;
	//set the pixel to the specified color
	setPixel(a_kriX, a_kriY, uiColor);
}

/** getPixel8 - returns color data for requested pixel in 8-bit color format */
Uint8 SDL_Sprite::getPixel8(const Sint32 & a_kriX, const Sint32 & a_kriY) const {
	//get pointer to pixels for sprite's surface
	Uint8 * pbyPixels = (Uint8*)m_pvSurface->pixels;
	//return the specified pixel from the sprite's surface
	return pbyPixels[a_kriY * m_pvSurface->w + a_kriX];
}

/** setPixel8 - sets a requested pixel's color data in 8-bit color format */
void SDL_Sprite::setPixel8(const Sint32 & a_kriX, const Sint32 & a_kriY, const Uint8 & a_kruiColor) {
	//get pointer to pixels for sprite's surface
	Uint8* pbyPixels = (Uint8*)m_pvSurface->pixels;
	//set requested pixel to the specified
	pbyPixels[a_kriY * m_pvSurface->w + a_kriX] = a_kruiColor;
}

/** getPixel16 - returns color data for requested pixel in 16-bit color format */
Uint16 SDL_Sprite::getPixel16(const Sint32 & a_kriX, const Sint32 & a_kriY) const {
	//get pointer to pixels for sprite's surface
	Uint16* pusPixels = (Uint16*)m_pvSurface->pixels;
	//return the requested pixel from the sprite's surface
	return pusPixels[a_kriY * m_pvSurface->w + a_kriX];
}

/** setPixel16 - sets a requested pixel's color data in 16-bit color format */
void SDL_Sprite::setPixel16(const Sint32 & a_kriX, const Sint32 & a_kriY, const Uint16 & a_kruiColor) {
	//get pointer to pixels for sprite's surface
	Uint16* pusPixels = (Uint16*)m_pvSurface->pixels;
	//set requested pixel to the specified
	pusPixels[a_kriY * m_pvSurface->w + a_kriX] = a_kruiColor;
}

/** getPixel32 - returns color data for requested pixel in 32-bit color format */
Uint32 SDL_Sprite::getPixel32(const Sint32 & a_kriX, const Sint32 & a_kriY) const {
	//get pointer to pixels for sprite's surface
	Uint32* puiPixels = (Uint32*)m_pvSurface->pixels;
	//return the requested pixel from the sprite's surface
	return puiPixels[a_kriY * m_pvSurface->w + a_kriX];
}

/** setPixel32 - sets a requested pixel's color data in 32-bit color format */
void SDL_Sprite::setPixel32(const Sint32 & a_kriX, const Sint32 & a_kriY, const Uint32 & a_kruiColor) {
	//get pointer to pixels for sprite's surface
	Uint32* puiPixels = (Uint32*)m_pvSurface->pixels;
	//set requested pixel to the specified
	puiPixels[a_kriY * m_pvSurface->w + a_kriX] = a_kruiColor;
}

/** getRect - returns new surface with rect portion of this sprite's surface */
SDL_Surface* SDL_Sprite::getRect(const Sint32 & a_kriX, const Sint32 & a_kriY, const Uint32 & a_kruiWidth, const Uint32 & a_kruiHeight) const {
	//return the requested rect
	return getRect(*this, a_kriX, a_kriY, a_kruiWidth, a_kruiHeight);
}

/** rectCollide - returns true if another sprite collides with this one at the given coordinates */
const bool SDL_Sprite::rectCollide(const Sint32 & a_kriX1, const Sint32 & a_kriY1, const SDL_Sprite & a_krvSpriteB, const Sint32 & a_kriX2, const Sint32 & a_kriY2) const {
	//return the result of the rect collision
	return rectCollide(*this, a_kriX1, a_kriY1, a_krvSpriteB, a_kriX2, a_kriY2);
}

/** pixelCollide - returns true if another sprite collides with this one at the given coordinates, excluding transparent pixels */
const bool SDL_Sprite::pixelCollide(const Sint32 & a_kriX1, const Sint32 & a_kriY1, const SDL_Sprite & a_krvSpriteB, const Sint32 & a_kriX2, const Sint32 & a_kriY2) const {
	//return the result of the pixel collision
	return pixelCollide(*this, a_kriX1, a_kriY1, a_krvSpriteB, a_kriX2, a_kriY2);
}

/** rotate90 - rotates the sprite 90 degrees clockwise */
void SDL_Sprite::rotate90() {
	//rotate the sprite 90 degrees clockwise
	rotate(*this, 90);
}

/** rotate180 - rotates the sprite around half-way */
void SDL_Sprite::rotate180() {
	//rotate the sprite 180 degrees
	rotate(*this, 180);
}

/** rotate270 - rotates the sprite 90 degrees counter-clockwise */
void SDL_Sprite::rotate270() {
	//rotate the sprite 90 degrees counter-clockwise
	rotate(*this, 270);
}

/** flipHorizontal - flips the sprite horizontally */
void SDL_Sprite::flipHorizontal() {
	//flip the sprite horizontally
	flipHorizontal(*this);
}

/** flipVertical - flips the sprite vertically */
void SDL_Sprite::flipVertical() {
	//flip the sprite vertically
	flipVertical(*this);
}

/** fade - fades the sprite by given percentage */
void SDL_Sprite::fade(const float & a_krfFade) {
	//fade the sprite biy given percentage
	fade(*this, a_krfFade);
}

/** reverseAnimation - reverses the direction of the frame iteration */
void SDL_Sprite::reverseAnimation() {
	//switch the signed bit on the iterator
	m_iIndexIterator = -m_iIndexIterator;
}

/** zoom - rescales the sprite by given percentage */
void SDL_Sprite::zoom(const float & a_krfZ) {
	//rescale the sprite by given percentage
	zoom(*this, a_krfZ);
}

/** stretchX - stretches the sprite's width by given percentage */
void SDL_Sprite::stretchX(const float & a_krfX) {
	//stretch the sprite width by given percentage
	stretchX(*this, a_krfX);
}

/** stretchY - stretches the sprite's height by given percentage */
void SDL_Sprite::stretchY(const float & a_krfY) {
	//stretch the sprite height by given percentage
	stretchY(*this, a_krfY);
}

/** stretch - stretches the sprite's width and height by given percentages */
void SDL_Sprite::stretch(const float & a_krfX, const float & a_krfY) {
	//stretch the sprite width and height by given percentage
	stretch(*this, a_krfX, a_krfY);
}


/*
 ***********************************************************
 Static functions to cut down on memory usage
 ***********************************************************
 */

/** getRect - returns new surface with rect portion of given sprite's surface */
SDL_Surface* SDL_Sprite::getRect(const SDL_Sprite & a_krvSprite, const Sint32 & a_kriX, const Sint32 & a_kriY, const Uint32 & a_kruiWidth, const Uint32 & a_kruiHeight) {
	//hold a variable for a new surface pointer
	SDL_Surface * pvNewRect;
	//get the sprite's surface
	SDL_Surface * pvSpriteSurface = a_krvSprite.getSurface();
	//store color key flag
	Uint32 uiColorKeyFlag = pvSpriteSurface->flags & SDL_SRCCOLORKEY;
	// create a new surface
	SDL_PixelFormat * pvF = pvSpriteSurface->format;
	//create a new surface the size of the rect, with the sprite's properties
	pvNewRect = SDL_CreateRGBSurface(SDL_SWSURFACE, a_kruiWidth, a_kruiHeight, pvF->BitsPerPixel, pvF->Rmask, pvF->Gmask, pvF->Bmask, (uiColorKeyFlag?pvF->Amask:0) );

	//for all pixels from the rect
	for(Sint32 uiJ = 0; (unsigned)uiJ < a_kruiHeight; ++uiJ)
		for(Sint32 uiI = 0; (unsigned)uiI < a_kruiWidth; ++uiI)
			//set the pixels from the sprite to the rect
			setPixel(pvNewRect, uiI, uiJ, a_krvSprite.getPixel(a_kriX+uiI,a_kriY+uiJ));

	//if the color key is flagged
	if(uiColorKeyFlag)
		//copy the color key
		SDL_SetColorKey(pvNewRect, SDL_RLEACCEL|SDL_SRCCOLORKEY, pvF->colorkey );

	//return the rect surface
	return pvNewRect;
}

/** rectCollide - returns true if given sprite collides with other given sprite at the given coordinates */
bool SDL_Sprite::rectCollide(const SDL_Sprite & a_krvSpriteA, const Sint32 & a_kriX1, const Sint32 & a_kriY1, const SDL_Sprite & a_krvSpriteB, const Sint32 & a_kriX2, const Sint32 & a_kriY2) {
	//if sprites are not valid
	if(!a_krvSpriteA.isSprite() || !a_krvSpriteB.isSprite())
		//return false
		return false;
	//if sprites don't collide x-wise
	if( ((signed)(a_kriX1 + a_krvSpriteA.getWidth()) < a_kriX2) || ((signed)(a_kriX2 + a_krvSpriteB.getWidth()) < a_kriX1) )
		//return false
		return false;
	//if sprites don't collide y-wise
	if( ((signed)(a_kriY1 + a_krvSpriteA.getHeight()) < a_kriY2) || ((signed)(a_kriY2 + a_krvSpriteB.getHeight()) < a_kriY1) )
		//return false
		return false;
	//else return true for collision detected
	return true;
}

/** pixelCollide - returns true if given sprite collides with other given sprite at the given coordinates, excluding transparent pixels */
bool SDL_Sprite::pixelCollide(const SDL_Sprite & a_krvSpriteA, const Sint32 & a_kriX1, const Sint32 & a_kriY1, const SDL_Sprite & a_krvSpriteB, const Sint32 & a_kriX2, const Sint32 & a_kriY2) {
	//record bool for collision check
	bool bCollision = false;

	//if bounding boxes intersect
	if(rectCollide(a_krvSpriteA, a_kriX1, a_kriY1, a_krvSpriteB, a_kriX2, a_kriY2)) {
		//record widths and heights for sprites
		Uint32 uiWidthA = a_krvSpriteA.getWidth();
		Uint32 uiHeightA = a_krvSpriteA.getHeight();
		Uint32 uiWidthB = a_krvSpriteB.getWidth();
		Uint32 uiHeightB = a_krvSpriteB.getHeight();

		//get the overlapping box and overall bounding box
		Sint32 iInter_x0 = MAX(a_kriX2,a_kriX1);
		Sint32 iInter_x1 = MIN(a_kriX2+uiWidthB,a_kriX1+uiWidthA);

		Sint32 iOuter_x0 = MIN(a_kriX2, a_kriX1);
		Sint32 iOuter_x1 = MAX(a_kriX2+uiWidthB,a_kriX1+uiWidthA);

		Sint32 iInter_y0 = MAX(a_kriY2,a_kriY1);
		Sint32 iInter_y1 = MIN(a_kriY2+uiHeightB,a_kriY1+uiHeightA);

		Sint32 iOuter_y0 = MIN(a_kriY2, a_kriY1);
		Sint32 iOuter_y1 = MAX(a_kriY2+uiHeightB,a_kriY1+uiHeightA);

		//get the frame locations for sprites
		Sint32 iFrameAX = a_krvSpriteA.m_uiCIndex * uiWidthA;
		Sint32 iFrameBX = a_krvSpriteB.m_uiCIndex * uiWidthB;
		Sint32 iFrameAY = a_krvSpriteA.m_uiRIndex * uiHeightA;
		Sint32 iFrameBY = a_krvSpriteB.m_uiRIndex * uiHeightB;

		//get offsets for bounding box location
		Sint32 iAColOffX = (iInter_x0 == a_kriX1?iOuter_x0:iInter_x0);
		Sint32 iBColOffX = (iInter_x0 == a_kriX2?iOuter_x0:iInter_x0);
		Sint32 iAColOffY = (iInter_y0 == a_kriY1?iOuter_y0:iInter_y0);
		Sint32 iBColOffY = (iInter_y0 == a_kriY2?iOuter_y0:iInter_y0);

		//for all pixels in overlapping box
		for(Sint32 iY = iInter_y0 ; iY <= iInter_y1 && !bCollision; ++iY)
			for(Sint32 iX = iInter_x0 ; iX <= iInter_x1 && !bCollision; ++iX)
				//check pixels in sprite surfaces with frames and overlapping box offsets
				bCollision |= (!a_krvSpriteB.isTransparentPixel(iX-iBColOffX + iFrameBX, iY-iBColOffY + iFrameBY)) && (!a_krvSpriteA.isTransparentPixel(iX-iAColOffX + iFrameAX, iY-iBColOffY + iFrameAY));
	}

	//return result of collision check
    return bCollision;
}

/** rotate90 - rotates the given sprite 90 degrees clockwise */
void SDL_Sprite::rotate90(SDL_Sprite & a_rvSprite) {
	//rotate sprite 90 degrees clockwise
	rotate(a_rvSprite, 90);
}

/** rotate180 - rotates the given sprite around half-way */
void SDL_Sprite::rotate180(SDL_Sprite & a_rvSprite) {
	//rotate sprite 180 degrees
	rotate(a_rvSprite, 180);
}

/** rotate270 - rotates the given sprite 90 degrees counter-clockwise */
void SDL_Sprite::rotate270(SDL_Sprite & a_rvSprite) {
	//rotate sprite 90 degrees counter-clockwise
	rotate(a_rvSprite, 270);
}

/** flipHorizontal - flips the given sprite horizontally */
void SDL_Sprite::flipHorizontal(SDL_Sprite & a_rvSprite) {
	//flip sprite horizontally
	flip(a_rvSprite, FLIP_HORIZONTAL);
}

/** flipVertical - flips the given sprite vertically */
void SDL_Sprite::flipVertical(SDL_Sprite & a_rvSprite) {
	//flip sprite vertically
	flip(a_rvSprite, FLIP_VERTICAL);
}

/** rotate180 - rotates the given sprite clockwise by specified degrees */
void SDL_Sprite::rotate(SDL_Sprite & a_rvSprite, const Sint32 & a_kriDeg) {
	//create pointer to rotated surface
	SDL_Surface * pvRotated;
	//get the sprite's surface
	SDL_Surface * pvSurface = a_rvSprite.getSurface();
	SDL_PixelFormat * pvF = pvSurface->format;
	//store color key flag
	Uint32 uiColorKeyFlag = pvSurface->flags & SDL_SRCCOLORKEY;
	//record surface properties
	Uint32 uiWidth = a_rvSprite.getWidth();
	Uint32 uiHeight = a_rvSprite.getHeight();
	Uint32 uiMaxRows = a_rvSprite.getMaxRows();
	Uint32 uiMaxFrames = a_rvSprite.getMaxFrames();
	Uint32 uiMaxWidth;
	Uint32 uiMaxHeight;
	Uint32 uiNewWidth;
	Uint32 uiNewHeight;

	//check angle of rotation
	switch(a_kriDeg) {
	//if 90 degrees
	case 90:
	//or 270 degrees
	case 270:
		//set inverse width and height
		uiNewWidth = uiHeight;
		uiNewHeight = uiWidth;
		uiMaxWidth = uiNewWidth * uiMaxFrames;
		uiMaxHeight = uiNewHeight * uiMaxRows;
		break;
	//if 180 degrees
	case 180:
		//set width and height as normal
		uiMaxWidth = pvSurface->w;
		uiMaxHeight = pvSurface->h;
		uiNewWidth = uiWidth;
		uiNewHeight = uiHeight;
		break;
	//else
	default:
		//TODO: Handle misc. degrees
		return;
	}

	//create new surface with properties of sprite's surface
	pvRotated = SDL_CreateRGBSurface(SDL_SWSURFACE, uiMaxWidth, uiMaxHeight, pvF->BitsPerPixel, pvF->Rmask, pvF->Gmask, pvF->Bmask, (uiColorKeyFlag?pvF->Amask:0));
	
	//create bool to check if surface must be locked
	bool bLockedSurface = SDL_MUSTLOCK(pvSurface);
	//if surface needs to be locked
	if(bLockedSurface)
		//lock it
		SDL_LockSurface(pvSurface);
	
	//go through the new surface
	for(Uint32 uiR = 0; uiR < uiMaxRows; ++uiR) {
		//store the row coordinate for the current frame
		Sint32 iYo = uiR * uiHeight;
		//store rotated row coordinate for the current frame
		Sint32 iXor = uiR * uiWidth;
		for(Uint32 uiF = 0; uiF < uiMaxFrames; ++uiF) {
			//store the frame coordinate for the current frame
			Sint32 iXo = uiF * uiWidth;
			//store rotated frame coordinate for the current frame
			Sint32 iYof = uiF * uiHeight;
			for(Sint32 iY = 0; (unsigned)iY < uiHeight; ++iY) {
				//store the y coordinate for the current pixel
				Sint32 iYOff = iYo + iY;
				for(Sint32 iX = 0; (unsigned)iX < uiWidth; ++iX) {
					//store the x coordinate for the current pixel
					Sint32 iXOff = iXo + iX;
					//check angle of rotation
					switch(a_kriDeg) {
					//if 90 degrees
					case 90:
						//write pixels from right to left and top to bottom per frame
						setPixel(pvRotated, iYof + (uiHeight-1) - iY, iXor + iX, a_rvSprite.getPixel(iXOff, iYOff));
						break;
					//if 180 degrees
					case 180:
						//write pixels from right to left and bottom to top
						setPixel(pvRotated, iXo + (uiWidth-1) - iX, iYo + (uiHeight-1) - iY, a_rvSprite.getPixel(iXOff, iYOff));
						break;
					//if 270 degrees
					case 270:
						//write pixels from bottom to left and right to bottom
						setPixel(pvRotated, iYof + iY, iXor + (uiWidth-1) - iX, a_rvSprite.getPixel(iXOff, iYOff));
						break;
					default:
						//TODO: Handle misc. degrees
						return;
					}
				}
			}
		}
	}

	//if color key is flagged
	if(uiColorKeyFlag)
		//copy the color key
		SDL_SetColorKey(pvRotated, SDL_RLEACCEL|SDL_SRCCOLORKEY, pvF->colorkey);

	//if the surface was locked
	if(bLockedSurface)
		//unlock it
		SDL_UnlockSurface(pvSurface);

	//get number of rows on sprite
	Uint32 uiRows = a_rvSprite.getMaxRows();
	//create new max frame data
	Uint32 * prguiMaxFrames = new Uint32 [uiRows];
	//go through all rows
	for(Uint32 uiR = 0; uiR < uiRows; ++uiR)
		//get all max frames
		prguiMaxFrames[uiR] = a_rvSprite.getMaxFrames(uiR);

	//is sprite running?
	bool bIsRunning = a_rvSprite.running();
	//is sprite looping to beginning?
	bool bLoopToBeginning = a_rvSprite.looping();

	//set the sprite with the new surface
	a_rvSprite.setSprite(pvRotated, uiNewWidth, uiNewHeight, a_rvSprite.getSpeed(), uiRows);

	//if sprite is running
	if(bIsRunning)
		//start the new sprite
		a_rvSprite.start();

	//set loop bool
	a_rvSprite.setLoopToBegin(bLoopToBeginning);
	
	//go through all rows
	for(Uint32 uiR = 0; uiR < uiRows; ++uiR)
		//set the sprite with the old max frames info
		a_rvSprite.setMaxFrames(uiR, prguiMaxFrames[uiR]);

	//free the max frame data
	delete [] prguiMaxFrames;
}

/** flip - flips the given sprite in a given direction */
void SDL_Sprite::flip(SDL_Sprite & a_rvSprite, const Uint8 & a_krbyDir) {
	//pointer to flipped surface
	SDL_Surface * pvFlipped;
	//get pointer to the sprite's surface
	SDL_Surface * pvSpriteSurface = a_rvSprite.getSurface();
	//get pointer to the format for the sprite's surface
	SDL_PixelFormat * pvF = pvSpriteSurface->format;
	//get color key flag
	Uint32 uiColorKeyFlag = pvSpriteSurface->flags & SDL_SRCCOLORKEY;
	
	//create a new surface for the flipped variable, based off of the sprite's surface
	pvFlipped = SDL_CreateRGBSurface(SDL_SWSURFACE, pvSpriteSurface->w, pvSpriteSurface->h, pvF->BitsPerPixel, pvF->Rmask, pvF->Gmask, pvF->Bmask, (uiColorKeyFlag?pvF->Amask:0));
	
	//get boolean for whether the surface must be locked or not
	bool bLockedSurface = SDL_MUSTLOCK(pvSpriteSurface);

	//if the surface must be locked
	if(bLockedSurface)
		//lock it
		SDL_LockSurface(pvSpriteSurface);

	//record surface properties
	Uint32 uiWidth = a_rvSprite.getWidth();
	Uint32 uiHeight = a_rvSprite.getHeight();
	Uint32 uiMaxRows = a_rvSprite.getMaxRows();
	Uint32 uiMaxFrames = a_rvSprite.getMaxFrames();

	//go through the new surface
	for(Uint32 uiR = 0; uiR < uiMaxRows; ++uiR) {
		//store the row coordinate for the current frame
		Sint32 iYo = uiR * uiHeight;
		for(Uint32 uiF = 0; uiF < uiMaxFrames; ++uiF) {
			//store the frame coordinate for the current frame
			Sint32 iXo = uiF * uiWidth;
			for(Sint32 iY = 0; (unsigned)iY < uiHeight; ++iY) {
				//store the y coordinate for the current pixel
				Sint32 iYOff = iYo + iY;
				for(Sint32 iX = 0; (unsigned)iX < uiWidth; ++iX) {
					//store the x coordinate for the current pixel
					Sint32 iXOff = iXo + iX;
					//check flip direction
					switch(a_krbyDir) {
					//if horizontal
					case FLIP_HORIZONTAL:
						//write pixels from right to left per frame
						setPixel(pvFlipped, iXo + (uiWidth-1) - iX, iYOff, a_rvSprite.getPixel(iXOff, iYOff));
						break;
					//if vertical
					case FLIP_VERTICAL:
						//write pixels from bottom to top per frame
						setPixel(pvFlipped, iXOff, iYo + (uiHeight-1) - iY, a_rvSprite.getPixel(iXOff, iYOff));
						break;
					}
				}
			}
		}
	}

	//if color key is flagged
	if(uiColorKeyFlag)
		//copy the color key
		SDL_SetColorKey(pvFlipped, SDL_RLEACCEL|SDL_SRCCOLORKEY, pvF->colorkey);

	//if the surface was locked
	if(bLockedSurface)
		//unlock it
		SDL_UnlockSurface(pvSpriteSurface);

	//get number of rows on sprite
	Uint32 uiRows = a_rvSprite.getMaxRows();
	//create new max frame data
	Uint32 * prguiMaxFrames = new Uint32 [uiRows];
	//go through all rows
	for(Uint32 uiR = 0; uiR < uiRows; ++uiR)
		//get all max frames
		prguiMaxFrames[uiR] = a_rvSprite.getMaxFrames(uiR);

	//is sprite running?
	bool bIsRunning = a_rvSprite.running();
	//is sprite looping to beginning?
	bool bLoopToBeginning = a_rvSprite.looping();

	//set the sprite with the new surface
	a_rvSprite.setSprite(pvFlipped, a_rvSprite.getWidth(), a_rvSprite.getHeight(), a_rvSprite.getSpeed(), uiRows);

	//if sprite is running
	if(bIsRunning)
		//start the new sprite
		a_rvSprite.start();

	//set loop bool
	a_rvSprite.setLoopToBegin(bLoopToBeginning);
	
	//go through all rows
	for(Uint32 uiR = 0; uiR < uiRows; ++uiR)
		//set the sprite with the old max frames info
		a_rvSprite.setMaxFrames(uiR, prguiMaxFrames[uiR]);

	//free the max frame data
	delete [] prguiMaxFrames;
}

/** fade - fades the given sprite by given percentage */
void SDL_Sprite::fade(SDL_Sprite & a_rvSprite, const float & a_krfFade) {
	//record the sprite's surface
	SDL_Surface * pvSurface = a_rvSprite.getSurface();
	//record surface properties
	Uint32 uiWidth = a_rvSprite.getWidth();
	Uint32 uiHeight = a_rvSprite.getHeight();
	Uint32 uiMaxRows = a_rvSprite.getMaxRows();
	Uint32 uiMaxFrames = a_rvSprite.getMaxFrames();

	//get boolean for whether the surface must be locked or not
	bool bLockedSurface = SDL_MUSTLOCK(pvSurface);

	//if surface needs to be locked
	if(bLockedSurface)
		//lock it
		SDL_LockSurface(pvSurface);

	//go through the new surface
	for(Uint32 uiR = 0; uiR < uiMaxRows; ++uiR) {
		//store the row coordinate for the current frame
		Sint32 iYo = uiR * uiHeight;
		for(Uint32 uiF = 0; uiF < uiMaxFrames; ++uiF) {
			//store the frame coordinate for the current frame
			Sint32 iXo = uiF * uiWidth;
			for(Sint32 iY = 0; (unsigned)iY < uiHeight; ++iY) {
				//store the y coordinate for the current pixel
				Sint32 iYOff = iYo + iY;
				for(Sint32 iX = 0; (unsigned)iX < uiWidth; ++iX) {
					//store the x coordinate for the current pixel
					Sint32 iXOff = iXo + iX;
					if(!a_rvSprite.isTransparentPixel(iXOff, iYOff)) {
						//get the requested pixel
						Uint32 uiPixel = a_rvSprite.getPixel(iXOff, iYOff);
						//point to the color data by channel
						Uint8 * pbyChannel = (Uint8*)&uiPixel;
						//get the scale amount for the fade
						float fFadeScale = a_krfFade/100.0f;
						//hold variable for fade amount
						Sint32 iFade;
						//for the r, g, b color channels
						for(Uint32 uiI = 0; uiI < 3; ++uiI) {
							//record the fade amount
							iFade = (Sint32)(pbyChannel[uiI] * fFadeScale);
							//change the channel data by the fade amount
							pbyChannel[uiI] -= (Sint8)iFade;
						}
						//set the faded pixel on the sprite's surface
						setPixel(pvSurface, iXOff, iYOff, uiPixel);
					}
				}
			}
		}
	}

	//if the surface was locked
	if(bLockedSurface)
		//unlock it
		SDL_UnlockSurface(pvSurface);
}
	
/** zoom - rescales the given sprite by given percentage */
void SDL_Sprite::zoom(SDL_Sprite & a_rvSprite, const float & a_krfZ) {
	//rescale the sprite by the given percentage
	stretch(a_rvSprite, a_krfZ, a_krfZ);
}

/** stretchX - stretches the given sprite's width by given percentage */
void SDL_Sprite::stretchX(SDL_Sprite & a_rvSprite, const float & a_krfX) {
	//rescale the width of the sprite by the given percentage
	stretch(a_rvSprite, a_krfX, 100.0f);
}

/** stretchY - stretches the given sprite's height by given percentage */
void SDL_Sprite::stretchY(SDL_Sprite & a_rvSprite, const float & a_krfY) {
	//rescale the height of the sprite by the given percentage
	stretch(a_rvSprite, 100.0f, a_krfY);
}

/** stretch - stretches the given sprite's width and height by given percentages */
void SDL_Sprite::stretch(SDL_Sprite & a_rvSprite, const float & a_krfX, const float & a_krfY) {
	//pointer to rescaled surface
	SDL_Surface * pvZoomed;
	//record the sprite's surface
	SDL_Surface * pvSurface = a_rvSprite.getSurface();
	//record the pixel format for the sprite's surface
	SDL_PixelFormat * pvF = pvSurface->format;

	//if percentage is less than 1%
	if(a_krfX < 1.0f || a_krfY < 1.0f)
		//return
		return;
	
	//record magnitude of percentages
	float fStretchX = a_krfX / 100.0f;
	float fStretchY = a_krfY / 100.0f;

	//record surface properties
	Uint32 uiWidth = pvSurface->w;
	Uint32 uiHeight = pvSurface->h;
	Uint32 uiZoomWidth = (Uint32)(uiWidth * fStretchX);
	Uint32 uiZoomHeight = (Uint32)(uiHeight * fStretchY);

	//get color key flag
	Uint32 uiColorKeyFlag = pvSurface->flags & SDL_SRCCOLORKEY;

	//create a new surface for zoomed variable, based off the sprite's surface
	pvZoomed = SDL_CreateRGBSurface(SDL_SWSURFACE, uiZoomWidth, uiZoomHeight, pvF->BitsPerPixel, pvF->Rmask, pvF->Gmask, pvF->Bmask, (uiColorKeyFlag?pvF->Amask:0));
	
	//get boolean for whether the surface must be locked or not
	bool bLockedSurface = SDL_MUSTLOCK(pvSurface);

	//if surface needs to be locked
	if(bLockedSurface)
		//lock it
		SDL_LockSurface(pvSurface);

	//record surface properties
	Uint32 uiMaxRows = a_rvSprite.getMaxRows();
	Uint32 uiMaxFrames = a_rvSprite.getMaxFrames();
	Uint32 uiZFrameWidth = uiZoomWidth/uiMaxFrames;
	Uint32 uiZFrameHeight = uiZoomHeight/uiMaxRows;
	
	//go through the new surface
	for(Sint32 iY = 0; (unsigned)iY < uiZoomHeight; ++iY) {
		//record the y coordinate for the filtered pixel to read from
		Sint32 iYScale = (Sint32)(iY / fStretchY);
		for(Sint32 iX = 0; (unsigned)iX < uiZoomWidth; ++iX) {
			//record the x coordinate for the filtered pixel to read from
			Sint32 iXScale = (Sint32)(iX / fStretchX);
			//set the stretched pixel on the sprite's surface
			setPixel(pvZoomed, iX, iY, a_rvSprite.getPixel(iXScale, iYScale));
		}
	}

	//if color key is flagged
	if(uiColorKeyFlag)
		//copy the color key
		SDL_SetColorKey(pvZoomed, SDL_RLEACCEL|SDL_SRCCOLORKEY, pvF->colorkey);

	//if the surface was locked
	if(bLockedSurface)
		//unlock it
		SDL_UnlockSurface(pvSurface);

	//get number of rows on sprite
	Uint32 uiRows = a_rvSprite.getMaxRows();
	//create new max frame data
	Uint32 * prguiMaxFrames = new Uint32 [uiRows];
	//go through all rows
	for(Uint32 uiR = 0; uiR < uiRows; ++uiR)
		//get all max frames
		prguiMaxFrames[uiR] = a_rvSprite.getMaxFrames(uiR);

	//is sprite running?
	bool bIsRunning = a_rvSprite.running();
	//is sprite looping to beginning?
	bool bLoopToBeginning = a_rvSprite.looping();

	//set the sprite with the new surface
	a_rvSprite.setSprite(pvZoomed, uiZFrameWidth, uiZFrameHeight, a_rvSprite.getSpeed(), uiRows);

	//if sprite is running
	if(bIsRunning)
		//start the new sprite
		a_rvSprite.start();

	//set loop bool
	a_rvSprite.setLoopToBegin(bLoopToBeginning);
	
	//go through all rows
	for(Uint32 uiR = 0; uiR < uiRows; ++uiR)
		//set the sprite with the old max frames info
		a_rvSprite.setMaxFrames(uiR, prguiMaxFrames[uiR]);

	//free the max frame data
	delete [] prguiMaxFrames;
}

/** getPixel - returns color data for requested pixel from requested surface */
Uint32 SDL_Sprite::getPixel(SDL_Surface * a_pvSurface, const Sint32 & a_kriX, const Sint32 & a_kriY) {
	//record the bytes per pixel
	Uint32 uiBPP = a_pvSurface->format->BytesPerPixel;
	//record the address for the requested pixel to set
	Uint8 *p = (Uint8 *)a_pvSurface->pixels + a_kriY * a_pvSurface->pitch + a_kriX * uiBPP;
	
	//check the bytes per pixel
	switch(uiBPP) {
	//if the pixels are 1 byte
	case 1:
		//return the pixel
		return (Uint32)(*p);
	//if the pixels are 2 bytes
	case 2:
		//return the pixel
		return (Uint32)(*(Uint16 *)p);
	//if the pixels are 3 bytes
	case 3:
		//if the sdl byte order is big endian
		if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
			//return the color of the pixels, considering the component arrangement
			return (Uint32)(p[0] << 16 | p[1] << 8 | p[2]);
		//else if the byte order is little endian
		else
			//return the color of the pixels, considering the component arrangement
			return (Uint32)(p[0] | p[1] << 8 | p[2] << 16);
		break;
	//if pixels are 4 bytes
	case 4:
		//return the pixel
		return *(Uint32 *)p;
	}

	//return misc error code
	return UINT32_ERROR_MISC;
}

/** setPixel - sets color data for requested pixel for requested surface */
void SDL_Sprite::setPixel(SDL_Surface * a_pvSurface, const Sint32 & a_kriX, const Sint32 & a_kriY, const Uint32 & a_kruiColor) {
	//record the bytes per pixel
	Uint32 uiBPP = a_pvSurface->format->BytesPerPixel;
	//record the address for the requested pixel to set
	Uint8 *p = (Uint8 *)a_pvSurface->pixels + a_kriY * a_pvSurface->pitch + a_kriX * uiBPP;

	//check the bytes per pixel
	switch(uiBPP) {
	//if the pixels are 1 byte
	case 1:
		//set the color to the pixel
		*p = a_kruiColor;
		break;
	//if the pixels are 2 bytes
	case 2:
		//set the color to the pixel
		*(Uint16 *)p = a_kruiColor;
		break;
	//if the pixels are 3 bytes
	case 3:
		//if the sdl byte order is big endian
		if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
			//set the red component
			p[0] = (a_kruiColor >> 16) & 0xff;
			//set the green component
			p[1] = (a_kruiColor >> 8) & 0xff;
			//set the blue component
			p[2] = a_kruiColor & 0xff;
		}
		//else if the byte order is little endian
		else {
			//set the blue component
			p[0] = a_kruiColor & 0xff;
			//seet the green component
			p[1] = (a_kruiColor >> 8) & 0xff;
			//set the red component
			p[2] = (a_kruiColor >> 16) & 0xff;
		}
		break;
	//if pixels are 4 bytes
	case 4:
		//set the color to the pixel
		*(Uint32 *)p = a_kruiColor;
		break;
	}
}
