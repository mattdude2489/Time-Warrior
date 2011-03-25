#include "servermodule.h"

/** @param a_port the port the server listens to */
ServerModule::ServerModule(const int & a_port):NetModule(a_port),m_clients(0)
{
	m_clients = new TemplateVector<ClientModule*>();
}

/** @return the number of clients being managed */
int ServerModule::size()
{
	return m_clients->size();
}

ByteBuffer * ServerModule::getBroadcastBuffer()
{
	return &m_broadcastData;
}

/**
 * @param a_index which of the size() clients you want about
 * @return the connection manager (which has the inbox and outbox) to the given client 
 */
ClientModule * ServerModule::getClient(const int & a_index)
{
	return (ClientModule*)m_clients->get(a_index);
}

/**
 * @param a_listeningSocket a listening server socket
 * @return a new socket connection spawned from the given listening socket
 */
int ServerModule::addConnection(SOCKET a_listeningSocket)
{
	// create the client socket
	ClientModule * newSocket = new ClientModule(a_listeningSocket);
	// find an empty place in the list for the client socket
	int whereToPlaceTheNewSocket = -1;
	for(int i = 0; i < m_clients->size() && whereToPlaceTheNewSocket < 0; ++i)
	{
		if(!m_clients->get(i))
			whereToPlaceTheNewSocket = i;
	}
	// if no place was found for the socket (-1), expand the list by adding a new client
	if(whereToPlaceTheNewSocket < 0)
	{
		m_clients->add(newSocket);
	}
	else
	{
		m_clients->set(whereToPlaceTheNewSocket, newSocket);
	}
	return whereToPlaceTheNewSocket;
}

/** handles client connections */
void ServerModule::manageClients()
{
	ClientModule * client;
	// queue broadcast data to send to clients
	if(m_broadcastData.size() > 0)
	{
		for(int i = 0; i < m_clients->size(); ++i)
		{
			client = (ClientModule*)m_clients->get(i);
			if(client)
			{
				client->getOutbox()->add(&m_broadcastData);
			}
		}
		m_broadcastData.reset();
		if(m_state == LISTENING)
		{
			m_state = SENDING_DATA;
		}
	}
	// handle logic for all clients
	for(int i = 0; i < m_clients->size(); ++i)
	{
		client = (ClientModule*)m_clients->get(i);
		if(client)
		{
			if(client->getState() == NET_ERROR)
			{
				releaseConnection(i);
				if(m_state == LISTENING)
				{
					m_state = PROCESSING;
				}
			}
			else
			{
				int oldState = client->getState();
				client->run();
				if(client->getState() != oldState && m_state == LISTENING)
				{
					m_state = PROCESSING;
				}
				ByteBuffer * inbox = client->getInbox();
				// broadcast everything the server hears from the clients.
				if(inbox->size() > 0)
				{
					m_broadcastData.add(inbox);
					if(m_state == LISTENING)
					{
						m_state = READING_DATA;
					}
					// don't bother keeping track of everything the clients have recieved... these are dummy modules
					inbox->reset();
				}
			}
		}
	}
}

/** handles the current socket state. returns the state */
int ServerModule::run()
{
	int result;
	switch(m_state)
	{
	case INIT:
		if(initSockets())
		{
			m_state = READY_TO_LISTEN;
		}
		else
		{
			m_state = NET_ERROR;
		}
		break;
	case READY_TO_LISTEN:
		result = bind(m_socket, (sockaddr *) &m_address, sizeof(m_address));
		if (result != SOCKET_ERROR)
		{
			m_state = CONNECTED;
		}
		else
		{
			setError(errCodes_bind, WSAGetLastError());
		}
		break;
	case CONNECTED:
		result = listen(m_socket, /* size of connection queue */1);
		if (result != SOCKET_ERROR)
		{
			m_state = LISTENING;
		}
		else
		{
			setError(errCodes_listen, WSAGetLastError());
		}
		break;
	case LISTENING:
	case PROCESSING:
	case READING_DATA:
	case SENDING_DATA:
		result = getReadStatus();
		// TODO magic number...
		if(result == 1)
		{
			m_state = ESTABLISHING_CONNECTION;
		}
		else
		{
			m_state = LISTENING;
		}
		break;
	case ESTABLISHING_CONNECTION:
		addConnection(m_socket);
		m_state = LISTENING;
		break;
	case NET_ERROR:
		releaseSockets();
		m_state = INIT;
		break;
	}
	manageClients();
	return m_state;
}

/** @param a_index which client connection to disconnect from */
void ServerModule::releaseConnection(const int & a_index)
{
	ClientModule * client = (ClientModule *)m_clients->get(a_index);
	client->releaseSockets();
	delete client;
	m_clients->set(a_index, NULL);
}

/** frees all client sockets and shuts down the listening socket */
void ServerModule::release()
{
	if(m_clients)
	{
		for(int i = 0; i < m_clients->size(); ++i)
		{
			if(m_clients->get(i))
				releaseConnection(i);
		}
		delete m_clients;
		m_clients = 0;
	}
	releaseSockets();
}

ServerModule::~ServerModule()
{
	release();
}