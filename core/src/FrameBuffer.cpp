#include <limits>
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
    colorBuffer[x * width + y] = color;
    return ;
}

float& FrameBuffer::depth(int x, int y) {
    return depthBuffer[x * width + y];
}