#pragma once

#include <windows.h>
#include <vector>
#include "MathTypes.h"

class FrameBuffer {
public:
    FrameBuffer():
        m_width(0), m_height(0), m_bitmap(nullptr),
        m_memDC(nullptr), m_pixels(nullptr)
    {

    }
    ~FrameBuffer();

    bool Initialize(HDC hdc, int width, int height);
    void Clear(const Color& c);
    void Present(HDC hdc);

    void SetPixel(int x, int y, float z, const Color& c);

    int GetWidth() const { return m_width; }
    int GetHeight() const { return m_height; }

private:
    int m_width;
    int m_height;

    HBITMAP m_bitmap;
    HDC m_memDC;
    unsigned int* m_pixels;

    std::vector<float> m_zbuffer;
};