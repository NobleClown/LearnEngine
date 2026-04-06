#include <unordered_map>
#include "../include/Model.h"

bool Model::LoadOBJ(const std::string& path) {
    std::ifstream obj_file(path);
    if (!obj_file.is_open()) {
        std::cout << "[INFO] Model Load Failed, File is Invalid." << std::endl;
        return false;
    }

    std::string line;

    std::vector<Vec3> positions;
    std::vector<Vec2> uvs;
    std::vector<Vec3> normals;

    std::vector<uint32_t> posi_idxes;
    std::vector<uint32_t> uv_idxes;
    std::vector<uint32_t> normal_idxes;

    while (std::getline(obj_file, line)) {
        if (line.empty() || line[0] == '#') continue;
        if (line[0] == 'v' && line[1] != 'n' && line[1] != 't') {
            Vec3 posi;
            std::stringstream ss(line);
            std::string type;
            ss >> type;
            ss >> posi.x >> posi.y >> posi.z;
            positions.emplace_back(posi);
        } else if (line.substr(0, 2) == "vt") {
            Vec2 uv;
            std::stringstream ss(line);
            std::string type;
            ss >> type;
            ss >> uv.x >> uv.y;
            uvs.emplace_back(uv);
        } else if (line.substr(0, 2) == "vn") {
            Vec3 normal;
            std::stringstream ss(line);
            std::string type;
            ss >> type;
            ss >> normal.x >> normal.y >> normal.z;
            normals.emplace_back(normal);
        } else if (line[0] == 'f') {
            uint32_t posi_idx0, posi_idx1, posi_idx2;
            uint32_t uv_idx0, uv_idx1, uv_idx2;
            uint32_t normal_idx0, normal_idx1, normal_idx2;
            sscanf(line.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d", &posi_idx0, &uv_idx0, &normal_idx0, 
                                                                 &posi_idx1, &uv_idx1, &normal_idx1, 
                                                                 &posi_idx2, &uv_idx2, &normal_idx2);
            // obj索引从1开始
            posi_idx0 -= 1;
            posi_idx1 -= 1;
            posi_idx2 -= 1;

            uv_idx0 -= 1;
            uv_idx1 -= 1;
            uv_idx2 -= 1;

            normal_idx0 -= 1;
            normal_idx1 -= 1;
            normal_idx2 -= 1;

            posi_idxes.emplace_back(posi_idx0);
            posi_idxes.emplace_back(posi_idx1);
            posi_idxes.emplace_back(posi_idx2);

            uv_idxes.emplace_back(uv_idx0);
            uv_idxes.emplace_back(uv_idx1);
            uv_idxes.emplace_back(uv_idx2);

            normal_idxes.emplace_back(normal_idx0);
            normal_idxes.emplace_back(normal_idx1);
            normal_idxes.emplace_back(normal_idx2);
        }
    }

    Mesh mesh;
    std::unordered_map<VertexKey, uint32_t, VertexKeyHash> vertexMap;
    for (int i=0; i<posi_idxes.size(); i++) {
        uint32_t posi_idx = posi_idxes[i];
        uint32_t uv_idx = uv_idxes[i];
        uint32_t normal_idx = normal_idxes[i];
        Vertex v;
        v.position = positions[posi_idx];
        v.uv = uvs[uv_idx];
        v.normal = normals[normal_idx];
        if (vertexMap.find({posi_idx, uv_idx, normal_idx}) != vertexMap.end()) {
            mesh.addIndex(vertexMap[{posi_idx, uv_idx, normal_idx}]);
        } else {
            mesh.addVertex(v);
            vertexMap[{posi_idx, uv_idx, normal_idx}] = mesh.getVerticies().size() - 1;
            mesh.addIndex(mesh.getVerticies().size() - 1);
        }
        
    }

    m_meshes.emplace_back(mesh);
    return true;
}