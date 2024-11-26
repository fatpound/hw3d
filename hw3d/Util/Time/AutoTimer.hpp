#pragma once

#include <chrono>

namespace fatpound::util::time
{
    class AutoTimer final
    {
    public:
        explicit AutoTimer() = default;
        explicit AutoTimer(const AutoTimer& src) = delete;
        explicit AutoTimer(AutoTimer&& src) = delete;

        auto operator = (const AutoTimer& src) -> AutoTimer& = delete;
        auto operator = (AutoTimer&& src)      -> AutoTimer& = delete;
        ~AutoTimer() noexcept = default;


    public:
        auto Mark() noexcept -> float;
        
        [[nodiscard]]
        auto Peek() const noexcept -> float;


    protected:


    private:
        std::chrono::steady_clock::time_point m_last_ = std::chrono::steady_clock::now();
    };
}