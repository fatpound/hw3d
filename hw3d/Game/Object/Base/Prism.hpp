#pragma once

#include "../../../Math/FatMath.hpp"
#include "../../../Math/IndexedTriangleList.hpp"

#include <DirectXMath.h>

class Prism final
{
public:
    template <class V>
    static IndexedTriangleList<V> MakeTesselated(int longDiv)
    {
        assert(longDiv >= 3);

        namespace dx = DirectX;
        namespace fm = fatpound::math;

        const auto base = dx::XMVectorSet(1.0f, 0.0f, -1.0f, 0.0f);
        const auto offset = dx::XMVectorSet(0.0f, 0.0f, 2.0f, 0.0f);
        const float longitudeAngle = fm::twoPi<float> / longDiv;

        // near center
        std::vector<V> vertices;
        vertices.emplace_back();
        vertices.back().pos = { 0.0f,0.0f,-1.0f };
        const auto iCenterNear = static_cast<unsigned short int>(vertices.size() - 1);

        // far center
        vertices.emplace_back();
        vertices.back().pos = { 0.0f,0.0f,1.0f };
        const auto iCenterFar = static_cast<unsigned short int>(vertices.size() - 1);

        // base vertices
        for (int iLong = 0; iLong < longDiv; iLong++)
        {
            // near base
            {
                vertices.emplace_back();

                auto v = dx::XMVector3Transform(base, dx::XMMatrixRotationZ(longitudeAngle * iLong));

                dx::XMStoreFloat3(&vertices.back().pos, v);
            }

            // far base
            {
                vertices.emplace_back();

                auto v = dx::XMVector3Transform(base, dx::XMMatrixRotationZ(longitudeAngle * iLong));

                v = dx::XMVectorAdd(v, offset);

                dx::XMStoreFloat3(&vertices.back().pos, v);
            }
        }

        // side indices
        std::vector<unsigned short int> indices;

        for (unsigned short int iLong = 0; iLong < longDiv; iLong++)
        {
            const auto i = iLong * 2;
            const auto mod = longDiv * 2;

            indices.push_back(i + 2);
            indices.push_back((i + 2) % mod + 2);
            indices.push_back(i + 1 + 2);
            indices.push_back((i + 2) % mod + 2);
            indices.push_back((i + 3) % mod + 2);
            indices.push_back(i + 1 + 2);
        }

        // base indices
        for (unsigned short int iLong = 0; iLong < longDiv; iLong++)
        {
            const auto i = iLong * 2;
            const auto mod = longDiv * 2;

            indices.push_back(i + 2);
            indices.push_back(iCenterNear);
            indices.push_back((i + 2) % mod + 2);
            indices.push_back(iCenterFar);
            indices.push_back(i + 1 + 2);
            indices.push_back((i + 3) % mod + 2);
        }

        return IndexedTriangleList<V>{std::move(vertices), std::move(indices)};
    }
    template <class V>
    static IndexedTriangleList<V> Make()
    {
        return MakeTesselated<V>(24);
    }


protected:


private:
};