/* Event base class. This class handles the calls for all the major events.

Events are labeled 1-9 on the map. For every world, there is an Int (32 bits) to house the flags for all of them.

Event 1 on the main world map is not the same as Event 1 within the Dungeons.
*/

#pragma once
#include "World.h"

/*
This class is used for Events, essentially cutscenes. The NPC dialogue will be used for gameplay; this is out of gameplay.
*/
class EventHandler
{
private:
	bool inEvent;
	char * nameOfCurrentFile; //This will be used only in changeWorlds, as it would load up the needed file every time it goes into an event.
public:
	EventHandler(); //Constructor
	~EventHandler(); //Destructor
	EventHandler(int currentWorld);
	void playEvent(int eventNum, World * currentWorld);
	void endEvent() { inEvent = false; }
	void changeWorldEvents(int currentWorld);
};