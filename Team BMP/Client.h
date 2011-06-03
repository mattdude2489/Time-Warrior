//The client class. Takes in stuff from server...gives it to other places. Like, say, World. xD.
//Wow world's getting ABUSED...I think that's a good thing.
//Created by Matt Morrill. Inevitably edited by everyone else in the project. =D

#pragma once
//Winsock includes
#include <winsock2.h>
//#include "winsockerrorhelp.h"

//Game includes
#include "World.h"
#include "Entity.h"
#include "Player.h"
#include "UserInput.h"
#include "Server.h"

#define BUFFER_SIZE_IS			512
#define PORT_NUM				7777 //Why not?

class Client
{
private:
	completeSocket sSocket; //The socket to the server.
	World * cWorld; //The client's World, which the info in sSocket changes.
	WSADATA wsaData;
	timeval t;
	Entity * listOfOtherEntities[3]; //The list of other entities/players from the clients.
	bool chatMessageWaiting;
	char messageWaiting[41];
public:
	Client() {init();}
	Client(World * world)
	{
		cWorld = world;
		init();
	}
	void init()
	{
		chatMessageWaiting = false;
		for(int i = 0; i < 41; i++)
			messageWaiting[i] = ' ';
	}
	~Client()
	{
		shutDown();
	}
	void shutDown()
	{
		shutdown(sSocket.cSocket, SD_BOTH);
		closesocket(sSocket.cSocket); //Shut down the sockets and release their data.
	}
	bool startClient() //Returns false is WSA startup failed or the Socket failed to initialize.
	{
		if(WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
		{
			printf("WSAStartup() failed.");
			return false;
		}
		//Initialize the socket.
		sSocket.cSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
		//If it failed, returns invalid socket. Break out.
		if(sSocket.cSocket == INVALID_SOCKET)
		{
			printf("Error: Socket() failed with error number %d\n", WSAGetLastError());
			return false;
		}
		//Construct local address. 
		memset(&sSocket.clnt_addr, 0, sizeof(sSocket.clnt_addr));
		t.tv_sec = 0; t.tv_usec = 100; //Again, 3 seconds. Why not?
		return true;
	}
	bool connectToServer(char * ipAddress)
	{
		sSocket.clnt_addr.sin_addr.s_addr = inet_addr(ipAddress);
		sSocket.clnt_addr.sin_port = htons(PORT_NUM);
		sSocket.clnt_addr.sin_family = AF_INET; //I forgot to set this...
		int err = 0;
		err = connect(sSocket.cSocket, (SOCKADDR*)&sSocket.clnt_addr, sizeof(sSocket.clnt_addr));
		//I think I actually need to make this non-blocking first...
		if(err == SOCKET_ERROR)
		{
			printf("Fatal Error: %d\n", WSAGetLastError());
			shutdown(sSocket.cSocket, SD_BOTH);
			closesocket(sSocket.cSocket);
			return false;
		}
		unsigned long iMode = 1;
		err = ioctlsocket(sSocket.cSocket, FIONBIO, &iMode);
		if(err != NO_ERROR)
		{
			printf("ioctlsocket failed with error: %ld\n", err);
		}
		return true;
	}
	void setWorld(World * world) {cWorld = world;}
	World * getWorld() {return cWorld;}
	bool sendBufferToServer() {return false;}
	void getChatMessage(int start, int end)
	{
		//The message is in the recv_buf of sSocket. Get it and grab it.
		for(int i = start; i < end; i++) //To get rid of the C and the ' ' characters.
		{
			messageWaiting[i-start] = sSocket.recv_buf[i];
		}
		chatMessageWaiting = true;
	}
	char * getChat() //I guess I'll just have to remember to call clearChat immediately after getChat.
	{
		return messageWaiting;
	}
	void clearChat() {init();}
	void run() //Sends and Received data from the server. Because this is a client, it needs no FD_SET(listen_socket).
	{
		fd_set readfds, writefds;
		FD_ZERO(&readfds);
		FD_ZERO(&writefds);
		if(sSocket.cSocket != SOCKET_ERROR)
		{
			FD_SET(sSocket.cSocket, &readfds);
			FD_SET(sSocket.cSocket, &writefds);
		}

		int nfds = select(0, &readfds, &writefds, NULL, &t);
		if(nfds == SOCKET_ERROR)
		{
			//Something WRONG...
			printf("ClientSide Error: %d\n", WSAGetLastError());
			return;
		}

		if(FD_ISSET(sSocket.cSocket, &readfds))
		{
			//There is information in the buffer to receive.
			updateBuffer();
		}
		if(FD_ISSET(sSocket.cSocket, &writefds))
		{
			//There can be information in the buffer to write to. Do so.
			sendBufferToServer();
		}
	}
	void updateWorld() 
	{
		//For now, I'll just try to get a chat set up.

	}
	void sendMessage(char * chatMessage) //Mostly used for chat messages only.
	{

	}
	/**
	Buffer Key:
	P - Player Update
	M - Minion/Boss Update.
	E - Entity Update/Creation
	Y - ACK to update Yourself.
	C - Chat Message

	Buffer Style:
	C:		C "Insert Message Here"/
	P:		P CurrentWorld XCoord YCoord HP/ 
	Y:		Y/
	M:		M CurrentWorld ID XCoord YCoord HP currentTarget/
	E:		E CurrentWorld ID XCoord YCoord Type SubType SubSubType currentTarget/
	**/
	void updateBuffer() 
	{
		int error = recv(sSocket.cSocket, sSocket.recv_buf, sizeof(sSocket.recv_buf), 0);
		//receive the data and update the world with it. For now, just try...to...oh shit!
		if(error == SOCKET_ERROR)
		{
			//Um...yeah. Something went wrong.
			printf("What went wrong: %d\n", WSAGetLastError());
		}
		else
		{
			int s = 0, e = 0;
			char c = sSocket.recv_buf[s];
			while(c != 0)
			{
				switch(c)
				{
				case 'C':
					e = s;
					c = sSocket.recv_buf[s];
					while(c != '/')
					{
						e++;
						c = sSocket.recv_buf[e];
					}
					getChatMessage(s, e);
					s = e;
					break;
				case 'P':
					break;
				case 'Y':
					break;
				case 'M':
					break;
				case 'E':
					break;
				}
			}
		}
	}
};