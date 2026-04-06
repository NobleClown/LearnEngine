#include <iostream>
#include "../platform/include/DisplayWin32.h"
#include "../core/include/FrameBuffer.h"
#include "../core/include/Renderer.h"

int main() {
    DisplayWin32 display(800, 600, L"First Engine");

    FrameBuffer fb;
    Renderer renderer;
    renderer.setFrameBuffer(&fb);

    Model model;
    model.LoadOBJ("../assets/african_head/african_head.obj");

    Camera camera;
    Scene scene;

    display.setRenderCallback([&]() {
        fb.clearColor(PackRGBA({25, 25, 25}));
        renderer.render();
    });

    display.run();  // ⭐ 进入消息循环
}