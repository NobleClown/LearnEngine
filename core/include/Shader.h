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
    virtual Vec3 fragment(const FSIn& in, const std::vector<Light>& lights, const Camera& camera, const Texture2D& tex) = 0;
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

    Vec3 fragment(const FSIn& in, const std::vector<Light>& lights, const Camera& camera, const Texture2D& tex) override
    {
        Vec3 albedo = tex.sample(in.uv);
        Vec3 norm = in.normal.normalize();
        Vec3 lightDir = (lights[0].position - in.worldPos);
        float r2 = lightDir.dotProduct(lightDir);
        lightDir = lightDir.normalize();

        Vec3 inverseLookat = (camera.position - in.worldPos).normalize();
        Vec3 HalfVec = (lightDir + inverseLookat).normalize();
        float k_spec = std::powf(std::max(0.f, HalfVec.dotProduct(norm)), 32) / r2 * 10;
        // float k_spec = std::powf(std::max(0.f, HalfVec.dotProduct(norm)), 32) / (1 + r2 / 10);
        float k_amb = 0.01f;
        float k_diff = std::max(norm.dotProduct(lightDir), 0.f) / r2;
        Vec3 res = albedo * k_amb + albedo * k_diff + albedo * k_spec;
        // float k_diff = std::max(norm.dotProduct(lightDir), 0.f) / (1 + r2 / 10);
        // Vec3 res = albedo * k_amb + albedo * k_diff + Vec3(0.5f, 0.5f, 0.5f) * k_spec;
        res.x = std::min(1.f, res.x);
        res.y = std::min(1.f, res.y);
        res.z = std::min(1.f, res.z);
        return res * 255;
    }
};