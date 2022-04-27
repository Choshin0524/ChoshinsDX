#include "App.h"
#include "Box.h"
#include "ConeTest.h"
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
		std::unique_ptr<Drawable> operator()()
		{
			return std::make_unique<ConeTest>(
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
		std::uniform_real_distribution<float> bdist{ 0.5f, 2.0f };
	};

	Factory f(wnd.Gfx());
	drawables.reserve(drawableN);
	std::generate_n(std::back_inserter(drawables), drawableN, f);

	wnd.Gfx().SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 5.0f, 0.3f, 40.0f));
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
	wnd.Gfx().ClearBuffer(0.07f, 0.0f, 0.12f);
	wnd.Gfx().SetRenderTarget(); // flip mode removes binds every frame
	for  (auto& b : drawables)
	{
		b->Update(dt);
		b->Draw(wnd.Gfx());
	}
	wnd.Gfx().EndFrame();
}