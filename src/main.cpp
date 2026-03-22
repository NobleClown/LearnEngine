#define UNICODE
#define _UNICODE
#include <Windows.h>
#include <iostream>
#include "../include/Renderer.h"

FrameBuffer g_fb;
Renderer* g_renderer = nullptr;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_CREATE: {
            HDC hdc = GetDC(hwnd);
            g_fb.Initialize(hdc, 800, 600);
            g_renderer = new Renderer(&g_fb);
            ReleaseDC(hwnd, hdc);
            break;
        }
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            g_renderer->Clear({25, 25, 25, 0});
            Vertex v0 = {{-0.5f, -0.5f, 1.0f}, {255, 0, 0, 0}};
            Vertex v1 = {{0.5f, -0.5f, 0.0f}, {0, 255, 0, 0}};
            Vertex v2 = {{0.0f, 0.5f, -1.0f}, {0, 0, 255, 0}};

            g_renderer->DrawTriangle(v0, v1, v2);

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