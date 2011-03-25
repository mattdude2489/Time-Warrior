#include "netmodule.h"

#include <stdio.h>

/** zero seconds, zero milliseconds. max time select call aloud to block */
static const timeval someNumber = {0,0};

/** names of the states of the network module */
static const char * g_NetModuleState[] =
{
	"INIT",
	"READY TO CONNECT",
	"READY TO LISTEN",
	"CONNECTED",
	"LISTENING",
	"ESTABLISH CONN",
	"PROCESSING",
	"WAITING FOR DATA",
	"READING DATA",
	"SENDING DATA",
	"NET ERROR",
};

/** @return true if the sockets initialized correctly */
bool NetModule::initSockets()
{
	int errorCode;
	// structure gets populated w/ info explaining this sockets implementation
	WSADATA m_wsaData;
	// Load Winsock 2.0 DLL. initiates use of the Winsock DLL by a process
	if ((errorCode = WSAStartup(MAKEWORD(2, 0),&m_wsaData)) != 0)
	{
		setError(errCodes_WSAStartup, errorCode);
		return false;
	}

	// create this network object's main socket
	m_socket = socket(
/*
AF_UNSPEC: 0     The address family is unspecified.
AF_INET: 2       The Internet Protocol version 4 (IPv4) address family.
AF_IPX: 6        The IPX/SPX address family. This address family is only supported if the NWLink IPX/SPX NetBIOS Compatible Transport protocol is installed. This address family is not supported on Windows Vista and later.
AF_APPLETALK: 17 The AppleTalk address family. This address family is only supported if the AppleTalk protocol is installed. This address family is not supported on Windows Vista and later.
AF_NETBIOS: 17   The NetBIOS address family. This address family is only supported if a Windows Sockets provider for NetBIOS is installed.
AF_INET6: 23     The Internet Protocol version 6 (IPv6) address family.
AF_IRDA: 26      The Infrared Data Association (IrDA) address family. This address family is only supported if the computer has an infrared port and driver installed.
AF_BTH: 32       The Bluetooth address family. This address family is supported on Windows XP with SP2 or later if the computer has a Bluetooth adapter and driver installed.
*/
		AF_INET, 
/*
SOCK_STREAM: 1    A socket type that provides sequenced, reliable, two-way, connection-based byte streams with an OOB data transmission mechanism. This socket type uses the Transmission Control Protocol (TCP) for the Internet address family (AF_INET or AF_INET6).
SOCK_DGRAM: 2     A socket type that supports datagrams, which are connectionless, unreliable buffers of a fixed (typically small) maximum length. This socket type uses the User Datagram Protocol (UDP) for the Internet address family (AF_INET or AF_INET6).
SOCK_RAW: 3       A socket type that provides a raw socket that allows an application to manipulate the next upper-layer protocol header. To manipulate the IPv4 header, the IP_HDRINCL socket option must be set on the socket. To manipulate the IPv6 header, the IPV6_HDRINCL socket option must be set on the socket.
SOCK_RDM: 4       A socket type that provides a reliable message datagram. An example of this type is the Pragmatic General Multicast (PGM) multicast protocol implementation in Windows, often referred to as reliable multicast programming. This type is only supported if the Reliable Multicast Protocol is installed.
SOCK_SEQPACKET: 5 A socket type that provides a pseudo-stream packet based on datagrams.
*/
		SOCK_STREAM, 
/*
BTHPROTO_RFCOMM: 3 The Bluetooth Radio Frequency Communications (Bluetooth RFCOMM) protocol. This is a possible value when the af parameter is AF_BTH and the type parameter is SOCK_STREAM. This protocol is supported on Windows XP with SP2 or later.
IPPROTO_TCP: 6     The Transmission Control Protocol (TCP). This is a possible value when the af parameter is AF_INET or AF_INET6 and the type parameter is SOCK_STREAM.
IPPROTO_UDP: 17    The User Datagram Protocol (UDP). This is a possible value when the af parameter is AF_INET or AF_INET6 and the type parameter is SOCK_DGRAM.
IPPROTO_RM: 113    The PGM protocol for reliable multicast. This is a possible value when the af parameter is AF_INET and the type parameter is SOCK_RDM. On the Windows SDK released for Windows Vista and later, this value is also called IPPROTO_PGM. This protocol is only supported if the Reliable Multicast Protocol is installed.
*/
		IPPROTO_TCP);
	if (m_socket == INVALID_SOCKET )
	{
		setError(errCodes_socket, WSAGetLastError());
		return false;
	}

	// 1 to set non-blocking, 0 to set re-usable
	unsigned long argp = 1;

	// for complete info on this method, check out http://msdn.microsoft.com/en-us/library/ms740476(VS.85).aspx
	errorCode = setsockopt(m_socket, 
		SOL_SOCKET,
/*
SO_REUSEADDR: 4 Allows the socket to be bound to an address that is already in use. For more information, see bind. Not applicable on ATM sockets.
*/
		SO_REUSEADDR, 
		(char *)&argp, sizeof(argp));
	if (errorCode != 0)
	{
		setError(errCodes_setsockopt, WSAGetLastError());
		return false;
	}

	// 1 to set non-blocking, 0 to set blocking
	argp = 1;
	// attempt to setup the socket as non-blocking
	if(ioctlsocket(m_socket, 
/*
FIONBIO:    The *argp parameter is a pointer to an unsigned long value. Set *argp to a nonzero value if the nonblocking mode should be enabled, or zero if the nonblocking mode should be disabled. When a socket is created, it operates in blocking mode by default (nonblocking mode is disabled). This is consistent with BSD sockets.
FIONREAD:   Use to determine the amount of data pending in the network's input buffer that can be read from socket s. The argp parameter points to an unsigned long value in which ioctlsocket stores the result. FIONREAD returns the amount of data that can be read in a single call to the recv function, which may not be the same as the total amount of data queued on the socket. If s is message oriented (for example, type SOCK_DGRAM), FIONREAD still returns the amount of pending data in the network buffer, however, the amount that can actually be read in a single call to the recv function is limited to the data size written in the send or sendto function call.
SIOCATMARK: Use to determine if all out of band (OOB) data has been read. (See Windows Sockets 1.1 Blocking Routines and EINPROGRESS for a discussion on OOB data.) This applies only to a stream oriented socket (for example, type SOCK_STREAM) that has been configured for in-line reception of any OOB data (SO_OOBINLINE). On sockets with the SO_OOBINLINE socket option set, SIOCATMARK always returns TRUE and the OOB data is returned to the user as normal data.
*/
			FIONBIO,
			&argp) == SOCKET_ERROR)
	{
		setError(errCodes_ioctlsocket, WSAGetLastError());
		return false;
	}

	return true;
}

void NetModule::resetAddressStruct()
{
	// initialize the socket's address
	memset(&m_address, 0, sizeof(m_address)); 
	m_address.sin_family = AF_INET;
	m_address.sin_port = htons(m_port);
}


/** @return which port to target when the socket starts up */
int NetModule::getTargetPort()
{
	return m_port;
}

/** sets which port to target when the socket starts up */
void NetModule::setTargetPort(int a_port)
{
	m_port = a_port;
}

NetModule::NetModule(int a_port)
{
	setTargetPort(a_port);
	resetAddressStruct();
	// clear the state... this module is ready to finish initialization
	m_state = INIT;
}

/** shutdown the sockets */
void NetModule::releaseSockets()
{
	if(m_socket != INVALID_SOCKET)
	{
		int result;
		result = shutdown(m_socket, SD_BOTH);
		if (result != 0)
		{
			setError(errCodes_shutdown, WSAGetLastError());
		}
		result = closesocket(m_socket);
		if (result != 0)
		{
			setError(errCodes_closesocket, WSAGetLastError());
		}
		m_socket = INVALID_SOCKET;
	}
	resetAddressStruct();
}

/** @return 0 if not readable, 1 if readable, error code otherwise */
int NetModule::getReadStatus()
{
	fd_set a = {1, {m_socket}};
	int errorCode = select(0,&a,0,0,&someNumber);
	if(errorCode == SOCKET_ERROR)
	{
		errorCode = WSAGetLastError();
	}
	if(errorCode != 0 && errorCode != 1)
	{
		setError(errCodes_select, errorCode);
	}
	return errorCode;
}

void NetModule::setError(WinSockErrorHelp * a_help, int a_errorCode)
{
	m_lastErrorHelp = a_help;
	m_lastErrorCode = a_errorCode;
	m_state = NET_ERROR;
}

int NetModule::getLastErrorCode()
{
	return m_lastErrorCode;
}

///** [TODO get rid of] uses select to check if this socket is readable */
//bool NetModule::socketIsReadable(SOCKET & mysocket){
//	// which sockets to check
//	fd_set a = {1, {mysocket}};
//	int errorCode = select(0,&a,0,0,&someNumber);	// read check
////	int errorCode = select(0,0,&a,0,&someNumber);	// write check
//	if(errorCode == SOCKET_ERROR)
//	{
//		errorCode = WSAGetLastError();
//	}
//	if(errorCode != 0 && errorCode != 1)
//	{
//		setError(errCodes_select, errorCode);
//	}
//	return (errorCode > 0);
//}

/** @return how many bytes are in the socket's input buffer */
unsigned long NetModule::bytesInSocketInputBuffer(SOCKET & mysocket)
{
	unsigned long howMuchInBuffer = 0;
	ioctlsocket(m_socket, FIONREAD, &howMuchInBuffer);
	return howMuchInBuffer;
}

int NetModule::getState()
{
	return m_state;
}

/** @param forces the state to change to a_state. WARNING: dangerous */
void NetModule::setState(int a_state)
{
	m_state = a_state;
}

/** @return the state variable name of the current state */
const char * NetModule::getStateText()
{
	return g_NetModuleState[m_state];
}


char * NetModule::getIPAddress(char * a_buffer, int const & a_bufferSize, bool a_peerAddress)
{
	int sizeOfAddressStructure = sizeof(m_address);
	if(a_peerAddress)
	{
		getpeername(m_socket, (sockaddr *)&m_address, &sizeOfAddressStructure);
	}
	else
	{
		getsockname(m_socket, (sockaddr *)&m_address, &sizeOfAddressStructure);
	}
	char * ipAddress = inet_ntoa(m_address.sin_addr);
	sprintf_s(a_buffer, a_bufferSize, "%s", ipAddress);
	return a_buffer;
}

/** @return the port this socket is using, or the port this peer is probably connected to */
int NetModule::getPort(bool a_peerAddress)
{
	int sizeOfAddressStructure = sizeof(m_address);
	if(a_peerAddress)
	{
		getpeername(m_socket, (sockaddr *)&m_address, &sizeOfAddressStructure);
	}
	else
	{
		getsockname(m_socket, (sockaddr *)&m_address, &sizeOfAddressStructure);
	}
	return ntohs(m_address.sin_port);
}

/** @return the list of error codes that are relevant to the last sockets related call */
WinSockErrorHelp * NetModule::getLastErrorHelp()
{
	return m_lastErrorHelp;
}

/** returns the name of the last received error code */
char * NetModule::getLastErrorTitle()
{
	return WinSockErrorHelp::getName(m_lastErrorHelp, m_lastErrorCode);
}

/** returns the description of the last received error */
char * NetModule::getLastErrorDescription()
{
	return WinSockErrorHelp::getText(m_lastErrorHelp, m_lastErrorCode);
}

/** returns the name of the socket function that caused of the last received error */
char * NetModule::getLastSocketCall()
{
	return WinSockErrorHelp::socketFunctionName(m_lastErrorHelp);
}