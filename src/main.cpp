#include "core/Window.h"

int main()
{
    Window window(L"DirectX 11 Engine", 1280, 720);

    while (window.ProcessMessages())
    {
        // Per-frame update/render logic will go here.
    }

    return 0;
}
