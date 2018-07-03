#include "TCPSocket.h"

namespace PNet
{
	TCPSocket::TCPSocket(ConnectionType connectionType, SocketHandle socketHandle, bool isConnected)
		:Socket(SocketType::TCP, connectionType, socketHandle)
	{
		/*if (connectionType == ConnectionType::IPV6)
		{
			throw std::exception("IPV6 Support not yet implemented.");
		}*/
		this->connectionType = connectionType;
		isConnected = isConnected;
	}
	PRESULT TCPSocket::Connect(IPAddress ipAddress, uint32_t port, timeval timeOut)
	{
		isConnected = false;
		PRESULT result = PRESULT::SUCCESS;

		//Create socket if it does not exist
		if (GetHandle() == INVALID_SOCKET)
		{
			result = Create();
			if (result != PRESULT::SUCCESS)
				return result;
		}

		if (this->GetIPProtocol() == ConnectionType::IPV4)
		{
			//Create remote address
			sockaddr_in address;
			std::memset(&address, 0, sizeof(address)); //clear out memory for addr (same idea as zeromemory)
			address.sin_addr.s_addr = htonl(ipAddress.ToInteger());
			address.sin_family = AF_INET;
			address.sin_port = htons(port);

			if (timeOut.tv_sec == 0 && timeOut.tv_usec == 0)
			{
				if (connect(GetHandle(), reinterpret_cast<sockaddr*>(&address), sizeof(address)) == -1)
					return PRESULT::TCPSOCKET_ConnectionFailed;
				isConnected = true;
				return PRESULT::SUCCESS;
			}
			else
			{
				fd_set writefds, exceptfds;
				FD_ZERO(&writefds);
				FD_ZERO(&exceptfds);
				FD_SET(GetHandle(), &writefds);
				FD_SET(GetHandle(), &exceptfds);

				connect(GetHandle(), reinterpret_cast<sockaddr*>(&address), sizeof(address));

				select(0, NULL, &writefds, &exceptfds, &timeOut);

				if (FD_ISSET(GetHandle(), &exceptfds)) //Did exceptional condition occur?
				{
					int winsockError = 0;
					int size = sizeof(int);
					if (getsockopt(GetHandle(), SOL_SOCKET, SO_ERROR, (char*)&winsockError, &size) == 0)
					{
						return PRESULT::SOCKET_WSALASTERRORNOTSETUP;
					}
					else
					{
						return PRESULT::SOCKET_WSALASTERRORGETSOCKOPTFAIL;
					}
				}
				if (FD_ISSET(GetHandle(), &writefds)) //Is connection connected?
				{
					isConnected = true;
					return PRESULT::SUCCESS;
				}

				return PRESULT::TCPSOCKET_ConnectionFailed;
			}
		}
		else //ipv6
		{
			//Create remote address
			sockaddr_in6 address;
			std::memset(&address, 0, sizeof(address)); //clear out memory for addr (same idea as zeromemory)
			address.sin6_addr = ipAddress.ToIPV6Addr();
			address.sin6_family = AF_INET6;
			address.sin6_port = htons(port);

			if (timeOut.tv_sec == 0 && timeOut.tv_usec == 0)
			{
				if (connect(GetHandle(), reinterpret_cast<sockaddr*>(&address), sizeof(address)) == -1)
					return PRESULT::TCPSOCKET_ConnectionFailed;
				isConnected = true;
				return PRESULT::SUCCESS;
			}
			else
			{
				fd_set writefds, exceptfds;
				FD_ZERO(&writefds);
				FD_ZERO(&exceptfds);
				FD_SET(GetHandle(), &writefds);
				FD_SET(GetHandle(), &exceptfds);

				connect(GetHandle(), reinterpret_cast<sockaddr*>(&address), sizeof(address));

				select(0, NULL, &writefds, &exceptfds, &timeOut);

				if (FD_ISSET(GetHandle(), &exceptfds)) //Did exceptional condition occur?
				{
					int winsockError = 0;
					int size = sizeof(int);
					if (getsockopt(GetHandle(), SOL_SOCKET, SO_ERROR, (char*)&winsockError, &size) == 0)
					{
						return PRESULT::SOCKET_WSALASTERRORNOTSETUP;
					}
					else
					{
						return PRESULT::SOCKET_WSALASTERRORGETSOCKOPTFAIL;
					}
				}
				if (FD_ISSET(GetHandle(), &writefds)) //Is connection connected?
				{
					isConnected = true;
					return PRESULT::SUCCESS;
				}

				return PRESULT::TCPSOCKET_ConnectionFailed;
			}
		}
	}

	bool TCPSocket::IsConnected()
	{
		return isConnected;
	}
	PRESULT TCPSocket::Disconnect()
	{
		isConnected = false;
		return Close();
	}
}