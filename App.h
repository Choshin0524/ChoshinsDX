#pragma once
#include "Window.h"
#include "ChiliTimer.h"
#include "ImguiManager.h"
#include "Camera.h"

class App
{
public:
	App();
	// master frame and msg loop
	int Go();
	~App();
private:
	void DoFrame();
private:
	float speed_factor = 1.0f;
	ImguiManager imgui;
	Window wnd;
	Camera cam;
	ChiliTimer timer;
	std::vector<std::unique_ptr<class Drawable>> drawables;
	static constexpr size_t drawableN = 10;
};