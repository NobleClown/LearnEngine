#pragma once
#include "../../mathtool/include/MathType.h"

struct Material {
    Vec3 albedo;      // 基础颜色
    
    float kd;         // 漫反射系数
    float ks;         // 高光系数
    float shininess;  // 高光指数
};