#pragma once

#include <DirectXMath.h>

#include <cassert>

#include <vector>

template <class T>
class IndexedTriangleList
{
public:
    IndexedTriangleList() = default;

    IndexedTriangleList(std::vector<T> verts_in, std::vector<unsigned short int> indices_in)
        :
        vertices_(std::move(verts_in)),
        indices_(std::move(indices_in))
    {
        assert(vertices_.size() > 2);
        assert(indices_.size() % 3 == 0);
    }


public:
    void Transform(DirectX::FXMMATRIX matrix)
    {
        for (auto& v : vertices_)
        {
            const DirectX::XMVECTOR pos = DirectX::XMLoadFloat3(&v.pos);

            DirectX::XMStoreFloat3(&v.pos, DirectX::XMVector3Transform(pos, matrix));
        }
    }

    
public:
    std::vector<T> vertices_;
    std::vector<unsigned short int> indices_;


protected:


private:
};