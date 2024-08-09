#pragma once

#include "../../../Math/Math.hpp"
#include "../../../Math/IndexedTriangleList.hpp"

#include <DirectXMath.h>

namespace fatpound::hw3d::obj::base
{
    class Sphere final
    {
    public:
        template <class V>
        static IndexedTriangleList<V> MakeTesselated(int lat_div, int long_div)
        {
            const unsigned short int latDiv = static_cast<unsigned short int>(lat_div);
            const unsigned short int longDiv = static_cast<unsigned short int>(long_div);

            assert(latDiv >= 3);
            assert(longDiv >= 3);

            namespace dx = DirectX;
            namespace fm = fatpound::math;

            constexpr float radius = 1.0f;
            const auto base = dx::XMVectorSet(0.0f, 0.0f, radius, 0.0f);
            const float lattitudeAngle = std::numbers::pi_v<float> / latDiv;
            const float longitudeAngle = fm::twoPi<float> / longDiv;

            std::vector<V> vertices;

            for (int iLat = 1; iLat < latDiv; iLat++)
            {
                const auto latBase = dx::XMVector3Transform(base, dx::XMMatrixRotationX(lattitudeAngle * iLat));

                for (int iLong = 0; iLong < longDiv; iLong++)
                {
                    vertices.emplace_back();

                    auto v = dx::XMVector3Transform(latBase, dx::XMMatrixRotationZ(longitudeAngle * iLong));

                    dx::XMStoreFloat3(&vertices.back().pos, v);
                }
            }

            // add the cap vertices
            const auto iNorthPole = static_cast<unsigned short int>(vertices.size());
            vertices.emplace_back();
            dx::XMStoreFloat3(&vertices.back().pos, base);

            const auto iSouthPole = static_cast<unsigned short int>(vertices.size());
            vertices.emplace_back();
            dx::XMStoreFloat3(&vertices.back().pos, dx::XMVectorNegate(base));

            const auto calcIdx = [latDiv, longDiv](unsigned short int iLat, unsigned short int iLong) -> std::uint16_t
                {
                    return iLat * longDiv + iLong;
                };

            std::vector<unsigned short int> indices;

            for (unsigned short int iLat = 0; iLat < latDiv - 2; iLat++)
            {
                for (unsigned short int iLong = 0; iLong < longDiv - 1; iLong++)
                {
                    indices.push_back(calcIdx(iLat, iLong));
                    indices.push_back(calcIdx(iLat + 1, iLong));
                    indices.push_back(calcIdx(iLat, iLong + 1));
                    indices.push_back(calcIdx(iLat, iLong + 1));
                    indices.push_back(calcIdx(iLat + 1, iLong));
                    indices.push_back(calcIdx(iLat + 1, iLong + 1));
                }

                // wrap band
                indices.push_back(calcIdx(iLat, longDiv - 1));
                indices.push_back(calcIdx(iLat + 1, longDiv - 1));
                indices.push_back(calcIdx(iLat, 0));
                indices.push_back(calcIdx(iLat, 0));
                indices.push_back(calcIdx(iLat + 1, longDiv - 1));
                indices.push_back(calcIdx(iLat + 1, 0));
            }

            // cap fans
            for (unsigned short int iLong = 0; iLong < longDiv - 1; iLong++)
            {
                // north
                indices.push_back(iNorthPole);
                indices.push_back(calcIdx(0, iLong));
                indices.push_back(calcIdx(0, iLong + 1));
                // south
                indices.push_back(calcIdx(latDiv - 2, iLong + 1));
                indices.push_back(calcIdx(latDiv - 2, iLong));
                indices.push_back(iSouthPole);
            }

            // wrap triangles
            // north
            indices.push_back(iNorthPole);
            indices.push_back(calcIdx(0, longDiv - 1));
            indices.push_back(calcIdx(0, 0));

            // south
            indices.push_back(calcIdx(latDiv - 2, 0));
            indices.push_back(calcIdx(latDiv - 2, longDiv - 1));
            indices.push_back(iSouthPole);

            return IndexedTriangleList<V>{std::move(vertices), std::move(indices)};
        }

        template <class V>
        static IndexedTriangleList<V> Make()
        {
            return Sphere::MakeTesselated<V>(12, 24);
        }


    protected:


    private:
    };
}