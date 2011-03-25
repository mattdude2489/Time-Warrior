#include "clientmodule.h"

ByteBuffer * ClientModule::getInbox()
{
	return &m_inbox;
}

ByteBuffer * ClientModule::getOutbox()
{
	return &m_outbox;
}

/** sets the IP address (does not reset the connection) */
void ClientModule::setTargetIPAddress(char * a_ipString)
{
	int i;
	for(i = 0; a_ipString[i]; i++)
	{
		m_ipTargetAddress[i] = a_ipString[i];
	}
	m_ipTargetAddress[i] = 0;
}

/** returns the target IP address as set by setIPAdress */
char * ClientModule::getTargetIPAddress()
{
	return m_ipTargetAddress;
}


ClientModule::ClientModule(char * a_ipString, int a_port):NetModule(a_port)
{
	setTargetIPAddress(a_ipString);
}

int ClientModule::run()
{
	int result;
	switch(m_state)
	{
	case NET_ERROR:
		releaseSockets();
		m_state = INIT;
		break;
	case INIT:
		// if the network module couldn't initialize the sockets...
		if(initSockets())
		{
			// convert string version of IP to raw 4 byte IP
			// in network byte order.
			unsigned long raw_ip_nbo = inet_addr(m_ipTargetAddress);
			m_address.sin_addr.s_addr = raw_ip_nbo;
			if (raw_ip_nbo != INADDR_NONE)
			{
				m_state = READY_TO_CONNECT;
			}
			else
			{
				m_state = NET_ERROR;
			}
		}
		else
		{
			m_state = NET_ERROR;
		}
		break;
	case READY_TO_CONNECT:
		result = connect(m_socket, (const sockaddr*) &m_address, sizeof(m_address));
		if(result == SOCKET_ERROR)
		{
			int errorCode = WSAGetLastError();
			switch(errorCode)
			{
			case WSAEISCONN:	// already connected!
				m_state = CONNECTED;
				break;
			case WSAEWOULDBLOCK:
			case WSAEALREADY:
				break;
			default:
				setError(errCodes_connect, errorCode);
			}
		}
		else
		{
			m_state = CONNECTED;
		}
		break;
	case CONNECTED:
		m_state = WAITING_FOR_DATA;
		break;
	case WAITING_FOR_DATA:
		if(m_outbox.size() > 0)
		{
			m_state = SENDING_DATA;
		}
		// TODO magic number...
		else if((result = getReadStatus()) == 1)
		{
			m_state = READING_DATA;
		}
		break;
	case SENDING_DATA:
		// outbox size is reset to 0 after data is successfully sent
		result = m_outbox.socket_send(m_socket);
		if (result == SOCKET_ERROR)
		{
			setError(errCodes_send, WSAGetLastError());
		}
		else
		{
			m_state = WAITING_FOR_DATA;
		}
		break;
	case READING_DATA:
		result = m_inbox.socket_recv(m_socket);
		if (result == SOCKET_ERROR)
		{
			int errorCode = WSAGetLastError();
			switch(errorCode)
			{
			case WSAEWOULDBLOCK:
				m_state = WAITING_FOR_DATA;
				break;
			default:
				setError(errCodes_recv, errorCode);
			}
		}
		// if a packet of size 0 is sent, 
		else if(result == 0)
		{
			// check to see if the signaling socket has closed.
			m_outbox.add("?");
			m_state = SENDING_DATA;
		}
		else
		{
			m_state = WAITING_FOR_DATA;
		}
		break;
	}
	return m_state;
}

/**
 * will create a connected server-to-client socket.
 * @param a_listeningSocket the server socket that has a client on the line. 
 */
ClientModule::ClientModule(SOCKET a_listenSocket):NetModule(0)
{
	m_ipTargetAddress[0] = 0;
	int client_size = sizeof(m_address);
	m_socket = accept(
		a_listenSocket, 
		(struct sockaddr*)&(m_address), &client_size);
	if(m_socket == INVALID_SOCKET)
	{
		setError(errCodes_accept, WSAGetLastError());
	}
	else
	{
		m_state = READING_DATA;
	}
	strcpy_s(m_ipTargetAddress, sizeof(m_ipTargetAddress), "server peer");
	m_port = -1;
}
