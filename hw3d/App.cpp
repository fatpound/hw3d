#include "App.hpp"

#include <cmath>

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
    const float color = std::sin(timer.Peek());

    wnd.Gfx().ClearBuffer(color, color, 1.0f);
    wnd.Gfx().EndFrame();
}