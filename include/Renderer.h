#pragma once

#include "FrameBuffer.h"
#include "MathTypes.h"

class Renderer {
public:
    Renderer(FrameBuffer* fb):m_fb(fb) {}

    void Clear(const Color& c);
    void DrawTriangle(const Vertex& v0, const Vertex& v1, const Vertex& v2);

private:
    FrameBuffer* m_fb;
};