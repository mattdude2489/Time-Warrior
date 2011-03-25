#pragma once

#include "netmodule.h"
#include "clientmodule.h"
#include "bytebuffer.h"
#include "templatevector.h"

/**
 * a listening socket.
 * waits for connections from clients, and stores them as a list of client sockets.
 */
class ServerModule : public NetModule
{
private:
	/** the list of client connections spawned by the listening socket */
	TemplateVector<ClientModule*> * m_clients;

	/** what data to broadcast to all clients */
	ByteBuffer m_broadcastData;

public:
	/** @param a_port the port the server listens to */
	ServerModule(const int & a_port);

	/** @return the number of clients being managed */
	int size();

	/** what data to broadcast to all clients */
	ByteBuffer * getBroadcastBuffer();

	/**
	 * @param a_index which of the size() clients you want about
	 * @return the connection manager (which has the inbox and outbox) to the given client 
	 */
	ClientModule * getClient(const int & a_index);

private:
	/**
	 * @param a_listeningSocket a listening server socket
	 * @return the index in m_clients, or getClient(), of the new socket
	 */
	int addConnection(SOCKET a_listenintSocket);

public:

	/** @param a_index which client connection to disconnect from */
	void releaseConnection(const int & a_index);

	/** handles the current socket state. returns the state */
	int run();

	/** handles client connections */
	void manageClients();

	/** frees all client sockets and shuts down the listening socket */
	void release();

	~ServerModule();
};