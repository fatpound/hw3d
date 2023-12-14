#pragma once

#include "Window.hpp"
#include "FatTimer.hpp"

using namespace fatpound::time;

class App
{
public:
	App();


public:
	int Go();


private:
	void DoFrame();


private:
	Window wnd;
	FatTimer timer;
};