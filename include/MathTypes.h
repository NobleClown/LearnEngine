#pragma once
#include <algorithm>
#include <cstdint>
#include <vector>
#include <iostream>
#include <cmath>

constexpr double PI = 3.14159265358979323846;

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

struct Color {
    uint8_t r, g, b, a;
};

struct Vertex {
    Vec3 ndc;
    Color color;
};

struct Mat4xn
{
public:
    Mat4xn(int n) {
        col_size = n;
        mat.resize(4 * col_size, 0);
    }

    Mat4xn(std::vector<Vec3>& points) {
        col_size = points.size();
        mat.resize(4 * col_size, 0);
        for (int i=0; i<col_size; i++) {
            set(0, i, points[i].x);
            set(1, i, points[i].y);
            set(2, i, points[i].z);
            set(3, i, 1);
        }
    }

    Mat4xn(std::vector<Vertex>& vertex) {
        col_size = vertex.size();
        mat.resize(4 * col_size, 0);
        for (int i=0; i<col_size; i++) {
            set(0, i, vertex[i].ndc.x);
            set(1, i, vertex[i].ndc.y);
            set(2, i, vertex[i].ndc.z);
            set(3, i, 1);
        }
    }

    inline float get(int row, int col) const {
        if (row >= 4 || col >= col_size) {
            std::cout << "[ERROR] row or col is overflow" << std::endl; 
            return -1;
        }
        return mat[row * col_size + col];
    }

    inline void set(int row, int col, float value) {
        if (row >= 4 || col >= col_size) {
            std::cout << "[ERROR] row or col is overflow" << std::endl; 
            return;
        }
        mat[row * col_size + col] = value;
        return;
    }

    void toVertexBuffer(std::vector<Vertex>& vertex) {
        if (vertex.size() != col_size)
            return;
        for (int i=0; i<col_size; i++) {
            vertex[i].ndc.x = get(0, i);
            vertex[i].ndc.y = get(1, i);
            vertex[i].ndc.z = get(2, i);
            float w = get(3, i);
            if (w != 0)
                vertex[i].ndc = vertex[i].ndc / w;
        }
    }

    inline int getCol() const { return col_size; }
private:
    int col_size;
    std::vector<float> mat;
};

inline float CrossProduct(const Vec3& a, const Vec3& b, const Vec3& c) {
    return  (c.x - a.x) * (b.y - a.y) - (c.y - a.y) * (b.x - a.x);
}

// viewport transform
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

Mat4xn GetOrthoProjectionMat(float fov, float aspect, float near, float far);

Mat4xn GetPersp2OrthoMat(float n, float f);

Mat4xn GetProjectionTransMat(float fov, float aspect, float near, float far);

Mat4xn MVPTrans(const Vec3& resize, const Vec3& rotate, const Vec3& move,
                const Vec3& eye, const Vec3& center, const Vec3& up,
                float fov, float aspect, float near, float far,
                const Mat4xn& points);