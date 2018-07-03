#pragma once
#include "TCPSocket.h"
#include "PacketManager.h"

namespace PNet
{
	class TCPServer;

	class TCPConnection : public TCPSocket
	{
		friend class TCPServer;
	public:
		TCPConnection(ConnectionType connectionType = ConnectionType::IPV4, SocketHandle socketHandle = INVALID_SOCKET, bool isConnected = false);
	protected:
		PacketManager incoming_pm;
		PacketManager outgoing_pm;
		PacketRetrievalTask incoming_packet_task = PacketRetrievalTask::ReceivingPacketSize;
		PacketSendingTask outgoing_packet_task = PacketSendingTask::SendingPacketSize;
		uint32_t incoming_packet_length = 0;
		uint32_t incoming_bytes_received = 0;
		uint32_t outgoing_packet_length = 0;
		uint32_t outgoing_bytes_sent = 0;
		char buffer[MAX_PACKET_SIZE];
	};
}