#include "Socket.h"
#include <iostream>
#ifdef _WIN32
#include <ws2ipdef.h>
#else
#include <unistd.h>
 #include <sys/ioctl.h>
#endif

namespace PNet
{
	Socket::Socket(SocketType type, ConnectionType connectionType, SocketHandle handle)
	{
		if (type != SocketType::TCP)
		{
			throw std::runtime_error("ONLY TCP IS CURRENTLY SUPPORTED");
		}
		this->type = type;
		this->connectionType = connectionType;
		this->handle = handle;
	}
	PRESULT Socket::Create(bool isListener)
	{
		if (handle != INVALID_SOCKET_CONST)
		{
			return PRESULT::SOCKET_ALREADYINITIALIZED;
		}

		if (type == SocketType::TCP || type == SocketType::UDP)
		{
			if (this->connectionType == ConnectionType::IPV4)
			{
				handle = socket(PF_INET, type == SocketType::TCP ? SOCK_STREAM : SOCK_DGRAM, type == SocketType::TCP ? IPPROTO_TCP : IPPROTO_UDP);
			}
			else //ipv6
			{
				handle = socket(PF_INET6, type == SocketType::TCP ? SOCK_STREAM : SOCK_DGRAM, type == SocketType::TCP ? IPPROTO_TCP: IPPROTO_UDP);
			}

			if (handle == INVALID_SOCKET_CONST)
			{
				return PRESULT::SOCKET_FAILEDTOCREATE;
			}

			PRESULT result = SetBlocking(false);
			if (result != PRESULT::SUCCESS)
			{
				return result;
			}

			if (type == SocketType::TCP)
			{
				// Disable the Nagle algorithm (i.e. removes buffering of TCP packets)
				int yes = 1;
				if (setsockopt(handle, IPPROTO_TCP, TCP_NODELAY, reinterpret_cast<char*>(&yes), sizeof(yes)) == SOCKET_ERROR)
				{
					return PRESULT::SOCKET_FAILEDSETTCPNODELAY;
				}
				//set socket to reuse address
				int flag = 1;
				if (setsockopt(handle, SOL_SOCKET, SO_REUSEADDR, (char*)&flag, sizeof(flag)) == SOCKET_ERROR)
				{
					std::cerr << "Failed to set socket to reuse address." << std::endl;
				}
				if (this->connectionType == ConnectionType::IPV6)
				{
					if (isListener)
					{
						int no = 0;
						if (setsockopt(handle, IPPROTO_IPV6, IPV6_V6ONLY, (char *)&no, sizeof(no)) == SOCKET_ERROR)
						{
							std::cerr << "Failed to disable ipv6 only option. Only IPV6 connections will be able to be established." << std::endl;
						}
					}
				}
			}
			else //UDP
			{
				// Enable broadcast by default for UDP sockets
				int yes = 1;
				if (setsockopt(handle, SOL_SOCKET, SO_BROADCAST, reinterpret_cast<char*>(&yes), sizeof(yes)) == -1)
				{
					return PRESULT::SOCKET_FAILEDENABLEBROADCAST;
				}
			}
			return PRESULT::SUCCESS;
		}
		else
		{
			return PRESULT::SOCKET_NOTYPEDEFINED;
		}
		return PRESULT::SOCKET_UNEXPECTEDERROR;
	}
	PRESULT Socket::Close()
	{
		if (handle == INVALID_SOCKET_CONST)
			return PRESULT::SOCKET_CLOSEUNINITIALIZEDSOCKET;
        #ifdef _WIN32
		int result = closesocket(handle);
		#else
		int result = close(handle);
		#endif
		if (result == 0)
		{
			return PRESULT::SUCCESS;
		}
		else
		{
			return PRESULT::SOCKET_WSALASTERRORNOTSETUP;
		}
	}
	bool Socket::IsBlocking()
	{
		return isBlocking;
	}
	PRESULT	Socket::SetBlocking(bool isBlocking)
	{
		u_long blocking = isBlocking ? 0 : 1;
		#ifdef _WIN32
		int result = ioctlsocket(handle, FIONBIO, &blocking);
		#else
		int result = ioctl(handle, FIONBIO, &blocking);
		#endif
		if (result != 0)
		{
			return PRESULT::SOCKET_WSALASTERRORNOTSETUP;
		}
		return PRESULT::SUCCESS;
	}
	SocketHandle Socket::GetHandle()
	{
		return handle;
	}

	ConnectionType Socket::GetIPProtocol()
	{
		return connectionType;
	}


#ifdef _WIN32
	namespace priv
	{
		struct SocketInitializer
		{
			SocketInitializer()
			{

				WSADATA init;
				if (WSAStartup(MAKEWORD(2, 2), &init) != 0)
				{
					std::cerr << "WSA Startup failed with error code: " << WSAGetLastError() << std::endl;
				}
				else
				{
					std::cout << "WSA Startup looks fine." << std::endl;
				}

			}

			~SocketInitializer()
			{
				WSACleanup();
			}
		};

		SocketInitializer globalInitializer;
	}
#endif
}
