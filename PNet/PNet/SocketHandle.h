#pragma once

namespace PNet
{
#if defined _WIN32
	#if  defined(_WIN64)
		typedef unsigned __int64 UINT_PTR;
	#else 
		typedef unsigned int UINT_PTR;
	#endif 
	typedef UINT_PTR SocketHandle;
#else
	typedef int SocketHandle;
#endif
}
