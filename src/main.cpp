#include "core/Window.h"
#include "core/Timer.h"
#include "graphics/Direct3D.h"
#include "graphics/Shader.h"
#include "graphics/Vertex.h"
#include "graphics/Buffer.h"
#include "graphics/InputLayout.h"
#include <Windows.h>

int main()
{

    try
    {
        Window window(L"DirectX 11 Engine", 1280, 720);
        Direct3D d3d(window.GetHandle(), window.GetWidth(), window.GetHeight());

        VertexShader vertexShader(d3d.GetDevice(), L"assets/shaders/vs/BasicVertexShader.hlsl");
        PixelShader pixelShader(d3d.GetDevice(), L"assets/shaders/ps/BasicPixelShader.hlsl");

        std::vector<Vertex> triangleVertices = {
            {0.0f, 0.5f, 0.0f},
            {0.5f, -0.5f, 0.0f},
            {-0.5f, -0.5f, 0.0f},
        };

        VertexBuffer<Vertex> triangleBuffer(d3d.GetDevice(), triangleVertices);

        std::vector<D3D11_INPUT_ELEMENT_DESC> layoutElements = {
            {"POSITION",
             0,
             DXGI_FORMAT_R32G32B32_FLOAT,
             0,
             0,
             D3D11_INPUT_PER_VERTEX_DATA,
             0}};

        InputLayout triangleInputLayout(d3d.GetDevice(), layoutElements, vertexShader.GetBytecode());

        Timer timer;

        float fpsTimer = 0.0f;
        int frameCount = 0;

        while (window.ProcessMessages())
        {

            const float dt = timer.Tick();
            fpsTimer += dt;
            frameCount++;

            // Update the window title with the current FPS once per second ( which means once deltaSeconds reaches 1 or above ).
            if (fpsTimer >= 1.0f)
            {
                const std::wstring title = L"DirectX 11 Engine - FPS: " + std::to_wstring(frameCount);
                SetWindowText(window.GetHandle(), title.c_str());
                frameCount = 0;
                fpsTimer = 0.0f;
            }

            d3d.Clear(0.392f, 0.584f, 0.929f);

            // binds everything the pipeline needs for draw
            d3d.GetContext()->IASetInputLayout(triangleInputLayout.Get());

            ID3D11Buffer* vertexBuffers[] = { triangleBuffer.Get() };
            UINT strides[] = { triangleBuffer.GetStride() };
            UINT offsets[] = { 0 };
            d3d.GetContext()->IASetVertexBuffers(0, 1, vertexBuffers, strides, offsets);

            d3d.GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

            d3d.GetContext()->VSSetShader(vertexShader.Get(), nullptr, 0);
            d3d.GetContext()->PSSetShader(pixelShader.Get(), nullptr, 0);
            

            // Actual drawing goes here .
            d3d.GetContext()->Draw(triangleBuffer.GetVertexCount(), 0);

            d3d.Present();
        }
    }
    catch (const std::exception &e)
    {
        MessageBoxA(nullptr, e.what(), "Fatal Error", MB_OK | MB_ICONERROR);
        return -1;
    }

    return 0;
}
