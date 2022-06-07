#pragma once
#include "Window.h"
#include "ChiliTimer.h"
#include "ImguiManager.h"
#include "Camera.h"
#include "PointLight.h"
#include <set>

class App
{
public:
	App();
	// master frame and msg loop
	int Go();
	~App();
private:
	void DoFrame();
	void SpawnSimulationWindow() noexcept;
	void SpawnBoxWindowManagerWindow() noexcept;
	void SpawnBoxWindows() noexcept;

private:
	float speed_factor = 1.0f;
	ImguiManager imgui;
	Window wnd;
	Camera cam;
	PointLight light;
	ChiliTimer timer;
	std::vector<class Box*> boxes;
	std::vector<std::unique_ptr<class Drawable>> drawables;
	static constexpr size_t drawableN = 100;
	std::set<int> boxControlIds;
	int comboBoxIndex = 0;
};