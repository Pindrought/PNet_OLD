# PNet
Windows/Linux compatible networking library Work in Progress


Linux Notes:
Requires X11 lib (Can be installed with "sudo apt install libx11-dev" without the quotes)
Change directory to directory with CMakeLists.txt
Enter: "cmake ." without the quotes
I used codeblocks, so in my example I did: cmake . -G"CodeBlocks - Unix Makefiles"

Windows Notes:
Just open the solution in Visual Studio. If you don't use visual studio on windows, shame on you.

General Notes:
Only supports TCP at the moment. 

By default, IPV6 servers are set up to also receive IPV4 connections.
This can be modified by omitting the following code in Socket.cpp if for some reason you don't want to accept IPV4 connections on your IPV6 server.
int no = 0;
if (setsockopt(handle, IPPROTO_IPV6, IPV6_V6ONLY, (char *)&no, sizeof(no)) == SOCKET_ERROR)
{
	std::cerr << "Failed to disable ipv6 only option. Only IPV6 connections will be able to be established." << std::endl;
}

Packets are not meant to be extracted from or inserted into once they are queued to be sent. This is just undefined behavior so don't do it.

Packets use shared ptrs to prevent unnecessary allocation for when sending to multiple clients. 
If you want to copy a packet's contents to a new packet where more data will be appended, there is a constructor that takes a shared ptr to a packet and copies the existing data into the new packet.

Feel free to contact me if you have any questions. This is incomplete, anything is subject to change. No licenses.

The best way to contact me for a quick response is on Discord. My user is Pindrought#5849 i'm usually on the Planet Chili Discord.
Planet Chili Discord can be found at http://planetchili.net/
