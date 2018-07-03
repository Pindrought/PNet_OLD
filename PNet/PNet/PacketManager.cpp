#include "PacketManager.h"

namespace PNet
{
	void PacketManager::Clear()
	{
		m_queue_packets = std::queue<std::shared_ptr<Packet>>{};//Clear out packet queue
	}

	bool PacketManager::HasPendingPackets()
	{
		return (!m_queue_packets.empty()); //returns true if packets are pending
	}

	void PacketManager::Append(std::shared_ptr<Packet> p)
	{
		m_queue_packets.push(std::move(p)); //Add packet to queue
	}

	std::shared_ptr<Packet> PacketManager::Retrieve()
	{
		std::shared_ptr<Packet> p = m_queue_packets.front(); //Get packet from front of queue
		return p; //Return packet that was removed from the queue
	}

	void PacketManager::Pop()
	{
		m_queue_packets.pop(); //Remove packet from front of queue
	}
}