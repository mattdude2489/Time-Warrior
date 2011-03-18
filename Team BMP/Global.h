#ifndef _GLOBAL_H_
#define _GLOBAL_H_

//============================
//General Definitions/Enums
//============================

//GENERAL
//----------------------------<
#define PI				(3.141593f)
#define MAX(a,b)		(a > b?a:b)
#define MIN(a,b)		(a < b?a:b)

#define SIGNED_8				unsigned char				(0x80)
#define SIGNED_16				unsigned short				(0x8000)
#define SIGNED_32				unsigned long				(0x80000000)
#define SIGNED_64				unsigned long long			(0x8000000000000000)

#define UINT32_ERROR_OOB		unsigned int				(0xBADBADFF)
#define UINT32_ERROR_MISC		unsigned int				(0xBADBAD00)
//---------------------------->


//SDL_SPRITE
//----------------------------<
#define FLIP_HORIZONTAL			unsigned char				(0x00)
#define FLIP_VERTICAL			unsigned char				(0x01)
//---------------------------->


//SDL_RESOURCE
//----------------------------<
//---------------------------->

//============================




//============================
//Game Variables
//============================

//============================

#endif