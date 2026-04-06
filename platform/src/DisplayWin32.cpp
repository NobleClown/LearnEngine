#include "../include/DisplayWin32.h"

DisplayWin32::DisplayWin32(int width, int height, const wchar_t* title) {
    m_height = height;
    m_width = width;
    WNDCLASS wc = {};
    wc.lpfnWndProc = DisplayWin32::WndProc;
    wc.hInstance = GetModuleHandle(nullptr);
    wc.lpszClassName = L"FirstEngineWnd";

    RegisterClassW(&wc);

    hwnd = CreateWindow(
        wc.lpszClassName,
        title,
        WS_OVERLAPPEDWINDOW,
        100, 100, width + 20, height + 40,
        NULL, NULL,
        wc.hInstance,
        this   // ⭐ 关键：传 this
    );

    ShowWindow(hwnd, SW_SHOW);
}

void DisplayWin32::run() {
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}