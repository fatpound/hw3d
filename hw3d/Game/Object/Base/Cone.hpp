#pragma once

#include "../../../Math/FatMath.hpp"
#include "../../../Math/IndexedTriangleList.hpp"

#include <DirectXMath.h>

namespace fatpound::hw3d::obj::base
{
    class Cone final
    {
    public:
        template <class V>
        static IndexedTriangleList<V> MakeTesselated(int longDiv)
        {
            assert(longDiv >= 3);

            namespace dx = DirectX;
            namespace fm = fatpound::math;

            const auto base = dx::XMVectorSet(1.0f, 0.0f, -1.0f, 0.0f);
            const float longitudeAngle = fm::twoPi<float> / longDiv;

            // base vertices
            std::vector<V> vertices;
            for (int iLong = 0; iLong < longDiv; iLong++)
            {
                vertices.emplace_back();

                auto v = dx::XMVector3Transform(base, dx::XMMatrixRotationZ(longitudeAngle * iLong));

                dx::XMStoreFloat3(&vertices.back().pos, v);
            }

            // the center
            vertices.emplace_back();
            vertices.back().pos = { 0.0f, 0.0f, -1.0f };
            const auto iCenter = static_cast<unsigned short int>(vertices.size() - 1);

            // the tip :darkness:
            vertices.emplace_back();
            vertices.back().pos = { 0.0f, 0.0f, 1.0f };
            const auto iTip = static_cast<unsigned short int>(vertices.size() - 1);


            // base indices
            std::vector<unsigned short int> indices;
            for (unsigned short int iLong = 0; iLong < longDiv; iLong++)
            {
                indices.push_back(iCenter);
                indices.push_back((iLong + 1) % longDiv);
                indices.push_back(iLong);
            }

            // cone indices
            for (unsigned short int iLong = 0; iLong < longDiv; iLong++)
            {
                indices.push_back(iLong);
                indices.push_back((iLong + 1) % longDiv);
                indices.push_back(iTip);
            }

            return IndexedTriangleList<V>{std::move(vertices), std::move(indices)};
        }

        template <class V>
        static IndexedTriangleList<V> Make()
        {
            return Cone::MakeTesselated<V>(24);
        }


    protected:


    private:
    };
}