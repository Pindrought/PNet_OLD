#ifdef _WIN32
#include <Windows.h>
#else
#include <X11/Xlib.h>
#include <X11/keysym.h>

#endif

bool IsEscapePressed();
