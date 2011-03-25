//http://msdn.microsoft.com/en-us/library/ms741394(VS.85).aspx
//http://publib.boulder.ibm.com/infocenter/iseries/v5r3/index.jsp?topic=/rzab6/rzab6xnonblock.htm

/**
 * a struct so that data can be defined in static initializer lists.
 * many of these are stored in null-terminated arrays
 */
struct WinSockErrorHelp
{
	int code;
	char * codeName;
	char * text;
	// TODO bool/int recoverable?

	static WinSockErrorHelp * get(WinSockErrorHelp * a_list, int a_code);
	static char * getText(WinSockErrorHelp * a_list, int a_code);
	static char * getName(WinSockErrorHelp * a_list, int a_code);
	static char * socketFunctionName(WinSockErrorHelp * a_list);
	static void errorPrint(WinSockErrorHelp * a_list, int a_code);
};

extern WinSockErrorHelp errCodes_select[];
extern WinSockErrorHelp errCodes_WSAStartup[];
extern WinSockErrorHelp errCodes_socket[];
extern WinSockErrorHelp errCodes_ioctlsocket[];
extern WinSockErrorHelp errCodes_connect[];
extern WinSockErrorHelp errCodes_recv[];
extern WinSockErrorHelp errCodes_bind[];
extern WinSockErrorHelp errCodes_listen[];
extern WinSockErrorHelp errCodes_accept[];
extern WinSockErrorHelp errCodes_send[];
extern WinSockErrorHelp errCodes_setsockopt[];
extern WinSockErrorHelp errCodes_shutdown[];
extern WinSockErrorHelp errCodes_closesocket[];