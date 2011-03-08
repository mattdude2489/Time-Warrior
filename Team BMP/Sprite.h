#ifndef Sprite_h
#define Sprite_h

#include "SDL/SDL.h"

/*
Sprite library for SDL - using bitmaps
This library was originally put together by Kenny Cason (v1.0, Sep 20, 2009),
designed to be easily implemented into any C++ program using SDL. It was later
modified slightly by Michael Vaganov for a Sprite-oriented GSP280 lab. Even later modified slightly
by Matthew Morrill for Team BMP's Senior Project.

Feel free to do what ever you want with it. enjoy!

For potential support, contact
kenneth [DOT] cason [AT] gmail [DOT] com
(www.ken-soft.com)
or
michael [DOT] vaganov [AT] gmail [DOT] com
or, if you really want to take your chances
mattdude2489 [AT] gmail [DOT] com
*/

#define FLIP_HORIZONTAL 1
#define FLIP_VERTICAL   2

class Sprite {
public:
	/** whether to animate during the update */
	bool m_running;
	/** if true, iterate 0 to N, then reset to 0. if false, iterate 0 to N, then N to 0, and repeat */
	bool m_loopsToBeginning;
	/** how many milliseconds the current frame has been shown */
	Uint32 m_timer;
	/** how many milliseconds to show each frame */
    Uint32 m_speed;
	/** dimensions of the sprite */
    int m_width, m_height;
	/** which frame is being drawn */
    int m_index, m_hindex;
	/** which direction are frames animating in */
    int m_indexIterator;
	/** what is the total number of frames */
    int m_maxFrames;
	/** the image that this sprite uses */
    SDL_Surface* m_surface;

    Sprite();
    ~Sprite();
    Sprite(const char * a_fileName,const int & a_maxFrames, const int & a_animationSpeed);
    Sprite(const SDL_Surface* a_surface, const int & a_maxFrames,const int & a_animationSpeed);

	void setHIndex(int a_hindex, const int & a_maxFrames);
	void setSprite(const char * a_fileName, const int & a_maxFrames, const int & a_animationSpeed);
    void draw(SDL_Surface* a_screen, const int & x, const int & y);
    void setTransparency(const Uint32 & color);
    void setTransparency(const int & red, const int & green, const int & blue);
    void setSpeed(int animationSpeed);
    void start();
    void restart();
    void update(const int & a_msPassed);
    void setLoopToBegin(bool loop);
    bool running() const;
    void stop();
    bool isSprite() const;
	int getRowFrame() const;
    int getFrame() const;
    int getFrameWidth() const;
    int getFrameHeight() const;
    int getWidth() const;
    int getHeight() const;
    int getSpriteWidth() const;
    int getSpriteHeight() const;
    bool equals(Sprite compare) const;
    SDL_Surface* getSurface() const;
    void setSurface(SDL_Surface* surface);
    void destroy();

    bool isTransparentPixel(const int & x, const int & y) const;
    void setTransparentPixel(const int & x, const int & y);
    Uint32 getPixel(const int & x, const int & y) const;
    void setPixel(const int & x, const int & y, const Uint32 & color);
    void setPixel(const int & x, const int & y, const int & red, const int & green, const int & blue);
    Uint8 getPixel8(const int & x, const int & y) const;
    void setPixel8(const int & x, const int & y, const Uint8 & color);
    Uint16 getPixel16(const int & x, const int & y) const;
    void setPixel16(const int & x, const int & y, const Uint16 & color);
    Uint32 getPixel32(const int & x, const int & y) const;
    void setPixel32(const int & x, const int & y, const Uint32 & color);

    SDL_Surface* getRect(const int & x, const int & y, const int & width, const int & height) const;
    bool rectCollide(const int & x1, const int & y1, const Sprite & spriteB, const int & x2, const int & y2) const;
    bool pixelCollide(const int & x1, const int & y1, const Sprite & spriteB, const int & x2, const int & y2) const;
    void rotate90();
    void rotate180();
    void rotate270();
    void flipHorizontal();
    void flipVertical();
    void fade(const float & fade); // fade from 0 to 100%
    void reverseAnimation();

    void zoom(const float & z);  // percentage to zoom in
    void stretchX(const float & x); // percentage to stretchheight
    void stretchY(const float & y);  // percentage to stretchwidth
    void stretch(const float & x, const float & y); // percentage to strech X and Y

private:
    /* static helper functions */
    static SDL_Surface* getRect(const Sprite & sprite, const int & x, const int & y, const int & width, const int & height);
    static bool rectCollide(const Sprite & spriteA, const int & x1, const int & y1, const Sprite & spriteB, const int & x2, const int & y2);
    static bool pixelCollide(const Sprite & spriteA, const int & x1, const int & y1, const Sprite & spriteB, const int & x2, const int & y2);
    static void rotate90(Sprite &sprite);
    static void rotate180(Sprite &sprite);
    static void rotate270(Sprite &sprite);
    static void flipHorizontal(Sprite &sprite);
    static void flipVertical(Sprite &sprite);
    static void rotate(Sprite &sprite, const int & dir); // helper function for rotate90(),rotate180(), and rotate270()
    static void flip(Sprite &sprite, const int & dir);  // helper function for flipHorizontal() and flipVeritcal()
    static void fade(Sprite &sprite, const float & fade); // fade from 0 to 100%
    static void reverseAnimation(Sprite & sprite);

    static void zoom(Sprite & sprite, const float & x);  // percentage to zoom in
    static void stretchX(Sprite &sprite, const float & x); // percentage to stretchheight
    static void stretchY(Sprite &sprite, const float & y);  // percentage to stretchwidth
    static void stretch(Sprite &sprite, float x, float y); // percentage to strech X and Y

    static Uint32 getPixel(const SDL_Surface * a_surface, const int & x, const int & y);
    static void setPixel(SDL_Surface* a_surface, const int & x, const int & y, const Uint32 & color);

};
#endif
