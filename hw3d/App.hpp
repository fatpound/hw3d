#pragma once

#include "Window.hpp"
#include "FatTimer.hpp"

using fatpound::time::FatTimer;

class App
{
public:
	App();
    ~App() noexcept;
    App(const App& src) = delete;
    App(App&& src) = delete;
    App& operator = (const App& src) = delete;
    App& operator = (App&& src) = delete;


public:
	int Go();


protected:


private:
	void DoFrame();


private:
    std::vector<std::unique_ptr<class Drawable>> drawables_;

	Window wnd_;
	FatTimer timer_;

    static constexpr size_t drawable_count_ = 180u;
};