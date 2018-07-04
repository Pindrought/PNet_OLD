#pragma once
#include "Socket.h"
#include "IPAddress.h"


namespace PNet
{
	class TCPSocket : public Socket
	{
	public:
		TCPSocket(IPVersion ipversion = IPVersion::IPV4, SocketHandle socketHandle = INVALID_SOCKET_CONST, bool isConnected = false);

		PRESULT Connect(IPAddress ipAddress, uint32_t port, timeval timeOut = { 0,0 });
		bool IsConnected();

		PRESULT Disconnect();
	private:
		bool isConnected = false;
		IPVersion ipversion = IPVersion::IPV4;
	};
}
