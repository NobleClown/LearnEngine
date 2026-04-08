#include "../include/Camera.h"

Mat4 Camera::getViewMat() const {
    Vec3 f = target - position;
    f = f.normalize();
    Vec3 right = f.crossProduct(up);
    right = right.normalize();
    Vec3 u = right.crossProduct(f);
    u = u.normalize();
    Mat4 viewMat;
    viewMat.set(0, 0, right.x);
    viewMat.set(0, 1, right.y);
    viewMat.set(0, 2, right.z);
    viewMat.set(0, 3, -right.dotProduct(position));
    viewMat.set(1, 0, u.x);
    viewMat.set(1, 1, u.y);
    viewMat.set(1, 2, u.z);
    viewMat.set(1, 3, -u.dotProduct(position));
    viewMat.set(2, 0, -f.x);
    viewMat.set(2, 1, -f.y);
    viewMat.set(2, 2, -f.z);
    viewMat.set(2, 3, f.dotProduct(position));
    viewMat.set(3, 3, 1);
    return viewMat;
}


Mat4 Camera::getOrthoMat() const {
    Mat4 orthoMat;
    float rad = fov / 2 * PI / 180.0f;
    float tanHalfFov = tan(fov / 2 * PI / 180.0f);
    float top = nearPlane * tanHalfFov;
    float bottom = -top;
    float right = top * aspect;
    float left = - right;
    orthoMat.set(0, 0, 2 / (right - left));
    orthoMat.set(1, 1, 2 / (top - bottom));
    orthoMat.set(2, 2, - 2 / (farPlane - nearPlane));
    orthoMat.set(0, 3, - (right + left) / (right - left));
    orthoMat.set(1, 3, - (top + bottom) / (top - bottom));
    orthoMat.set(2, 3, - (farPlane + nearPlane) / (farPlane - nearPlane));
    orthoMat.set(3, 3, 1);
    return orthoMat;
}

Mat4 Camera::getPersp2Othro() const {
    Mat4 persp2orthoMat;
    persp2orthoMat.set(0, 0, nearPlane);
    persp2orthoMat.set(1, 1, nearPlane);
    persp2orthoMat.set(2, 2, -(nearPlane + farPlane));
    persp2orthoMat.set(2, 3, - 2 * nearPlane * farPlane);
    persp2orthoMat.set(3, 2, -1);
    return persp2orthoMat;
}

Mat4 Camera::getProjectionMat() const {
    Mat4 orthoMat = getOrthoMat();
    Mat4 persp2OrthoMat = getPersp2Othro();
    Mat4 projMat = orthoMat * persp2OrthoMat;
    return projMat;
}

Mat4 Camera::getVPMatrix() const {
    Mat4 projMat = getProjectionMat();
    Mat4 viewMat = getViewMat();
    return projMat * viewMat;
}