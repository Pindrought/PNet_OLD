#include "PRESULT.h"

namespace PNet
{
	std::string PRESULTToString(PRESULT presult)
	{
		switch (presult)
		{
		case PRESULT::SOCKET_NOTYPEDEFINED:
			return "Socket type is undefined. Expected socket type TCP or UDP.";
		case PRESULT::SOCKET_FAILEDTOCREATE:
			return "Failed to create socket.";
		case PRESULT::SOCKET_ALREADYINITIALIZED:
			return "Socket handle has already been initialized. Socket::Create should only be called for sockets that have not yet been initialized.";
		case PRESULT::SOCKET_UNEXPECTEDERROR:
			return "Unexpected socket error occurred. Please report to PNet dev team. Error Code: " + std::to_string(presult) + ".";
		case PRESULT::SOCKET_CLOSEUNINITIALIZEDSOCKET:
			return "Error occurred - Attempted to close uninitialized socket.";
		case PRESULT::SOCKET_WSALASTERRORNOTSETUP:
			return "This isn't set up currently. TODO...";

		case PRESULT::SOCKET_FAILEDSETTCPNODELAY:
			return "Failed to set TCP No Delay on socket to prevent buffering.";
		case PRESULT::SOCKET_FAILEDENABLEBROADCAST:
			return "Failed to enable broadcast for UDP socket.";
		case PRESULT::SOCKET_WSALASTERRORGETSOCKOPTFAIL:
			return "Winsock error occured and getsockopt() failed to retrieve the error code.";
		case PRESULT::TCPSOCKET_CONNECTIONFAILED:
			return "TCP Socket failed to connect.";
        case PRESULT::TCPSOCKET_INVALIDIP:
			return "Invalid IP Address entered for selected IP version.";
		case PRESULT::TCPLISTENER_UNABLE_TO_BIND_SOCKET:
			return "TCP Listener unable to bind listening socket.";
		case PRESULT::SUCCESS:
			return "No error occurred. Operation successful.";
		default:
			return "Unexpected PRESULT code: " + std::to_string(presult) + " - Please report to PNet dev team.";
		}
	}
}
