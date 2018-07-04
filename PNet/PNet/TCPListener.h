#pragma once
#include "TCPClient.h"

namespace PNet
{
	class TCPListener : public Socket
	{
	public:
		TCPListener(IPVersion ipversion = IPVersion::IPV4);
		unsigned short GetLocalPort(); //Returns port that the socket is listening on - returns 0 if no socket bound/listening
		PRESULT Listen(unsigned short port);
	private:

	};
}