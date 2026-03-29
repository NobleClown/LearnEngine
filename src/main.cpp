#define UNICODE
#define _UNICODE
#include <Windows.h>
#include <iostream>
#include <vector>
#include "../include/Renderer.h"
#include "../include/Model.h"
#include "../include/Camera.h"

FrameBuffer g_fb;
Renderer* g_renderer = nullptr;
Model* g_model = nullptr;
Camera* g_camera = nullptr;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_CREATE: {
            HDC hdc = GetDC(hwnd);
            g_fb.Initialize(hdc, 800, 600);
            g_renderer = new Renderer(&g_fb);
            g_model = new Model();
            g_model->LoadOBJ("D:/workstation/cpp/FirstEngine/assets/african_head/african_head.obj");
            g_camera = new Camera();
            ReleaseDC(hwnd, hdc);
            break;
        }
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            g_renderer->Clear({25, 25, 25, 0});
            std::vector<Vertex> vertexBuffer = g_model->GetVertexBuffer();
            std::vector<Triangle> indexBuffer = g_model->GetIndexBuffer();

            Mat4xn points(vertexBuffer);
            Vec3 center = {0, 0, -0.6};
            Vec3 world_up = {0, 1, 0};
            Vec3 resize = {0.9, 1, 1};
            Vec3 rotate = {0.f, 0.f, 0.f};
            Vec3 move = {0, 0, -0.2};
            float fov = 60.f; // 垂直可视角度
            float aspect = 800 / 600; // 宽高比
            float near_z = 1.0f;
            float far_z = 10.f;
            Mat4xn new_points = MVPTrans(resize, rotate, move, g_camera->GetPosition(), center, world_up, 
                                        fov, aspect, near_z, far_z, points);
            new_points.toVertexBuffer(vertexBuffer);
            for (int i=0; i<indexBuffer.size(); i++) {
                Triangle tri = indexBuffer[i];
                vertexBuffer[tri.idx0].color = {255, 0, 0, 0};
                vertexBuffer[tri.idx1].color = {0, 255, 0, 0};
                vertexBuffer[tri.idx2].color = {0, 0, 255, 0};
                g_renderer->DrawTriangle(vertexBuffer[tri.idx0], vertexBuffer[tri.idx1], vertexBuffer[tri.idx2]);
            }

            g_fb.Present(hdc);

            EndPaint(hwnd, &ps);
            break;
        }
        case WM_DESTROY: {
            PostQuitMessage(0);
            break;
        }
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int nCmdShow) {
    WNDCLASS wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInst;
    wc.lpszClassName = L"FirstEngineWnd";
    RegisterClassW(&wc);

    HWND hwnd = CreateWindow(wc.lpszClassName, L"FIRST ENGINE",
    WS_OVERLAPPEDWINDOW, 100, 100, 820, 640, NULL, NULL, hInst, NULL);

    ShowWindow(hwnd, nCmdShow);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}