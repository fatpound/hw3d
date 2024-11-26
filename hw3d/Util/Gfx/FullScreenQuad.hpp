#pragma once

#include <vector>

namespace fatpound::util::gfx
{
    struct FullScreenQuad final
    {
        struct Vertex final
        {
            float x;
            float y;
            float z;

            float u;
            float v;
        };

        inline static const std::vector<Vertex> sc_vertices =
        {
            Vertex{ -1.0f,  1.0f,  0.5f,  0.0f,  0.0f },
            Vertex{  1.0f,  1.0f,  0.5f,  1.0f,  0.0f },
            Vertex{  1.0f, -1.0f,  0.5f,  1.0f,  1.0f },
            Vertex{ -1.0f,  1.0f,  0.5f,  0.0f,  0.0f },
            Vertex{  1.0f, -1.0f,  0.5f,  1.0f,  1.0f },
            Vertex{ -1.0f, -1.0f,  0.5f,  0.0f,  1.0f }
        };
    };
}