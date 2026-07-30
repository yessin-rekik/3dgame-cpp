#include "core/Window.h"
#include "core/Timer.h"
#include "graphics/Direct3D.h"
#include "graphics/Shader.h"
#include "graphics/Vertex.h"
#include "graphics/Buffer.h"
#include "graphics/InputLayout.h"
#include <Windows.h>

#include <iostream> // Required header

int main()
{

    try
    {
        Window window(L"DirectX 11 Engine", 1280, 720);
        Direct3D d3d(window.GetHandle(), window.GetWidth(), window.GetHeight());

        VertexShader vertexShader(d3d.GetDevice(), L"assets/shaders/vs/BasicVertexShader.hlsl");
        PixelShader pixelShader(d3d.GetDevice(), L"assets/shaders/ps/BasicPixelShader.hlsl");

        // std::vector<Vertex> triangleVertices = {
        //     {0.0f, 0.5f, 0.0f},
        //     {0.5f, -0.5f, 0.0f},
        //     {-0.5f, -0.5f, 0.0f},
        // };

        // Cube Vertices (8 vertices for a cube)
        std::vector<Vertex> cubeVertices = {
            {-0.5f, -0.5f, -0.5f}, // 0: left  bottom back
            {-0.5f, 0.5f, -0.5f},  // 1: left  top    back
            {0.5f, 0.5f, -0.5f},   // 2: right top    back
            {0.5f, -0.5f, -0.5f},  // 3: right bottom back
            {-0.5f, -0.5f, 0.5f},  // 4: left  bottom front
            {-0.5f, 0.5f, 0.5f},   // 5: left  top    front
            {0.5f, 0.5f, 0.5f},    // 6: right top    front
            {0.5f, -0.5f, 0.5f},   // 7: right bottom front
        };

        // 12 triangles (2 per face x 6 faces), each listed with clockwise
        // winding as seen from outside the cube - D3D11's default front-face
        // winding, matching the same convention established for the
        // triangle
        std::vector<UINT> cubeIndices = {
            // back face
            0,
            1,
            2,
            0,
            2,
            3,
            // front face
            4,
            6,
            5,
            4,
            7,
            6,
            // left face
            4,
            5,
            1,
            4,
            1,
            0,
            // right face
            3,
            2,
            6,
            3,
            6,
            7,
            // top face
            1,
            5,
            6,
            1,
            6,
            2,
            // bottom face
            4,
            0,
            3,
            4,
            3,
            7,
        };

        // VertexBuffer<Vertex> triangleBuffer(d3d.GetDevice(), triangleVertices);
        VertexBuffer<Vertex> cubeVertexBuffer(d3d.GetDevice(), cubeVertices);
        IndexBuffer cubeIndexBuffer(d3d.GetDevice(), cubeIndices);

        std::vector<D3D11_INPUT_ELEMENT_DESC> layoutElements = {
            {"POSITION",
             0,
             DXGI_FORMAT_R32G32B32_FLOAT,
             0,
             0,
             D3D11_INPUT_PER_VERTEX_DATA,
             0}};

        // InputLayout triangleInputLayout(d3d.GetDevice(), layoutElements, vertexShader.GetBytecode());
        InputLayout cubeInputLayout(d3d.GetDevice(), layoutElements, vertexShader.GetBytecode());

        Timer timer;

        float fpsTimer = 0.0f;
        int frameCount = 0;

        while (window.ProcessMessages())
        {

             if (window.ConsumeResizeEvent())
            {

                d3d.Resize(window.GetWidth(), window.GetHeight());
            }

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
            // d3d.GetContext()->IASetInputLayout(triangleInputLayout.Get());
            d3d.GetContext()->IASetInputLayout(cubeInputLayout.Get());

            // ID3D11Buffer *vertexBuffers[] = {triangleBuffer.Get()};
            ID3D11Buffer *vertexBuffers[] = {cubeVertexBuffer.Get()};

            // UINT strides[] = {triangleBuffer.GetStride()};
            UINT strides[] = {cubeVertexBuffer.GetStride()};
            UINT offsets[] = {0};
            d3d.GetContext()->IASetVertexBuffers(0, 1, vertexBuffers, strides, offsets);

            // DXGI_FORMAT_R32_UINT matches IndexBuffer's fixed 32-bit (UINT)
            d3d.GetContext()->IASetIndexBuffer(cubeIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

            d3d.GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

            d3d.GetContext()->VSSetShader(vertexShader.Get(), nullptr, 0);
            d3d.GetContext()->PSSetShader(pixelShader.Get(), nullptr, 0);

            // Actual drawing goes here .
            // d3d.GetContext()->Draw(triangleBuffer.GetVertexCount(), 0);

            // DrawIndexed instead of Draw, now that we're going through an
            // index buffer - the GPU reads indices from it to decide which
            // vertices (and how many times each) to pull from the vertex
            // buffer, rather than reading it start-to-end in order.
            d3d.GetContext()->DrawIndexed(cubeIndexBuffer.GetIndexCount(), 0, 0);

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
