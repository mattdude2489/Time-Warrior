//The main server class. This is the class that will do...just about everything. xD
//Originally made by Matthew Morrill, inevitably edited by just about everyone in the project.

#pragma once

//Socket includes
#include <winsock2.h>
#include "winsockerrorhelp.h"
#include "templateVector.h"

//Game includes that will be necessary.
#include "Entity.h"
#include "UserInput.h"
#include "World.h"

//Defines
#define BUFF_SIZE			512

class Server
{
private:
	World * sWorld;
	templateVector<SOCKET> sSockets;
	char buffer[BUFF_SIZE];
public:
	Server(); //Default constructor.
	Server(World * world); //Constructor that sets the World.
	~Server(); //Destructor that calls the shutDown.
	bool addSocket(char * ipAddress); //Adds a socket to the templateVector. Returns success.
	void sendBufferToClients(); //Sends the current buffer to all the clients that are connected.
	void shutDown(); //Shuts down Winsock.
	bool startServer(); //Starts the server up. Initializes winSock. Returns success.
	void updateBuffer(); //Updates the buffer through its connection to World.
	void changeWorld(World * w); //Changes the sWorld to the w World. Use only once.
	World * getWorld(); //Returns the world pointer.
	SOCKET getSocketAt(int index); //Returns the socket at the index.
}