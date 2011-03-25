#pragma once

// sockets
#include <winsock2.h>

// huge amount of help descriptions for error codes encountered using BSD-like sockets
#include "winsockerrorhelp.h"

/**
 * Basic network module that abstracts common BSD Socket processes.
 */
class NetModule
{
public:
	/** socket needs to be initialized */
	static const int INIT = 0;
	/** the socket has initialized, and is ready to connect */
	static const int READY_TO_CONNECT = 1;
	/** the socket has initialized, and is ready to bind & listen */
	static const int READY_TO_LISTEN = 2;
	/** mostly a place holder state. client about to send */
	static const int CONNECTED = 3;
	/** server waiting for a connection */
	static const int LISTENING = 4;
	/** server creating a connection with a client */
	static const int ESTABLISHING_CONNECTION = 5;
	/** server is filling client inboxes, ordering client activity */
	static const int PROCESSING = 6;
	/** client waiting to receive data */
	static const int WAITING_FOR_DATA = 7;
	/** client reading data (pretty sure some has been recieved) */
	static const int READING_DATA = 8;
	/** client sending data */
	static const int SENDING_DATA = 9;
	/** some error has occured. continuing will likely re-initialize the network module */
	static const int NET_ERROR = 10;
protected:
	/** this network module's main socket */
	SOCKET m_socket;
	/** the address structure, keeps track of port and IP address */
	struct sockaddr_in m_address;
	/** keeps track of what state the network module is in */
	int m_state;
	/** the list of error codes that are relevant to the last sockets related call */
	WinSockErrorHelp * m_lastErrorHelp;
	/** the last error code that occured */
	int m_lastErrorCode;
	/** the port to connect to */
	int m_port;
	/** sets the most relevant error code for the current socket related call */
	void setError(WinSockErrorHelp * a_help, int a_errorCode);
	/** @return true if the sockets initialized correctly */
	bool initSockets();
	/** cleans up m_address */
	void resetAddressStruct();
public:
	NetModule(int a_port);

	/** @return which port to target when the socket starts up */
	int getTargetPort();

	/** sets which port to target when the socket starts up */
	void setTargetPort(int a_port);

	/** shutdown the sockets */
	void releaseSockets();

	/** @return 0 if not readable, 1 if readable, error code otherwise */
	int getReadStatus();

	/** @return how many bytes are in the socket's input buffer */
	unsigned long bytesInSocketInputBuffer(SOCKET & mysocket);

	/** @return NET_ERROR if implementation specific socket logic results in malfunction */
	virtual int run() = 0;

	/** @return TODO list constants here */
	int getState();

	/** @param forces the state to change to a_state. WARNING: dangerous */
	void setState(int a_state);

	/** @return the state variable name of the current state */
	const char * getStateText();

	/**
	 * @param a_buffer where to write the address
	 * @param a_bufferSize how much area there is to write the address
	 * @param a_peerAddress, false for this address
	 */
	char * getIPAddress(char * a_buffer, int const & a_bufferSize, bool a_peerAddress);

	/** @return the port this socket is using, or the port this peer is probably connected to */
	int getPort(bool a_peerAddress);

	/** @return the list of error codes that are relevant to the last sockets related call */
	WinSockErrorHelp * getLastErrorHelp();

	/** @return the last WSA error code */
	int getLastErrorCode();

	/** returns the name of the last received error code */
	char * getLastErrorTitle();
	/** returns the description of the last received error */
	char * getLastErrorDescription();
	/** returns the name of the socket function that caused of the last received error */
	char * getLastSocketCall();
};