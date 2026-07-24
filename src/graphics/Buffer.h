#pragma once
#include <d3d11.h>
#include <wrl/client.h>
#include <vector>
#include <stdexcept>

template <typename VertexT>
class VertexBuffer
{
public:
    VertexBuffer(ID3D11Device* device, const std::vector<VertexT>& vertices)
        : m_vertexCount(static_cast<UINT>(vertices.size()))
    {
        D3D11_BUFFER_DESC desc = {};
        desc.Usage = D3D11_USAGE_IMMUTABLE;
        desc.ByteWidth = static_cast<UINT>(sizeof(VertexT) * vertices.size());
        desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        desc.CPUAccessFlags = 0;

        D3D11_SUBRESOURCE_DATA initData = {};
        initData.pSysMem = vertices.data();

        HRESULT hr = device->CreateBuffer(&desc, &initData, m_buffer.GetAddressOf());
        if (FAILED(hr))
        {
            throw std::runtime_error("Failed to create vertex buffer");
        }
    }

    ID3D11Buffer* Get() const { return m_buffer.Get(); }
    UINT GetVertexCount() const { return m_vertexCount; }
    UINT GetStride() const { return sizeof(VertexT); }

private:
    Microsoft::WRL::ComPtr<ID3D11Buffer> m_buffer;
    UINT m_vertexCount;
};