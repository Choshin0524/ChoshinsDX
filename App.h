#pragma once
#include "Window.h"
#include "ChiliTimer.h"
#include "ImguiManager.h"

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
	ImguiManager imgui;
	Window wnd;
	ChiliTimer timer;
	std::vector<std::unique_ptr<class Drawable>> drawables;
	static constexpr size_t drawableN = 30;
};