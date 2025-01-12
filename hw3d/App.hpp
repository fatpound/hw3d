#pragma once

#include <FatNamespaces.hpp>

#include <Util/Timer.hpp>
#include <Win32_/WindowEx.hpp>
#include <Win32_/D3D11/Graphics.hpp>

#include "View/Camera.hpp"

#include "ImguiManager.hpp"

namespace fatpound::win32::d3d11::visual
{
    class Drawable;
}

namespace hw3d
{
    class App final
    {
        static constexpr auto scx_DrawableCount_ = 200u;

    public:
        App();
        App(const App& src) = delete;
        App(App&& src) = delete;

        auto operator = (const App& src) -> App& = delete;
        auto operator = (App&& src)      -> App& = delete;
        ~App() noexcept(false);


    public:
        auto IsRunning() const -> bool;
        auto IsOver()    const -> bool;

        void Go();


    protected:


    private:
        void Init_();

        void DoFrame_();


    private:
        inline static std::size_t s_game_id_{};


    private:
        ImguiManager m_imgui_;

        FATSPACE_WIN32::WindowEx m_wnd_;
        FATSPACE_D3D11::Graphics<> m_gfx_;

        view::Camera m_camera_;

        FATSPACE_UTIL::ViewXM m_viewXM_;
        FATSPACE_UTIL::Timer m_timer_;

        std::vector<std::unique_ptr<FATSPACE_VISUAL::Drawable>> m_drawables_;

        float m_simulation_speed_ = 1.0f;
    };
}