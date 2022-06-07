#pragma once
#include "App.h"
#include "Box.h"
#include "SkinnedBox.h"
#include "Cylinder.h"
#include "Pyramid.h"
#include <memory>
#include <algorithm>
#include "GDIPlusManager.h"
#include "ProjectMath.h"
#include "imgui/imgui.h"

GDIPlusManager gdipm;

App::App()
	:
	wnd(1500, 1200, "WIWNWIWINDOW"),
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
			const DirectX::XMFLOAT3 mat = { cdist(rng), cdist(rng), cdist(rng) };
			switch (sdist(rng))
			{
			case 0:
				return std::make_unique<Box>(
					gfx, rng, adist, ddist,
					odist, rdist, bdist, mat
					);
			case 1:
				return std::make_unique<Cylinder>(
					gfx, rng, adist, ddist,
					odist, rdist, bdist, tdist
					);
			case 2:
				return std::make_unique<Pyramid>(
					gfx, rng, adist, ddist,
					odist, rdist, tdist
					);
			case 3:
				return std::make_unique<SkinnedBox>(
					gfx, rng, adist, ddist,
					odist, rdist
					);
			default:
				assert(false && "impossible drawable option");
				return {};
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
		std::uniform_real_distribution<float> cdist{ 0.0f, 1.0f };
		std::uniform_int_distribution<int> sdist{ 0, 3 };
		std::uniform_int_distribution<int> tdist{ 4, 15 };


	};

	Factory f(wnd.Gfx());
	drawables.reserve(drawableN);
	std::generate_n(std::back_inserter(drawables), drawableN, f);

	// init box pointers for editing instance parameters
	for (auto& pd : drawables)
	{
		if (auto pb = dynamic_cast<Box*>(pd.get()))
		{
			boxes.push_back(pb);
		}
	}

	wnd.Gfx().SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 12.0f / 15.0f, 0.5f, 80.0f));
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
	light.Bind(wnd.Gfx(), wnd.Gfx().GetCamera());
	wnd.Gfx().SetRenderTarget(); // flip mode removes binds every frame
	
	for  (auto& b : drawables)
	{
		b->Update(wnd.kbd.KeyIsPressed(VK_SHIFT) ? 0.0f : dt);
		b->Draw(wnd.Gfx());
	}
	light.Draw(wnd.Gfx());
	
	// imgui window to control camera
	SpawnSimulationWindow();
	cam.SpawnControlWindow();
	light.SpawnControlWindow();
	SpawnBoxWindowManagerWindow();
	SpawnBoxWindows();
	// present
	wnd.Gfx().EndFrame();
}

void App::SpawnSimulationWindow() noexcept
{
	// imgui window to control simulaiton speed
	if (ImGui::Begin("Simulation Speed")) //return false when minimized
	{
		ImGui::SliderFloat("Speed Factor", &speed_factor, 0.0f, 4.0f);
		ImGui::Text("App costs %.3f / frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::Text("Status: %s", wnd.kbd.KeyIsPressed(VK_SHIFT) ? "PAUSED" : "RUNNING (hold shift to pause)");
	}
	ImGui::End();
}

void App::SpawnBoxWindowManagerWindow() noexcept
{
	if (ImGui::Begin("Boxes"))
	{
		using namespace std::string_literals;
		const auto preview = comboBoxIndex != 0 ? std::to_string(comboBoxIndex) : "Choose a box..."s;
		if (ImGui::BeginCombo("Box Number", preview.c_str()))
		{
			for (int i = 0; i < boxes.size(); i++)
			{
				const bool selected = comboBoxIndex == i + 1;
				if (ImGui::Selectable(std::to_string(i + 1).c_str(), selected))
				{
					comboBoxIndex = i + 1;
				}
				if (selected)
				{
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}
		if (ImGui::Button("Spawn Control Window") && comboBoxIndex)
		{
			boxControlIds.insert(comboBoxIndex);
			comboBoxIndex = 0;
		}
	}
	ImGui::End();
}

void App::SpawnBoxWindows() noexcept
{
	// imgui box attribute control windows
	for (auto i = boxControlIds.begin(); i != boxControlIds.end();)
	{
		if (!boxes[*i - 1]->SpawnControlWindow(*i, wnd.Gfx()))
		{
			i = boxControlIds.erase(i);
		}
		else
		{
			i++;
		}
	}
}
