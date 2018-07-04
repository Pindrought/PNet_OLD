#include "PNet/Include.h"
#include <iostream>
#include <thread>
#include "Timer.h"
#ifndef _WIN32 //Linux Sleep that functions like win32 sleep
#define Sleep(duration) sleep(duration/1000)
#include <unistd.h>
#endif
#include "IsEscapePressed.h" //rigged up win32/linux compatible way of checking if escape is pressed to terminate server early

using namespace PNet;

const timeval timeout = { 1, 0 };

int serverthread()
{
    Timer t;
    t.Start();
	TCPServer server(IPVersion::IPV4);
	auto result = server.StartListener();
	if (result == PRESULT::SUCCESS)
	{
		while (IsEscapePressed() == false) //server ends on tick if escape is pressed
		{
			server.Loop(timeout); //accept connections / process messages / do stuff with timeout up to "timeout"
		}
	}
	else
	{
		std::cout << PRESULTToString(result) << std::endl;
	}
	return 0;
}

int main()
{
	std::thread st(serverthread);
	Sleep(500); //500 miliseconds (half a second)

	TCPClient myConn(IPVersion::IPV4); //Create IPV4 Client
	auto result = myConn.Connect(IPAddress("127.0.0.1", IPVersion::IPV4), 8888, timeout); //Connect ipv6 client to localhost 127.0.0.1 on port 8888 with a timeout up to "timeout"
	if (myConn.IsConnected())
	{
		std::cout << "Connected!" << std::endl;
		myConn.Initialize(); //After connecting, need to initialize the master fd_set for the TCPClient
	}
	else
	{
		std::cout << "Failed to connect." << std::endl;
		std::cout << PRESULTToString(result) << std::endl;
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
