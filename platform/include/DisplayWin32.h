#pragma once
#define UNICODE
#define _UNICODE
#include <Windows.h>
#include <functional>
#include "../../core/include/FrameBuffer.h"

using RenderCallback = void(*)(HDC, void*);
class DisplayWin32 {
public:
    DisplayWin32(int width, int height, const wchar_t* title);

    void run();

    void setRenderCallback(RenderCallback cb, void* userData) {
        renderCallback = cb;
        this->userData = userData;
    }

private:
    static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
        DisplayWin32* self;

        if (msg == WM_NCCREATE) {
            CREATESTRUCT* cs = (CREATESTRUCT*)lParam;
            self = (DisplayWin32*)cs->lpCreateParams;
            SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)self);
        } else {
            self = (DisplayWin32*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
        }

        if (self) {
            return self->handleMessage(hwnd, msg, wParam, lParam);
        }

        return DefWindowProc(hwnd, msg, wParam, lParam);
    }

    LRESULT handleMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
        switch (msg) {
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            if (renderCallback) {
                renderCallback(hdc, userData);  // ⭐ 调用外部渲染
            }   

            EndPaint(hwnd, &ps);
            return 0;
        }

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        }

        return DefWindowProc(hwnd, msg, wParam, lParam);
    }

private:
    HWND hwnd;
    int m_width, m_height;
    void* userData;
    std::function<void(HDC, void*)> renderCallback;
};