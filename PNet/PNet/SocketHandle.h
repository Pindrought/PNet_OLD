#pragma once

namespace PNet
{
#if defined _WIN32
	typedef unsigned int SocketHandle;
#else
	typedef int SocketHandle;
#endif
}
