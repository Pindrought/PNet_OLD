#pragma once

namespace PNet
{
#if defined _WIN32
	typedef unsigned int SocketHandle;
#else
	#error "Only Windows operating systems currently supoported. [PNet/SocketHandle.h]"
#endif
}