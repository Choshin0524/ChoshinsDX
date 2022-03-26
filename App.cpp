#include "App.h"
#include <sstream>
#include <iomanip>

App::App()
	:
	wnd(1500, 600, "WIWNWIWINDOW")
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
	wnd.Gfx().DrawTestTriangle(
		timer.Peek(),
		(float)wnd.mouse.GetPosX() / 750.0f - 1.0f,
		-(float)wnd.mouse.GetPosY() / 300.0f + 1.0f
	);
	wnd.Gfx().EndFrame();
}