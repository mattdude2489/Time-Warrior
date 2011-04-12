#include "../Global.h"

#include "../SDL_GUI.h"
#include "../function.h"

#include <stdio.h>
#include <conio.h>
#include <iostream>
using namespace std;
#include "../sdl/SDL.h"

#define TOOL_TITLE				"Map Editor"

#define SCREEN_WIDTH			(800)
#define SCREEN_HEIGHT			(600)
#define SCREEN_BPP				(32)

#define TILE_WIDTH				(32)
#define TILE_HEIGHT				(32)
#define TILE_X					(26)
#define TILE_Y					(20)
#define MAP_X					(TILE_X*TILE_WIDTH)
#define MAP_Y					(TILE_Y*TILE_WIDTH)

void printClick(char * a_param);
void printClick2(char * a_param);

void printClick(char * a_param) {
	unsigned int size;
	for(size = 0; a_param[size]; ++size);
	assert(size <= STRING_BUFFER_SIZE);
	char strBuffer[STRING_BUFFER_SIZE];
	StringVar varBuffer[9];
	unsigned int sbIndex = 0;
	unsigned int vbIndex = 0;
	char cur;
	for(unsigned int i = 0; i < size; ++i) {
		cur = a_param[i];
		strBuffer[sbIndex] = cur;
		if(cur == ';') {
			strBuffer[sbIndex] = 0;
			sbIndex = 0;
			StringVar * vCur = &varBuffer[vbIndex++];
			vCur->copyString(strBuffer);
			vCur->parseStr();
		}
		else
			++sbIndex;
	}
	void * temp = varBuffer[0].getVarReference();
	unsigned int type = varBuffer[0].getContainerType();
	char cParam = TYPECAST_CHAR_PARSE(temp, type);
	temp = varBuffer[1].getVarReference();
	type = varBuffer[1].getContainerType();
	unsigned char byParam = (unsigned)TYPECAST_CHAR_PARSE(temp, type);
	temp = varBuffer[2].getVarReference();
	type = varBuffer[2].getContainerType();
	short sParam = TYPECAST_SHORT_PARSE(temp, type);
	temp = varBuffer[3].getVarReference();
	type = varBuffer[3].getContainerType();
	int iParam = TYPECAST_LONG_PARSE(temp, type);
	temp = varBuffer[4].getVarReference();
	type = varBuffer[4].getContainerType();
	long long llParam = TYPECAST_LLONG_PARSE(temp, type);
	temp = varBuffer[5].getVarReference();
	type = varBuffer[5].getContainerType();
	float fParam = TYPECAST_FLOAT_PARSE(temp, type);
	temp = varBuffer[6].getVarReference();
	type = varBuffer[6].getContainerType();
	double dParam = TYPECAST_DOUBLE_PARSE(temp, type);
	temp = varBuffer[7].getVarReference();
	type = varBuffer[7].getContainerType();
	char * szParam = TYPECAST_STRING_PARSE(temp, type);
	temp = varBuffer[8].getVarReference();
	type = varBuffer[8].getContainerType();
	SDL_GUI_Button * thisButton = (SDL_GUI_Button *)TYPECAST_ADDRESS_PARSE(temp, type);

	Function func;
	func.m_func = printClick2;
	func.m_param = a_param;
	thisButton->setClickAction(func);
	cout << "Action 1!\nStringParam: " << a_param << "\ncParam: " << cParam << "\nbyParam: " << (int)byParam << "\nsParam: " << sParam << "\niParam: " << iParam << "\nllParam: " << llParam << "\nfParam: " << fParam << "\ndParam: " << dParam << "\nszParam: " << szParam << "\nthisButton: " << thisButton << endl;
}

void printClick2(char * a_param) {
	unsigned int size;
	for(size = 0; a_param[size]; ++size);
	assert(size <= STRING_BUFFER_SIZE);
	char strBuffer[STRING_BUFFER_SIZE];
	StringVar varBuffer[9];
	unsigned int sbIndex = 0;
	unsigned int vbIndex = 0;
	char cur;
	for(unsigned int i = 0; i < size; ++i) {
		cur = a_param[i];
		strBuffer[sbIndex] = cur;
		if(cur == ';') {
			strBuffer[sbIndex] = 0;
			sbIndex = 0;
			StringVar * vCur = &varBuffer[vbIndex++];
			vCur->copyString(strBuffer);
			vCur->parseStr();
		}
		else
			++sbIndex;
	}
	void * temp = varBuffer[0].getVarReference();
	unsigned int type = varBuffer[0].getContainerType();
	char cParam = TYPECAST_CHAR_PARSE(temp, type);
	temp = varBuffer[1].getVarReference();
	type = varBuffer[1].getContainerType();
	unsigned char byParam = (unsigned)TYPECAST_CHAR_PARSE(temp, type);
	temp = varBuffer[2].getVarReference();
	type = varBuffer[2].getContainerType();
	short sParam = TYPECAST_SHORT_PARSE(temp, type);
	temp = varBuffer[3].getVarReference();
	type = varBuffer[3].getContainerType();
	int iParam = TYPECAST_LONG_PARSE(temp, type);
	temp = varBuffer[4].getVarReference();
	type = varBuffer[4].getContainerType();
	long long llParam = TYPECAST_LLONG_PARSE(temp, type);
	temp = varBuffer[5].getVarReference();
	type = varBuffer[5].getContainerType();
	float fParam = TYPECAST_FLOAT_PARSE(temp, type);
	temp = varBuffer[6].getVarReference();
	type = varBuffer[6].getContainerType();
	double dParam = TYPECAST_DOUBLE_PARSE(temp, type);
	temp = varBuffer[7].getVarReference();
	type = varBuffer[7].getContainerType();
	char * szParam = TYPECAST_STRING_PARSE(temp, type);
	temp = varBuffer[8].getVarReference();
	type = varBuffer[8].getContainerType();
	SDL_GUI_Button * thisButton = (SDL_GUI_Button *)TYPECAST_ADDRESS_PARSE(temp, type);

	Function func;
	func.m_func = printClick;
	func.m_param = a_param;
	thisButton->setClickAction(func);
	cout << "Action 2!\nStringParam: " << a_param << "\ncParam: " << cParam << "\nbyParam: " << (int)byParam << "\nsParam: " << sParam << "\niParam: " << iParam << "\nllParam: " << llParam << "\nfParam: " << fParam << "\ndParam: " << dParam << "\nszParam: " << szParam << "\nthisButton: " << thisButton << endl;
}

int main ( int argc, char ** argv ) {
	//init SDL
	SDL_Init(SDL_INIT_EVERYTHING);

	//set the title of the tool
	SDL_WM_SetCaption(TOOL_TITLE, NULL);

	//create the screen surface
	SDL_Surface * screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE);
	//grab the screen's pixel format
	SDL_PixelFormat * scrFmt = screen->format;

	//create the map surface
	SDL_Surface * map = SDL_CreateRGBSurface( SDL_SWSURFACE, MAP_X, MAP_Y, SCREEN_BPP, scrFmt->Rmask, scrFmt->Gmask, scrFmt->Bmask, scrFmt->Amask);
	
	//create the screen rect
	SDL_Rect screenRect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	//create the map rect
	SDL_Rect mapRect = { 0, 0, MAP_X, MAP_Y };
	//fill the map rect with the color blue
	SDL_FillRect(map, &mapRect, 0x000000FF);
	//create a cam rect to browse through the map rect
	SDL_Rect camRect = { MAP_X/2 - SCREEN_WIDTH/2, MAP_Y/2 - SCREEN_HEIGHT/2, SCREEN_WIDTH, SCREEN_HEIGHT };
	
	//hold boolean for whether the game should run
	bool running = true;
	//hold event variable for input
	SDL_Event e;
	
	SDL_GUI_Button btn;

	const char param[] = "\'A';c123;s3;i65;l932;f100.125;d250.0625;\"Hello World! This is a string inside of a string!\";";
	unsigned int size;
	for(size = 0; param[size]; ++size);
	char paramCopy[STRING_BUFFER_SIZE];
	for(unsigned int i = 0; i < STRING_BUFFER_SIZE; ++i)
		paramCopy[i] = 0;
	for(unsigned int i = 0; i < size; ++i)
		paramCopy[i] = param[i];
	StringVar address;
	address.setContainer((void *)&btn);
	const char * addParam = address.getString();
	for(unsigned int i = 0; addParam[i]; ++i)
		paramCopy[size++] = addParam[i];
	paramCopy[size] = ';';

	Function func;
	func.m_func = printClick;
	func.m_param = paramCopy;

	btn.setGUIPos(0, 0);
	btn.setScreen(screen);
	btn.loadGraphic("../Sprites/test_button.bmp");
	btn.setClickAction(func);

	Sint32 mouseX = 0, mouseY = 0;
	bool click = false;
	bool rolledOn = false;

	//hold variables for time
	Uint32 now, then, soon, ms;
	//set then time to the current time
	then = SDL_GetTicks();
	//while the game is running
	while(running) {
		//while events need to be polled
		while(SDL_PollEvent(&e)) {
			//check the event type
			switch(e.type) {
			//if quit event
			case SDL_QUIT:
				//set running to false
				running = false;
				break;
			case SDL_MOUSEBUTTONDOWN:
				click = true;
				mouseX = e.motion.x;
				mouseY = e.motion.y;
				break;
			case SDL_MOUSEMOTION:
				mouseX = e.motion.x;
				mouseY = e.motion.y;
				break;
			case SDL_KEYDOWN:
				switch(e.key.keysym.sym) {
				case 0:
				default:
					break;
				}
				break;
			case SDL_KEYUP:
				switch(e.key.keysym.sym) {
				case 0:
				default:
					break;
				}
				break;
			}
		}
		//set now to the current time
		now = SDL_GetTicks();
		//set the ms passed as now - then
		ms = now - then;
		//set then to now
		then = now;

		if(btn.mouseCollide(mouseX, mouseY)) {
			if(!rolledOn) {
				rolledOn = true;
				btn.onRollOver(mouseX, mouseY);
			}
			else
				btn.onHover(mouseX, mouseY);
			if(click)
				btn.onClick(mouseX, mouseY);
		}
		else {
			if(rolledOn) {
				rolledOn = false;
				btn.onRollOff(mouseX, mouseY);
			}
			else
				btn.onInactive(mouseX, mouseY);
		}

		//reset the screen rect
		SDL_FillRect(screen, &screenRect, 0x00000000);
		//blit the map rect (using the cam rect) onto the screen rect
		SDL_BlitSurface(map, &camRect, screen, &screenRect);
		btn.draw();
		//flip the screen buffer
		SDL_Flip(screen);
		
		//reset the mouse click
		click = false;

		//set soon to now plus the default throttle event
		soon = now + DEFAULT_THROTTLE;

		//while events don't need to be polled, and the current time is less than soon
		while(!SDL_PollEvent(0) && SDL_GetTicks() < soon)
			//delay the app by one milisecond
			SDL_Delay(1);
	}
	
	//free the map surface
	SDL_FreeSurface(map);
	//free the screen surface
	SDL_FreeSurface(screen);

	//quit SDL
	SDL_Quit();

	return 0;
}
