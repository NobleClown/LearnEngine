#include "../../mathtool/include/MathType.h"

class Transform {
public:
    Vec3 position;
    Vec3 rotation;   // 欧拉角（先简单实现）
    Vec3 scale;

public:
    Mat4 getModelMatrix() const {
        Mat4 rotateMat = Mat4::getRotateMat(rotation);
        Mat4 scaleMat = Mat4::getScaleMat(scale);
        Mat4 transMat = Mat4::getTranslateMat(position);
        return transMat * rotateMat * scaleMat;
    }
};