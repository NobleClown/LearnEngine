#include <cmath>
#include <iostream>
#include "../include/Renderer.h"

void Renderer::Clear(const Color& c) {
    m_fb->Clear(c);
}

void Renderer::DrawTriangle(const Vertex& v0, const Vertex& v1, const Vertex& v2) {
    int width = m_fb->GetWidth();
    int height = m_fb->GetHeight();

    Vec3 p0 = NDC2Screen(v0.ndc, width, height);
    Vec3 p1 = NDC2Screen(v1.ndc, width, height);
    Vec3 p2 = NDC2Screen(v2.ndc, width, height);

    // 根据三个顶点的最值框定一个需要着色的矩形，将其中的三角形内部顶点进行着色
    int minX = (int)std::floor(std::min({p0.x, p1.x, p2.x}));
    int minY = (int)std::floor(std::min({p0.y, p1.y, p2.y}));
    int maxX = (int)std::floor(std::max({p0.x, p1.x, p2.x}));
    int maxY = (int)std::floor(std::max({p0.y, p1.y, p2.y}));

    float area = CrossProduct(p0, p1, p2);

    // 叉积为0表示三点共线，无内部顶点
    if (area == 0) {
        std::cout << "[Warning]: three points are in one line." << std::endl;
        return ;
    }

    // 开始着色
    for (int i=minY; i<=maxY; i++) {
        for (int j=minX; j<=maxX; j++) {
            // p为每个顶点的着色中心
            Vec3 p = {j + 0.5, i + 0.5, 0};
            // 计算顶点p在三个向量方向上的权重
            float w0 = CrossProduct(p1, p2, p);
            float w1 = CrossProduct(p2, p0, p);
            float w2 = CrossProduct(p0, p1, p);
            if (w0 >= 0 && w1 >= 0 && w2 >= 0) {
                // 权重归一化
                w0 /= area;
                w1 /= area;
                w2 /= area;
                
                // 插值着色
                Color c = InterpColor(v0.color, v1.color, v2.color, w0, w1, w2);

                float depth = v0.ndc.z * w0 + v1.ndc.z * w1 + v2.ndc.z * w2;
                m_fb->SetPixel(j, i, depth, c);
            }
        }
    }
}