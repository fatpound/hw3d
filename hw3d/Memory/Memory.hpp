#pragma once

#include <cstdlib>

#include <memory>

namespace fatpound::memory
{
	template <typename T> using unique_ptr_aligned       = ::std::unique_ptr<T,   decltype(&::_aligned_free)>;
    template <typename T> using unique_ptr_aligned_array = ::std::unique_ptr<T[], decltype(&::_aligned_free)>;

    template <typename T>
    auto make_unique_aligned(const std::size_t alignBytes) -> unique_ptr_aligned<T>
    {
        auto* const ptr = static_cast<T*>(::_aligned_malloc(sizeof(T), alignBytes));

        if (ptr == nullptr)
        {
            throw std::bad_alloc();
        }

        return unique_ptr_aligned<T>(ptr, &::_aligned_free);
    }

    template <typename T>
    auto make_unique_aligned_array(const std::size_t size, const std::size_t alignBytes) -> unique_ptr_aligned_array<T>
    {
        auto* const ptr = static_cast<T*>(::_aligned_malloc(size * sizeof(T), alignBytes));

        if (ptr == nullptr)
        {
            throw std::bad_alloc();
        }

        return unique_ptr_aligned_array<T>(ptr, &::_aligned_free);
    }
}