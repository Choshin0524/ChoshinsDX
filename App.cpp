#pragma once
#include "App.h"
#include "Box.h"
#include "ConeTest.h"
#include "SphereTest.h"
#include "PrismTest.h"
#include "Sheet.h"
#include "SkinnedBox.h"
#include "Surface.h"
#include "GDIPlusManager.h"
#include "ProjectMath.h"
#include "imgui/imgui.h"

GDIPlusManager gdipm;

App::App()
	:
	wnd(1000, 600, "WIWNWIWINDOW")
{
	class Factory
	{
	public:
		Factory(Graphics& gfx)
			:
			gfx(gfx)
		{}
		// () -> function call operator
		std::unique_ptr<Drawable> operator()()
		{
			switch (typedist(rng))
			{
			case 0:
				return std::make_unique<PrismTest>(
					gfx, rng, adist, ddist,
					odist, rdist, bdist
					);
			case 1:
				return std::make_unique<ConeTest>(
					gfx, rng, adist, ddist,
					odist, rdist, bdist
					);
			case 2: 
				return std::make_unique<SphereTest>(
					gfx, rng, adist, ddist,
					odist, rdist, bdist
					);
			case 3:
				return std::make_unique<Sheet>(
					gfx, rng, adist, ddist,
					odist, rdist
					);
			case 4:
				return std::make_unique<SkinnedBox>(
					gfx, rng, adist, ddist,
					odist, rdist
					);
			default:
				break;
			}
			
		}
	private:
		Graphics& gfx;
		std::mt19937 rng{ std::random_device{}()};
		std::uniform_real_distribution<float> adist{ 0.0f, 3.1415f * 2.0f };
		std::uniform_real_distribution<float> ddist{ 0.0f, 3.1415f * 2.0f };
		std::uniform_real_distribution<float> odist{ 0.0f, 3.1415f * 0.3f };
		std::uniform_real_distribution<float> rdist{ 10.0f, 20.0f };
		std::uniform_real_distribution<float> bdist{ 1.0f, 1.1f };
		std::uniform_int_distribution<int> typedist{ 0,4 };
	};

	Factory f(wnd.Gfx());
	drawables.reserve(drawableN);
	std::generate_n(std::back_inserter(drawables), drawableN, f);

	wnd.Gfx().SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 5.0f, 0.5f, 40.0f));

}

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

App::~App()
{
}

void App::DoFrame()
{
	auto dt = timer.Mark();

	if (wnd.kbd.KeyIsPressed(VK_SPACE))
	{
		wnd.Gfx().DisableImgui();
	}
	else
	{
		wnd.Gfx().EnableImgui();
	}

	wnd.Gfx().BeginFrame(0.07f, 0.0f, 0.12f);
	wnd.Gfx().SetRenderTarget(); // flip mode removes binds every frame
	for  (auto& b : drawables)
	{
		b->Update(wnd.kbd.KeyIsPressed(VK_SHIFT) ? 0.0f : dt);
		b->Draw(wnd.Gfx());
	}

	

	if (show_demo_window)
	{
		ImGui::ShowDemoWindow(&show_demo_window);
	}

	// present
	wnd.Gfx().EndFrame();
}