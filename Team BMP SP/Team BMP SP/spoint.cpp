#include "spoint.h"
#include "graphics_constants.h"
#include "srect.h"

const SPoint SPoint::ZERO(0,0);
const int SPoint::NUM_DIMENSIONS = sizeof(SPoint)/sizeof(short);	// 2

SPoint SPoint::flipped(const int & a_flip, const SPoint & dim) const
{
	int nx = ((a_flip&VSDL_FLIP_X)?(( -((a_flip&VSDL_FLIP_XY)?dim.y:dim.x)-1)-x):x), 
		ny = ((a_flip&VSDL_FLIP_Y)?(( -((a_flip&VSDL_FLIP_XY)?dim.x:dim.y)-1)-y):y);
	return ((a_flip&VSDL_FLIP_XY)?(SPoint(ny,nx)):(SPoint(nx,ny)));
}
SPoint SPoint::flipped(const int & a_flip) const
{
	int nx = ((a_flip&VSDL_FLIP_X)?(-x):x), 
		ny = ((a_flip&VSDL_FLIP_Y)?(-y):y);
	return ((a_flip&VSDL_FLIP_XY)?(SPoint(ny,nx)):(SPoint(nx,ny)));
}

/** 
 * replaces a nested for-loop, where there would be one for-loop per
 * dimension, and the point would iterate from a_begin.x to a_end.x, and
 * after iterating x, it would iterate a_begin.y to a_end.y, and same for
 * z, and t, etc...
 * the iteration will be rectangluar, meaning that the point will never 
 * exist before a_begin, and never after a_end, on any dimension.
 * @param i_out which dimension to iterate (should probably be 0 every time). 
 * will come out of this function indicating which dimension incremented last
 * @param a_begin where to start iterating from. for example, Point(0,0,0)
 * @param a_end where to end the iteration. for example, Point(width, 
 * height, depth)
 * @return true if this point can still iterate
 */
int SPoint::iterate(int & i_out, const SPoint & a_begin, const SPoint & a_end)
{
	// as long as we're iterating in a valid dimension
	while(i_out < SPoint::NUM_DIMENSIONS)
	{
		// if iterating along this dimension does not reach the end
		if(++(&x)[i_out] < (&a_end.x)[i_out])
		{
			// one iteration completed without problem
			return true;
		}
		// if we reached the end of the dimension, reset this dimension
		(&x)[i_out] = (&a_begin.x)[i_out];
		// advance the next dimension
		++i_out;
	}
	return false;
}

int SPoint::iterate(int & i_out, const SRect & a_area)
{
	// as long as we're iterating in a valid dimension
	while(i_out < SPoint::NUM_DIMENSIONS)
	{
		// if iterating along this dimension does not reach the end
		if(++(&x)[i_out] < (((signed short *)(&a_area.w))[i_out]+((signed short *)(&a_area.x))[i_out]))
		{
			// one iteration completed without problem
			return true;
		}
		// if we reached the end of the dimension, reset this dimension
		(&x)[i_out] = ((signed short *)(&a_area.x))[i_out];
		// advance the next dimension
		++i_out;
	}
	return false;
}