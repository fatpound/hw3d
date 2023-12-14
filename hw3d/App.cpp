#include "App.hpp"

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
	}

	if (gResult == -1)
	{
		throw FHWND_LAST_EXCEPT();
	}

	return msg.wParam;
}

void App::DoFrame()
{

}