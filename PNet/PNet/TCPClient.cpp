#include "TCPClient.h"
#include <iostream>

namespace PNet
{
	TCPClient::TCPClient(ConnectionType connectionType, SocketHandle socketHandle, bool isConnected)
		:TCPConnection(connectionType, socketHandle, isConnected)
	{
	}
	void TCPClient::Loop(const timeval timeout)
	{
		if (IsConnected() == false)
		{
			return;
		}

		fd_set readfds = master;
		fd_set writefds = master;
		fd_set exceptfds = master;

		select(0, &readfds, &writefds, &exceptfds, &timeout);

		
		if (FD_ISSET(this->GetHandle(), &exceptfds))
		{
			//PrintExceptionalCondition(connections[i].socket);
			std::cout << "Winsock Error occurred on TCPClient:" << this->GetHandle() << ". Disconnecting client." << std::endl;
			//closesocket(connections[i].socket);
			this->Disconnect();
			return;
		}

		if (FD_ISSET(this->GetHandle(), &readfds))
		{

			int retval = SOCKET_ERROR;
			if (this->incoming_packet_task == PacketRetrievalTask::ReceivingPacketSize)
			{
				retval = recv(this->GetHandle(), (char*)&this->incoming_packet_length + this->incoming_bytes_received, sizeof(uint32_t) - this->incoming_bytes_received, NULL);
			}
			else //RetrievingPacketBuffer
			{
				retval = recv(this->GetHandle(), (char*)this->buffer + this->incoming_bytes_received, this->incoming_packet_length - this->incoming_bytes_received, NULL);
			}

			if (retval == SOCKET_ERROR)
			{
				if (WSAGetLastError() != WSAEWOULDBLOCK)
				{
					std::cout << "Lost connection to " << this->GetHandle() << " --- Reason: " << WSAGetLastError() << std::endl;
					FD_CLR(this->GetHandle(), &master);
					this->Disconnect();
					return;
				}
			}
			if (retval == 0)
			{
				std::cout << "Lost connection to " << this->GetHandle() << std::endl;
				FD_CLR(this->GetHandle(), &master);
				this->Disconnect();
				return;
			}
			if (retval > 0)
			{
				this->incoming_bytes_received += retval; //Add the # of bytes received to our bytesReceived variable so we can check if we have received the full packet

				if (this->incoming_packet_task == PacketRetrievalTask::ReceivingPacketSize)
				{
					if (this->incoming_bytes_received == sizeof(int32_t)) //If full packet size received
					{
						ZeroMemory(this->buffer, this->incoming_packet_length);
						this->incoming_bytes_received = 0;
						this->incoming_packet_task = PacketRetrievalTask::ReceivingPacketBuffer;
					}
				}
				else //If ReceivingPacketBuffer
				{
					if (this->incoming_bytes_received == this->incoming_packet_length) //If full packet buffer received
					{
						this->incoming_pm.Append(std::make_shared<Packet>(this->buffer, this->incoming_packet_length));
						this->incoming_bytes_received = 0;
						this->incoming_packet_length = 0;
						this->incoming_packet_task = PacketRetrievalTask::ReceivingPacketSize;
					}
				}
			}
		}

		if (FD_ISSET(this->GetHandle(), &writefds))
		{
			int retval = 1;
			while (this->outgoing_pm.HasPendingPackets() && retval > 0)
			{
				if (this->outgoing_packet_task == PacketSendingTask::SendingPacketSize) //If Sending Packet Size (int32_t)
				{
					retval = send(this->GetHandle(), (char*)(&this->outgoing_pm.Retrieve()->size) + this->outgoing_bytes_sent, sizeof(uint32_t) - this->outgoing_bytes_sent, NULL);
					if (retval > 0)
					{
						this->outgoing_bytes_sent += retval;
					}
					if (this->outgoing_bytes_sent == sizeof(uint32_t))
					{
						this->outgoing_bytes_sent = 0;
						this->outgoing_packet_task = PacketSendingTask::SendingPacketBuffer;
					}
				}
				else //If Sending Packet Buffer
				{
					retval = send(this->GetHandle(), (char*)(&this->outgoing_pm.Retrieve()->buffer[0]) + this->outgoing_bytes_sent, this->outgoing_pm.Retrieve()->size - this->outgoing_bytes_sent, NULL);
					if (retval > 0)
					{
						this->outgoing_bytes_sent += retval;
					}
					if (this->outgoing_bytes_sent == this->outgoing_pm.Retrieve()->size)
					{
						this->outgoing_bytes_sent = 0;
						this->outgoing_packet_task = PacketSendingTask::SendingPacketSize;
						//std::cout << "Packet Sent..." << std::endl;
						this->outgoing_pm.Pop(); //Remove packet from queue after finished processing
					}
				}
			}
		}
		ProcessPackets();
	}

	bool TCPClient::Initialize()
	{
		if (IsConnected() == false)
		{
			return false;
		}
		else
		{
			FD_ZERO(&master);
			FD_SET(this->GetHandle(), &master);
			return true;
		}
		return false;
	}

	void TCPClient::ProcessPackets()
	{
		while (this->incoming_pm.HasPendingPackets())
		{
			std::shared_ptr<Packet> packet = this->incoming_pm.Retrieve(); //Retrieve packet for processing
			std::cout << "TCPClient Received packet of size: " << packet->size << std::endl;
			this->incoming_pm.Pop(); //Remove packet from queue
		}
	}

	void TCPClient::SendPacket(std::shared_ptr<Packet> p)
	{
		this->outgoing_pm.Append(p);
	}

}

