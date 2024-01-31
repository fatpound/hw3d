#include "App.hpp"
#include "Box.hh"

#include <cmath>

#include <memory>
#include <numbers>

namespace dx = DirectX;

App::App()
    :
    wnd(800, 600, "The FatBox")
{
    std::mt19937 rng(std::random_device{}());
    std::uniform_real_distribution<float> adist(0.0f, std::numbers::pi_v<float> * 2.0f);
    std::uniform_real_distribution<float> ddist(0.0f, std::numbers::pi_v<float> * 1.0f);
    std::uniform_real_distribution<float> odist(0.0f, std::numbers::pi_v<float> * 0.08f);
    std::uniform_real_distribution<float> rdist(6.0f, 20.0f);

    for (auto i = 0; i < 180; i++)
    {
        boxes.push_back(std::make_unique<Box>(
            wnd.Gfx(), rng, adist,
            ddist, odist, rdist
        ));
    }

    wnd.Gfx().SetProjection(dx::XMMatrixPerspectiveLH(1.0f, wnd.GetHeight<float>() / wnd.GetWidth<float>(), 0.5f, 40.0f));
}

App::~App() noexcept
{

}


int App::Go()
{
    std::optional<int> errorCode;

    while (true)
    {
        errorCode = Window::ProcessMessages();

        if (errorCode)
        {
            return *errorCode;
        }

        DoFrame();
    }
}

void App::DoFrame()
{
    auto dt = timer.Mark();

    wnd.Gfx().ClearBuffer(0.0f, 0.0f, 0.0f);

    for (auto& b : boxes)
    {
        b->Update(dt);
        b->Draw(wnd.Gfx());
    }

    wnd.Gfx().EndFrame();
}