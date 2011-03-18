#ifndef _SDL_SPRITE_H_
#define _SDL_SPRITE_H_

#include "Global.h"
#include "sdl/SDL.h"

/*
Sprite library for SDL - using bitmaps
This library was originally put together by Kenny Cason (v1.0, Sep 20, 2009),
designed to be easily implemented into any C++ program using SDL. It was later
modified slightly by Michael Vaganov for a Sprite-oriented GSP280 lab. Even later modified slightly
by Matthew Morrill and Jared Bills for Team BMP's Senior Project.

Feel free to do what ever you want with it. enjoy!

For potential support, contact
kenneth [DOT] cason [AT] gmail [DOT] com
(www.ken-soft.com)
or
michael [DOT] vaganov [AT] gmail [DOT] com
or (if you really want to take your chances)
mattdude2489 [AT] gmail [DOT] com
bills [DOT] jared [AT] gmail [DOT] com
*/

class SDL_Sprite {

private:
	/** whether or not sprite's surface is being managed outside the sprite class */
	bool m_bExternalSurfaceManagement;
	/** whether to animate during the update */
	bool m_bRunning;
	/** if true, iterate 0 to N, then reset to 0. if false, iterate 0 to N, then N to 0, and repeat */
	bool m_bLoopsToBeginning;
	/** how many milliseconds the current frame has been shown */
	Uint32 m_uiTimer;
	/** how many milliseconds to show each frame */
    Uint32 m_uiSpeed;
	/** width of the sprite frames */
	Uint32 m_uiWidth;
	/** height of the sprite frames */
	Uint32 m_uiHeight;
	/** which frame is being drawn */
	Uint32 m_uiRIndex, m_uiCIndex;
	/** number of rows of frames for sprite */
	Uint32 m_uiMaxRows;
	/** direction and skip amount for frame iteration during animation */
    Sint32 m_iIndexIterator;
	/** total number of frames in a certain row */
    Uint32 * m_prguiMaxFrames;
	/** the SDL surface the sprite reads from */
    SDL_Surface * m_pvSurface;

public:

	/** Default Constructor */
    SDL_Sprite();
	/** Destructor */
    ~SDL_Sprite();
	/** Constructor (1) - load in sprite from file with properties */
	SDL_Sprite(char * a_szFileName, const Uint32 & a_kruiFrameWidth, const Uint32 & a_kruiFrameHeight, const Uint32 & a_kruiAnimSpeed, const Uint32 & a_kruiRows);
	/** Constructor (2) - load in sprite from copied surface with properties */
    SDL_Sprite(SDL_Surface * a_pvSurface, const Uint32 & a_kruiFrameWidth, const Uint32 & a_kruiFrameHeight, const Uint32 & a_kruiAnimSpeed, const Uint32 & a_kruiRows);
	
	/** setSprite (1) - load in new sprite from file with properties */
	void setSprite(char * a_szFileName, const Uint32 & a_kruiFrameWidth, const Uint32 & a_kruiFrameHeight, const Uint32 & a_kruiAnimSpeed, const Uint32 & a_kruiRows);
	/** setSprite (2) - load in new sprite from copied surface with properties */
	void setSprite(SDL_Surface * a_pvSurface, const Uint32 & a_kruiFrameWidth, const Uint32 & a_kruiFrameHeight, const Uint32 & a_kruiAnimSpeed, const Uint32 & a_kruiRows);
	/** start - starts the sprite animation */
    void start();
	/** stop - stops the sprite animation */
    void stop();
	/** restart (1) - restarts the sprite animation */
    void restart();
	/** restart (2) - restarts the sprite animation at a given row */
	const bool restart(const Uint32 & a_kruiRIndex);
	/** update - updates the sprite based on the change in time */
    void update(const Uint32 & a_kruiMSPassed);
	/** draw - draws the sprite to the specified screen at given x and y coords */
    void draw(SDL_Surface * a_pvScreenSurface, const Sint32 & a_kriX, const Sint32 & a_kriY);
	/** destroy - frees sprite's resources */
    void destroy();
	/** setLoopToBegin - sets animation to loop from beginning or not */
    void setLoopToBegin(const bool & a_krbLoop);
	/** setFrame - sets the frame dimensions to the specified */
	void setFrame(const Uint32 & a_kruiWidth, const Uint32 & a_kruiHeight);
	/** setMaxRows - sets maximum number of rows for sprite */
	void setMaxRows(const Uint32 & a_kruiRows);
	/** setMaxFrames (1) - set maximum number of frames for all rows */
	void setMaxFrames(const Uint32 & a_kruiMaxFrames);
	/** setMaxFrames (2) - set maximum number of frames for specific row */
	const bool setMaxFrames(const Uint32 & a_kruiRIndex, const Uint32 & a_kruiMaxFrames);
	/** setRIndex - set sprite to use new animation row */
	void setRIndex(const Uint32 & a_kruiRIndex);
	/** setTransparency (1) - sets color keycode to correlate to transparency for sprite */
    void setTransparency(const Uint32 & a_kruiColorKey);
	/** setTransparency (2) - sets color keycode to correlate to transparency for sprite */
    void setTransparency(const Uint8 & a_krbyRed, const Uint8 & a_krbyGreen, const Uint8 & a_krbyBlue);
	/** setSpeed - sets the animation speed of the sprite */
    void setSpeed(const Uint32 & a_kruiAnimSpeed);
	/** setIndexIterator - change direction and skip amount for frame iteration during animation */
	void setFrameIterator(const Sint32 & a_kriFrameIterator);
	/** setSurface - sets the sprite's surface to a new one, but needs to know if surface will be managed outside sprite class */
    void setSurface(SDL_Surface * a_pvSurface, const bool & a_krbExternalManagement);

	/** running - returns true if sprite is started */
    const bool & running() const;
	/** looping - returns true if sprite is looping to beginning */
	const bool & looping() const;
	/** isSprite - returns true if sprite is valid */
    const bool isSprite() const;
	/** getSpeed - returns the animation speed */
	const Uint32 & getSpeed() const;
	/** getRow - returns row index */
	const Uint32 & getRow() const;
	/** getFrame - returns frame index */
    const Uint32 & getFrame() const;
	/** getWidth - returns width of frame */
    const Uint32 & getWidth() const;
	/** getHeight - return height of frame */
    const Uint32 & getHeight() const;
	/** getSpriteWidth - return width of sprite's surface */
    const Uint32 getSpriteWidth() const;
	/** getSpriteHeight - return height of sprite's surface */
    const Uint32 getSpriteHeight() const;
	/** getMaxRows - return number of rows */
	const Uint32 & getMaxRows() const;
	/** getMaxFrames (1) - return number of frames at a row with highest number of frames */
	const Uint32 getMaxFrames() const;
	/** getMaxFrames (2) - return number of frames at a specified row */
	const Uint32 getMaxFrames(const Uint32 & a_kruiRIndex) const;
	/** equals - returns true if the sprite's surface is the same as this one's in memory */
    const bool equals(const SDL_Sprite & a_krvCompare) const;
	/** getSurface - returns the sprite's surface */
    SDL_Surface * getSurface() const;

	/** isTransparentPixel - returns true if the requested pixel is transparent on the sprite */
    const bool isTransparentPixel(const Sint32 & a_kriX, const Sint32 & a_kriY) const;
	/** setTransparentPixel - sets a requested pixel's color to be the keycode color for transparency */
    void setTransparentPixel(const Sint32 & a_kriX, const Sint32 & a_kriY);
	/** getPixel - returns color data for requested pixel */
    Uint32 getPixel(const Sint32 & a_kriX, const Sint32 & a_kriY) const;
	/** setPixel (1) - sets a requested pixel's color data */
    void setPixel(const Sint32 & a_kriX, const Sint32 & a_kriY, const Uint32 & a_kruiColor);
	/** setPixel (2) - sets a requested pixel's color data */
    void setPixel(const Sint32 & a_kriX, const Sint32 & a_kriY, const Uint8 & a_kruiRed, const Uint8 & a_kruiGreen, const Uint8 & a_kruiBlue);
	/** getPixel8 - returns color data for requested pixel in 8-bit color format */
    Uint8 getPixel8(const Sint32 & a_kriX, const Sint32 & a_kriY) const;
	/** setPixel8 - sets a requested pixel's color data in 8-bit color format */
    void setPixel8(const Sint32 & a_kriX, const Sint32 & a_kriY, const Uint8 & a_kruiColor);
	/** getPixel16 - returns color data for requested pixel in 16-bit color format */
    Uint16 getPixel16(const Sint32 & a_kriX, const Sint32 & a_kriY) const;
	/** setPixel16 - sets a requested pixel's color data in 16-bit color format */
    void setPixel16(const Sint32 & a_kriX, const Sint32 & a_kriY, const Uint16 & a_kruiColor);
	/** getPixel32 - returns color data for requested pixel in 32-bit color format */
    Uint32 getPixel32(const Sint32 & a_kriX, const Sint32 & a_kriY) const;
	/** setPixel32 - sets a requested pixel's color data in 32-bit color format */
    void setPixel32(const Sint32 & a_kriX, const Sint32 & a_kriY, const Uint32 & a_kruiColor);
	/** getRect - returns new surface with rect portion of this sprite's surface */
	SDL_Surface* getRect(const Sint32 & a_kriX, const Sint32 & a_kriY, const Uint32 & a_kruiWidth, const Uint32 & a_kruiHeight) const;

	/** rectCollide - returns true if another sprite collides with this one at the given coordinates */
    const bool rectCollide(const Sint32 & a_kriX1, const Sint32 & a_kriY1, const SDL_Sprite & a_krvSpriteB, const Sint32 & a_kriX2, const Sint32 & a_kriY2) const;
	/** pixelCollide - returns true if another sprite collides with this one at the given coordinates, excluding transparent pixels */
    const bool pixelCollide(const Sint32 & a_kriX1, const Sint32 & a_kriY1, const SDL_Sprite & a_krvSpriteB, const Sint32 & a_kriX2, const Sint32 & a_kriY2) const;
	/** rotate90 - rotates the sprite 90 degrees clockwise */
    void rotate90();
	/** rotate180 - rotates the sprite around half-way */
    void rotate180();
	/** rotate270 - rotates the sprite 90 degrees counter-clockwise */
    void rotate270();
	/** flipHorizontal - flips the sprite horizontally */
    void flipHorizontal();
	/** flipVertical - flips the sprite vertically */
    void flipVertical();
	/** fade - fades the sprite by given percentage */
    void fade(const float & a_krfFade);
	/** reverseAnimation - reverses the direction of the frame iteration */
    void reverseAnimation();

	/** zoom - rescales the sprite by given percentage */
    void zoom(const float & a_krfZ);
	/** stretchX - stretches the sprite's width by given percentage */
    void stretchX(const float & a_krfX);
	/** stretchY - stretches the sprite's height by given percentage */
    void stretchY(const float & a_krfY);
	/** stretch - stretches the sprite's width and height by given percentages */
    void stretch(const float & a_krfX, const float & a_krfY);

	
	/** getRect - returns new surface with rect portion of given sprite's surface */
    static SDL_Surface* getRect(const SDL_Sprite & a_krvSprite, const Sint32 & a_kriX, const Sint32 & a_kriY, const Uint32 & a_kruiWidth, const Uint32 & a_kruiHeight);
	/** rectCollide - returns true if given sprite collides with other given sprite at the given coordinates */
    static bool rectCollide(const SDL_Sprite & a_krvSpriteA, const Sint32 & a_kriX1, const Sint32 & a_kriY1, const SDL_Sprite & a_krvSpriteB, const Sint32 & a_kriX2, const Sint32 & a_kriY2);
	/** pixelCollide - returns true if given sprite collides with other given sprite at the given coordinates, excluding transparent pixels */
    static bool pixelCollide(const SDL_Sprite & a_krvSpriteA, const Sint32 & a_kriX1, const Sint32 & a_kriY1, const SDL_Sprite & a_krvSpriteB, const Sint32 & a_kriX2, const Sint32 & a_kriY2);
	/** rotate90 - rotates the given sprite 90 degrees clockwise */
	static void rotate90(SDL_Sprite & a_rvSprite);
	/** rotate180 - rotates the given sprite around half-way */
    static void rotate180(SDL_Sprite & a_rvSprite);
	/** rotate270 - rotates the given sprite 90 degrees counter-clockwise */
    static void rotate270(SDL_Sprite & a_rvSprite);
	/** flipHorizontal - flips the given sprite horizontally */
    static void flipHorizontal(SDL_Sprite & a_rvSprite);
	/** flipVertical - flips the given sprite vertically */
    static void flipVertical(SDL_Sprite & a_rvSprite);
	/** rotate180 - rotates the given sprite clockwise by specified degrees */
    static void rotate(SDL_Sprite & a_rvSprite, const Sint32 & a_kriDeg);
	/** flip - flips the given sprite in a given direction */
    static void flip(SDL_Sprite & a_rvSprite, const Uint8 & a_krbyDir);
	/** fade - fades the given sprite by given percentage */
    static void fade(SDL_Sprite & a_rvSprite, const float & a_krfFade);
	
	/** zoom - rescales the given sprite by given percentage */
    static void zoom(SDL_Sprite & a_rvSprite, const float & a_krfZ);
	/** stretchX - stretches the given sprite's width by given percentage */
    static void stretchX(SDL_Sprite & a_rvSprite, const float & a_krfX);
	/** stretchY - stretches the given sprite's height by given percentage */
    static void stretchY(SDL_Sprite & a_rvSprite, const float & a_krfY);
	/** stretch - stretches the given sprite's width and height by given percentages */
    static void stretch(SDL_Sprite & a_rvSprite, const float & a_krfX, const float & a_krfY);

	/** getPixel - returns color data for requested pixel from requested surface */
    static Uint32 getPixel(SDL_Surface * a_pvSurface, const Sint32 & a_kriX, const Sint32 & a_kriY);
	/** setPixel - sets color data for requested pixel for requested surface */
    static void setPixel(SDL_Surface * a_pvSurface, const Sint32 & a_kriX, const Sint32 & a_kriY, const Uint32 & a_kruiColor);

};

#endif
