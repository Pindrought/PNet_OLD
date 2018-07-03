#include "TCPListener.h"
#include <iostream>

namespace PNet
{
	TCPListener::TCPListener(ConnectionType connectionType)
		:Socket(SocketType::TCP, connectionType, INVALID_SOCKET_CONST)
	{
	}

	
	PRESULT TCPListener::Listen(unsigned short port)
	{

		auto result = this->Create(true);
		if (result != PRESULT::SUCCESS)
		{
			return result;
		}

		if (this->GetIPProtocol() == ConnectionType::IPV4)
		{
			SOCKADDR_IN listener_info;
			listener_info.sin_family = AF_INET;
			listener_info.sin_addr.s_addr = INADDR_ANY; //anyone can connect (don't loopback to self)
			listener_info.sin_port = htons(port);

			if (bind(this->GetHandle(), (SOCKADDR*)(&listener_info), sizeof(listener_info)) == SOCKET_ERROR) //bind socket (required before listening)
			{
				return PRESULT::TCPLISTENER_UNABLE_TO_BIND_SOCKET;
			}
			listen(this->GetHandle(), SOMAXCONN); //listen on socket with a max of SOMAXCONN connections in backlog
		}
		else //ipv6
		{
			sockaddr_in6 server_addr;
			memset(&server_addr, 0, sizeof(server_addr));
			server_addr.sin6_family = AF_INET6;
			server_addr.sin6_addr = in6addr_any;
			server_addr.sin6_port = htons(port);

			if (bind(this->GetHandle(), (SOCKADDR*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR)
			{
				std::cerr << "Bind failed with error code: " << WSAGetLastError() << std::endl;
				std::cerr << "Handle: " << this->GetHandle() << std::endl;
				return PRESULT::TCPLISTENER_UNABLE_TO_BIND_SOCKET;
			}
			listen(this->GetHandle(), SOMAXCONN);
		}
		return PRESULT::SUCCESS;
	}
}