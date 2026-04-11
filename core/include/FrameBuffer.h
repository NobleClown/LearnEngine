#pragma once
#include <vector>
#include <stdint.h>
#include <windows.h>
class FrameBuffer {
public:
    int width;
    int height;

public:
    /**
     * 初始化buffer
     */
    void resize(int w, int h);

    /**
     * 输入：颜色值
     */
    void clearColor(uint32_t color);

    /**
     * 输入：深度值（通常为1.0）
     */
    void clearDepth(float depth);

    /**
     * 输入：像素坐标 + 颜色
     */
    void setPixel(int x, int y, uint32_t color);

    /**
     * 输出：像素颜色
     */
    uint32_t getPixel(int x, int y) const;

    float getDepth(int x, int y, int k) const;

    void setDepth(int x, int y, int k, float val);

    void Present(HDC hdc);

private:
    std::vector<uint32_t> colorBuffer;
    std::vector<float> depthBuffer;
};