#include <vector>

#include "../../mathtool/include/MathType.h"

class Texture2D {
public:
    int width;
    int height;
    int channels; // 一般是3或4

    std::vector<unsigned char> data; // 原始像素数据

public:
    Texture2D(int w, int h, int c, std::vector<unsigned char>&& d)
        : width(w), height(h), channels(c), data(std::move(d)) {}

    // ===== UV采样 =====
    Vec3 sample(const Vec2& uv) const
    {
        // 1️⃣ UV wrap（repeat）
        float u = uv.x - floor(uv.x);
        float v = uv.y - floor(uv.y);

        // 2️⃣ 转换到像素坐标
        float x = u * (width - 1);
        float y = (1.0f - v) * (height - 1); // 注意：通常纹理v方向是反的

        int ix = static_cast<int>(x);
        int iy = static_cast<int>(y);

        // 3️⃣ clamp（防越界）
        ix = std::max(0, std::min(ix, width - 1));
        iy = std::max(0, std::min(iy, height - 1));

        // 4️⃣ 读取像素
        int idx = (iy * width + ix) * channels;

        float r = data[idx] / 255.0f;
        float g = data[idx + 1] / 255.0f;
        float b = data[idx + 2] / 255.0f;

        return Vec3(r, g, b);
    }
};