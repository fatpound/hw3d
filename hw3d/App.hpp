#pragma once

#include "Window.hpp"
#include "FatTimer.hpp"
#include "ImguiManager.hpp"

class App final
{
public:
    App();
    App(const App& src) = delete;
    App(App&& src) = delete;
    App& operator = (const App& src) = delete;
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

    fatpound::util::Timer timer_;

    bool show_imgui_demo_window_ = true;

    static constexpr size_t drawable_count_ = 180u;
};