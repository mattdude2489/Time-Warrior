#pragma once
//http://msdn.microsoft.com/en-us/library/ms741394(VS.85).aspx
//http://publib.boulder.ibm.com/infocenter/iseries/v5r3/index.jsp?topic=/rzab6/rzab6xnonblock.htm

#include "netmodule.h"
#include "bytebuffer.h"

/** 
 * a connection between clients.
 *  - one exists on the client side
 *  - multiple exist on the server side, managed by the ServerModule
 */
class ClientModule : public NetModule
{
private:
	/** bytes to send, and bytes received */
	ByteBuffer m_outbox, m_inbox;

	/** the IP address to connect to on init... is null for server drone socket "XXX.XXX.XXX.XXX" */
	char m_ipTargetAddress[16];

public:
	/** sets the IP address (does not reset the connection) */
	void setTargetIPAddress(char * a_ipString);

	/** @return the target IP address as set by setIPAdress */
	char * getTargetIPAddress();

	/** bytes received */
	ByteBuffer * getInbox();

	/** bytes to send */
	ByteBuffer * getOutbox();

	/**
	 * @param ipString in XXX.XXX.XXX.XXX IPv4 format
	 * @param a_port a computer port
	 */
	ClientModule(char * a_ipString, int a_port);

	/** @return NET_ERROR if socket logic resulted in malfunction */
	int run();

	/**
	 * will create a connected server-to-client socket.
	 * @param a_listeningSocket the server socket that has a client on the line. 
	 */
	ClientModule(SOCKET a_listenSocket);
};