#include "App.hpp"

#include <_macros/Compiler.hpp>

#include <Win32_/WinAPI.hpp>

#include <Math/Math.hpp>
#include <Win32_/GDI_Plus/Manager.hpp>

#pragma warning (push)
#pragma warning (disable : FATLIB_EXTERNAL_WARNINGS)
#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>
#pragma warning (pop)

#include "Object/Object.hpp"

#include <cmath>

#include <memory>
#include <numbers>
#include <random>

#if IN_RELEASE
#define SCREEN_WIDTH    static_cast<UINT>(::GetSystemMetrics(SM_CXSCREEN))
#define SCREEN_HEIGHT   static_cast<UINT>(::GetSystemMetrics(SM_CYSCREEN))
#else
#define SCREEN_WIDTH    1024u
#define SCREEN_HEIGHT    768u
#endif // IN_RELEASE

namespace dx = DirectX;

using FATSPACE_UTILITY_GFX::SizePack;

namespace hw3d
{
    App::App()
        :
        m_wnd_(std::make_shared<FATSPACE_WIN32::WndClassEx>(L"fat->pound WindowClassEx: " + std::to_wstring(s_game_id_++)), L"fat hw3d " + std::to_wstring(s_game_id_), SizePack{ SCREEN_WIDTH, SCREEN_HEIGHT }),
        m_gfx_(m_wnd_.GetHandle(), SizePack{ SCREEN_WIDTH, SCREEN_HEIGHT }),
        m_camera_(m_far_z_, m_wnd_.m_pKeyboard, m_wnd_.m_pMouse)
    {
        ::ImGui_ImplDX11_Init(m_gfx_.GetDevice(), m_gfx_.GetImmediateContext());
    }
    App::~App()
    {
        ::ImGui_ImplDX11_Shutdown();
    }

    auto App::IsRunning() const noexcept -> bool
    {
        return not IsOver();
    }
    auto App::IsOver() const noexcept -> bool
    {
        return m_wnd_.IsClosing();
    }

    void App::Go()
    {
        Init_();

        while (IsRunning())
        {
            m_gfx_.BeginFrame<>();

            PrepareImgui_();
            DoFrame_();
            RenderImgui_();

            m_gfx_.EndFrame<>();
        }
    }

    void App::Init_()
    {
        class Factory final
        {
        public:
            Factory(ID3D11Device* const pDevice, App& app)
                :
                pDevice_(pDevice),
                app_(app)
            {

            }

            Factory()                   = delete;
            Factory(const Factory&)     = delete;
            Factory(Factory&&) noexcept = delete;

            auto operator = (const Factory&)     -> Factory& = delete;
            auto operator = (Factory&&) noexcept -> Factory& = delete;
            ~Factory() noexcept                              = default;

        public:
            auto operator () () -> std::unique_ptr<obj::Drawable>
            {
                switch (typedist_(rng_))
                {
                case 0:
                    return std::make_unique<obj::Pyramid>(
                        pDevice_, rng_, adist_, ddist_,
                        odist_, rdist_, app_.m_viewXM_
                    );

                case 1:
                    return std::make_unique<obj::Box>(
                        pDevice_, rng_, adist_, ddist_,
                        odist_, rdist_, bdist_, app_.m_viewXM_
                    );

                case 2:
                    return std::make_unique<obj::Melon>(
                        pDevice_, rng_, adist_, ddist_,
                        odist_, rdist_, longdist_, latdist_, app_.m_viewXM_
                    );

                // case 3:
                //     return std::make_unique<obj::Sheet>(
                //         pDevice_, rng_, adist_, ddist_,
                //         odist_, rdist_, app_.m_viewXM_
                //     );
                // 
                // case 4:
                //     return std::make_unique<obj::SkinnedBox>(
                //         pDevice_, rng_, adist_, ddist_,
                //         odist_, rdist_, app_.m_viewXM_
                //     );

                default:
                    assert(false and "bad drawable type in factory");
                    return {};
                }
            }

        protected:

        private:
            std::minstd_rand rng_{ std::random_device{}() };

            std::uniform_real_distribution<float> adist_{ 0.0f, std::numbers::pi_v<float> * 2.0f };
            std::uniform_real_distribution<float> ddist_{ 0.0f, std::numbers::pi_v<float> * 0.5f };
            std::uniform_real_distribution<float> odist_{ 0.0f, std::numbers::pi_v<float> * 0.08f };
            std::uniform_real_distribution<float> rdist_{ 6.0f, 20.0f };
            std::uniform_real_distribution<float> bdist_{ 0.4f, 3.0f };

            std::uniform_int_distribution<int> latdist_{ 5, 20 };
            std::uniform_int_distribution<int> longdist_{ 10, 40 };
            std::uniform_int_distribution<int> typedist_{ 0, 2 }; // disabled Sheet and SkinnedBox objects for now

            ID3D11Device* const pDevice_;

            App& app_;
        };

        m_drawables_.reserve(scx_DrawableCount_);

        std::generate_n(std::back_inserter(m_drawables_), scx_DrawableCount_, Factory{ m_gfx_.GetDevice(), *this});

        m_viewXM_.SetProjectionXM(
            dx::XMMatrixPerspectiveLH(
                1.0f,
                m_wnd_.GetClientHeight<float>() / m_wnd_.GetClientWidth<float>(), // 1 / Aspect Ratio
                0.5f,
                m_far_z_
            )
        );
    }
    void App::PrepareImgui_() const
    {
        ::ImGui_ImplDX11_NewFrame();
        ::ImGui_ImplWin32_NewFrame();
        ::ImGui::NewFrame();
    }
    void App::DrawObjects_()
    {
        auto* const pImmediateContext = m_gfx_.GetImmediateContext();

        for (auto& obj : m_drawables_)
        {
            obj->Update(m_wnd_.m_pKeyboard->KeyIsPressed(VK_SPACE) ? 0.0f : m_delta_time_);
            obj->Draw(pImmediateContext);
        }
    }
    void App::DrawImguiCamera_()
    {
        if (::ImGui::Begin("Camera"))
        {
            ::ImGui::Text("Position");
            ::ImGui::SliderFloat("R",     &m_camera_.m_r_,        0.1f, m_far_z_, "%.1f");
            ::ImGui::SliderAngle("Theta", &m_camera_.m_theta_, -180.0f, 180.0f);
            ::ImGui::SliderAngle("Phi",   &m_camera_.m_phi_,    -89.0f,  89.0f);
                                                                 
            ::ImGui::Text("Orientation");                        
            ::ImGui::SliderAngle("Roll",  &m_camera_.m_roll_,  -180.0f, 180.0f);
            ::ImGui::SliderAngle("Pitch", &m_camera_.m_pitch_, -180.0f, 180.0f);
            ::ImGui::SliderAngle("Yaw",   &m_camera_.m_yaw_,   -180.0f, 180.0f);

            if (::ImGui::Button("Reset"))
            {
                m_camera_.Reset();
            }
        }

        ::ImGui::End();
    }
    void App::DrawImguiSimulation_()
    {
        if (::ImGui::Begin("Simulation Speed"))
        {
            ::ImGui::SliderFloat("Speed Factor", &m_simulation_speed_, 0.0f, 5.0f);
            ::ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ::ImGui::GetIO().Framerate, ::ImGui::GetIO().Framerate);
            ::ImGui::Text("Status: %s", m_wnd_.m_pKeyboard->KeyIsPressed(VK_SPACE) ? "PAUSED" : "RUNNING (hold spacebar to pause!)");
        }

        ::ImGui::End();
    }
    void App::RenderImgui_() const
    {
        ::ImGui::Render();
        ::ImGui_ImplDX11_RenderDrawData(::ImGui::GetDrawData());
    }

    void App::DoFrame_()
    {
        m_viewXM_.SetCameraXM(m_camera_.GetMatrix());

        m_timer_.Stop();
        m_delta_time_ = m_timer_.GetElapsed_s() * m_simulation_speed_;
        m_timer_.Start();

        DrawObjects_();
        DrawImguiCamera_();
        DrawImguiSimulation_();
    }
}