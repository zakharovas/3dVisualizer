//
// Created by User on 12.04.2016.
//

#include "World.h"

void World::SetCamera(const Camera &camera) {
    camera_ = camera;
}

void World::AddLight(const LightSource &light) {
    lights_.push_back(light);
}


Image World::CreateImage(unsigned int height, unsigned int width) {
    Image image_without_anti_aliasing(height + 1, width + 1);
    std::vector<std::vector<Ray>> rays = CalculateRays_(height + 1, width + 1);
    return image_without_anti_aliasing.Smooth();
}

void World::AddPrimitive(std::unique_ptr<Primitive> &&primitive) {
    objects_.push_back(std::move(primitive));
}

std::vector<std::vector<Ray>> World::CalculateRays_(unsigned int height, unsigned int width) {
    std::vector<std::vector<Ray>> rays;
    for (unsigned int y = 0; y < height; ++y) {
        std::vector<Ray> rays_of_same_horizontal;
        for (unsigned int x = 0; x < width; ++x) {
            double horizontal_angle = -camera_.get_horizontal_angle() / 2
                                      + camera_.get_horizontal_angle() * x / (width - 1);
            double vertical_angle = camera_.get_vertical_angle() / 2
                                    - camera_.get_vertical_angle() * y / (height - 1);
            Vector rotated_vector = camera_.GetVector().HorizontalRotation(horizontal_angle);
            rotated_vector = rotated_vector.VerticalRotation(vertical_angle);
            rays_of_same_horizontal.push_back(Ray(camera_.GetPoint(), rotated_vector));
        }
        rays.push_back(rays_of_same_horizontal);
    }
    return rays;
}













