#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl/client.h>
#include <string>

class VertexShader
{

public:
    VertexShader(ID3D11Device* device, const std::wstring& hlslFilePath);

    ID3D11VertexShader* Get() const { return m_shader.Get(); };
    ID3DBlob* GetBytecode() const { return m_bytecode.Get(); };

private:
    Microsoft::WRL::ComPtr<ID3D11VertexShader> m_shader;
    Microsoft::WRL::ComPtr<ID3DBlob> m_bytecode;

};

class PixelShader
{

public:
    PixelShader(ID3D11Device* m_device, const std::wstring& hlslFilePath);

    ID3D11PixelShader* Get() const { return m_shader.Get(); };

private:
    Microsoft::WRL::ComPtr<ID3D11PixelShader> m_shader;

};