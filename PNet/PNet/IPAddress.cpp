#include "IPAddress.h"
#include <iostream>

namespace PNet
{
	IPAddress::IPAddress(std::string ip, ConnectionType protocol)
	{
		/*if (protocol == ConnectionType::IPV6)
		{
			throw std::exception("IPV6 not yet implemented.");
		}*/
		this->protocol = protocol;
		resolve(ip);
	}

	std::string IPAddress::ToString()
	{
		std::string result;

		if (this->protocol == ConnectionType::IPV4)
		{
			result.resize(INET_ADDRSTRLEN);
			in_addr address;
			address.s_addr = this->address;
			inet_ntop(AF_INET, &address, reinterpret_cast<char*>(&result[0]), INET_ADDRSTRLEN);
		}
		else
		{
			result.resize(INET6_ADDRSTRLEN);
			sockaddr_in6 address;
			address.sin6_addr = this->ipv6addr;
			inet_ntop(AF_INET6, &address, reinterpret_cast<char*>(&result[0]), INET6_ADDRSTRLEN);
		}
		return result;
	}

	uint32_t IPAddress::ToInteger()
	{
		return ntohl(address);
	}

	in6_addr IPAddress::ToIPV6Addr()
	{
		return ipv6addr;
	}

	void IPAddress::resolve(const std::string& address)
	{
		this->address = 0;
		if (this->protocol == ConnectionType::IPV4)
		{
			if (address == "255.255.255.255")
			{
				// The broadcast address needs to be handled explicitly,
				// because it is also the value returned by inet_addr on error
				this->address = INADDR_BROADCAST;
				isValid = true;
			}
			else if (address == "0.0.0.0")
			{
				this->address = INADDR_ANY;
				isValid = true;
			}
			else
			{
				// Try to convert the address as a byte representation ("xxx.xxx.xxx.xxx")
				uint32_t ip = INADDR_NONE;
				inet_pton(AF_INET, address.c_str(), &ip);

				if (ip != INADDR_NONE)
				{
					this->address = ip;
					isValid = true;
				}
				else
				{
					// Not a valid address, try to convert it as a host name
					addrinfo hints;
					memset(&hints, 0, sizeof(hints));
					hints.ai_family = AF_INET;
					addrinfo* result = NULL;
					if (getaddrinfo(address.c_str(), NULL, &hints, &result) == 0)
					{
						if (result)
						{
							ip = reinterpret_cast<sockaddr_in*>(result->ai_addr)->sin_addr.s_addr;
							freeaddrinfo(result);
							this->address = ip;
							isValid = true;
						}
						else
						{
							std::cerr << "Invalid IPV4 IP entered: " << address << std::endl;
						}
					}
					else
					{
						std::cerr << "Invalid IPV4 IP entered: " << address << std::endl;
					}
				}
			}
		}
		else if (this->protocol == ConnectionType::IPV6)
		{
			// Try to convert the address as a byte representation ("xxx.xxx.xxx.xxx")
			in6_addr ip;
			if (inet_pton(AF_INET6, address.c_str(), &ip) != 1)
			{
				std::cerr << "Failed to convert IPV6 IP: " << address << std::endl;
			}
			else
			{
                this->ipv6addr = ip;
                isValid = true;
			}
		}
		else
		{
			throw std::runtime_error("IPAddress protocol is not IPV4 or IPV6. Something went wrong.");
		}
	}
	bool IPAddress::IsValid()
	{
        return isValid;
	}
}
