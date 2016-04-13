//
// Created by User on 12.04.2016.
//

#include "World.h"

void World::SetCamera(Camera camera) {
    camera_ = camera;
}

void World::AddLight(LightSource &light) {
    lights_.push_back(light);
}


Image World::CreateImage(unsigned int height, unsigned int width) {
    Image image_without_anti_aliasing(height + 1, width + 1);
    return image_without_anti_aliasing.Smooth();
}

void World::AddPrimitive(std::unique_ptr<Primitive> &&primitive) {
    objects_.push_back(std::move(primitive));
}









