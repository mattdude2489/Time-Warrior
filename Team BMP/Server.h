//The main server class. This is the class that will do...just about everything. xD
//Originally made by Matthew Morrill, inevitably edited by just about everyone in the project.

#pragma once

//Socket includes
#include <winsock2.h>
//#include "winsockerrorhelp.h"
#include "templateVector.h"

//Game includes that will be necessary.
#include "Entity.h"
#include "UserInput.h"
#include "World.h"

//Defines
#define BUFF_SIZE			512
#define PORT_NUM			7777 //Why not?

struct completeSocket
{
	SOCKET cSocket;
	struct sockaddr_in clnt_addr;
	char recv_buf[BUFF_SIZE];
	char send_buf[BUFF_SIZE];
	int bytesSentToServer;
	int bytesRecFromServer;
	int currentBufferSize;
};

class Server
{
private:
	SOCKET listen_socket;
	struct sockaddr_in srvr_addr; //Local address
	int clntLen;
	World * sWorld; //The server's pointer to the world. This will be used to alter stuff to the buffer.
	TemplateVector2<completeSocket> listOfClients; //The list of clients currently connected.
	Entity * listOfAdditionalEntities[3]; //The "entities" (AKA players) that are from the other clients.
	char buffer[BUFF_SIZE]; //The buffer that you will send to the clients.
	WSADATA wsaData;
	timeval t; //The time for the timeout function of "Select".
	char currentMessage[41];
	bool chatMessageWaiting;

public:
	Server() //Default constructor.
	{
		//Nothing in particular needs to happen. Start server will do ALL INITIALIZING.
	}
	Server(World * world) //Constructor that sets the World.
	{
		sWorld = world;
	}
	~Server() {shutDown();} //Destructor that calls the shutDown.
	bool addSocket(char * ipAddress) {return false;} //Adds a socket to the templateVector. Returns success.
	//Get message returns success if there is a message. If there isn't, it returns false. The parameter becomes the message if true.
	bool getMessage(char * message) {if(chatMessageWaiting) { message = currentMessage; return true;} else return false;}
	void reinitChat() {for(int i = 0; i < 41; i++) currentMessage[i] = ' '; chatMessageWaiting = false;}
	void sendMessage(char * chatMessage)
	{
		//Takes a message, puts it in the sendbuf for all connected clients. When they next receive packets, it will be in one of them.
		for(int i = 0; i < listOfClients.size(); i++)
		{
			//completeSocket * cs = &listOfClients.get(i);
			//Use a second for loop to append the chatMessage into each of the clients send_buffer.
			//Because the index can be different for each buffer, use each clients own currentBufferSize, server-side.
			int var = 0; //A random integer variable that will be used to determine the chatMessage's current index.
			for(int k = listOfClients.get(i).currentBufferSize; k < (sizeof(chatMessage)+listOfClients.get(i).currentBufferSize); k++)
			{
				listOfClients.get(i).send_buf[k] = chatMessage[var];
				var++;
			}
			listOfClients.get(i).currentBufferSize += sizeof(chatMessage); //Set the currentBufferSize to its higher amount.
		}
	}
	void sendBufferToClients(int clientToSendTo) //Sends the current buffer to the client specified, if there is still a connection.
	{
		completeSocket * clientSock;
		if(listOfClients.get(clientToSendTo).cSocket != SOCKET_ERROR)
		{
			clientSock = &listOfClients.get(clientToSendTo);
			clientSock->bytesSentToServer = send(clientSock->cSocket, clientSock->send_buf, sizeof(clientSock->send_buf), 0);
			//INSERT ERROR CHECKING.
		}
		else
		{

		}
			//Do nothing. YOU FAIL. Eventually there might be some error checking to say "Hai...Yeah I no here."
	} 
	void receiveBufferFromClients(int receiveFromClient) //Receives the buffer from the client specified.
	{
		completeSocket * clientSock;
		if(listOfClients.get(receiveFromClient).cSocket != SOCKET_ERROR)
		{
			clientSock = &listOfClients.get(receiveFromClient);
			clientSock->bytesRecFromServer = recv(clientSock->cSocket, clientSock->recv_buf, sizeof(clientSock->recv_buf), 0);
			if(clientSock->bytesRecFromServer == WSAEWOULDBLOCK) //INSERT ERROR CHECKING HERE.
			{
				//Hai, the socket is dead. Shut it down, close it, and then remove it from the templateVector.
				shutdown(clientSock->cSocket, SD_BOTH);
				closesocket(clientSock->cSocket);
				listOfClients.remove(receiveFromClient);
			}
			//This would do all the main checking for messages, right here. This would also call sendMessage.
			int currentBuffer = 0;
			while(clientSock->recv_buf[currentBuffer] != '#') //The ending symbol.
			{
				if(clientSock->recv_buf[currentBuffer] == 'C')
				{
					int totalAmount = 0;
					while(clientSock->recv_buf[currentBuffer] != '/') //The ending character for chat messages.
					{
						currentMessage[totalAmount] = clientSock->recv_buf[currentBuffer];
						currentBuffer++; totalAmount++;
					}
					sendMessage(currentMessage); //Send the message to ALL of the clients.
					chatMessageWaiting = true;
				}
			}
		}
	}
	void shutDown() //Shuts down Winsock.
	{
		shutdown(listen_socket, SD_BOTH);
		closesocket(listen_socket);
		for(int i = 0; i < listOfClients.size(); i++)
		{
			shutdown(listOfClients.get(i).cSocket, SD_BOTH);
			closesocket(listOfClients.get(i).cSocket);
		}
	}
	bool startServer() //Starts the server up. Initializes winSock. Returns true if successful.
	{
		if(WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
		{
			printf("WSAStartup() failed\n");
			return false;
		}
		//Initialize the listen socket.
		if((listen_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET)
		{
			printf("socket() failed due to error %d\n", GetLastError());
			return false;
		}
		//Construct local address structure.
		memset(&srvr_addr, 0, sizeof(srvr_addr));
		srvr_addr.sin_family = AF_INET;
		srvr_addr.sin_addr.s_addr = INADDR_ANY;
		srvr_addr.sin_port = htons(PORT_NUM);

		//Bind to local address.
		if(bind(listen_socket, (SOCKADDR*) &srvr_addr, sizeof(srvr_addr)) == SOCKET_ERROR)
		{
			printf("Bind() failed due to %d\n", GetLastError());
			//Because we opened the socket, now we gotta let it fail.
			shutdown(listen_socket, SD_BOTH);
			closesocket(listen_socket);
			return false;
		}

		int result = 0;
		result = listen(listen_socket, SOMAXCONN);
		if(result == SOCKET_ERROR)
		{
			printf("You failed a listen check. %d\n", GetLastError());
			return false;
		}
		unsigned long iMode = 1;
		clntLen = ioctlsocket(listen_socket, FIONBIO, &iMode);
		if(clntLen != NO_ERROR)
			printf("ioctlsocket failed with error: %ld\n", clntLen);
		clntLen = sizeof(srvr_addr);
		t.tv_sec = 0; t.tv_usec = 100; //Nearly 3 seconds for timeout.
		return true;
	}	
	void updateBuffer() //Updates the send_buf through its connection to World.
	{

	}
	void changeWorld(World * w) {sWorld = w;} //Changes the sWorld to the w World. Use only once.
	void updateWorld() //Update the world through the recv_buf.
	{
		
	}
	void run() //Listens to accept new connections, sets the FD_SETS required for SendBufferToClients.
	{
		updateBuffer(); //Update the send_buffer.
		fd_set readfds, writefds;
		FD_ZERO(&readfds);
		FD_ZERO(&writefds);
		//Create and Zero out the FD Sets.
		//Then set the listening socket to accept new connections.
		FD_SET(listen_socket, &readfds);
//		printf("Port number is: %d\n", ntohs(srvr_addr.sin_port));
		for(int i = 0; i < listOfClients.size(); i++)
		{
			FD_SET(listOfClients.get(i).cSocket, &readfds); //Sets the client sockets for read and write.
			FD_SET(listOfClients.get(i).cSocket, &writefds);
		}
		int nfds = select(0, &readfds, &writefds, NULL, &t);
		if(nfds == SOCKET_ERROR) //basic error catching.
		{
			//SOMETHING IS WRONG. I DON'T KNOW WHAT.
			shutDown();
			printf("This went wrong: %d\n", WSAGetLastError());
			return;
		}
		for(int i = 0; i < nfds; i++)
		{
			if(FD_ISSET(listen_socket, &readfds))
			{
				completeSocket cs;
				cs.cSocket = accept(listen_socket, (SOCKADDR*)&cs.clnt_addr, &clntLen);
				printf("Player @ %s has connected.\n", inet_ntoa(cs.clnt_addr.sin_addr));
				listOfClients.add(cs);
			}
			for(int k = 0; k < listOfClients.size(); k++)
			{
				//Wow, five tabs in.
				if(FD_ISSET(listOfClients.get(k).cSocket, &readfds))
				{
					//Data from the client. Get it.
					receiveBufferFromClients(k);
				}
				if(FD_ISSET(listOfClients.get(k).cSocket, &writefds))
				{
					//Data can be written to the client. Send it.
					sendBufferToClients(k); //Send it to the client.
				}
			}
		}
	}
	World * getWorld() {return sWorld;} //Returns the world pointer.
	SOCKET getSocketAt(int index) {return 0;} //Returns the socket at the index.
};