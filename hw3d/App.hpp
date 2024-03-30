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
    fatpound::time::FatTimer timer_;

    static constexpr size_t drawable_count_ = 180u;
};