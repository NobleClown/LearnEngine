#include <limits>
#include <iostream>
#include "../include/FrameBuffer.h"

void FrameBuffer::resize(int w, int h) {
    width = w;
    height = h;
    depthBuffer.resize(w * h, -100);
    colorBuffer.resize(w * h, 0);
    return ;
}

void FrameBuffer::clearColor(uint32_t color) {
    std::fill(colorBuffer.begin(), colorBuffer.end(), color);
    return ;
}

void FrameBuffer::clearDepth(float depth) {
    std::fill(depthBuffer.begin(), depthBuffer.end(), depth);
    return ;
}

void FrameBuffer::setPixel(int x, int y, uint32_t color) {
    colorBuffer[y * width + x] = color;
    return ;
}

float FrameBuffer::getDepth(int x, int y) const {
    if (x >= width || y >= height)
        return 100.f;
    return depthBuffer[y * width + x];
}

void FrameBuffer::setDepth(int x, int y, float val) {
    if (x >= width || y >= height)
        return;
    depthBuffer[y * width + x] = val;
}

void FrameBuffer::Present(HDC hdc) {
    BITMAPINFO bmi = {};
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = width;
    bmi.bmiHeader.biHeight = -height; // 负数 = 不翻转（关键！）
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biCompression = BI_RGB;


    SetDIBitsToDevice(hdc, 0, 0, width, height, 0, 0, 0, height, colorBuffer.data(), &bmi, DIB_RGB_COLORS);
}