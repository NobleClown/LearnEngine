#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "MathTypes.h"

struct Triangle {
    uint32_t idx0, idx1, idx2;
};

struct Mesh {
    std::vector<Vertex> verticies;
    std::vector<Triangle> triangles;
};

class Model {
public:
    bool LoadOBJ(const std::string& path);
    std::vector<Vertex> GetVertexBuffer() const { return m_mesh.verticies; }
    std::vector<Triangle> GetIndexBuffer() const { return m_mesh.triangles; }
private:
    Mesh m_mesh;
};