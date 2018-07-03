#pragma once
#include "SocketHandle.h"
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")
#include "SocketType.h"
#include "PRESULT.h"
#include "ConnectionType.h"

namespace PNet
{
	class Socket
	{
	public:
		Socket(SocketType type, ConnectionType connectionType = ConnectionType::IPV4, SocketHandle handle = INVALID_SOCKET);
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
		SocketHandle handle = INVALID_SOCKET;
		bool isBlocking = false;
	};
}