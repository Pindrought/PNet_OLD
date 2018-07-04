cmake_minimum_required(VERSION 3.1)
project(PNetOut VERSION 1.0
		DESCRIPTION "No Description"
		LANGUAGES C CXX)
include_directories(PNet)
FIND_PACKAGE ( Threads REQUIRED )
add_executable(PNetOut 	Source.cpp 
			Timer.h Timer.cpp
			IsEscapePressed.h IsEscapePressed.cpp
			PNet/Include.h
			PNet/ConnectionType.h 
			PNet/IPAddress.h PNet/IPAddress.cpp
			PNet/Packet.h PNet/Packet.cpp
			PNet/PacketException.h
			PNet/PacketManager.h PNet/PacketManager.cpp
			PNet/PacketType.h
			PNet/PRESULT.h PNet/PRESULT.cpp
			PNet/Socket.h PNet/Socket.cpp
			PNet/SocketHandle.h
			PNet/SocketType.h
			PNet/TCPClient.h PNet/TCPClient.cpp
			PNet/TCPConnection.h PNet/TCPConnection.cpp
			PNet/TCPListener.h PNet/TCPListener.cpp
			PNet/TCPServer.h PNet/TCPServer.cpp
			PNet/TCPSocket.h PNet/TCPSocket.cpp)
TARGET_LINK_LIBRARIES ( PNetOut ${CMAKE_THREAD_LIBS_INIT} )
