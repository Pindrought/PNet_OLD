#include "Packet.h"
#include <cstdint> //Required to use std::std::int32_t
#ifdef _WIN32
#include <WinSock2.h> //for htonl/htons
#else
#include <arpa/inet.h> //for htonl/htons
#endif

namespace PNet
{
	Packet::Packet()
	{

	}

	Packet::Packet(const std::shared_ptr<Packet> packet)
	{
		*this << packet;
	}

	Packet::Packet(const char * buffer, const int size)
	{
		Append(buffer, size);
	}

	Packet::Packet(PacketType pt)
	{
		*this << pt;
	}

	void Packet::Append(const char * dataBuffer, const int size)
	{
		buffer.insert(buffer.end(), dataBuffer, dataBuffer + size);
		this->size += size;
	}

	Packet& Packet::operator <<(PacketType packetType)
	{
		uint32_t data = static_cast<uint32_t>(packetType);
		*this << data;
		return *this;
	}

	Packet& Packet::operator <<(std::shared_ptr<Packet> packet)
	{
		Append(reinterpret_cast<const char*>(&packet->buffer[0]), packet->buffer.size());
		return *this;
	}

	Packet& Packet::operator <<(std::string data)
	{
		uint32_t len = static_cast<uint32_t>(data.size());
		*this << len;
		Append(data.c_str(), static_cast<int>(data.size()));
		return *this;
	}

	Packet& Packet::operator <<(uint32_t data)
	{
		data = htonl(data);
		Append(reinterpret_cast<char*>(&data), sizeof(uint32_t));
		return *this;
	}

	Packet& Packet::operator <<(uint16_t data)
	{
		data = htons(data);
		Append(reinterpret_cast<char*>(&data), sizeof(uint16_t));
		return *this;
	}

	Packet& Packet::operator <<(uint8_t data)
	{
		Append(reinterpret_cast<char*>(&data), sizeof(uint8_t));
		return *this;
	}

	Packet& Packet::operator <<(int32_t data)
	{
		data = htonl(data);
		Append(reinterpret_cast<char*>(&data), sizeof(int32_t));
		return *this;
	}

	Packet& Packet::operator <<(int16_t data)
	{
		data = htons(data);
		Append(reinterpret_cast<char*>(&data), sizeof(int16_t));
		return *this;
	}

	Packet& Packet::operator <<(int8_t data)
	{
		Append(reinterpret_cast<char*>(&data), sizeof(int8_t));
		return *this;
	}

	Packet& Packet::operator >> (PacketType & packetType)
	{
		if (extractionOffset + sizeof(uint32_t) > buffer.size())
			throw PacketException("[Packet::operator >>(PacketType & data)] - Extraction offset exceeded buffer size.");
		uint32_t data = 0;
		*this >> data;
		packetType = static_cast<PacketType>(data);
		return *this;
	}

	Packet& Packet::operator >> (std::string & data)
	{
		uint32_t len = 0;
		*this >> len;

		data.clear();

		if (extractionOffset + len > buffer.size())
			throw PacketException("[Packet::operator >>(std::string & data)] - Extraction offset exceeded buffer size.");

		data.assign(reinterpret_cast<char*>(&buffer[extractionOffset]), len);
		extractionOffset += len;

		return *this;
	}

	Packet& Packet::operator >>(uint32_t & data)
	{
		if (extractionOffset + sizeof(uint32_t) > buffer.size())
			throw PacketException("[Packet::operator >>(uint32_t & data)] - Extraction offset exceeded buffer size.");
		data = *reinterpret_cast<const uint32_t*>(&buffer[extractionOffset]);
		extractionOffset += sizeof(uint32_t);
		data = ntohl(data);
		return *this;
	}

	Packet& Packet::operator >> (uint16_t & data)
	{
		if (extractionOffset + sizeof(uint16_t) > buffer.size())
			throw PacketException("[Packet::operator >> (uint16_t & data)] - Extraction offset exceeded buffer size.");
		data = *reinterpret_cast<const uint16_t*>(&buffer[extractionOffset]);
		extractionOffset += sizeof(uint16_t);
		data = ntohs(data);
		return *this;
	}

	Packet& Packet::operator >> (uint8_t & data)
	{
		if (extractionOffset + sizeof(uint8_t) > buffer.size())
			throw PacketException("[Packet::operator >> (uint8_t & data)] - Extraction offset exceeded buffer size.");
		data = *reinterpret_cast<const uint8_t*>(&buffer[extractionOffset]);
		extractionOffset += sizeof(uint8_t);
		return *this;
	}

	Packet& Packet::operator >> (int32_t & data)
	{
		if (extractionOffset + sizeof(int32_t) > buffer.size())
			throw PacketException("[Packet::operator >> (int32_t & data)] - Extraction offset exceeded buffer size.");
		data = *reinterpret_cast<const int32_t*>(&buffer[extractionOffset]);
		extractionOffset += sizeof(int32_t);
		return *this;
	}

	Packet& Packet::operator >> (int16_t & data)
	{
		if (extractionOffset + sizeof(int16_t) > buffer.size())
			throw PacketException("[Packet::operator >> (int16_t & data)] - Extraction offset exceeded buffer size.");
		data = *reinterpret_cast<const int16_t*>(&buffer[extractionOffset]);
		extractionOffset += sizeof(int16_t);
		return *this;
	}

	Packet& Packet::operator >> (int8_t & data)
	{
		if (extractionOffset + sizeof(int8_t) > buffer.size())
			throw PacketException("[Packet::operator >> (int8_t & data)] - Extraction offset exceeded buffer size.");
		data = *reinterpret_cast<const int8_t*>(&buffer[extractionOffset]);
		extractionOffset += sizeof(int8_t);
		return *this;
	}
}
