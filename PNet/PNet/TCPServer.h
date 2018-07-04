#pragma once
#include "TCPListener.h"
#include <vector>

#define MAX_CONNECTIONS 1000

namespace PNet
{
	class TCPServer
	{
	public:
		TCPServer(IPVersion ipversion = IPVersion::IPV4, unsigned short listen_port = 8888);
		PRESULT StartListener();
		void Loop(timeval timeout = { 0,0 });
		virtual void ProcessPackets();
		void SendPacket(TCPConnection & connection, std::shared_ptr<Packet> packet);
		~TCPServer();
	private:
		IPVersion ipversion;
		unsigned short listen_port;
		TCPListener * listener = nullptr;
		std::vector<TCPConnection> connections;
		fd_set master;

		void PrintExceptionalCondition(const SocketHandle socket);
		#ifndef _WIN32
		SocketHandle max_fd = 0;
		#endif

	};
}
