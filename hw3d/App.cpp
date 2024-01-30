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
    std::uniform_real_distribution<float> ddist(0.0f, std::numbers::pi_v<float> * 2.0f);
    std::uniform_real_distribution<float> odist(0.0f, std::numbers::pi_v<float> * 0.3f);
    std::uniform_real_distribution<float> rdist(6.0f, 20.0f);

    for (auto i = 0; i < 80; i++)
    {
        boxes.push_back(std::make_unique<Box>(
            wnd.Gfx(), rng, adist,
            ddist, odist, rdist
        ));
    }

    wnd.Gfx().SetProjection(dx::XMMatrixPerspectiveLH(1.0f, wnd.GetHeight<float>() / wnd.GetWidth<float>(), 0.5f, 40.0f));
}

App::~App()
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

    wnd.Gfx().ClearBuffer(0.07f, 0.0f, 0.12f);

    for (auto& b : boxes)
    {
        b->Update(dt);
        b->Draw(wnd.Gfx());
    }

    wnd.Gfx().EndFrame();
}