#include "AutoTimer.hpp"

namespace fatpound::util
{
    float AutoTimer::Mark() noexcept
    {
        const auto old = last;

        last = std::chrono::steady_clock::now();

        const std::chrono::duration<float> frameTime = last - old;

        return frameTime.count();
    }
    float AutoTimer::Peek() const noexcept
    {
        return std::chrono::duration<float>(std::chrono::steady_clock::now() - last).count();
    }
}