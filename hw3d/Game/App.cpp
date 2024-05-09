#include "App.hpp"

#include "../Math/FatMath.hpp"

#include "../Win32_/FatWin32_.hpp"

#include "../Win32_/GDI/Surface.hpp"
#include "../Win32_/GDI/GDIPlusManager.hpp"

#include "../imgui/imgui.h"

#include "Object/Box.hpp"
#include "Object/Melon.hpp"
#include "Object/Pyramid.hpp"
#include "Object/Sheet.hpp"
#include "Object/SkinnedBox.hpp"

#include <cmath>

#include <memory>
#include <numbers>
#include <random>

namespace dx = DirectX;

GDIPlusManager gdipm;

App::App()
    :
    wnd_("The FatBox", GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN)),
    gfx_(wnd_.Gfx())
{
    class Factory final
    {
    public:
        Factory(Graphics& gfx)
            :
            gfx_(gfx)
        {
    
        }
    
    public:
        std::unique_ptr<Drawable> operator () ()
        {
            switch (typedist_(rng_))
            {
            case 0:
                return std::make_unique<Pyramid>(
                    gfx_, rng_, adist_, ddist_,
                    odist_, rdist_
                );
    
            case 1:
                return std::make_unique<Box>(
                    gfx_, rng_, adist_, ddist_,
                    odist_, rdist_, bdist_
                );
    
            case 2:
                return std::make_unique<Melon>(
                    gfx_, rng_, adist_, ddist_,
                    odist_, rdist_, longdist_, latdist_
                );
    
            case 3:
                return std::make_unique<Sheet>(
                    gfx_, rng_, adist_, ddist_,
                    odist_, rdist_
                );
    
            case 4:
                return std::make_unique<SkinnedBox>(
                    gfx_, rng_, adist_, ddist_,
                    odist_, rdist_
                );
    
            default:
                assert(false && "bad drawable type in factory");
                return {};
            }
        }
    
    protected:
    
    private:
        std::mt19937 rng_{ std::random_device{}() };
    
        std::uniform_real_distribution<float> adist_{ 0.0f, std::numbers::pi_v<float> * 2.0f };
        std::uniform_real_distribution<float> ddist_{ 0.0f, std::numbers::pi_v<float> * 0.5f };
        std::uniform_real_distribution<float> odist_{ 0.0f, std::numbers::pi_v<float> * 0.08f };
        std::uniform_real_distribution<float> rdist_{ 6.0f, 20.0f };
        std::uniform_real_distribution<float> bdist_{ 0.4f, 3.0f };
    
        std::uniform_int_distribution<int> latdist_{ 5, 20 };
        std::uniform_int_distribution<int> longdist_{ 10, 40 };
        std::uniform_int_distribution<int> typedist_{ 0, 4 };
    
        Graphics& gfx_;
    };
    
    drawables_.reserve(App::drawable_count_);
    std::generate_n(std::back_inserter(drawables_), App::drawable_count_, Factory{ gfx_ });
    
    gfx_.SetProjection(dx::XMMatrixPerspectiveLH(1.0f, wnd_.GetHeight<float>() / wnd_.GetWidth<float>(), 0.5f, 40.0f));
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

        // [[unlikely]]
        if (errorCode)
        {
            return *errorCode;
        }

        // [[unlikely]]
        if (wnd_.kbd_.KeyIsPressed(VK_ESCAPE))
        {
            wnd_.Kill();
            
            return 0;
        }

        gfx_.BeginFrame(0.07f, 0.0f, 0.12f);
        DoFrame_();
        gfx_.EndFrame();
    }
}

void App::DoFrame_()
{
    auto dt = timer_.Mark() * simulation_speed_;

    gfx_.SetCamera(camera_.GetMatrix());
    
    for (auto& obj : drawables_)
    {
        obj->Update(wnd_.kbd_.KeyIsPressed(VK_SPACE) ? 0.0f : dt);
        obj->Draw(gfx_);
    }

    camera_.SpawnControlImguiWindow();

    if (ImGui::Begin("Simulation Speed"))
    {
        ImGui::SliderFloat("Speed Factor", &simulation_speed_, 0.0f, 5.0f);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::Text("Status: %s", wnd_.kbd_.KeyIsPressed(VK_SPACE) ? "PAUSED" : "RUNNING (hold spacebar to pause!)");
    }

    ImGui::End();
}