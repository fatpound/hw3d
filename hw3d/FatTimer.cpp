#include "FatTimer.hpp"

namespace fatpound::time
{
    float FatTimer::Mark() noexcept
    {
        const auto old = last;

        last = steady_clock::now();

        const duration<float> frameTime = last - old;

        return frameTime.count();
    }
}