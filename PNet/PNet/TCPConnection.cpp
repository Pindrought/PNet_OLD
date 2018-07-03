#include "TCPConnection.h"

namespace PNet
{
	TCPConnection::TCPConnection(ConnectionType connectionType, SocketHandle socketHandle, bool isConnected)
		:TCPSocket(connectionType, socketHandle, isConnected)
	{

	}
}
