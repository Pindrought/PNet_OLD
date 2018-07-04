#pragma once
#include <string>

namespace PNet
{
	enum PRESULT
	{
		SOCKET_NOTYPEDEFINED, //When a socket either has an invalid type or does not have the socket type defined. Expected TCP or UDP.
		SOCKET_FAILEDTOCREATE, //When socket() returns NULL
		SOCKET_ALREADYINITIALIZED, //when Socket::Create is called, but handle is not INVALID_SOCKET_CONST
		SOCKET_UNEXPECTEDERROR,
		SOCKET_CLOSEUNINITIALIZEDSOCKET, //when Socket::Close is called on a socket handle that is not valid
		SOCKET_WSALASTERRORNOTSETUP, //this isn't set up at the moment properly
		SOCKET_FAILEDSETTCPNODELAY, //Failed to set TCP No Delay
		SOCKET_FAILEDENABLEBROADCAST, //Failed to enable broadcast for UDP sockets
		SOCKET_WSALASTERRORGETSOCKOPTFAIL, //WINSOCK ERROR OCCURED AND GETSOCKOPT FAILED

		TCPSOCKET_ConnectionFailed,
		TCPSOCKET_INVALIDIP,

		TCPLISTENER_UNABLE_TO_BIND_SOCKET,

		SUCCESS,
	};
	std::string PRESULTToString(PRESULT presult);
}
