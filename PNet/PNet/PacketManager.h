#pragma once
#include <queue> //for std::queue
#include <memory> //for std::shared_ptr
#include "Packet.h"

namespace PNet
{
	class PacketManager
	{
	private:
		std::queue<std::shared_ptr<Packet>> m_queue_packets;
	public:
		void Clear();
		bool HasPendingPackets();
		void Append(std::shared_ptr<Packet> p);
		std::shared_ptr<Packet> Retrieve();
		void Pop();
	};
}