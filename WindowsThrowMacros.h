// error exception helper macro
#define	CHWND_EXCEPT(hr) Window::Exception(__LINE__, __FILE__, hr)
#define CHWND_NOGFX_EXCEPT() Window::NoGfxException(__LINE__, __FILE__)
#define	CHWND_LAST_EXCEPT() Window::Exception(__LINE__, __FILE__, GetLastError())