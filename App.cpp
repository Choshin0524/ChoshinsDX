#include "App.h"
#include <sstream>
#include <iomanip>

App::App()
	:
	wnd(800, 600, "WIWNWIWINDOW")
{}

int App::Go()
{
	while (true)
	{
		if (const auto ecode = Window::ProcessMessages())
		{
			// if faild return nullptr 
			return *ecode;
		}
		DoFrame();
	}
}

void App::DoFrame()
{
	const float c = abs(sin(timer.Peek()));
	wnd.Gfx().ClearBuffer(c, 1.0f, c);
	wnd.Gfx().DrawTestTriangle();
	wnd.Gfx().EndFrame();
}