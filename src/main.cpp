#include "core/Window.h"
#include "core/Timer.h"
#include "graphics/Direct3D.h"
#include "graphics/Shader.h"
#include "graphics/Vertex.h"
#include "graphics/Buffer.h"
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

            // Actual drawing goes here (Step 2 onward).

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
