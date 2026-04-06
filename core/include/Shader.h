#pragma once
#include "../../mathtool/include/MathType.h"
#include "../../scene/include/Scene.h"
#include "../../scene/include/Camera.h"

struct VSOut {
    Vec4 clipPos;     // 裁剪空间
    Vec3 worldPos;    // 用于光照
    Vec3 normal;      // 用于光照
    Vec2 uv;

    float invW;       // 透视校正关键
};

struct FSIn {
    Vec3 worldPos;
    Vec3 normal;
    Vec2 uv;
};

class Shader {
public:
    virtual VSOut vertex(const Vertex&, const Mat4& MVP, const Mat4& modelMat, const Mat3& normalMat) = 0;
    virtual Vec3 fragment(const FSIn&, const std::vector<Light>& lights, const Camera& camera) = 0;
};

class SimpleShader : public Shader {
public:
    VSOut vertex(const Vertex& v,const Mat4& MVP, const Mat4& modelMat, const Mat3& normalMat)
    {
        VSOut out;
        Vec4 worldPosi = modelMat * Vec4{v.position, 1};
        out.worldPos = {worldPosi.x / worldPosi.w, worldPosi.y / worldPosi.w, worldPosi.z / worldPosi.w};
        out.normal = (normalMat * v.normal).normalize();
        out.uv = v.uv;
        out.clipPos = MVP * Vec4(v.position, 1);
        out.invW = 1.0f / out.clipPos.w;
        return out;
    }

    Vec3 fragment(FSIn& in, const std::vector<Light>& lights, const Camera& camera)
    {
        Vec3 norm = in.normal.normalize();
        Vec3 lightDir = lights[0].position - in.worldPos;
        float r2 = lightDir.dotProduct(lightDir);

        Vec3 inverseLookat = (camera.position - in.worldPos).normalize();
        Vec3 HalfVec = (lightDir + inverseLookat).normalize();

        
        return Vec3(1, 0, 0); // 红色
    }
};