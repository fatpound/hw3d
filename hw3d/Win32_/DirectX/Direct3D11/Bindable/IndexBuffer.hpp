#pragma once

#include "Bindable.hpp"

class IndexBuffer : public Bindable
{
public:
    IndexBuffer(Graphics& gfx, const std::vector<unsigned short int>& indices);


public:
    UINT GetCount() const noexcept;

    void Bind(Graphics& gfx) noexcept override;


protected:
    Microsoft::WRL::ComPtr<ID3D11Buffer> pIndexBuffer_;

    UINT count_;


private:
};