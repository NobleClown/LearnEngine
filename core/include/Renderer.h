#pragma once
#include "../../mathtool/include/MathType.h"
#include "FrameBuffer.h"
#include "../../scene/include/Camera.h"
#include "Shader.h"
#include "../../scene/include/Scene.h"

class Renderer {
public:
    void setFrameBuffer(FrameBuffer* fb);
    void setCamera(Camera* cam);
    void setShader(Shader* shader);

    void render(const Scene& scene);

private:
    FrameBuffer* framebuffer;
    Camera* camera;
    Shader* shader;

private:
    void drawModel(const Model& model, const Mat4& modelMat, const std::vector<Light>& lights);

    void drawMesh(const Mesh& mesh, const Mat4& modelMat, const std::vector<Light>& lights);

    void drawTriangle(const VSOut v0, const VSOut& v1, const VSOut& v2, const Mat4& mvpMat, const std::vector<Light>& lights);

    void rasterizeTriangle(...);

    // viewport transform
    inline Vec3 NDC2Screen(const Vec3& ndc) {
        int width = framebuffer->width;
        int height = framebuffer->height;
        Vec3 out;
        out.x = (ndc.x * 0.5f + 0.5f) * (width - 1);
        out.y = (0.5f - ndc.y * 0.5f) * (height - 1);
        out.z = ndc.z;
        return out;
    }
};