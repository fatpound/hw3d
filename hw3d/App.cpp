#include "App.hpp"

App::App()
	:
	wnd(800, 600, "The FatBox")
{
	
}

int App::Go()
{
	std::optional<int> errorCode;

	while (true)
	{
		if (errorCode = Window::ProcessMessages())
		{
			return *errorCode;
		}

		DoFrame();
	}
}

void App::DoFrame()
{
	
}