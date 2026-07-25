#pragma once
#include <d3d11.h>
#include <wrl/client.h>
#include <vector>

// Describes, byte-by-byte, how to interpret a vertex buffer's raw data -
// bridging our C++ Vertex struct's memory layout to the semantics
// (POSITION, etc.) used in the vertex shader.
//
// Created from an array of D3D11_INPUT_ELEMENT_DESC (one per vertex field)
// plus the vertex shader's compiled bytecode, since D3D11 validates the
// layout against the shader's actual declared input signature at creation
// time - this is why VertexShader (commit 12) retained its bytecode blob
// rather than discarding it after creating the live shader object.

class InputLayout
{

public:
    InputLayout(
        ID3D11Device* device,
        const std::vector<D3D11_INPUT_ELEMENT_DESC>& elements,
        ID3DBlob* vertexShaderBytecode
    );

    ID3D11InputLayout* Get() const { return m_layout.Get(); };

private:
    Microsoft::WRL::ComPtr<ID3D11InputLayout> m_layout;
};