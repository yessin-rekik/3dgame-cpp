#pragma once
#include <d3d11.h>
#include <wrl/client.h>
#include <vector>
#include <stdexcept>

// Wraps an immutable GPU vertex buffer.
//
// Templated on the vertex type so it can be reused for any mesh later
// (cube, model-loaded meshes, ...) without rewriting this creation logic
// each time - only the Vertex struct passed in changes.

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

// Wraps an immutable GPU index buffer - a flat list of integers describing
// which vertices (by position in the vertex buffer) form each triangle.
// Lets shared vertices (e.g. a cube's 8 corners, each touching 3 faces) be
// stored exactly once instead of duplicated per face.
//
// Fixed to 32-bit indices (UINT) for simplicity. 16-bit indices exist and
// halve memory use, but only matter once you have meshes with tens of
// thousands of vertices - not a concern yet.
class IndexBuffer
{
public:
    IndexBuffer(ID3D11Device* device, const std::vector<UINT>& indices)
        : m_indexCount(static_cast<UINT>(indices.size()))
    {
        D3D11_BUFFER_DESC desc = {};
        desc.Usage = D3D11_USAGE_IMMUTABLE;
        desc.ByteWidth = static_cast<UINT>(sizeof(UINT) * indices.size());
        desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
        desc.CPUAccessFlags = 0;
 
        D3D11_SUBRESOURCE_DATA initData = {};
        initData.pSysMem = indices.data();
 
        HRESULT hr = device->CreateBuffer(&desc, &initData, m_buffer.GetAddressOf());
        if (FAILED(hr))
        {
            throw std::runtime_error("Failed to create index buffer");
        }
    }
 
    ID3D11Buffer* Get() const { return m_buffer.Get(); }
    UINT GetIndexCount() const { return m_indexCount; }
 
private:
    Microsoft::WRL::ComPtr<ID3D11Buffer> m_buffer;
    UINT m_indexCount;
};