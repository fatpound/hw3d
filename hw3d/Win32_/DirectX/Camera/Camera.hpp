#pragma once

#include "../Direct3D11/Graphics.hpp"

namespace fatpound::win32::directx
{
    class Camera final
    {
    public:
        auto GetMatrix() const noexcept -> ::DirectX::XMMATRIX;

        void SpawnControlImguiWindow() noexcept;
        void Reset() noexcept;


    protected:


    private:
        float r_ = 20.0f;// distance from the origin
        float theta_ = 0.0f; // these are for rotation around the origin
        float phi_ = 0.0f; // these are for rotation around the origin
        float pitch_ = 0.0f; // these are for camera orientation
        float yaw_ = 0.0f; // these are for camera orientation
        float roll_ = 0.0f; // these are for camera orientation
    };
}