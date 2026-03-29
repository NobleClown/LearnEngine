#include "../include/MathTypes.h"

Mat4xn MatMul(const Mat4xn& mat_a, const Mat4xn& mat_b) {
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

    // rotate with x
    float sin_x = std::sinf(theta.x);
    float cos_x = std::cosf(theta.x);
    rotateX.set(0, 0, 1);
    rotateX.set(1, 1, cos_x);
    rotateX.set(2, 2, cos_x);
    rotateX.set(1, 2, -sin_x);
    rotateX.set(2, 1, sin_x);
    rotateX.set(3, 3, 1);

    // rotate with y 因为 y = z \times x, 因此绕y轴的旋转角度为顺时针，即\theta为负数
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
    mvMat.set(0, 0, 1);
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

Mat4xn GetViewTransMat(const Vec3& eye, const Vec3& center, const Vec3& up) {
    Vec3 f = center - eye;
    f = f.normalize();
    Vec3 right = f.crossProduct(up);
    right = right.normalize();
    Vec3 u = right.crossProduct(f);
    u = u.normalize();
    Mat4xn viewMat(4);
    viewMat.set(0, 0, right.x);
    viewMat.set(0, 1, right.y);
    viewMat.set(0, 2, right.z);
    viewMat.set(0, 3, -right.dotProduct(eye));
    viewMat.set(1, 0, u.x);
    viewMat.set(1, 1, u.y);
    viewMat.set(1, 2, u.z);
    viewMat.set(1, 3, -u.dotProduct(eye));
    viewMat.set(2, 0, -f.x);
    viewMat.set(2, 1, -f.y);
    viewMat.set(2, 2, -f.z);
    viewMat.set(2, 3, f.dotProduct(eye));
    viewMat.set(3, 3, 1);
    return viewMat;
}

Mat4xn GetOrthoProjectionMat(float fov, float aspect, float near, float far) {
    Mat4xn orthoMat(4);
    float tanHalfFov = tan(fov / 2 * PI / 180.0f);
    float top = near * tanHalfFov;
    float bottom = -top;
    float right = top * aspect;
    float left = - right;
    orthoMat.set(0, 0, 2 / (right - left));
    orthoMat.set(1, 1, 2 / (top - bottom));
    orthoMat.set(2, 2, - 2 / (far - near));
    orthoMat.set(0, 3, - (right + left) / (right - left));
    orthoMat.set(1, 3, - (top + bottom) / (top - bottom));
    orthoMat.set(2, 3, - (far + near) / (far - near));
    orthoMat.set(3, 3, 1);
    return orthoMat;
}

Mat4xn GetPersp2OrthoMat(float n, float f) {
    Mat4xn persp2orthoMat(4);
    persp2orthoMat.set(0, 0, n);
    persp2orthoMat.set(1, 1, n);
    persp2orthoMat.set(2, 2, -(n + f));
    persp2orthoMat.set(2, 3, - 2 * n * f);
    persp2orthoMat.set(3, 2, -1);
    return persp2orthoMat;
}

Mat4xn GetProjectionTransMat(float fov, float aspect, float near, float far) {
    Mat4xn orthoMat = GetOrthoProjectionMat(fov, aspect, near, far);
    Mat4xn persp2OrthoMat = GetPersp2OrthoMat(near, far);
    Mat4xn projMat = MatMul(orthoMat, persp2OrthoMat);
    return projMat;
}

Mat4xn MVPTrans(const Vec3& resize, const Vec3& rotate, const Vec3& move,
                const Vec3& eye, const Vec3& center, const Vec3& up,
                float fov, float aspect, float near, float far,
                const Mat4xn& points) {
    Mat4xn modelMat = GetModelTransMat(resize, rotate, move);
    Mat4xn viewMat = GetViewTransMat(eye, center, up);
    Mat4xn projMat = GetProjectionTransMat(fov, aspect, near, far);
    Mat4xn orthoMat = GetOrthoProjectionMat(fov, aspect, near, far);
    Mat4xn mvpMat = MatMul(MatMul(projMat, viewMat), modelMat);
    Mat4xn newPoints = MatMul(mvpMat, points);
    // Mat4xn newPoints = MatMul(MatMul(viewMat, modelMat), points);
    return newPoints;
}