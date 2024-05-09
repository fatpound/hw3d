#pragma once

#include "../Util/FatTimer.hpp"
#include "../Util/ImguiManager.hpp"

#include "../Win32_/DirectX/Direct3D11/Camera/Camera.hpp"

#include "../Win32_/DirectX/Direct3D11/Window.hpp"

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
    std::vector<std::unique_ptr<class Drawable>> drawables_;

    ImguiManager imgui_;
    Window wnd_;
    Graphics& gfx_;
    Camera camera_;

    fatpound::util::Timer timer_;

    float simulation_speed_ = 1.0f;

    static constexpr size_t drawable_count_ = 180u;
};