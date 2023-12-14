#pragma once

#include "Window.hpp"

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
};