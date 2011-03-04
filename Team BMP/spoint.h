#pragma once

/** used to iterate around */
struct SRect;

/** an x/y coordinate or math-vector */
struct SPoint
{
	short x, y;
	/** complete constructor */
	SPoint(const int & x, const int & y):x(x),y(y){}
	/** copy constructor */
	SPoint(const SPoint & p):x(p.x),y(p.y){}
	/** default constructor (0,0) */
	SPoint():x(0),y(0){}
	/** initialize the point */
	inline void set(const int & a_x, const int & a_y){x=a_x;y=a_y;}
	/** copy another point */
	inline void set(const SPoint & p){x=p.x;y=p.y;}
	/** @return true if the point is 0,0 */
	inline bool isZero() const {return !x && !y;}
	/** @return true if the given point has identical values to this one */
	inline bool equals(const SPoint & p)const{return x==p.x&&y==p.y;}
	/** @return if both x and y are less than the given x and y */
	inline bool isLessThan(const SPoint & p)const{return x<p.x&&y<p.y;}
	/** @return if both x and y are greaterthan or equl to the given x and y */
	inline bool isGreaterThanOrEqualTo(const SPoint & p)const{return x>=p.x&&y>=p.y;}
	/** @return the x location */
	inline int getX() const {return x;}
	/** @return the y location */
	inline int getY() const {return y;}
	/** change the x location */
	inline void setX(const int & a_value){x=a_value;}
	/** change the y location */
	inline void setY(const int & a_value){y=a_value;}
	/** @return pointer to the dimension variables */
	inline const short * getDimensions()const{return &x;}
	/** used by getField */
	static const int X = 0, Y = 1;
	/** @param a_field (X, Y) */
	inline short getField(const int & a_field)const{return getDimensions()[a_field];}
	/** constant at the origin */
	static const SPoint ZERO;
	/** how many dimensions this vector keeps track of (2) */
	static const int NUM_DIMENSIONS;
	// math. TODO convince me that these can be operator-overloaded without confusing people
	/** add the given point to this one */
	void add(const SPoint & p){x+=p.x;y+=p.y;}
	/** add to this x value */
	inline void addX(const int & a_value){x+=a_value;}
	/** add to this y value */
	inline void addY(const int & a_value){y+=a_value;}
	/** @return the given poitn and this point added together */
	SPoint sum(const SPoint & p) const {return SPoint(x+p.x,y+p.y);}
	/** subtract the given point from this point */
	void subtract(const SPoint & p){x-=p.x;y-=p.y;}
	/** @return this point and the given point subtracted */
	SPoint difference(const SPoint & p) const {return SPoint(x-p.x,y-p.y);}
	/** divide this point by the given point */
	void divide(const SPoint & p){x/=p.x;y/=p.y;}
	/** @return this point divided by the given point */
	SPoint quotient(const SPoint & p) const {return SPoint(x/p.x,y/p.y);}
	/** divide this point's values by the given value */
	void divide(const int & a_value){x/=a_value;y/=a_value;}
	/** @return this point's values divided by the given value */
	SPoint quotient(const int & a_value) const {return SPoint(x/a_value,y/a_value);}
	/** mutiply this point's values by the given value */
	void multiply(const int & a_value){x*=a_value;y*=a_value;}
	/** @return this point's values multiplied by the given value */
	SPoint product(const SPoint & p) const {return SPoint(x*p.x,y*p.y);}
	/** mutiply this point by the given point */
	void multiply(const SPoint & p){x*=p.x;y*=p.y;}
	/** @return this point multiplied by the given point */
	SPoint product(const int & value) const {return SPoint(x*value,y*value);}
	/** flip this point using the given dimension as a flipping context */
	SPoint flipped(const int & a_flip, const SPoint & dim) const;
	/** flip this point around the origin */
	SPoint flipped(const int & a_flip) const;
	/** 
	 * replaces a nested for-loop, where there would be one for-loop per
	 * dimension, and the point would iterate from a_begin.x to a_end.x, and
	 * after iterating x, it would iterate a_begin.y to a_end.y, and same for
	 * z, and t, etc...
	 * the iteration will be rectangluar, meaning that the point will never 
	 * exist before a_begin, and never after a_end, on any dimension.
	 * @param a_begin where to start iterating from. for example, Point(0,0,0)
	 * @param a_end where to end the iteration. for example, Point(width, 
	 * height, depth)
	 * @return the dimension that was last iterated. returns -1 when all 
	 * dimensions have been iterated through
	 */
	int iterate(int & i_out, const SPoint & a_begin, const SPoint & a_end);
	/**
	 * example:<code>
	 * SPoint p(area.getMin());
	 * int dim;					// variable that keeps track of the last dimension iterated
	 * do{
	 * setPixelAt(p, a_color);	// code that uses the location
	 * dim = 0;					// iterate over the 0th dimension first if possible
	 * }while(p.iterate(dim, area));</code>
	 */
	int iterate(int & i_out, const SRect & a_area);
};
