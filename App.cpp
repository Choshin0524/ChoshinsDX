#pragma once
#include "App.h"
#include "Box.h"
#include "GDIPlusManager.h"
#include "ProjectMath.h"
#include "imgui/imgui.h"

GDIPlusManager gdipm;

App::App()
	:
	wnd(1000, 600, "WIWNWIWINDOW"),
	light(wnd.Gfx())
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
			return std::make_unique<Box>(
				gfx, rng, adist, ddist,
				odist, rdist, bdist
				);
		}
	private:
		Graphics& gfx;
		std::mt19937 rng{ std::random_device{}()};
		std::uniform_real_distribution<float> adist{ 0.0f, 3.1415f * 2.0f };
		std::uniform_real_distribution<float> ddist{ 0.0f, 3.1415f * 2.0f };
		std::uniform_real_distribution<float> odist{ 0.0f, 3.1415f * 0.3f };
		std::uniform_real_distribution<float> rdist{ 10.0f, 20.0f };
		std::uniform_real_distribution<float> bdist{ 1.0f, 1.1f };
	};

	Factory f(wnd.Gfx());
	drawables.reserve(drawableN);
	std::generate_n(std::back_inserter(drawables), drawableN, f);

	wnd.Gfx().SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 5.0f, 0.5f, 80.0f));
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
	auto dt = timer.Mark() * speed_factor;

	wnd.Gfx().BeginFrame(0.07f, 0.0f, 0.12f);
	wnd.Gfx().SetCamera(cam.GetMatrix());
	light.Bind(wnd.Gfx());
	wnd.Gfx().SetRenderTarget(); // flip mode removes binds every frame
	
	for  (auto& b : drawables)
	{
		b->Update(wnd.kbd.KeyIsPressed(VK_SHIFT) ? 0.0f : dt);
		b->Draw(wnd.Gfx());
	}
	light.Draw(wnd.Gfx());
	
	// imgui window to control simulaiton speed
	if (ImGui::Begin("Simulation Speed")) //return false when minimized
	{
		ImGui::SliderFloat("Speed Factor", &speed_factor, 0.0f, 4.0f);
		ImGui::Text("App costs %.3f / frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
	ImGui::End();
	// imgui window to control camera
	cam.SpawnControlWindow();
	light.SpawnControlWindow();
	// present
	wnd.Gfx().EndFrame();
}