#include "TCPConnection.h"

namespace PNet
{
	TCPConnection::TCPConnection(IPVersion ipversion, SocketHandle socketHandle, bool isConnected)
		:TCPSocket(ipversion, socketHandle, isConnected)
	{
	}
}
