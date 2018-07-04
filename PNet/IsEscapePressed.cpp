#include "IsEscapePressed.h"

bool IsEscapePressed()
{
#ifdef _WIN32
	return GetAsyncKeyState(VK_ESCAPE);
#else
	// Get the corresponding X11 keysym
	KeySym keysym = XK_Escape;

	// Open a connection with the X server
	Display* display = XOpenDisplay(NULL);

	// Convert to keycode
	KeyCode keycode = XKeysymToKeycode(display, keysym);
	if (keycode != 0)
	{
		// Get the whole keyboard state
		char keys[32];
		XQueryKeymap(display, keys);

		// Close the connection with the X server
		XCloseDisplay(display);

		// Check our keycode
		return (keys[keycode / 8] & (1 << (keycode % 8))) != 0;
	}
	else
	{
		// Close the connection with the X server
		XCloseDisplay(display);

		return false;
	}
#endif
}
