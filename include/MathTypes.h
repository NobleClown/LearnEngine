#pragma once
#include <algorithm>
#include <cstdint>
#include <vector>
#include <iostream>
#include <cmath>

struct Vec3 {
    float x, y, z;
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

Mat4xn MatMul(Mat4xn& mat_a, Mat4xn& mat_b) {
    Mat4xn res(mat_b.getCol());
    for (int i=0; i<mat_b.getCol(); i++) {
        for (int j=0; j<4; j++) {
            float value = 0;
            for (int k=0; k<4; k++) {
                value += mat_a.get(j, k) * mat_b.get(k, i);
            }
            res.set(j, i, value);
        }
    }

    return res;
}

Mat4xn GetResizeMat(const Vec3& resize) {
    Mat4xn resizeMat(4);
    resizeMat.set(0, 0, resize.x);
    resizeMat.set(1, 1, resize.y);
    resizeMat.set(2, 2, resize.z);
    resizeMat.set(3, 3, 1);
    return resizeMat;
}

Mat4xn GetRotateMat(const Vec3& theta) {
    Mat4xn rotateX(4);
    Mat4xn rotateY(4);
    Mat4xn rotateZ(4);

    float sin_x = std::sinf(theta.x);
    float cos_x = std::cosf(theta.x);
    rotateX.set(0, 0, 1);
    rotateX.set(1, 1, cos_x);
    rotateX.set(2, 2, cos_x);
    rotateX.set(1, 2, -sin_x);
    rotateX.set(2, 1, sin_x);
    rotateX.set(3, 3, 1);

    float sin_y = std::sinf(theta.y);
    float cos_y = std::cosf(theta.y);
    rotateY.set(0, 0, cos_y);
    rotateY.set(1, 1, 1);
    rotateY.set(2, 2, cos_y);
    rotateY.set(0, 2, sin_y);
    rotateY.set(2, 0, -sin_y);
    rotateY.set(3, 3, 1);
    
    float sin_z = std::sinf(theta.z);
    float cos_z = std::cosf(theta.z);
    rotateZ.set(0, 0, cos_z);
    rotateZ.set(1, 1, cos_z);
    rotateZ.set(2, 2, 1);
    rotateZ.set(0, 1, -sin_x);
    rotateZ.set(1, 0, sin_x);
    rotateZ.set(3, 3, 1);

    Mat4xn rotateXY = MatMul(rotateX, rotateY);
    Mat4xn rotateXYZ = MatMul(rotateXY, rotateZ);
    return rotateXYZ;
}

Mat4xn GetMoveMat(const Vec3& move) {
    Mat4xn mvMat(4);
    mvMat.set(0, 3, move.x);
    mvMat.set(1, 3, move.y);
    mvMat.set(2, 3, move.z);
    mvMat.set(1, 1, 1);
    mvMat.set(2, 2, 1);
    mvMat.set(3, 3, 1);
    return mvMat;
}

Mat4xn GetModelTransMat(const Vec3& resize, const Vec3& rotate, const Vec3& move) {
    Mat4xn resizeMat = GetResizeMat(resize);
    Mat4xn rotateMAt = GetRotateMat(rotate);
    Mat4xn moveMat = GetMoveMat(move);
    Mat4xn mr = MatMul(moveMat, rotateMAt);
    Mat4xn mrr = MatMul(mr, resizeMat);
    return mrr;
}

Mat4xn GetViewTransMat() {

}

Mat4xn GetProjectionTransMat() {

}

Mat4xn MVPTrans() {

}