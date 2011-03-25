//http://msdn.microsoft.com/en-us/library/ms741394(VS.85).aspx
//http://publib.boulder.ibm.com/infocenter/iseries/v5r3/index.jsp?topic=/rzab6/rzab6xnonblock.htm

#include "winsockerrorhelp.h"
#include "winsock2.h"
#include <stdio.h>

WinSockErrorHelp * WinSockErrorHelp::get(WinSockErrorHelp * a_list, int a_code)
{
	for(int i = 0; a_list[i].code; ++i)
	{
		if(a_list[i].code == a_code)
			return &a_list[i];
	}
	return 0;
}

char * WinSockErrorHelp::getText(WinSockErrorHelp * a_list, int a_code)
{
	WinSockErrorHelp * code = get(a_list, a_code);
	if(code)
	{
		return code->text;
	}
	return "????";
}

char * WinSockErrorHelp::getName(WinSockErrorHelp * a_list, int a_code)
{
	WinSockErrorHelp * code = get(a_list, a_code);
	if(code)
	{
		return code->codeName;
	}
	return "????";
}

WinSockErrorHelp errCodes_select[] =
{
	{WSANOTINITIALISED, "WSANOTINITIALISED", "A successful WSAStartup call must occur before using this function."},
	{WSAEFAULT, "WSAEFAULT", "The Windows Sockets implementation was unable to allocate needed resources for its internal operations, or the readfds, writefds, exceptfds, or timeval parameters are not part of the user address space."},
	{WSAENETDOWN, "WSAENETDOWN", "The network subsystem has failed."},
	{WSAEINVAL, "WSAEINVAL", "The time-out value is not valid, or all three descriptor parameters were null."},
	{WSAEINTR, "WSAEINTR", "A blocking Windows Socket 1.1 call was canceled through WSACancelBlockingCall."},
	{WSAEINPROGRESS, "WSAEINPROGRESS", "A blocking Windows Sockets 1.1 call is in progress, or the service provider is still processing a callback function."},
	{WSAENOTSOCK, "WSAENOTSOCK", "One of the descriptor sets contains an entry that is not a socket."},
	{SOCKET_ERROR, "SOCKET_ERROR", "WSAGetLastError can be used to retrieve a specific error code."},
	{0,0,0}
};

WinSockErrorHelp errCodes_WSAStartup[] =
{
	{WSASYSNOTREADY, "WSASYSNOTREADY", "The underlying network subsystem is not ready for network communication."},
	{WSAVERNOTSUPPORTED, "WSAVERNOTSUPPORTED", "The version of Windows Sockets support requested is not provided by this particular Windows Sockets implementation."},
	{WSAEINPROGRESS, "WSAEINPROGRESS", "A blocking Windows Sockets 1.1 operation is in progress."},
	{WSAEPROCLIM, "WSAEPROCLIM", "A limit on the number of tasks supported by the Windows Sockets implementation has been reached."},
	{WSAEFAULT, "WSAEFAULT", "The lpWSAData parameter is not a valid pointer."},
	{0,0,0}
};

WinSockErrorHelp errCodes_socket[] =
{
	{WSANOTINITIALISED, "WSANOTINITIALISED", "A successful WSAStartup call must occur before using this function."},
	{WSAENETDOWN, "WSAENETDOWN", "The network subsystem or the associated service provider has failed."},
	{WSAEAFNOSUPPORT, "WSAEAFNOSUPPORT", "The specified address family is not supported. For example, an application tried to create a socket for the AF_IRDA address family but an infrared adapter and device driver is not installed on the local computer."},
	{WSAEINPROGRESS, "WSAEINPROGRESS", "A blocking Windows Sockets 1.1 call is in progress, or the service provider is still processing a callback function."},
	{WSAEMFILE, "WSAEMFILE", "No more socket descriptors are available."},
	{WSAENOBUFS, "WSAENOBUFS", "No buffer space is available. The socket cannot be created."},
	{WSAEPROTONOSUPPORT, "WSAEPROTONOSUPPORT", "The specified protocol is not supported."},
	{WSAEPROTOTYPE, "WSAEPROTOTYPE", "The specified protocol is the wrong type for this socket."},
	{WSAESOCKTNOSUPPORT, "WSAESOCKTNOSUPPORT", "The specified socket type is not supported in this address family."},
	{0,0,0}
};

WinSockErrorHelp errCodes_ioctlsocket[] =
{
	{WSANOTINITIALISED, "WSANOTINITIALISED", "A successful WSAStartup call must occur before using this function."},
	{WSAENETDOWN, "WSAENETDOWN", "The network subsystem has failed."},
	{WSAEINPROGRESS, "WSAEINPROGRESS", "A blocking Windows Sockets 1.1 call is in progress, or the service provider is still processing a callback function."},
	{WSAENOTSOCK, "WSAENOTSOCK", "The descriptor s is not a socket."},
	{WSAEFAULT, "WSAEFAULT", "The argp parameter is not a valid part of the user address space."},
	{0,0,0}
};

WinSockErrorHelp errCodes_connect[] =
{
	{WSANOTINITIALISED, "WSANOTINITIALISED", "A successful WSAStartup call must occur before using this function."},
	{WSAENETDOWN, "WSAENETDOWN", "The network subsystem has failed."},
	{WSAEADDRINUSE, "WSAEADDRINUSE", "The socket's local address is already in use and the socket was not marked to allow address reuse with SO_REUSEADDR. This error usually occurs when executing bind, but could be delayed until the connect function if the bind was to a wildcard address (INADDR_ANY or in6addr_any) for the local IP address. A specific address needs to be implicitly bound by the connect function."},
	{WSAEINTR, "WSAEINTR", "The blocking Windows Socket 1.1 call was canceled through WSACancelBlockingCall."},
	{WSAEINPROGRESS, "WSAEINPROGRESS", "A blocking Windows Sockets 1.1 call is in progress, or the service provider is still processing a callback function."},
	{WSAEALREADY, "WSAEALREADY", "A nonblocking connect call is in progress on the specified socket. Note  In order to preserve backward compatibility, this error is reported as WSAEINVAL to Windows Sockets 1.1 applications that link to either Winsock.dll or Wsock32.dll."},
	{WSAEADDRNOTAVAIL, "WSAEADDRNOTAVAIL", "The remote address is not a valid address (such as INADDR_ANY or in6addr_any) ."},
	{WSAEAFNOSUPPORT, "WSAEAFNOSUPPORT", "Addresses in the specified family cannot be used with this socket."},
	{WSAECONNREFUSED, "WSAECONNREFUSED", "The attempt to connect was forcefully rejected."},
	{WSAEFAULT, "WSAEFAULT", "The sockaddr structure pointed to by the name contains incorrect address format for the associated address family or the namelen parameter is too small. This error is also returned if the sockaddr structure pointed to by the name parameter with a length specified in the namelen parameter is not in a valid part of the user address space."},
	{WSAEINVAL, "WSAEINVAL", "The parameter s is a listening socket."},
	{WSAEISCONN, "WSAEISCONN", "The socket is already connected (connection-oriented sockets only)."},
	{WSAENETUNREACH, "WSAENETUNREACH", "The network cannot be reached from this host at this time."},
	{WSAEHOSTUNREACH, "WSAEHOSTUNREACH", "A socket operation was attempted to an unreachable host."},
	{WSAENOBUFS, "WSAENOBUFS", "Note  No buffer space is available. The socket cannot be connected."},
	{WSAENOTSOCK, "WSAENOTSOCK", "The descriptor specified in the s parameter is not a socket."},
	{WSAETIMEDOUT, "WSAETIMEDOUT", "An attempt to connect timed out without establishing a connection."},
	{WSAEWOULDBLOCK, "WSAEWOULDBLOCK", "The socket is marked as nonblocking and the connection cannot be completed immediately."},
	{WSAEACCES, "WSAEACCES", "An attempt to connect a datagram socket to broadcast address failed because setsockopt option SO_BROADCAST is not enabled."},
	{0,0,0}
};

WinSockErrorHelp errCodes_recv[] =
{
	{WSANOTINITIALISED, "WSANOTINITIALISED", "A successful WSAStartup call must occur before using this function."},
	{WSAENETDOWN, "WSAENETDOWN", "The network subsystem has failed."},
	{WSAEFAULT, "WSAEFAULT", "The buf parameter is not completely contained in a valid part of the user address space."},
	{WSAENOTCONN, "WSAENOTCONN", "The socket is not connected."},
	{WSAEINTR, "WSAEINTR", "The (blocking) call was canceled through WSACancelBlockingCall."},
	{WSAEINPROGRESS, "WSAEINPROGRESS", "A blocking Windows Sockets 1.1 call is in progress, or the service provider is still processing a callback function."},
	{WSAENETRESET, "WSAENETRESET", "For a connection-oriented socket, this error indicates that the connection has been broken due to keep-alive activity detecting a failure while the operation was in progress. For a datagram socket, this error indicates that the time to live has expired."},
	{WSAENOTSOCK, "WSAENOTSOCK", "The descriptor is not a socket."},
	{WSAEOPNOTSUPP, "WSAEOPNOTSUPP", "MSG_OOB was specified, but the socket is not stream-style such as type SOCK_STREAM, OOB data is not supported in the communication domain associated with this socket, or the socket is unidirectional and supports only send operations."},
	{WSAESHUTDOWN, "WSAESHUTDOWN", "The socket has been shut down; it is not possible to receive on a socket after shutdown has been invoked with how set to SD_RECEIVE or SD_BOTH."},
	{WSAEWOULDBLOCK, "WSAEWOULDBLOCK", "The socket is marked as nonblocking and the receive operation would block."},
	{WSAEMSGSIZE, "WSAEMSGSIZE", "The message was too large to fit into the specified buffer and was truncated."},
	{WSAEINVAL, "WSAEINVAL", "The socket has not been bound with bind, or an unknown flag was specified, or MSG_OOB was specified for a socket with SO_OOBINLINE enabled or (for byte stream sockets only) len was zero or negative."},
	{WSAECONNABORTED, "WSAECONNABORTED", "The virtual circuit was terminated due to a time-out or other failure. The application should close the socket as it is no longer usable."},
	{WSAETIMEDOUT, "WSAETIMEDOUT", "The connection has been dropped because of a network failure or because the peer system failed to respond."},
	{WSAECONNRESET, "WSAECONNRESET", "The virtual circuit was reset by the remote side executing a hard or abortive close. The application should close the socket as it is no longer usable. On a UPD-datagram socket this error would indicate that a previous send operation resulted in an ICMP \"Port Unreachable\" message."},
	{0,0,0}
};

WinSockErrorHelp errCodes_bind[] =
{
	{WSANOTINITIALISED, "WSANOTINITIALISED", "Note  A successful WSAStartup call must occur before using this function."},
	{WSAENETDOWN, "WSAENETDOWN", "The network subsystem has failed."},
	{WSAEACCES, "WSAEACCES", "An attempt to bind a datagram socket to the broadcast address failed because the setsockopt option SO_BROADCAST is not enabled."},
	{WSAEADDRINUSE, "WSAEADDRINUSE", "A process on the computer is already bound to the same fully-qualified address and the socket has not been marked to allow address reuse with SO_REUSEADDR. For example, the IP address and port specified in the name parameter are already bound to another socket being used by another application. For more information, see the SO_REUSEADDR socket option in the SOL_SOCKET Socket Options reference, Using SO_REUSEADDR and SO_EXCLUSIVEADDRUSE, and SO_EXCLUSIVEADDRUSE."},
	{WSAEADDRNOTAVAIL, "WSAEADDRNOTAVAIL", "The specified address pointed to by the name parameter is not a valid local IP address on this computer."},
	{WSAEFAULT, "WSAEFAULT", "The name or namelen parameter is not a valid part of the user address space, the namelen parameter is too small, the name parameter contains an incorrect address format for the associated address family, or the first two bytes of the memory block specified by name does not match the address family associated with the socket descriptor s."},
	{WSAEINPROGRESS, "WSAEINPROGRESS", "A blocking Windows Sockets 1.1 call is in progress, or the service provider is still processing a callback function."},
	{WSAEINVAL, "WSAEINVAL", "The socket is already bound to an address."},
	{WSAENOBUFS, "WSAENOBUFS", "Not enough buffers are available, too many connections."},
	{WSAENOTSOCK, "WSAENOTSOCK", "The descriptor in the s parameter is not a socket."},
	{0,0,0}
};

WinSockErrorHelp errCodes_listen[] =
{
	{WSANOTINITIALISED, "WSANOTINITIALISED", "A successful WSAStartup call must occur before using this function."},
	{WSAENETDOWN, "WSAENETDOWN", "The network subsystem has failed."},
	{WSAEADDRINUSE, "WSAEADDRINUSE", "The socket's local address is already in use and the socket was not marked to allow address reuse with SO_REUSEADDR. This error usually occurs during execution of the bind function, but could be delayed until this function if the bind was to a partially wildcard address (involving ADDR_ANY) and if a specific address needs to be committed at the time of this function."},
	{WSAEINPROGRESS, "WSAEINPROGRESS", "A blocking Windows Sockets 1.1 call is in progress, or the service provider is still processing a callback function."},
	{WSAEINVAL, "WSAEINVAL", "The socket has not been bound with bind."},
	{WSAEISCONN, "WSAEISCONN", "The socket is already connected."},
	{WSAEMFILE, "WSAEMFILE", "No more socket descriptors are available."},
	{WSAENOBUFS, "WSAENOBUFS", "No buffer space is available."},
	{WSAENOTSOCK, "WSAENOTSOCK", "The descriptor is not a socket."},
	{WSAEOPNOTSUPP, "WSAEOPNOTSUPP", "The referenced socket is not of a type that supports the listen operation."},
	{0,0,0}
};

WinSockErrorHelp errCodes_accept[] =
{
	{WSANOTINITIALISED, "WSANOTINITIALISED", "A successful WSAStartup call must occur before using this function."},
	{WSAECONNRESET, "WSAECONNRESET", "An incoming connection was indicated, but was subsequently terminated by the remote peer prior to accepting the call."},
	{WSAEFAULT, "WSAEFAULT", "The addrlen parameter is too small or addr is not a valid part of the user address space."},
	{WSAEINTR, "WSAEINTR", "A blocking Windows Sockets 1.1 call was canceled through WSACancelBlockingCall."},
	{WSAEINVAL, "WSAEINVAL", "The listen function was not invoked prior to accept."},
	{WSAEINPROGRESS, "WSAEINPROGRESS", "A blocking Windows Sockets 1.1 call is in progress, or the service provider is still processing a callback function."},
	{WSAEMFILE, "WSAEMFILE", "The queue is nonempty upon entry to accept and there are no descriptors available."},
	{WSAENETDOWN, "WSAENETDOWN", "The network subsystem has failed."},
	{WSAENOBUFS, "WSAENOBUFS", "No buffer space is available."},
	{WSAENOTSOCK, "WSAENOTSOCK", "The descriptor is not a socket."},
	{WSAEOPNOTSUPP, "WSAEOPNOTSUPP", "The referenced socket is not a type that supports connection-oriented service."},
	{WSAEWOULDBLOCK, "WSAEWOULDBLOCK", "The socket is marked as nonblocking and no connections are present to be accepted."},
	{0,0,0}
};

WinSockErrorHelp errCodes_send[] =
{
	{WSANOTINITIALISED, "WSANOTINITIALISED", "A successful WSAStartup call must occur before using this function."},
	{WSAENETDOWN, "WSAENETDOWN", "The network subsystem has failed."},
	{WSAEACCES, "WSAEACCES", "The requested address is a broadcast address, but the appropriate flag was not set. Call setsockopt with the SO_BROADCAST socket option to enable use of the broadcast address."},
	{WSAEINTR, "WSAEINTR", "A blocking Windows Sockets 1.1 call was canceled through WSACancelBlockingCall."},
	{WSAEINPROGRESS, "WSAEINPROGRESS", "A blocking Windows Sockets 1.1 call is in progress, or the service provider is still processing a callback function."},
	{WSAEFAULT, "WSAEFAULT", "The buf parameter is not completely contained in a valid part of the user address space."},
	{WSAENETRESET, "WSAENETRESET", "The connection has been broken due to the keep-alive activity detecting a failure while the operation was in progress."},
	{WSAENOBUFS, "WSAENOBUFS", "No buffer space is available."},
	{WSAENOTCONN, "WSAENOTCONN", "The socket is not connected."},
	{WSAENOTSOCK, "WSAENOTSOCK", "The descriptor is not a socket."},
	{WSAEOPNOTSUPP, "WSAEOPNOTSUPP", "MSG_OOB was specified, but the socket is not stream-style such as type SOCK_STREAM, OOB data is not supported in the communication domain associated with this socket, or the socket is unidirectional and supports only receive operations."},
	{WSAESHUTDOWN, "WSAESHUTDOWN", "The socket has been shut down; it is not possible to send on a socket after shutdown has been invoked with how set to SD_SEND or SD_BOTH."},
	{WSAEWOULDBLOCK, "WSAEWOULDBLOCK", "The socket is marked as nonblocking and the requested operation would block."},
	{WSAEMSGSIZE, "WSAEMSGSIZE", "The socket is message oriented, and the message is larger than the maximum supported by the underlying transport."},
	{WSAEHOSTUNREACH, "WSAEHOSTUNREACH", "The remote host cannot be reached from this host at this time."},
	{WSAEINVAL, "WSAEINVAL", "The socket has not been bound with bind, or an unknown flag was specified, or MSG_OOB was specified for a socket with SO_OOBINLINE enabled."},
	{WSAECONNABORTED, "WSAECONNABORTED", "The virtual circuit was terminated due to a time-out or other failure. The application should close the socket as it is no longer usable."},
	{WSAECONNRESET, "WSAECONNRESET", "The virtual circuit was reset by the remote side executing a hard or abortive close. For UDP sockets, the remote host was unable to deliver a previously sent UDP datagram and responded with a \"Port Unreachable\" ICMP packet. The application should close the socket as it is no longer usable."},
	{WSAETIMEDOUT, "WSAETIMEDOUT", "The connection has been dropped, because of a network failure or because the system on the other end went down without notice."},
	{0,0,0}
};

WinSockErrorHelp errCodes_setsockopt[] =
{
	{WSANOTINITIALISED, "WSANOTINITIALISED", "A successful WSAStartup call must occur before using this function."},
	{WSAENETDOWN, "WSAENETDOWN", "The network subsystem has failed."},
	{WSAEFAULT, "WSAEFAULT", "The buffer pointed to by the optval parameter is not in a valid part of the process address space or the optlen parameter is too small."},
	{WSAEINPROGRESS, "WSAEINPROGRESS", "A blocking Windows Sockets 1.1 call is in progress, or the service provider is still processing a callback function."},
	{WSAEINVAL, "WSAEINVAL", "The level parameter is not valid, or the information in the buffer pointed to by the optval parameter is not valid."},
	{WSAENETRESET, "WSAENETRESET", "The connection has timed out when SO_KEEPALIVE is set."},
	{WSAENOPROTOOPT, "WSAENOPROTOOPT", "The option is unknown or unsupported for the specified provider or socket (see SO_GROUP_PRIORITY limitations)."},
	{WSAENOTCONN, "WSAENOTCONN", "The connection has been reset when SO_KEEPALIVE is set."},
	{WSAENOTSOCK, "WSAENOTSOCK", "The descriptor is not a socket."},
	{0,0,0}
};

WinSockErrorHelp errCodes_shutdown[] =
{
	{WSANOTINITIALISED, "WSANOTINITIALISED", "A successful WSAStartup call must occur before using this function."},
	{WSAENETDOWN, "WSAENETDOWN", "The network subsystem has failed."},
	{WSAEINVAL, "WSAEINVAL", "The how parameter is not valid, or is not consistent with the socket type. For example, SD_SEND is used with a UNI_RECV socket type."},
	{WSAEINPROGRESS, "WSAEINPROGRESS", "A blocking Windows Sockets 1.1 call is in progress, or the service provider is still processing a callback function."},
	{WSAENOTCONN, "WSAENOTCONN", "The socket is not connected (connection-oriented sockets only)."},
	{WSAENOTSOCK, "WSAENOTSOCK", "Note  The descriptor is not a socket."},
	{0,0,0}
};

WinSockErrorHelp errCodes_closesocket[] =
{
	{WSANOTINITIALISED, "WSANOTINITIALISED", "A successful WSAStartup call must occur before using this function."},
	{WSAENETDOWN, "WSAENETDOWN", "The network subsystem has failed."},
	{WSAENOTSOCK, "WSAENOTSOCK", "The descriptor is not a socket."},
	{WSAEINPROGRESS, "WSAEINPROGRESS", "A blocking Windows Sockets 1.1 call is in progress, or the service provider is still processing a callback function."},
	{WSAEINTR, "WSAEINTR", "The (blocking) Windows Socket 1.1 call was canceled through WSACancelBlockingCall."},
	{WSAEWOULDBLOCK, "WSAEWOULDBLOCK", "The socket is marked as nonblocking, but the l_onoff member of the linger structure is set to non-zero and the l_linger member of the linger structure is set to a nonzero timeout value."},
	{0,0,0}
};

struct FunctionErrorLookup
{
	char * functionName;
	WinSockErrorHelp * errorCodes;
};

static FunctionErrorLookup g_functionErrorLookup[] =
{
	{"select", errCodes_select},
	{"WSAStartup", errCodes_WSAStartup},
	{"socket", errCodes_socket},
	{"ioctlsocket", errCodes_ioctlsocket},
	{"connect", errCodes_connect},
	{"recv", errCodes_recv},
	{"bind", errCodes_bind},
	{"listen", errCodes_listen},
	{"accept", errCodes_accept},
	{"send", errCodes_send},
	{"setsockopt", errCodes_setsockopt},
	{"shutdown", errCodes_shutdown},
	{"closesocket", errCodes_closesocket},
	{0,0}
};

char * WinSockErrorHelp::socketFunctionName(WinSockErrorHelp * a_list)
{
	char * functionName = "????";
	for(int i = 0; g_functionErrorLookup[i].errorCodes; ++i)
	{
		if(g_functionErrorLookup[i].errorCodes == a_list)
			functionName = g_functionErrorLookup[i].functionName;
	}
	return functionName;
}

void WinSockErrorHelp::errorPrint(WinSockErrorHelp * a_list, int a_code)
{
	WinSockErrorHelp * help = get(a_list, a_code);
	char * functionName = socketFunctionName(a_list);
	char * errorName = help?help->codeName:functionName;
	char * errorText = help?help->text:functionName;
	fprintf(stderr, "%s failed %s(%d): %s\n", functionName, errorName, a_code, errorText);
}
