//The client class. Takes in stuff from server...gives it to other places. Like, say, World. xD.
//Wow world's getting ABUSED...I think that's a good thing.
//Created by Matt Morrill. Inevitably edited by everyone else in the project. =D

#pragma once
//Winsock includes
#include <winsock2.h>
#include "winsockerrorhelp.h"

//Game includes
#include "World.h"
#include "Entity.h"
#include "Player.h"
#include "UserInput.h"

#define BUFFER_SIZE_IS			512

class Client
{
private:
	SOCKET sockToServer;
	char buffer[BUFFER_SIZE_IS];
	World * cWorld;
public:
	Client();
	Client(World * world);
	~Client();
	bool startClient();
	bool connectToServer(char * ipAddress, short portNum);
	void addWorld(World * world);
	World * getWorld();
	bool sendBufferToServer();
	void updateWorld();
	void updateBuffer();
};