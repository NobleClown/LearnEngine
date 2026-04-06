#pragma once
#include <vector>
#include <stdint.h>
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

    /**
     * 输出：深度引用（可读写）
     */
    float& depth(int x, int y);

private:
    std::vector<uint32_t> colorBuffer;
    std::vector<float> depthBuffer;
};