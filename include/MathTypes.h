#pragma once
#include <algorithm>
#include <cstdint>
#include <vector>
#include <iostream>
#include <cmath>

struct Vec3 {
    float x, y, z;
    Vec3 operator-(const Vec3& other) const {
        Vec3 res;
        res.x = x - other.x;
        res.y = y - other.y;
        res.z = z - other.z;
        return res;
    }

    Vec3 operator/(const float div) const {
        Vec3 res;
        res.x = x / div;
        res.y = y / div;
        res.z = z / div;
        return res;
    }

    Vec3 crossProduct(const Vec3& other) const {
        Vec3 res;
        res.x = y * other.z - z * other.y;
        res.y = z * other.x - x * other.z;
        res.z = x * other.y - y * other.x;
        return res;
    }

    float dotProduct(const Vec3& other) const {
        return x * other.x + y * other.y + z * other.z;
    }

    float getMagnitude() const {
        return std::sqrt(x * x + y * y + z * z);
    }

    Vec3 normalize() const {
        float mat = getMagnitude();
        Vec3 res;
        res.x = x / mat;
        res.y = y / mat;
        res.z = z / mat;
        return res;
    }
};

struct Vec4
{
    float x, y, z, w;
};

struct Mat4xn
{
public:
    Mat4xn(int n) {
        col_size = n;
        mat.resize(4 * col_size, 0);
    }

    inline float get(int row, int col) const {
        if (row >= 4 || col >= col_size) {
            std::cout << "[ERROR] row or col is overflow" << std::endl; 
            return -1;
        }
        return mat[row * col_size + col];
    }

    inline void set(int row, int col, int value) {
        mat[row * col_size + col] = value;
        if (row >= 4 || col >= col_size) {
            std::cout << "[ERROR] row or col is overflow" << std::endl; 
            return;
        }
        return;
    }

    inline int getCol() const { return col_size; }
private:
    int col_size;
    std::vector<float> mat;
};


struct Color {
    uint8_t r, g, b, a;
};

struct Vertex {
    Vec3 ndc;
    Color color;
};

inline float CrossProduct(const Vec3& a, const Vec3& b, const Vec3& c) {
    return  (c.x - a.x) * (b.y - a.y) - (c.y - a.y) * (b.x - a.x);
}

inline Vec3 NDC2Screen(const Vec3& ndc, int width, int height) {
    Vec3 out;
    out.x = (ndc.x * 0.5f + 0.5f) * (width - 1);
    out.y = (0.5f - ndc.y * 0.5f) * (height - 1);
    out.z = ndc.z;
    return out;
}

inline Color InterpColor(const Color& c0, const Color& c1, const Color& c2, float w0, float w1, float w2) {
    Color out;
    out.a = c0.a * w0 + c1.a * w1 + c2.a * w2;
    out.r = c0.r * w0 + c1.r * w1 + c2.r * w2;
    out.g = c0.g * w0 + c1.g * w1 + c2.g * w2;
    out.b = c0.b * w0 + c1.b * w1 + c2.b * w2;
    return out;
}

inline uint32_t PackRGBA(const Color& c) {
    return (uint32_t(c.a) << 24 | uint32_t(c.r) << 16 | uint32_t(c.g) << 8 | uint32_t(c.b));
}

Mat4xn MatMul(const Mat4xn& mat_a, const Mat4xn& mat_b);

Mat4xn GetResizeMat(const Vec3& resize);

Mat4xn GetRotateMat(const Vec3& theta);

Mat4xn GetMoveMat(const Vec3& move);

Mat4xn GetModelTransMat(const Vec3& resize, const Vec3& rotate, const Vec3& move);

Mat4xn GetViewTransMat(const Vec3& eye, const Vec3& center, const Vec3& up);

Mat4xn GetOrthoProjectionMat(float l, float r, float b, float t, float n, float f);

Mat4xn GetPersp2OrthoMat(float n, float f);

Mat4xn GetProjectionTransMat(float l, float r, float b, float t, float n, float f);

Mat4xn MVPTrans(const Vec3& resize, const Vec3& rotate, const Vec3& move,
                const Vec3& eye, const Vec3& center, const Vec3& up,
                float l, float r, float b, float t, float n, float f,
                const Mat4xn& points);