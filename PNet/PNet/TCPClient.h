#pragma once
#include "TCPConnection.h"

namespace PNet
{
	class TCPClient : public TCPConnection
	{
	public:
		TCPClient(IPVersion ipversion = IPVersion::IPV4, SocketHandle socketHandle = INVALID_SOCKET_CONST, bool isConnected = false);
		void Loop(timeval timeout);
		bool Initialize(); //Initializes master fd set
		virtual void ProcessPackets();
		void SendPacket(std::shared_ptr<Packet> p);
	private:
		fd_set master;
	};
}
