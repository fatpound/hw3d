#include "App.hpp"
#include "FatMath.hpp"
#include "Box.hh"
#include "Pyramid.hh"
#include "Melon.hh"

#include <cmath>

#include <memory>
#include <numbers>

namespace dx = DirectX;

App::App()
    :
    wnd_(800, 600, "The FatBox")
{
    class Factory
    {
    public:
        Factory(Graphics& gfx)
            :
            gfx_(gfx)
        {

        }

    public:
        std::unique_ptr<Drawable> operator()()
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
        std::uniform_int_distribution<int> typedist_{ 0, 2 };

        Graphics& gfx_;
    };

    Factory factory(wnd_.Gfx());

    drawables_.reserve(drawable_count_);
    std::generate_n(std::back_inserter(drawables_), drawable_count_, factory);

    wnd_.Gfx().SetProjection(dx::XMMatrixPerspectiveLH(1.0f, wnd_.GetHeight<float>() / wnd_.GetWidth<float>(), 0.5f, 40.0f));
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
    auto dt = timer_.Mark();

    wnd_.Gfx().ClearBuffer(0.0f, 0.0f, 0.0f);

    for (auto& obj : drawables_)
    {
        obj->Update(dt);
        obj->Draw(wnd_.Gfx());
    }

    wnd_.Gfx().EndFrame();
}