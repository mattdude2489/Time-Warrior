#pragma once
#include "SDL/SDL.h"	// for the SDL_Rect extension...
#include "spoint.h"

/**
 * a rectangle class that can be interfaced as a x,y,w,h rectangle as well as a
 * 2-corners rectangle (min, max):
 *
 * minx,miny
 * ^--W--+
 * |     |
 * H     |
 * | maxx,maxy
 * +-----^
 *
 * @note: using getWidth() and getHeight() methods treats the height and width
 * as signed, which is a change from the SDL_Rect spec
 */
struct SRect : public SDL_Rect
{
	/**
	 * given min and max points, makes this a valid inscribed rectangle
	 * @param a_min minimum x/y coordinate of the rectangle
	 * @param a_max maximum x/y coordinate of the rectangle
	 */
	inline void set(const SPoint & a_min, const SPoint & a_max)
	{
		x=a_min.x;
		y=a_min.y;
		w=a_max.x-x;
		h=a_max.y-y;
	}

	/** given points in arbitrary order, makes this an inscribed rectangle */
	void setFromPoints(const SPoint & a, const SPoint & b)
	{
		SPoint min(((a.x<b.x)?a.x:b.x),((a.y<b.y)?a.y:b.y));
		SPoint max(((a.x>b.x)?a.x:b.x),((a.y>b.y)?a.y:b.y));
		set(min, max);
	}

	/** creates a valid rectangle inscribed into two points in arbitrary order */
	SRect(const SPoint & a, const SPoint & b)
	{
		setFromPoints(a,b);
	}

	/** copy constructor */
	SRect(const SDL_Rect & r)
	{
		*((SDL_Rect*)this) = r;
	}

	/** initialization method */
	inline void set(const int & a_x, const int & a_y, const int & a_w, const int & a_h)
	{x=a_x;y=a_y;w=a_w;h=a_h;}

	/** sets the rectangle to be all zeros (zero size at the origin) */
	inline void clear(){set(0,0,0,0);}

	/** default constructor creates an invalid rectangle (zero size) at the origin */
	SRect(){clear();}

	/** complete constructor */
	SRect(const int & x, const int & y, const int & w, const int & h){set(x,y,w,h);}

	/** copy method */
	inline void set(const SDL_Rect & r){x=r.x;y=r.y;w=r.w;h=r.h;}
	/** set the position of the rectangle (upper-left hand corner x/y, moves the rectangle) */
	inline void setPosition(const SPoint & p){x=p.x;y=p.y;}
	/** sets the size of the rectangle (width/height) */
	inline void setDimension(const SPoint & p){w=p.x;h=p.y;}
	/** gets the position of the rectangle (upper-left hand corner x/y) */
	inline SPoint getPosition() const {return SPoint(x,y);}
	/** gets the dimensions of the rectangle (width/height) */
	inline SPoint getDimension() const {return SPoint(w,h);}
	/** @return x position of rectangle (left edge location) */
	inline short getX() const {	return x;	}
	/** @return y position of rectangle (top edge location) */
	inline short getY() const {	return y;	}
	/** @return left edge location */
	inline short getMinX() const {	return x;	}
	/** @return right edge location */
	inline short getMaxX() const {	return x+(signed)w;	}
	/** @return top edge location */
	inline short getMinY() const {	return y;	}
	/** @return bottom edge location */
	inline short getMaxY() const {	return y+(signed)h;	}
	/** @return upper-left corner */
	inline SPoint getMin() const {	return SPoint(getMinX(),getMinY());	}
	/** @return lower-right corner */
	inline SPoint getMax() const {	return SPoint(getMaxX(),getMaxY());	}
	/** @return width */
	inline short getWidth() const {	return (signed)w;	}
	/** @return height */
	inline short getHeight() const{	return (signed)h;	}
	/** set the x position value (moves the rectangle) */
	inline void setX(const int & a_value){	x = a_value;	}
	/** set the y position value (moves the rectangle) */
	inline void setY(const int & a_value){	y = a_value;	}
	/** set the width value (moves the max value) */
	inline void setWidth(const int & a_value){	w=a_value;	}
	/** set the height value (moves the max value) */
	inline void setHeight(const int & a_value){	h=a_value;	}
	/** set the left-edge location (resizes the rectangle) */
	inline void setMinX(const int & a_value){	w+=x-a_value;x=a_value;	}
	/** set the right-edge location (resizes the rectangle) */
	inline void setMaxX(const int & a_value){	w=a_value-x;	}
	/** set the top-edge location (resizes the rectangle) */
	inline void setMinY(const int & a_value){	h+=y-a_value;y=a_value;	}
	/** set the bottom-edge location (resizes the rectangle) */
	inline void setMaxY(const int & a_value){	h=a_value-y;	}
	/** set the upper-left corner location (resizes the rectangle) */
	inline void setMin(const SPoint & p){setMinX(p.x);setMinY(p.y);}
	/** set the lower-right corner location (resizes the rectangle) */
	inline void setMax(const SPoint & p){setMaxX(p.x);setMaxY(p.y);}
	/** @return true if the width and height are both zero */
	inline bool isZero(){return !w && !h;}
	/** @return true if the width and height are both greater than one */
	inline bool isValid()const{return getWidth()>0&&getHeight()>0;}
	/** @return if this intersects (overlaps at all) the given rectangle */
	inline bool intersects(const SRect & r)
	{
		return	!( r.getMaxX() < getMinX() || r.getMinX() > getMaxX()
				|| r.getMaxY() < getMinY() || r.getMinY() > getMaxY() );
	}
	/** @return true if this rectangle contains the given location */
	inline bool contains(const SPoint & p)
	{
		return getMinX() <= p.x && getMinY() <= p.y
			&& getMaxX() >  p.x && getMaxY() >  p.y;
	}
	/** @return true if this and the given rectangle have identical values */
	inline bool equals(const SRect & r){return x==r.x&&y==r.y&&w==r.w&&h==r.h;}
	/** adds a given rectangle's area to this rectangle (may resize the rectangle) */
	void add(const SRect & r)
	{
		if(r.getMinX() < getMinX())	setMinX(r.getMinX());
		if(r.getMaxX() > getMaxX())	setMaxX(r.getMaxX());
		if(r.getMinY() < getMinY())	setMinY(r.getMinY());
		if(r.getMaxY() > getMaxY())	setMaxY(r.getMaxY());
	}
	/** makes this rectangle only the intersection with the given rectangle */
	void clip(const SRect & r)
	{
		if(r.getMinX() > getMinX())	setMinX(r.getMinX());
		if(r.getMaxX() < getMaxX())	setMaxX(r.getMaxX());
		if(r.getMinY() > getMinY())	setMinY(r.getMinY());
		if(r.getMaxY() < getMaxY())	setMaxY(r.getMaxY());
	}
	/** @return what this rectangle would be if it were clipped */
	SRect clipped(const SRect & a_clippingRect)
	{
		SRect r(*this);
		r.clip(a_clippingRect);
		return r;
	}
	/** reduces the rectangles size in all directions by the given value */
	void inset(const int & a_border)
	{set(x+a_border, y+a_border, w-a_border*2, h-a_border*2);}
	/** @return what this rectangle would be if it were inset the given value */
	SRect insetted(const int & a_border)
	{SRect insetRect(*this);insetRect.inset(a_border);return insetRect;}
	/** @return a rectangle moved over a_direction units (this rectangle is a unit-size) */
	SRect unitsOver(const SPoint & a_direction)
	{
		return SRect(x+getWidth()*a_direction.getX(),
			y+getHeight()*a_direction.getY(),getWidth(),getHeight());
	}
	/** add a_direction to the rectangle's position */
	void move(const SPoint & a_direction)
	{
		x+=a_direction.getX();
		y+=a_direction.getY();
	}
	/** @return this rectangle if it were moved a_direction */
	SRect moved(const SPoint & a_direction)
	{
		SRect r(*this);
		r.move(a_direction);
		return r;
	}
	/** used by getField() and setField() */
	static const int X = 0, Y = 1;
	static const int MIN = 2;	//SPoint::NUM_DIMENSIONS*1
	/** used by getField() and setField() */
	static const int MIN_X = MIN+X, MIN_Y = MIN+Y;
	static const int MAX = 4;	//SPoint::NUM_DIMENSIONS*2
	/** used by getField() and setField() */
	static const int MAX_X = MAX+X, MAX_Y = MAX+Y;
	static const int DIMENSION = 6;	//SPoint::NUM_DIMENSIONS*3
	/** used by getField() and setField() */
	static const int WIDTH = DIMENSION+X, HEIGHT = DIMENSION+Y;
	/** @param a_field (X, Y, MIN_X, MIN_Y, MAX_X, MAX_Y, WIDTH, HEIGHT) */
	short getField(const int & a_field) const
	{
		switch(a_field)
		{
		case MIN_X:	return getMinX();
		case MIN_Y:	return getMinY();
		case MAX_X:	return getMaxX();
		case MAX_Y:	return getMaxY();
		case X:		return getX();
		case Y:		return getY();
		case WIDTH:	return getWidth();
		case HEIGHT:	return getHeight();
		}
		return 0;
	}
	/** @param a_field (X, Y, MIN_X, MIN_Y, MAX_X, MAX_Y, WIDTH, HEIGHT) */
	void setField(const int & a_field, const short & a_value)
	{
		switch(a_field)
		{
		case MIN_X:	return setMinX(a_value);
		case MIN_Y:	return setMinY(a_value);
		case MAX_X:	return setMaxX(a_value);
		case MAX_Y:	return setMaxY(a_value);
		case X:		return setX(a_value);
		case Y:		return setY(a_value);
		case WIDTH:	return setWidth(a_value);
		case HEIGHT:	return setHeight(a_value);
		}
	}
	/** force this rectangle to overlap as much area as possible with the given rectangle */
	void keepBound(const SRect & area)
	{
		bool isSmallerThanArea;
		// go through X and Y dimensions
		for(int d = 0; d < 2; ++d)
		{
			isSmallerThanArea = getField(DIMENSION+d) < area.getField(DIMENSION+d);
			if(isSmallerThanArea
			?(getField(MIN+d) <= area.getField(MIN+d))
			:(getField(MIN+d) >= area.getField(MIN+d)))
				setField(d, area.getField(d));
			else if(isSmallerThanArea 
			?(getField(MAX+d) >= area.getField(MAX+d))
			:(getField(MAX+d) <= area.getField(MAX+d)))
				setField(d, area.getField(MAX+d)-getField(DIMENSION+d));
		}
	}
	/** multiply the dimensions and position of this rectangle */
	void multiply(const int & a_value){
		setX(getX()*a_value);
		setY(getY()*a_value);
		setWidth(getWidth()*a_value);
		setHeight(getHeight()*a_value);
	}
};

