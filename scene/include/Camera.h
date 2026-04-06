#pragma once
#include "../../mathtool/include/MathType.h"

class Camera {
public:
    Vec3 position;
    Vec3 target;
    Vec3 up;

    float fov;
    float aspect;
    float nearPlane;
    float farPlane;

public:
    /**
     * 输出：View矩阵
     */
    Mat4 getViewMat() const;


    Mat4 getOrthoMat() const;

    Mat4 getPersp2Othro() const;
    /**
     * 输出：Projection矩阵
     */
    Mat4 getProjectionMat() const;

    /**
     * 输出：VP矩阵
     */
    Mat4 getVPMatrix() const;
};