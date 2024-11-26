#pragma once

#include <FatNamespaces.hpp>

#include "../Util/Time/Time.hpp"
#include "../Util/ImguiManager.hpp"

#include "../Util/Camera.hpp"

#include "../Win32_/WindowEx.hpp"

namespace fatpound::win32::d3d11::visual
{
    class Drawable;
}

namespace fatpound::hw3d
{
    class App final
    {
        static constexpr std::size_t DrawableCount = 200u;

    public:
        App();
        App(const App& src) = delete;
        App(App&& src) = delete;

        auto operator = (const App& src) -> App& = delete;
        auto operator = (App&& src)      -> App& = delete;
        ~App() noexcept = default;


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
        FATSPACE_UTIL::ImguiManager m_imgui_;

        FATSPACE_WIN32::WindowEx m_wnd_;
        FATSPACE_D3D11::Graphics<> m_gfx_;

        FATSPACE_UTIL::ViewXM m_viewXM_;
        FATSPACE_UTIL::Camera m_camera_;
        FATSPACE_UTIL_TIME::AutoTimer m_timer_;

        std::vector<std::unique_ptr<FATSPACE_VISUAL::Drawable>> m_drawables_;

        float m_simulation_speed_ = 1.0f;
    };
}