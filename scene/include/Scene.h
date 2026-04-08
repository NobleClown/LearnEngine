#pragma once
#include "Model.h"
#include "Transform.h"
#include "Material.h"
#include "Light.h"
struct Object {
    Model* model;
    Transform transform;
    Material* material;   // ⭐ 必须加
};

class Scene {
public:
    std::vector<Object> objects;
    std::vector<Light> lights;

public:
    void addObject(const Object& obj) { objects.push_back(obj); }
    void addLight(const Light& light) { lights.push_back(light); }
};