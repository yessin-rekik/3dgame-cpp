#include "InputLayout.h"
#include <stdexcept>

InputLayout::InputLayout(
    ID3D11Device* device,
    const std::vector<D3D11_INPUT_ELEMENT_DESC>& elements,
    ID3DBlob* vertexShaderBytecode)
{
    HRESULT hr = device->CreateInputLayout(
        elements.data(),
        static_cast<UINT>(elements.size()),
        vertexShaderBytecode->GetBufferPointer(),
        vertexShaderBytecode->GetBufferSize(),
        m_layout.GetAddressOf()
    );

    if (FAILED(hr))
    {
        throw std::runtime_error(
            "Failed to create input layout - check that the element list matches "
            "the vertex shader's input struct exactly (semantics, order, types)");
    }
}