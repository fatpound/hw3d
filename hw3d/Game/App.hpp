#pragma once

#include "../Util/AutoTimer.hpp"
#include "../Util/ImguiManager.hpp"

#include "../Util/Camera/Camera.hpp"

#include "../Win32_/Window.hpp"

namespace fatpound::win32::d3d11::visual
{
    class Drawable;
}

namespace fatpound::hw3d
{
    class App final
    {
    public:
        App();
        App(const App& src) = delete;
        App& operator = (const App& src) = delete;

        App(App&& src) = delete;
        App& operator = (App&& src) = delete;
        ~App() noexcept;


    public:
        int Go();


    protected:


    private:
        void DoFrame_();


    private:
        NAMESPACE_UTIL::ImguiManager imgui_;

        NAMESPACE_WIN32::Window wnd_;
        
        NAMESPACE_D3D11::Graphics gfx_;

        NAMESPACE_UTIL::Camera camera_;
        NAMESPACE_UTIL::AutoTimer timer_;

        std::vector<std::unique_ptr<NAMESPACE_VISUAL::Drawable>> drawables_;

        float simulation_speed_ = 1.0f;

        static constexpr std::size_t drawable_count_ = 180u;
    };
}