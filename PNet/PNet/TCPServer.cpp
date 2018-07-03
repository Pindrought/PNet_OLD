#include "TCPServer.h"
#include <iostream>

namespace PNet
{
	TCPServer::TCPServer(ConnectionType ip_protocol, unsigned short listen_port)
		:ip_protocol(ip_protocol), listen_port(listen_port)
	{
		/*if (ip_protocol == ConnectionType::IPV6)
		{
			throw std::exception("IPV6 not yet implemented.");
		}*/

		connections.reserve(MAX_CONNECTIONS);
		FD_ZERO(&master);
	}

	PRESULT TCPServer::StartListener()
	{
		if (listener != nullptr)
		{
			listener->Close();
			delete listener;
		}
		listener = new TCPListener(this->ip_protocol);
		auto result = listener->Listen(listen_port);
		if (result != PRESULT::SUCCESS)
			return result;
		FD_SET(listener->GetHandle(), &master);
		return PRESULT::SUCCESS;
	}

	void TCPServer::Loop(const timeval timeout)
	{
		if (listener == nullptr)
		{
			std::cerr << "Listener not yet initialized for TCPServer." << std::endl;
			return;
		}

		fd_set readfds = master;
		fd_set writefds = master;
		fd_set exceptfds = master;
		
		select(0, &readfds, &writefds, &exceptfds, &timeout);

		if (connections.size() < connections.capacity())
		{
			if (FD_ISSET(listener->GetHandle(), &exceptfds))
			{
				//PrintExceptionalCondition(listener->GetHandle());
				std::cout << "Winsock Error occurred on listenSocket." << std::endl;
			}

			if (FD_ISSET(listener->GetHandle(), &readfds)) //if listenSocket was in readfds
			{
				SOCKET accept_socket_handle = accept(listener->GetHandle(), NULL, NULL);
				if (accept_socket_handle != INVALID_SOCKET) //If new connection was properly accepted
				{
					TCPConnection new_connection(this->ip_protocol, accept_socket_handle, true);
					connections.emplace_back(new_connection);
					FD_SET(new_connection.GetHandle(), &master);
					std::cout << "New Connection: " << accept_socket_handle << std::endl;
				}
				else
				{
					std::cout << "Connection Acception Failed: " << std::endl;
					PrintExceptionalCondition(listener->GetHandle());
				}
			}
		}

		for (int i = connections.size()-1; i >= 0; i--)
		{
			if (FD_ISSET(connections[i].GetHandle(), &exceptfds))
			{
				std::cout << "Winsock Error occurred on client:" << connections[i].GetHandle() << ". Disconnecting client." << std::endl;
				FD_CLR(connections[i].GetHandle(), &master);
				connections[i].Disconnect();
				connections.erase(connections.begin() + i);
				continue;
			}

			if (FD_ISSET(connections[i].GetHandle(), &readfds))
			{

				int retval = SOCKET_ERROR;
				if (connections[i].incoming_packet_task == PacketRetrievalTask::ReceivingPacketSize)
				{
					retval = recv(connections[i].GetHandle(), (char*)&connections[i].incoming_packet_length + connections[i].incoming_bytes_received, sizeof(uint32_t) - connections[i].incoming_bytes_received, NULL);
				}
				else //RetrievingPacketBuffer
				{
					retval = recv(connections[i].GetHandle(), (char*)connections[i].buffer + connections[i].incoming_bytes_received, connections[i].incoming_packet_length - connections[i].incoming_bytes_received, NULL);
				}

				if (retval == SOCKET_ERROR)
				{
					if (WSAGetLastError() != WSAEWOULDBLOCK)
					{
						std::cout << "TCPServer Lost connection to " << connections[i].GetHandle() << " --- Reason: " << WSAGetLastError() << std::endl;
						FD_CLR(connections[i].GetHandle(), &master);
						connections[i].Disconnect();
						connections.erase(connections.begin() + i);
						continue;
					}
				}
				if (retval == 0)
				{
					std::cout << "TCPServer Lost connection to " << connections[i].GetHandle() << std::endl;
					FD_CLR(connections[i].GetHandle(), &master);
					connections[i].Disconnect();
					connections.erase(connections.begin() + i);
					continue;
				}
				if (retval > 0)
				{
					connections[i].incoming_bytes_received += retval; //Add the # of bytes received to our bytesReceived variable so we can check if we have received the full packet

					if (connections[i].incoming_packet_task == PacketRetrievalTask::ReceivingPacketSize)
					{
						if (connections[i].incoming_bytes_received == sizeof(uint32_t)) //If full packet size received
						{
							ZeroMemory(connections[i].buffer, connections[i].incoming_packet_length);
							connections[i].incoming_bytes_received = 0;
							connections[i].incoming_packet_task = PacketRetrievalTask::ReceivingPacketBuffer;
						}
					}
					else //If ReceivingPacketBuffer
					{
						if (connections[i].incoming_bytes_received == connections[i].incoming_packet_length) //If full packet buffer received
						{
							connections[i].incoming_pm.Append(std::make_shared<Packet>(connections[i].buffer, connections[i].incoming_packet_length));
							connections[i].incoming_bytes_received = 0;
							connections[i].incoming_packet_length = 0;
							connections[i].incoming_packet_task = PacketRetrievalTask::ReceivingPacketSize;
						}
					}
				}
			}

			if (FD_ISSET(connections[i].GetHandle(), &writefds))
			{
				int retval = 1;
				while (connections[i].outgoing_pm.HasPendingPackets() && retval > 0)
				{
					if (connections[i].outgoing_packet_task == PacketSendingTask::SendingPacketSize) //If Sending Packet Size (int32_t)
					{
						retval = send(connections[i].GetHandle(), (char*)(&connections[i].outgoing_pm.Retrieve()->size) + connections[i].outgoing_bytes_sent, sizeof(uint32_t) - connections[i].outgoing_bytes_sent, NULL);
						if (retval > 0)
						{
							connections[i].outgoing_bytes_sent += retval;
						}
						if (connections[i].outgoing_bytes_sent == sizeof(int32_t))
						{
							connections[i].outgoing_bytes_sent = 0;
							connections[i].outgoing_packet_task = PacketSendingTask::SendingPacketBuffer;
						}
					}
					else //If Sending Packet Buffer
					{
						retval = send(connections[i].GetHandle(), (char*)(&connections[i].outgoing_pm.Retrieve()->buffer[0]) + connections[i].outgoing_bytes_sent, connections[i].outgoing_pm.Retrieve()->size - connections[i].outgoing_bytes_sent, NULL);
						if (retval > 0)
						{
							connections[i].outgoing_bytes_sent += retval;
						}
						if (connections[i].outgoing_bytes_sent == connections[i].outgoing_pm.Retrieve()->size)
						{
							connections[i].outgoing_bytes_sent = 0;
							connections[i].outgoing_packet_task = PacketSendingTask::SendingPacketSize;
							connections[i].outgoing_pm.Pop(); //Remove packet from queue after finished processing
						}
					}
				}
			}


			ProcessPackets(); //Process incoming packets
		}
	}

	void TCPServer::ProcessPackets()
	{
		for (int i = 0; i < connections.size(); i++) //for each connection
		{
			try
			{
				while (connections[i].incoming_pm.HasPendingPackets()) //if this connection has a packet that needs to be read by server
				{
					std::shared_ptr<Packet> packet = connections[i].incoming_pm.Retrieve(); //Retrieve packet for processing
					std::cout << "TCPServer Received packet of size: " << packet->size << std::endl;
					PacketType pt = PacketType::Unknown; //initialize packet type to unknown before reading
					*packet >> pt; //extract packet type
					switch (pt)
					{
					case PacketType::StringList:
					{
						std::cout << "Packet Received of Type: StringList" << std::endl;
						uint32_t stringcount = 0;
						*packet >> stringcount;
						std::cout << "StringList Count: " << stringcount << std::endl;
						for (int i = 0; i < stringcount; i++)
						{
							std::string stringval = "";
							*packet >> stringval;
							std::cout << "StringList String:" << stringval << std::endl;
						}
						std::cout << std::endl;
						break;
					}
					default:
						std::cout << "Unknown received packet from connection: " << connections[i].GetHandle() << std::endl;
						connections[i].Disconnect(); //Force disconnect which will be handled on next call to Loop()
					}
					connections[i].incoming_pm.Pop(); //Remove packet from queue
				}
			}
			catch (PacketException &pe)
			{
				std::cout << "Packet exception occurred on connection: " << connections[i].GetHandle() << ". Force closing connection." << std::endl;
				std::cout << "Packet Exception: " << pe.what() << std::endl;
				connections[i].Disconnect();
			}
		}
	}

	TCPServer::~TCPServer()
	{
		if (listener != nullptr)
		{
			listener->Close();
			delete listener;
		}
		listener = nullptr;
		for (int i = connections.size() - 1; i >= 0; i--)
		{
			connections[i].Disconnect();
			connections.erase(connections.begin() + i);
		}
	}

	void TCPServer::PrintExceptionalCondition(const SocketHandle socket)
	{
		int errorcode = 0;
		int errorcodesize = sizeof(errorcode);
		if (getsockopt(socket, SOL_SOCKET, SO_ERROR, (char*)&errorcode, &errorcodesize) == 0)
		{
			std::cout << "Winsock Error: " << errorcode << "." << std::endl;
		}
		else
		{
			std::cout << "Winsock error occurred. getsockopt() failed. " << std::endl;
		}
	}
}
