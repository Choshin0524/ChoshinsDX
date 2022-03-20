#include "App.h"

int CALLBACK WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ PSTR szCmdLine,
	_In_ int iCmdShow)
{
	try
	{
		return App{}.Go();
	}
	catch (const ChiliException& e)
	{
		MessageBox(nullptr, e.what(), e.GetType(), MB_OK | MB_ICONERROR);
	}
	catch (...)
	{
		MessageBox(nullptr, "unknown", "unknown", MB_OK | MB_ICONERROR);
	}
	return -1;
}