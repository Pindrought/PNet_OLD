#include "PNet/Include.h"
#include <iostream>
#include <thread>
#include "Timer.h"

using namespace PNet;

const timeval timeout = { 1, 0 };

int serverthread()
{
	TCPServer server(ConnectionType::IPV4);
	auto result = server.StartListener();
	if (result == PRESULT::SUCCESS)
	{
		while (GetAsyncKeyState(VK_F1) == false) //if user presses f1, end server
		{
			server.Loop(timeout); //accept connections / process messages / do stuff with timeout up to "timeout"
		}
	}
	return 0;
}

int main()
{
	std::thread st(serverthread);
	Sleep(500);

	TCPClient myConn(ConnectionType::IPV4); //Create IPV4 Client
	auto result = myConn.Connect(IPAddress("127.0.0.1", ConnectionType::IPV4), 8888, timeout); //Connect ipv6 client to localhost 127.0.0.1 on port 8888 with a timeout up to "timeout"
	if (myConn.IsConnected())
	{
		std::cout << "Connected!" << std::endl;
		myConn.Initialize(); //After connecting, need to initialize the master fd_set for the TCPClient
	}
	else
	{
		std::cout << "Failed to connect." << std::endl;
	}

	Timer t;
	t.Start();
	while (myConn.IsConnected())
	{
		myConn.Loop(timeout); //Send/Receive/Etc with a timeout up to "timeout" 
		if (t.GetMilisecondsElapsed() > 1000.0) //Every 1 second
		{
			t.Restart(); //restart timer
			std::shared_ptr<Packet> p = std::make_shared<Packet>(PacketType::StringList); //Create packet of type (Test)
			*p << 3 << "John" << "Hates" << "Susan"; //append data to packet [3 strings] "John" "Hates" "Susan"
			myConn.SendPacket(p); //queue packet to be sent
		}
	}

	result = myConn.Disconnect();

	std::cout << "Waiting for server thread to end..." << std::endl;
	st.join();
	return 0;
}