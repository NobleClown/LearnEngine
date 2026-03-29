#include <limits>

#include "../include/FrameBuffer.h"

FrameBuffer::~FrameBuffer() {
    if (m_memDC) DeleteDC(m_memDC);
    if (m_bitmap) DeleteObject(m_bitmap);
}

bool FrameBuffer::Initialize(HDC hdc, int width, int height) {
    m_width = width;
    m_height = height;

    BITMAPINFO bmi = {};
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);  // 表示BITMAPINFOHEADER结构体本身大小
    bmi.bmiHeader.biWidth = width;  // 位图宽度，单位：像素
    bmi.bmiHeader.biHeight = -height;   // 位图高度+存储方向，单位：像素，
    bmi.bmiHeader.biPlanes = 1; // 历史遗留字段，必须为1，不能改
    bmi.bmiHeader.biBitCount = 32;  // 每像素位数，8：调色板；24：RGB；32：BGRA
    bmi.bmiHeader.biCompression = BI_RGB;   // 压缩方式，BI_RGB：不压缩；

    m_memDC = CreateCompatibleDC(hdc);
    m_bitmap = CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, (void**)&m_pixels, nullptr, 0);
    SelectObject(m_memDC, m_bitmap);

    m_zbuffer.resize(width * height, -100.0f);

    return m_pixels != nullptr;
}

void FrameBuffer::Clear(const Color& c) {
    // TODO: set zbuffer
    unsigned int color = PackRGBA(c);
    for (int i=0; i<m_width * m_height; i++)
        m_pixels[i] = color;
}

void FrameBuffer::SetPixel(int x, int y, float z, const Color& c) {
    if (x < 0 || y < 0 || x >= m_width || y >= m_height)
        return ;
    
    if (z < m_zbuffer[y * m_width + x])
        return ;
    m_zbuffer[y * m_width + x] = z;
    unsigned int color = PackRGBA(c);
    // TODO: add comparison with zbuffer
    m_pixels[y * m_width + x] = color;
}

void FrameBuffer::Present(HDC hdc) {
    BitBlt(hdc, 0, 0, m_width, m_height, m_memDC, 0, 0, SRCCOPY);
}

