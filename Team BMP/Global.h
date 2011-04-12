#ifndef _GLOBAL_H_
#define _GLOBAL_H_

//============================
//Definitions/Enums
//============================

//GENERAL
//----------------------------<
#define GAME_TITLE				"Time Warrior: Colosseum of Time"
#define BUFFER_SIZE				(4096)
#define STRING_BUFFER_SIZE		(256)
#define PI						(3.141593f)
#define FLOAT_PRECISION			(7)
#define DOUBLE_PRECISION		(15)
#define MAX(a,b)				(a > b?a:b)
#define MIN(a,b)				(a < b?a:b)
#define DEFAULT_THROTTLE		unsigned long				(16)

#define SIGNED_8				unsigned char				(0x80)
#define SIGNED_16				unsigned short				(0x8000)
#define SIGNED_32				unsigned long				(0x80000000)
#define SIGNED_64				unsigned long long			(0x8000000000000000)

#define UINT32_ERROR_OOB		unsigned long				(0xBADBADFF)
#define UINT32_ERROR_MISC		unsigned long				(0xBADBAD00)
//---------------------------->

//FUNCTION
//----------------------------<
typedef void (*FunctionPtr)(char * a_param);

enum ParseContainer {
	PARSE_NONE,
	PARSE_BYTE,
	PARSE_WORD,
	PARSE_DWORD,
	PARSE_QWORD,
	PARSE_FLOAT,
	PARSE_DOUBLE,
	PARSE_STRING,
	PARSE_ADDRESS,
};

enum ParseMode {
	PARSE_WHITESPACE,
	PARSE_HEADER,
	PARSE_DATA,
	PARSE_POST_HEADER,
	PARSE_END,
};

#define TYPECAST_CHAR_PARSE(data_reference, container_type)			(container_type == PARSE_BYTE?*(char *)data_reference:0)
#define TYPECAST_SHORT_PARSE(data_reference, container_type)		(container_type == PARSE_WORD?*(short *)data_reference:0)
#define TYPECAST_LONG_PARSE(data_reference, container_type)			(container_type == PARSE_DWORD?*(long *)data_reference:0)
#define TYPECAST_LLONG_PARSE(data_reference, container_type)		(container_type == PARSE_QWORD?*(long long *)data_reference:0)
#define TYPECAST_FLOAT_PARSE(data_reference, container_type)		(container_type == PARSE_FLOAT?*(float *)data_reference:0)
#define TYPECAST_DOUBLE_PARSE(data_reference, container_type)		(container_type == PARSE_DOUBLE?*(double *)data_reference:0)
#define TYPECAST_STRING_PARSE(data_reference, container_type)		(container_type == PARSE_STRING?(char *)data_reference:0)
#define TYPECAST_ADDRESS_PARSE(data_reference, container_type)		(container_type == PARSE_ADDRESS?data_reference:0)

#define PARSE_CHAR				unsigned long				(0x00000001)
#define PARSE_NEGATIVE			unsigned long				(0x00000002)
#define PARSE_NUMERIC			unsigned long				(0x00000004)
#define PARSE_HEX				unsigned long				(0x00000008)
#define PARSE_REAL				unsigned long				(0x00000010)
#define PARSE_FINISHED			unsigned long				(0x00000020)
#define PARSE_TRAILING_DATA		unsigned long				(0x00000040)
#define PARSE_SYNTAX_ERROR		unsigned long				(0x80000000)
//---------------------------->

//SDL_SPRITE
//----------------------------<
#define FLIP_HORIZONTAL			unsigned char				(0x00)
#define FLIP_VERTICAL			unsigned char				(0x01)
//---------------------------->


//SDL_RESOURCE
//----------------------------<
#define RESOURCE_UNKNOWN		unsigned long				(0x00000000)
#define RESOURCE_SURFACE		unsigned long				(0x00000001)
#define RESOURCE_SPRITE			unsigned long				(0x00000002)
#define RESOURCE_SPLASH			unsigned long				(0x00000003)
#define RESOURCE_MENU			unsigned long				(0x00000004)
//---------------------------->

//SDL_GUI
//----------------------------<
#define MARGIN_PX				(2)
#define BORDER_PX				(3)
#define PADDING_PX				(2)
#define BORDER_COL				(0x00FF0000)
#define BACKGROUND_COL			(0x00333333)
#define HIGHLIGHT_COL_OFF		(0x00003333)
#define GUI_POS_OFF				(MARGIN_PX + BORDER_PX + PADDING_PX)
#define GUI_DIM_OFF				(GUI_POS_OFF << 1)
//---------------------------->

//============================




//============================
//Variables
//============================

//============================

#endif
