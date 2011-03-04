#pragma once

#define VSDL_FLIP_NONE		0
#define VSDL_FLIP_X			(1<<0)
#define VSDL_FLIP_Y			(1<<1)
#define VSDL_FLIP_XY		(1<<2)
#define VSDL_FLIP_90		(VSDL_FLIP_XY|            VSDL_FLIP_X)
#define VSDL_FLIP_180		(             VSDL_FLIP_Y|VSDL_FLIP_X)
#define VSDL_FLIP_270		(VSDL_FLIP_XY|VSDL_FLIP_Y            )
#define VSDL_FLIP_ALL		(VSDL_FLIP_XY|VSDL_FLIP_Y|VSDL_FLIP_X)

#define VSDL_FLIP_CW(f)		{f^=((f&VSDL_FLIP_XY)?VSDL_FLIP_Y:VSDL_FLIP_X);f^=VSDL_FLIP_XY;}
#define VSDL_FLIP_CCW(f)	{f^=((f&VSDL_FLIP_XY)?VSDL_FLIP_X:VSDL_FLIP_Y);f^=VSDL_FLIP_XY;}

