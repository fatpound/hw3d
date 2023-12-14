#include "App.hpp"

#include <sstream>
#include <iomanip>

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
	const float time = timer.Peek();
	std::ostringstream oss;
	oss << "Time elapsed : " << std::setprecision(1) << std::fixed << time << "s";
	wnd.SetTitle(oss.str());
}