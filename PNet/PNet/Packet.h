#pragma once
#include <stdint.h>
#include <cstdint>
#include <vector>
#include <memory>
#include "PacketException.h"
#include "PacketType.h"


#define MAX_PACKET_SIZE 8192 //Maximum packet size, can be changed to whatever - If a connection tries to send packet > MAX_PACKET_SIZE, the connection will be forcefully closed

namespace PNet
{
	enum class PacketRetrievalTask
	{
		ReceivingPacketSize,
		ReceivingPacketBuffer
	};

	enum class PacketSendingTask
	{
		SendingPacketSize,
		SendingPacketBuffer
	};

	class Packet
	{
	public:
		Packet();
		Packet(const PacketType p); //Used for when sending a packet that only contains a packet type (Ex. End of File Packet)
		Packet(const std::shared_ptr<Packet> p);
		Packet(const char * buffer, const int size); //assumes first 4 bytes of buffer contain packet size
		void Append(const char * buffer, const int size); //Will append buffer to packet of "size" bytes
		std::vector<int8_t> buffer; //Packet Buffer

		int32_t size = 0;

		Packet& operator << (PacketType packetType);
		Packet& operator << (std::shared_ptr<Packet> packet);
		Packet& operator << (std::string data);
		Packet& operator << (uint32_t data);
		Packet& operator << (uint16_t data);
		Packet& operator << (uint8_t data);
		Packet& operator << (int32_t data);
		Packet& operator << (int16_t data);
		Packet& operator << (int8_t data);

		Packet& operator >> (PacketType & packetType);
		Packet& operator >> (std::string & data);
		Packet& operator >> (uint32_t & data);
		Packet& operator >> (uint16_t & data);
		Packet& operator >> (uint8_t & data);
		Packet& operator >> (int32_t & data);
		Packet& operator >> (int16_t & data);
		Packet& operator >> (int8_t & data);
	private:
		int extractionOffset = 0;
	};
}
