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
#include "IPProtocol.h"
#include "PRESULT.h"
#include "IPVersion.h"

namespace PNet
{
#ifndef INVALID_SOCKET
    #define INVALID_SOCKET  ((SocketHandle)(~0))
#endif
#ifndef SOCKET_ERROR
    #define SOCKET_ERROR    (-1)
#endif
	const SocketHandle INVALID_SOCKET_CONST = INVALID_SOCKET;

	class Socket
	{
	public:
		Socket(IPProtocol ipprotocol, IPVersion ipversion = IPVersion::IPV4, SocketHandle handle = INVALID_SOCKET_CONST);
	public:
		SocketHandle GetHandle();
		IPVersion GetIPVersion();
		PRESULT Close();
	protected:
		PRESULT Create(bool isListener = false);
		bool IsBlocking();
		PRESULT SetBlocking(bool isBlocking);
	private:
		IPProtocol ipprotocol = IPProtocol::INVALID;
		IPVersion ipversion = IPVersion::IPV4;
		SocketHandle handle = INVALID_SOCKET_CONST;
		bool isBlocking = false;
	};
}
