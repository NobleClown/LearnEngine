#include "../include/Renderer.h"

void Renderer::setFrameBuffer(FrameBuffer* fb) {
    framebuffer = fb;
    return ;
}

void Renderer::setCamera(Camera* cam) {
    camera = cam;
    return ;
}

void Renderer::setShader(Shader* sd) {
    shader = sd;
    return ;
}

void Renderer::render(const Scene& scene) {
    framebuffer->clearColor(PackRGBA({25, 25, 25}));
    framebuffer->clearDepth(10.f);

    for (const Object& obj : scene.objects) {
        Mat4 modelMat = obj.transform.getModelMatrix();
        drawModel(*(obj.model), modelMat, scene.lights);
    }
}

void Renderer::drawModel(const Model& model, const Mat4& modelMat, const std::vector<Light>& lights) {
    std::vector<Mesh> meshes = model.GetMeshes();
    for (const Mesh& mesh : meshes) {
        drawMesh(mesh, modelMat, lights);
    }
    
}

void Renderer::drawMesh(const Mesh& mesh, const Mat4& modelMat, const std::vector<Light>& lights) {
    Mat4 mvpMat = camera->getVPMatrix() * modelMat;
    Mat3 normalMat = Mat3(modelMat).getInverseMat().getTransposeMat();

    std::vector<Vertex> vertexBuffer = mesh.getVerticies();
    std::vector<uint32_t> indexBuffer = mesh.getIndexBuffer();

    for (int i=0; i<indexBuffer.size(); i+=3) {
        uint32_t idx0 = indexBuffer[i];
        uint32_t idx1 = indexBuffer[i+1];
        uint32_t idx2 = indexBuffer[i+2];
        VSOut out0 = shader->vertex(vertexBuffer[idx0], mvpMat, modelMat, normalMat);
        VSOut out1 = shader->vertex(vertexBuffer[idx1], mvpMat, modelMat, normalMat);
        VSOut out2 = shader->vertex(vertexBuffer[idx2], mvpMat, modelMat, normalMat);
        out0.clipPos = out0.clipPos * out0.invW;
        out1.clipPos = out1.clipPos * out1.invW;
        out2.clipPos = out2.clipPos * out2.invW;
        Material* mtr = mesh.getMaterial();
        Texture2D* tex = mtr->diffuseMap;
        drawTriangle(out0, out1, out2, lights, *tex);
    }
}

void Renderer::drawTriangle(const VSOut v0, const VSOut& v1, const VSOut& v2, const std::vector<Light>& lights, const Texture2D& tex) {
    int width = framebuffer->width;
    int height = framebuffer->height;

    Vec3 p0 = NDC2Screen({v0.clipPos.x, v0.clipPos.y, v0.clipPos.z});
    Vec3 p1 = NDC2Screen({v1.clipPos.x, v1.clipPos.y, v1.clipPos.z});
    Vec3 p2 = NDC2Screen({v2.clipPos.x, v2.clipPos.y, v2.clipPos.z});

    // 根据三个顶点的最值框定一个需要着色的矩形，将其中的三角形内部顶点进行着色
    int minX = (int)std::floor(std::min({p0.x, p1.x, p2.x}));
    int minY = (int)std::floor(std::min({p0.y, p1.y, p2.y}));
    int maxX = (int)std::floor(std::max({p0.x, p1.x, p2.x}));
    int maxY = (int)std::floor(std::max({p0.y, p1.y, p2.y}));

    float area = CrossProduct(p0, p1, p2);

    if (area == 0) {
        std::cout << "[Warning]: three points are in one line." << std::endl;
        return;
    }

    // 开始着色
    for (int i=std::max(0, minY); i<=maxY && i<height; i++) {
        for (int j=std::max(0, minX); j<=maxX && j<width; j++) {
            // 获取着色中心
            Vec3 p_center = {j + 0.5, i + 0.5, 0};
            // 计算三角形三个顶点与着色中心围成的面积，当做权重(计算重心坐标)
            // 顶点的对边与p_center围成的面积作为该顶点的权重
            float w0 = CrossProduct(p1, p2, p_center);
            float w1 = CrossProduct(p2, p0, p_center);
            float w2 = CrossProduct(p0, p1, p_center);
            // 权重均>0表示在三角形内部
            if (w0 >= 0 && w1 >= 0 && w2 >= 0) {
                w0 /= area;
                w1 /= area;
                w2 /= area;
                float cur_depth = p0.z * w0 + p1.z * w1 + p2.z * w2;
                float depth = framebuffer->getDepth(j, i);
                if (depth <= cur_depth) 
                    continue;
                framebuffer->setDepth(j, i, cur_depth);
                FSIn fsIn;
                fsIn.worldPos = v0.worldPos * w0 + v1.worldPos * w1 + v2.worldPos * w2;
                fsIn.normal = v0.normal * w0 + v1.normal * w1 + v2.normal * w2;
                fsIn.uv = v0.uv * w0 + v1.uv * w1 + v2.uv * w2;
                
                Vec3 color = shader->fragment(fsIn, lights, *camera, tex);
                framebuffer->setPixel(j, i, PackRGBA(color));
            }
        }
    }
}