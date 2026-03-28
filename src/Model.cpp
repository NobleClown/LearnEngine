#include "../include/Model.h"

bool Model::LoadOBJ(const std::string& path) {
    std::ifstream obj_file(path);
    if (!obj_file.is_open()) {
        std::cout << "[INFO] Model Load Failed, File is Invalid." << std::endl;
        return false;
    }

    std::string line;
    while (std::getline(obj_file, line)) {
        std::stringstream ss(line);
        std::string type;
        ss >> type;

        if (type == "v") {
            Vertex v;
            ss >> v.ndc.x >> v.ndc.y >> v.ndc.z;
            m_mesh.verticies.emplace_back(v);
        } else if (type == "f") {
            Triangle tri;
            sscanf(line.c_str(), "f %d/%*d/%*d %d/%*d/%*d %d/%*d/%*d", &tri.idx0, &tri.idx1, &tri.idx2);
            // obj索引从1开始
            tri.idx0--;
            tri.idx1--;
            tri.idx2--;
            m_mesh.triangles.emplace_back(tri);
        }
    }
    return true;
}