#pragma once

#include <chrono>

namespace fatpound::time
{
    class FatTimer
    {
	public:
		FatTimer();


	public:
        float Mark() noexcept;
        float Peek() const noexcept;


    private:
		std::chrono::steady_clock::time_point last;
    };
}