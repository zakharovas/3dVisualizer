//
// Created by User on 12.04.2016.
//

#ifndef INC_3DVISUALIZER_WORLD_H
#define INC_3DVISUALIZER_WORLD_H


#include <bits/unique_ptr.h>
#include "Primitive.h"
#include "Image.h"
#include "LightSource.h"
#include "Camera.h"

class World {
public:
    World() : camera_(Camera()) { };

    void AddPrimitive(std::unique_ptr<Primitive> &&primitive);

    void AddLight(const LightSource &light);

    void SetCamera(const Camera &camera);

    Image CreateImage(unsigned int height, unsigned int width);

private:
    std::vector<std::unique_ptr<Primitive>> objects_;
    Camera camera_;
    std::vector<LightSource> lights_;

    std::vector<std::vector<Ray>> CalculateRays_(unsigned int height, unsigned int width);
};


#endif //INC_3DVISUALIZER_WORLD_H
