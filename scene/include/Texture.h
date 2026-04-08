#pragma once
#include <vector>
#include <fstream>
#include <sstream>

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

    static Texture2D* LoadPPM(const std::string& path) {
        std::ifstream file(path, std::ios::binary);
        if (!file) {
            throw std::runtime_error("Failed to open file: " + path);
        }

        std::string line;
        
        // 1️⃣ 读取 magic number
        std::getline(file, line);
        if (line != "P6") {
            throw std::runtime_error("Not a valid PPM (P6) file");
        }

        // 2️⃣ 跳过注释
        do {
            std::getline(file, line);
        } while (line[0] == '#');

        // 3️⃣ 读取宽高
        std::istringstream dimStream(line);
        int width, height;
        dimStream >> width >> height;

        // 4️⃣ 读取 max value
        int maxVal;
        file >> maxVal;
        file.get(); // 吃掉换行符

        if (maxVal != 255) {
            throw std::runtime_error("Only 8-bit PPM supported");
        }

        // 5️⃣ 读取像素数据
        std::vector<unsigned char> data(width * height * 3);
        file.read(reinterpret_cast<char*>(data.data()), data.size());

        return new Texture2D(width, height, 3, std::move(data));
    }

    // ===== UV采样 =====
    Vec3 sample(const Vec2& uv) const
    {
        // 1️⃣ UV wrap（repeat）
        // float u = uv.x - std::floor(uv.x);
        // float v = uv.y - std::floor(uv.y);
        float u = uv.x;
        float v = uv.y;

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

        float r = data[idx];
        float g = data[idx + 1];
        float b = data[idx + 2];

        return Vec3(r, g, b);
    }
};