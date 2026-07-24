#include "Shader.h"
#include <stdexcept>

static Microsoft::WRL::ComPtr<ID3DBlob> CompileShaderFile(
    const std::wstring& hlslFilePath,
    const char* targetProfile)
{
    UINT compileFlags = 0;
#ifdef _DEBUG
    compileFlags |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

    Microsoft::WRL::ComPtr<ID3DBlob> bytecode;
    Microsoft::WRL::ComPtr<ID3DBlob> errorMessages;

    HRESULT hr = D3DCompileFromFile(
        hlslFilePath.c_str(),
        nullptr,
        D3D_COMPILE_STANDARD_FILE_INCLUDE,
        "main",
        targetProfile,
        compileFlags,
        0,
        bytecode.GetAddressOf(),
        errorMessages.GetAddressOf()
    );

    if (FAILED(hr))
    {
        std::string message = "Failed to compile shader: " +
            std::string(errorMessages ? static_cast<const char*>(errorMessages->GetBufferPointer())
                                       : "(no error message - likely a file-not-found issue; check the path)");
        throw std::runtime_error(message);
    }

    return bytecode;
}

VertexShader::VertexShader(ID3D11Device* device, const std::wstring& hlslFilePath)
{
    m_bytecode = CompileShaderFile(hlslFilePath, "vs_5_0");

    HRESULT hr = device->CreateVertexShader(
        m_bytecode->GetBufferPointer(),
        m_bytecode->GetBufferSize(),
        nullptr,
        m_shader.GetAddressOf()
    );

    if (FAILED(hr))
    {
        throw std::runtime_error("Failed to create vertex shader from compiled bytecode");
    }
}

PixelShader::PixelShader(ID3D11Device* device, const std::wstring& hlslFilePath)
{
    Microsoft::WRL::ComPtr<ID3DBlob> bytecode = CompileShaderFile(hlslFilePath, "ps_5_0");

    HRESULT hr = device->CreatePixelShader(
        bytecode->GetBufferPointer(),
        bytecode->GetBufferSize(),
        nullptr,
        m_shader.GetAddressOf()
    );

    if (FAILED(hr))
    {
        throw std::runtime_error("Failed to create pixel shader from compiled bytecode");
    }
}