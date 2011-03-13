#include "Sprite.h"

#define SPRITE_MAX(a,b)	((a > b) ? a : b)
#define SPRITE_MIN(a,b)	((a < b) ? a : b)

//Default constructor.
Sprite::Sprite()
{
	m_surface = NULL;
	m_width = 0;
	m_height = 0;
	m_speed = 0;
	m_running = false;
	m_loopsToBeginning = true;
	m_indexIterator = 0;
	m_index = 0;
	m_hindex = 0;
}

//Second constructor.
Sprite::Sprite(const char * file, const int & a_frames, const int & a_speed, const int & a_rows) {
    SDL_Surface *temp = SDL_LoadBMP(file);
    m_surface = SDL_DisplayFormat(temp);
    SDL_FreeSurface(temp);
    if(m_surface == NULL) {
        m_width = 0;
        m_height = 0;
        m_speed = 0;
    } else {
        m_width = m_surface->w/a_frames;
        m_height = m_surface->h/a_rows;
        m_speed = a_speed;
    }
    m_maxFrames = a_frames;
	m_maxRows = a_rows;
    m_running = (m_maxFrames > 1);
	restart();
    m_indexIterator = 1;
    m_loopsToBeginning = true;
}

//Third constructor.
void Sprite::setSprite(const char * file, const int & a_frames, const int & a_speed, const int & a_rows) {
	SDL_Surface *temp = SDL_LoadBMP(file);
    m_surface = SDL_DisplayFormat(temp);
    SDL_FreeSurface(temp);
    if(m_surface == NULL) {
        m_width = 0;
        m_height = 0;
        m_speed = 0;
    } else {
        m_width = m_surface->w/a_frames;
        m_height = m_surface->h/a_rows;
        m_speed = a_speed;
    }
    m_maxFrames = a_frames;
    m_running = (m_maxFrames > 1);
	restart();
    m_indexIterator = 1;
    m_loopsToBeginning = true;
}

//MY GOD. HOW MANY CONSTRUCTORS ARE THERE. Fourth Constructor.
Sprite::Sprite(const SDL_Surface* a_surface, const int & a_frames, const int & a_speed, const int & a_rows) {
	if(a_surface == NULL) {
		m_surface = NULL;
		m_width = 0;
		m_height = 0;
		m_speed = 0;
	} else {
		SDL_PixelFormat * f = a_surface->format;
		// create a new surface
		m_surface = SDL_CreateRGBSurfaceFrom(
			a_surface->pixels, a_surface->w, a_surface->h, f->BitsPerPixel,
			a_surface->pitch, f->Rmask, f->Gmask, f->Bmask,
				(a_surface->flags & SDL_SRCCOLORKEY)?0:f->Amask);
		if(a_surface->flags & SDL_SRCCOLORKEY) {
			SDL_SetColorKey(m_surface, SDL_RLEACCEL|SDL_SRCCOLORKEY, f->colorkey);
		}
		m_width = m_surface->w/a_frames;
		m_height = m_surface->h/a_rows;
		m_speed = a_speed;
	}
	m_maxFrames = a_frames;
	m_running = (m_maxFrames > 1);
	restart();
	m_indexIterator = 1;
	m_loopsToBeginning = true;
}

void Sprite::draw(SDL_Surface* a_screen, const int & x, const int & y) {
	SDL_Rect dstrect = {x,y,32,32};
	// this blits the current frame from the sprite sheet
	SDL_Rect animRect = {m_width*m_index,m_height*m_hindex,m_width,m_height}; //The starting position, which I only changed the original 0 to m_height*m_hindex.
	SDL_BlitSurface(m_surface, &animRect, a_screen,&dstrect); //That should allow us to easily have multiple rows.
}

Sprite::~Sprite() {
	SDL_FreeSurface(m_surface);
}

void Sprite::setTransparency(const int & r, const int & g, const int & b) {
	SDL_SetColorKey(m_surface, SDL_SRCCOLORKEY, SDL_MapRGB(m_surface->format, r, g, b));
}

void Sprite::setTransparency(const Uint32 & colorkey) {
	SDL_SetColorKey(m_surface, SDL_SRCCOLORKEY, colorkey);
}

void Sprite::setSpeed(int i) {
	m_speed = i;
}

void Sprite::start() {
	m_running = true;
}

void Sprite::restart() {
	m_index = 0;
	m_timer = 0;
	m_hindex = 0;
}

/** @param a_msPassed the number of milliseconds since the last time update was called */
void Sprite::update(const int & a_msPassed) {
	if(m_running)
	{
		m_timer += a_msPassed;
		if(m_timer > m_speed)
		{
			m_timer -= m_speed;
			m_index += m_indexIterator;
			if(m_index >= m_maxFrames) {
				if(m_loopsToBeginning) {
					m_indexIterator = 1;
					m_index = 0;
				} else {
					m_indexIterator = -1;
					m_index = m_maxFrames - 1;
				}
			} else if(m_index < 0) {
				m_indexIterator = 1;
				m_index = 0;
			}
		}
	}
}

void Sprite::setLoopToBegin(bool loop) {
	m_loopsToBeginning = loop;
}

bool Sprite::running() const {
	return m_running;
}

void Sprite::stop() {
	m_running = false;
}

bool Sprite::isSprite() const {
    return m_surface && m_width > 0 && m_height > 0;
}


void Sprite::setTransparentPixel(const int & x, const int & y) {
	setPixel(x, y, m_surface->format->colorkey);
}

bool Sprite::isTransparentPixel(const int & x, const int & y) const {
	Uint32 pixelcolor = getPixel(x, y);
	//test whether pixels color == color of transparent pixels for that surface
	return (pixelcolor == m_surface->format->colorkey);
}

void Sprite::setPixel(const int & x, const int & y, const int & r, const int & g, const int & b) {
	Uint32 color;
	color += (b << 16);
	color += (g << 8);
	color += r;
	setPixel(x,y,color);
}

void Sprite::setPixel(const int & x, const int & y, const Uint32 & pixel) {
	setPixel(m_surface, x, y, pixel);
}

Uint32 Sprite::getPixel(const int & x, const int & y) const {
	return getPixel(m_surface, x, y);
}

Uint8 Sprite::getPixel8(const int & x, const int & y) const {
	Uint8* pixels = (Uint8*)m_surface->pixels;
	return pixels[y * m_surface->w + x];
}

void Sprite::setPixel8(const int & x, const int & y, const Uint8 & pixel) {
	Uint8* pixels = (Uint8*)m_surface->pixels;
	pixels[y * m_surface->w + x] = pixel;
}

Uint16 Sprite::getPixel16(const int & x, const int & y) const {
	Uint16* pixels = (Uint16*)m_surface->pixels;
	return pixels[y * m_surface->w + x];
}

void Sprite::setPixel16(const int & x, const int & y, const Uint16 & pixel) {
	Uint16* pixels = (Uint16*)m_surface->pixels;
	pixels[y * m_surface->w + x] = pixel;
}

Uint32 Sprite::getPixel32(const int & x, const int & y) const {
	Uint32* pixels = (Uint32*)m_surface->pixels;
	return pixels[y * m_surface->w + x];
}

void Sprite::setPixel32(const int & x, const int & y, const Uint32 & pixel) {
	Uint32* pixels = (Uint32*)m_surface->pixels;
	pixels[y * m_surface->w + x] = pixel;
}

int Sprite::getRowFrame() const {
	return m_hindex;
}

int Sprite::getFrame() const {
	return m_index;
}

int Sprite::getFrameWidth() const { // width of each frame
	return m_width;
}

int Sprite::getFrameHeight() const { // height of each frame, in this implementation it is the sprites actual height
	return m_height;
}

int Sprite::getWidth() const { // this is most likely the width a user will need to know
	return getFrameWidth();
}

int Sprite::getSpriteWidth() const { // sprites Actual width
	return m_surface->w;
}

int Sprite::getHeight() const { // in this Class the frame height is the same as the sprites Actual height
	return m_surface->h;
}

int Sprite::getSpriteHeight() const { // sprites Actual height
	return m_surface->h;
}

bool Sprite::equals(Sprite cmp) const {
	if(m_surface == cmp.getSurface()) {
		return true;
	}
	return false;
}

SDL_Surface* Sprite::getSurface() const {
	 return m_surface;
}

void Sprite::setSurface(SDL_Surface* surface) {
	m_surface = surface;
}

SDL_Surface* Sprite::getRect(const int & x, const int & y, const int & width, const int & height) const {
	return getRect(*this, x, y, width, height);
}

bool Sprite::rectCollide(const int & x1, const int & y1, const Sprite & spriteB, const int & x2, const int & y2) const {
	return rectCollide(*this, x1, y1, spriteB, x2, y2);
}

bool Sprite::pixelCollide(const int & x1, const int & y1, const Sprite & spriteB, const int & x2, const int & y2) const {
	return pixelCollide(*this, x1, y1, spriteB, x2, y2);
}

void Sprite::rotate90() {
	rotate90(*this);
}

void Sprite::rotate180() {
	rotate180(*this);
}

void Sprite::rotate270() {
	rotate270(*this);
}

void Sprite::flipHorizontal() {
	flipHorizontal(*this);
}

void Sprite::flipVertical() {
	flipVertical(*this);
}

void Sprite::fade(const float & f) {
	fade(*this, f);
}

void Sprite::reverseAnimation() {
	reverseAnimation(*this);
}

void Sprite::zoom(const float & z) {
	zoom(*this, z);
}

void Sprite::stretchX(const float & x) {
	stretchX(*this, x);
}

void Sprite::stretchY(const float & y) {
	stretchY(*this, y);
}

void Sprite::stretch(const float & x, const float & y) {
	stretch(*this, x, y);
}

void Sprite::destroy() {
	SDL_FreeSurface(m_surface);
}

/*
 ***********************************************************
 Static functions to cut down on memory usage
 *********************************************************** */

SDL_Surface* Sprite::getRect(const Sprite & sprite, const int & x, const int & y, const int & w, const int & h) {
	SDL_Surface* newrect;
	// create a new surface
	SDL_PixelFormat * f = sprite.getSurface()->format;
	if(sprite.getSurface()->flags & SDL_SRCCOLORKEY) {
		newrect = SDL_CreateRGBSurface(SDL_SWSURFACE, w, h, f->BitsPerPixel,
		f->Rmask, f->Gmask, f->Bmask, f->Amask );
	} else {
		newrect = SDL_CreateRGBSurface(SDL_SWSURFACE, w, h, f->BitsPerPixel,
			f->Rmask, f->Gmask, f->Bmask, 0 );
	}

	for(int j = 0; j < h; j++) {
		for(int i = 0; i < w; i++) {
			setPixel(newrect, i, j, sprite.getPixel(x+i,y+j));
		}
	}
	//Copy color key
	if(sprite.getSurface()->flags & SDL_SRCCOLORKEY) {
		SDL_SetColorKey(newrect, SDL_RLEACCEL|SDL_SRCCOLORKEY, f->colorkey );
	}
	return newrect;
}


    /*
       test
	    testX,testY____ testX+width
		|   	       |
		| sprite	   |
		|  thisX,thisY |__ thisX+width
		|  |	       |  |
		|__|___________|  |
		testY+height	  |
			|_____________|
			thisY+height
    */
bool Sprite::rectCollide(const Sprite & spriteA, const int & aX, const int & aY, 
						 const Sprite & spriteB, const int & bX, const int & bY) {
	if(!spriteA.isSprite() || !spriteB.isSprite()) {
		return false;
	}
	if((aX + spriteA.getWidth() < bX) || (bX + spriteB.getWidth() < aX)) {
		return false;
	}
	if((aY + spriteA.getHeight() < bY) || (bY + spriteB.getHeight() < aY)) {
		return false;
	}
	return true;
}

bool Sprite::pixelCollide(const Sprite & spriteA, const int & aX, const int & aY, 
						  const Sprite & spriteB, const int & bX, const int & bY) {
	/*check if bounding boxes intersect*/
	if(!rectCollide(spriteA, aX, aY, spriteB, bX, bY)) {
         return false;
    }

    // get the overlaping box
	int inter_x0 = SPRITE_MAX(bX,aX);
	int inter_x1 = SPRITE_MIN(bX+spriteB.getWidth(),aX+spriteA.getWidth());

	int inter_y0 = SPRITE_MAX(bY,aY);
	int inter_y1 = SPRITE_MIN(bY+spriteB.getHeight(),aY+spriteA.getHeight());

	for(int y = inter_y0 ; y <= inter_y1 ; y++) {
		for(int x = inter_x0 ; x <= inter_x1 ; x++) {
		    /*compute offsets for surface, but dont forget to account for the current animation*/
            if((!spriteB.isTransparentPixel(x-aX + spriteB.m_index * spriteB.getWidth() , y-bY))
			&& (!spriteA.isTransparentPixel(x-aX + spriteA.m_index * spriteA.getWidth(), y-aY))) {/*before pass to isTransparentPixel*/
				return true;
			}
		}
	}
    return false;
}

void Sprite::flip(Sprite & sprite, const int & val) {
	SDL_Surface* flipped;
	// create a new surface
	if(sprite.getSurface()->flags & SDL_SRCCOLORKEY) {
		flipped = SDL_CreateRGBSurface(SDL_SWSURFACE, sprite.getSurface()->w, sprite.getSurface()->h, sprite.getSurface()->format->BitsPerPixel,
		sprite.getSurface()->format->Rmask, sprite.getSurface()->format->Gmask, sprite.getSurface()->format->Bmask, sprite.getSurface()->format->Amask  );
	} else {
		flipped = SDL_CreateRGBSurface(SDL_SWSURFACE, sprite.getSurface()->w, sprite.getSurface()->h, sprite.getSurface()->format->BitsPerPixel,
		sprite.getSurface()->format->Rmask, sprite.getSurface()->format->Gmask, sprite.getSurface()->format->Bmask, 0 );
	}
	// check to see if the surface must be locked
	if(SDL_MUSTLOCK(sprite.getSurface())) {
		SDL_LockSurface(sprite.getSurface());
	}
	for(int y = 0; y < sprite.getSurface()->h; y++) {
		for(int x = 0; x < sprite.getSurface()->w; x++) {
			if(val == FLIP_HORIZONTAL) {
				setPixel(flipped, sprite.getSurface()->w - 1 - x, y, sprite.getPixel(x,y));
			} else if(val == FLIP_VERTICAL) {
				setPixel(flipped, x, sprite.getSurface()->h - 1 - y, sprite.getPixel(x,y));
			}
		}
	}
	//Copy color key
	if(sprite.getSurface()->flags & SDL_SRCCOLORKEY) {
		SDL_SetColorKey(flipped, SDL_RLEACCEL|SDL_SRCCOLORKEY, sprite.getSurface()->format->colorkey );
	}
	if(SDL_MUSTLOCK(sprite.getSurface())) {
		SDL_UnlockSurface(sprite.getSurface());
	}
	SDL_FreeSurface(sprite.getSurface());
	sprite.setSurface(flipped);
}

void Sprite::flipVertical(Sprite & sprite) {
    return flip(sprite, FLIP_VERTICAL);
}

void Sprite::flipHorizontal(Sprite & sprite) {
    return flip(sprite, FLIP_HORIZONTAL);
}


void Sprite::rotate90(Sprite & sprite) {
    rotate(sprite, 90);
}

void Sprite::rotate180(Sprite & sprite) {
    rotate(sprite, 180);
}

void Sprite::rotate270(Sprite & sprite) {
    rotate(sprite, 270);
}

void Sprite::rotate(Sprite & sprite, const int & deg) {
	SDL_Surface* rotated;
	int w,h;
	if(deg == 90 || deg == 270) {
		w = sprite.getHeight() * sprite.m_maxFrames;
		h = sprite.getWidth();
	} else if(deg == 180) {
		w = sprite.getWidth() * sprite.m_maxFrames;
		h = sprite.getHeight();
	} else {
		return;
	}
	// create a new surface
	if(sprite.getSurface()->flags & SDL_SRCCOLORKEY) {
		rotated = SDL_CreateRGBSurface(SDL_SWSURFACE, w, h, sprite.getSurface()->format->BitsPerPixel,
		sprite.getSurface()->format->Rmask, sprite.getSurface()->format->Gmask, sprite.getSurface()->format->Bmask, sprite.getSurface()->format->Amask  );
	} else {
		rotated = SDL_CreateRGBSurface(SDL_SWSURFACE, w, h, sprite.getSurface()->format->BitsPerPixel,
		sprite.getSurface()->format->Rmask, sprite.getSurface()->format->Gmask, sprite.getSurface()->format->Bmask, 0 );
	}
		if(SDL_MUSTLOCK(sprite.getSurface())) {
		SDL_LockSurface(sprite.getSurface());
	}
	for(int i = 0; i < sprite.m_maxFrames; i++) {
		for(int y = 0; y < sprite.getHeight(); y++) {
			for(int x = 0; x < sprite.getWidth(); x++) {
				if(deg == 90) {
					setPixel(rotated, sprite.getHeight() * i + sprite.getHeight() - y - 1, x, sprite.getPixel((i * sprite.getWidth()) + x,y));
				} else if(deg == 180) {
					setPixel(rotated, sprite.getWidth() * i + sprite.getWidth() - x - 1, rotated->h - y - 1, sprite.getPixel((i * sprite.getWidth()) + x,y));
				} else if(deg == 270) {
					setPixel(rotated, sprite.getHeight() * i + y, rotated->h - x - 1, sprite.getPixel((i * sprite.getWidth()) + x,y));
				} else {
					return;
				}
			}
		}
	}
	sprite.m_width = rotated->w/sprite.m_maxFrames;
	sprite.m_height = rotated->h;
	if(sprite.getSurface()->flags & SDL_SRCCOLORKEY) {
		SDL_SetColorKey(rotated, SDL_RLEACCEL|SDL_SRCCOLORKEY, sprite.getSurface()->format->colorkey );
	}
	if(SDL_MUSTLOCK(sprite.getSurface())) {
		SDL_UnlockSurface(sprite.getSurface());
	}
	SDL_FreeSurface(sprite.getSurface());
	sprite.setSurface(rotated);
}



void Sprite::fade(Sprite & sprite, const float & fade) {
    return;
}

void Sprite::reverseAnimation(Sprite & sprite) {
	SDL_Surface* reversed = NULL;
	// create a new surface
	if(sprite.getSurface()->flags & SDL_SRCCOLORKEY) {
		reversed = SDL_CreateRGBSurface(SDL_SWSURFACE, sprite.getSurface()->w, sprite.getSurface()->h, sprite.getSurface()->format->BitsPerPixel,
		sprite.getSurface()->format->Rmask, sprite.getSurface()->format->Gmask, sprite.getSurface()->format->Bmask, sprite.getSurface()->format->Amask );
	} else {
		reversed = SDL_CreateRGBSurface(SDL_SWSURFACE, sprite.getSurface()->w, sprite.getSurface()->h, sprite.getSurface()->format->BitsPerPixel,
		sprite.getSurface()->format->Rmask, sprite.getSurface()->format->Gmask, sprite.getSurface()->format->Bmask, 0);
	}
	// check to see if the surface must be locked
	if(SDL_MUSTLOCK(sprite.getSurface())) {
		SDL_LockSurface(sprite.getSurface());
	}

	for(int f = 0; f < sprite.m_maxFrames; f++) {
		for(int y = 0; y < sprite.getSurface()->h; y++) {
			for(int x = 0; x < sprite.getWidth(); x++) {
				setPixel(reversed, (sprite.m_maxFrames - f - 1)*sprite.getWidth() + x , y, sprite.getPixel(f*sprite.getWidth()+x,y));
			}
		}
	}
	//Copy color key
	if(sprite.getSurface()->flags & SDL_SRCCOLORKEY) {
		SDL_SetColorKey(reversed, SDL_RLEACCEL|SDL_SRCCOLORKEY, sprite.getSurface()->format->colorkey );
	}
	if(SDL_MUSTLOCK(sprite.getSurface())) {
		SDL_UnlockSurface(sprite.getSurface());
	}
	SDL_FreeSurface(sprite.getSurface());
	sprite.setSurface(reversed);
}

void Sprite::stretchX(Sprite & sprite, const float & stretchX) {
    return stretch(sprite, stretchX, 100);
}

void Sprite::stretchY(Sprite & sprite, const float & stretchY) {
    return stretch(sprite, 100, stretchY);
}

void Sprite::zoom(Sprite & sprite, const float & zoom) {
    return stretch(sprite, zoom,zoom);
}


void Sprite::stretch(Sprite & sprite, float stretchX, float stretchY) {
	SDL_Surface* zoomed;
	if(stretchX < 1 || stretchY < 1) {
		return;
	}
	stretchX /= 100;
	stretchY /= 100;
	SDL_PixelFormat * f = sprite.getSurface()->format;
	// create a new surface
	if(sprite.getSurface()->flags & SDL_SRCCOLORKEY) {
		zoomed = SDL_CreateRGBSurface(SDL_SWSURFACE, (int)(sprite.getSurface()->w*stretchX), (int)(sprite.getSurface()->h*stretchY), 
		f->BitsPerPixel, f->Rmask, f->Gmask, f->Bmask, f->Amask  );
	} else {
		zoomed = SDL_CreateRGBSurface(SDL_SWSURFACE, (int)(sprite.getSurface()->w*stretchX), (int)(sprite.getSurface()->h*stretchY), 
		f->BitsPerPixel, f->Rmask, f->Gmask, f->Bmask, 0);
	}
	if(SDL_MUSTLOCK(sprite.getSurface())) {
		SDL_LockSurface(sprite.getSurface());
	}
	int zoomedWidth = zoomed->w/sprite.m_maxFrames;
	int zoomedHeight = zoomed->h/sprite.m_maxRows;

	for(int y = 0; y < (zoomedHeight*sprite.m_maxRows); y++) { //really, please don't. no really. DON'T.
		for(int x = 0; x < zoomedWidth; x++) {
		// iterate over each animation as opposed to the whole sprite, to ensure that each animation is resized properly
			for(int i = 0; i < sprite.m_maxFrames; i++) {
				setPixel(zoomed, (zoomedWidth * i) + x, y, sprite.getPixel((sprite.getWidth() * i) + (int)(x/stretchX),(int)(y/stretchY) ));
			}
		}
	}

	sprite.m_width = zoomedWidth;
	sprite.m_height = zoomedHeight;

	if(sprite.getSurface()->flags & SDL_SRCCOLORKEY ) {
		SDL_SetColorKey(zoomed, SDL_RLEACCEL|SDL_SRCCOLORKEY, f->colorkey );
	}
	if(SDL_MUSTLOCK(sprite.getSurface())) {
		SDL_UnlockSurface(sprite.getSurface());
	}
	SDL_FreeSurface(sprite.getSurface());
	sprite.setSurface(zoomed);
}



void Sprite::setPixel(SDL_Surface* sprite, const int & x, const int & y, const Uint32 & pixel) {
	int bpp = sprite->format->BytesPerPixel;
	/* p is the address to the pixel we want to set */
	Uint8 *p = (Uint8 *)sprite->pixels + y * sprite->pitch + x * bpp;
	switch(bpp) {
	case 1:
		*p = pixel;
		break;
	case 2:
		*(Uint16 *)p = pixel;
		break;
	case 3:
		if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
			p[0] = (pixel >> 16) & 0xff;
			p[1] = (pixel >> 8) & 0xff;
			p[2] = pixel & 0xff;
		} else {
			p[0] = pixel & 0xff;
			p[1] = (pixel >> 8) & 0xff;
			p[2] = (pixel >> 16) & 0xff;
		}
		break;
	case 4:
		*(Uint32 *)p = pixel;
		break;
	}
}

Uint32 Sprite::getPixel(const SDL_Surface * a_surface, const int & x, const int & y) {
	int bpp = a_surface->format->BytesPerPixel;
	/* p is the address to the pixel we want to retrieve */
	Uint8 *p = (Uint8 *)a_surface->pixels + y * a_surface->pitch + x * bpp;
	switch(bpp) {
	case 1:
		return *p;
	case 2:
		return *(Uint16 *)p;
	case 3:
		if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
			return p[0] << 16 | p[1] << 8 | p[2];
		} else {
			return p[0] | p[1] << 8 | p[2] << 16;
		}
		break;
	case 4:
		return *(Uint32 *)p;
	default:
		return 0;
	}
}

void Sprite::setHIndex(int a_hindex, const int &a_maxFrames)
{
	m_maxFrames = a_maxFrames;
	m_hindex = a_hindex;
}