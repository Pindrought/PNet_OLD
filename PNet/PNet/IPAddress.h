#pragma once

#include "IPVersion.h"
#include <string>
#ifdef _WIN32
#include <WS2tcpip.h>
#else
#include <string.h>
#include <arpa/inet.h>
#include <netdb.h>
#endif
#include <exception>

namespace PNet
{
	class IPAddress
	{
	public:
		IPAddress(std::string ip, IPVersion protocol);
		std::string ToString();
		uint32_t ToInteger();
		in6_addr ToIPV6Addr();
		bool IsValid();
	private:
		void resolve(const std::string & address);
		uint32_t address;
		IPVersion protocol;
		in6_addr ipv6addr;
		bool isValid = false;
	};
}
