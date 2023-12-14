#include "FatTimer.hpp"

using namespace std::chrono;

namespace fatpound::time
{
    FatTimer::FatTimer()
    {
		last = steady_clock::now();
    }

    float FatTimer::Mark() noexcept
    {
        const auto old = last;
        last = steady_clock::now();

        const duration<float> frameTime = last - old;

        return frameTime.count();
    }
	float FatTimer::Peek() const noexcept
	{
		return duration<float>(steady_clock::now() - last).count();
	}
}