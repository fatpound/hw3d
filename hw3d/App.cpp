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
    const float color = std::abs(std::sin(timer.Peek()));

    wnd.Gfx().ClearBuffer(color, color, 1.0f);

    wnd.Gfx().DrawTestTriangle(
        -timer.Peek(),
        0.0f,
        0.0f
    );

    wnd.Gfx().DrawTestTriangle(
        timer.Peek(),
        static_cast<float>(wnd.mouse_.GetPosX()) / 400.0f - 1.0f,
        -static_cast<float>(wnd.mouse_.GetPosY()) / 300.0f + 1.0f
    );

    wnd.Gfx().EndFrame();
}