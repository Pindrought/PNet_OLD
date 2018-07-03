#pragma once
#include <string>
#include "ConnectionType.h"
#include <WS2tcpip.h>
#include <exception>

namespace PNet
{
	class IPAddress
	{
	public:
		IPAddress(std::string ip, ConnectionType protocol);
		std::string ToString();
		uint32_t ToInteger();
		in6_addr ToIPV6Addr();
	private:
		void resolve(const std::string & address);
		uint32_t address;
		ConnectionType protocol;
		in6_addr ipv6addr;
	};
}