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
	wnd.Gfx().ClearBuffer(c, c, 1.0f);
	wnd.Gfx().DrawTestTriangle(
		-timer.Peek(),
		0.0f,
		0.0f
	);
	wnd.Gfx().DrawTestTriangle(
		timer.Peek(),
		(float)wnd.mouse.GetPosX() / 400.0f - 1.0f,
		-(float)wnd.mouse.GetPosY() / 300.0f + 1.0f
	);
	wnd.Gfx().EndFrame();
}