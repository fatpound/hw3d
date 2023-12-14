#pragma once

#include <chrono>

using namespace std::chrono;

namespace fatpound::time
{
    class FatTimer
    {
	public:
        float Mark() noexcept;


    private:
        steady_clock::time_point last = steady_clock::now();
    };
}