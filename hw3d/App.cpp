#include "App.hpp"

#include <FatWin32.hpp>

#include <Math/Math.hpp>
#include <Win32_/GDI_Plus/Manager.hpp>

#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>

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

using FATSPACE_UTIL::Color;
using FATSPACE_UTIL::Surface;
using FATSPACE_UTIL::ScreenSizeInfo;

namespace hw3d
{
    App::App()
        :
        m_wnd_(std::make_shared<FATSPACE_WIN32::WndClassEx>(L"fat->pound WindowClassEx: " + std::to_wstring(s_game_id_++)), L"The FatBox " + std::to_wstring(s_game_id_), ScreenSizeInfo{ SCREEN_WIDTH, SCREEN_HEIGHT }),
        m_gfx_(m_wnd_.GetHandle(), ScreenSizeInfo{ SCREEN_WIDTH, SCREEN_HEIGHT }),
        m_camera_(100.0f, m_wnd_.m_pKeyboard, m_wnd_.m_pMouse)
    {
        ::ImGui_ImplDX11_Init(m_gfx_.GetDevice(), m_gfx_.GetImmediateContext());
        ::ImGui_ImplWin32_Init(m_wnd_.GetHandle());
    }
    App::~App() noexcept(false)
    {
        ::ImGui_ImplWin32_Shutdown();
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
            ::ImGui_ImplDX11_NewFrame();
            ::ImGui_ImplWin32_NewFrame();
            ::ImGui::NewFrame();

            m_gfx_.BeginFrame<>();
            DoFrame_();

            ::ImGui::Render();
            ::ImGui_ImplDX11_RenderDrawData(::ImGui::GetDrawData());

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

            Factory() = delete;
            Factory(const Factory& src)     = delete;
            Factory(Factory&& src) noexcept = delete;

            auto operator = (const Factory& src)     -> Factory& = delete;
            auto operator = (Factory&& src) noexcept -> Factory& = delete;
            ~Factory() noexcept = default;

        public:
            auto operator () () -> std::unique_ptr<FATSPACE_VISUAL::Drawable>
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

        m_drawables_.reserve(App::scx_DrawableCount_);

        std::generate_n(std::back_inserter(m_drawables_), App::scx_DrawableCount_, Factory{ m_gfx_.GetDevice(), *this});

        m_viewXM_.SetProjectionXM(
            dx::XMMatrixPerspectiveLH(
                1.0f,
                m_wnd_.GetClientHeight<float>() / m_wnd_.GetClientWidth<float>(), // 1 / Aspect Ratio
                0.5f,
                40.0f
            )
        );
    }
    void App::DoFrame_()
    {
        m_timer_.Stop();
        const auto& deltaTime = m_timer_.GetElapsed_s();
        m_timer_.Start();

        m_camera_.Update();

        m_viewXM_.SetCameraXM(m_camera_.GetMatrix());

        auto* const pImmediateContext = m_gfx_.GetImmediateContext();

        for (auto& obj : m_drawables_)
        {
            obj->Update(m_wnd_.m_pKeyboard->KeyIsPressed(VK_SPACE) ? 0.0f : deltaTime);
            obj->Draw(pImmediateContext);
        }

        m_camera_.SpawnControlImguiWindow();

        if (::ImGui::Begin("Simulation Speed"))
        {
            ::ImGui::SliderFloat("Speed Factor", &m_simulation_speed_, 0.0f, 5.0f);
            ::ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ::ImGui::GetIO().Framerate, ::ImGui::GetIO().Framerate);
            ::ImGui::Text("Status: %s", m_wnd_.m_pKeyboard->KeyIsPressed(VK_SPACE) ? "PAUSED" : "RUNNING (hold spacebar to pause!)");
        }

        ::ImGui::End();
    }
}