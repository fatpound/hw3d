#pragma once

#include <_macros/Namespaces.hpp>

#include <Utility/Timer.hpp>
#include <Win32_/D3D11/Graphics.hpp>

#include "Object/Drawable.hpp"
#include "View/Camera.hpp"

#include "ImguiManager.hpp"
#include "Window.hpp"

namespace hw3d
{
    class App final
    {
        static constexpr auto scx_DrawableCount_ = 200u;

    public:
        App();
        App(const App&)     = delete;
        App(App&&) noexcept = delete;

        auto operator = (const App&)     -> App& = delete;
        auto operator = (App&&) noexcept -> App& = delete;
        ~App();


    public:
        auto IsRunning() const noexcept -> bool;
        auto IsOver()    const noexcept -> bool;

        void Go();


    protected:


    private:
        void Init_();
        void PrepareImgui_() const;
        void DrawObjects_();
        void DrawImguiCamera_();
        void DrawImguiSimulation_();
        void RenderImgui_() const;

        void DoFrame_();


    private:
        inline static std::size_t                   s_game_id_{};


    private:
        ImguiManager                                m_imgui_mgr_;
        Window                                      m_wnd_;
        FATSPACE_D3D11::Graphics<>                  m_gfx_;

        float                                       m_delta_time_       =   0.0f;
        float                                       m_simulation_speed_ =   1.0f;
        const float                                 mc_far_z_           = 100.0f;

        view::Camera                                m_camera_;
        FATSPACE_UTILITY::ViewXM                    m_viewXM_;
        FATSPACE_UTILITY::Timer                     m_timer_;

        std::vector<std::unique_ptr<obj::Drawable>> m_drawables_;
    };
}