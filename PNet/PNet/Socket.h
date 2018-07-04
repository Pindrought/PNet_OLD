#pragma once
#include "SocketHandle.h"
#if defined _WIN32
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#endif
#include "SocketType.h"
#include "PRESULT.h"
#include "ConnectionType.h"

namespace PNet
{
    #define INVALID_SOCKET  ((SocketHandle)(~0))
    #define SOCKET_ERROR    (-1)
	const SocketHandle INVALID_SOCKET_CONST = INVALID_SOCKET;

	class Socket
	{
	public:
		Socket(SocketType type, ConnectionType connectionType = ConnectionType::IPV4, SocketHandle handle = INVALID_SOCKET_CONST);
	public:
		SocketHandle GetHandle();
		ConnectionType GetIPProtocol();
		PRESULT Close();
	protected:
		PRESULT Create(bool isListener = false);
		bool IsBlocking();
		PRESULT SetBlocking(bool isBlocking);
	private:
		SocketType type = SocketType::INVALID;
		ConnectionType connectionType = ConnectionType::IPV4;
		SocketHandle handle = INVALID_SOCKET_CONST;
		bool isBlocking = false;
	};
}
