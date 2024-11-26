#pragma once

#include <type_traits>

#include <cstdint>

namespace fatpound::io
{
    struct KeyEvent final
    {
        enum struct Type : ::std::uint8_t
        {
            Press,
            Release,

            Invalid
        };

        Type type{ Type::Invalid };

        std::underlying_type_t<Type> code{};
    };
}