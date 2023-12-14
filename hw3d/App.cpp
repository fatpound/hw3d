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
	MSG msg;
	BOOL gResult = GetMessage(&msg, nullptr, 0, 0);

	while (gResult > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);

		gResult = GetMessage(&msg, nullptr, 0, 0);

		DoFrame();
	}

	if (gResult == -1)
	{
		throw FHWND_LAST_EXCEPT();
	}

	return msg.wParam;
}

void App::DoFrame()
{
	const float time = timer.Peek();
	std::ostringstream oss;
	oss << "Time elapsed : " << std::setprecision(1) << std::fixed << time << "s";
	wnd.SetTitle(oss.str());
}