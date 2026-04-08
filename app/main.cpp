#include <iostream>
#include "../platform/include/DisplayWin32.h"
#include "../core/include/FrameBuffer.h"
#include "../core/include/Renderer.h"

struct RenderContext {
    FrameBuffer* fb;
    Renderer* renderer;
    Scene* scene;
};

void RenderFunc(HDC hdc, void* userData)
{
    auto* ctx = static_cast<RenderContext*>(userData);

    ctx->fb->clearColor(PackRGBA({250, 250, 250}));
    ctx->renderer->render(*ctx->scene);
    ctx->fb->Present(hdc);
}

int main() {
    DisplayWin32 display(800, 600, L"First Engine");

    FrameBuffer fb;
    fb.resize(800, 600);

    Renderer renderer;
    renderer.setFrameBuffer(&fb);

    Model model;
    model.LoadOBJ("D:/workstation/cpp/FirstEngine/assets/african_head/african_head.obj");
    Texture2D* texture = Texture2D::LoadPPM("D:/workstation/cpp/FirstEngine/assets/african_head/african_head_SSS.ppm");
    Material mtr;
    mtr.diffuseMap = texture;
    model.setMaterial(0, &mtr);

    Camera camera;
    camera.fov = 60.f;
    camera.aspect = 800 / 600;
    camera.nearPlane = 1.f;
    camera.farPlane = 10.f;
    camera.position = {0, 0, 3};
    camera.target = {0, 0, -0.6};
    camera.up = {0, 1, 0};
    renderer.setCamera(&camera);
    renderer.setShader(new SimpleShader());

    Scene scene;
    Light light;
    light.position = {0, 10, 0};
    scene.addLight(light);
    Object obj;
    obj.transform.position = {0, 0, -0.2};
    obj.transform.scale = {0.9, 1, 1};
    obj.transform.rotation = {0.f, 0.f, 0.f};
    obj.model = &model;
    obj.material = &mtr;
    scene.addObject(obj);

    RenderContext ctx { &fb, &renderer, &scene };

    display.setRenderCallback(RenderFunc, &ctx);

    display.run();  // ⭐ 进入消息循环
}