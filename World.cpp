//
// Created by User on 12.04.2016.
//

#include <limits>
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
    for (unsigned int y = 0; y < height + 1; ++y) {
        for (unsigned int x = 0; x < width + 1; ++x) {
            image_without_anti_aliasing.SetPixel(x, y, CalculateColor_(rays[y][x]));
        }
    }
    return image_without_anti_aliasing.Smooth();
}

void World::AddPrimitive(const std::shared_ptr<Primitive> &primitive) {
    objects_.push_back(std::move(primitive));
}

std::vector<std::vector<Ray>> World::CalculateRays_(unsigned int height, unsigned int width) {
    std::vector<std::vector<Ray>> rays;
    for (unsigned int y = 0; y < height; ++y) {
        std::vector<Ray> rays_of_same_horizontal;
        for (unsigned int x = 0; x < width; ++x) {
            double horizontal_angle = -camera_.get_horizontal_angle() / 2
                                      + camera_.get_horizontal_angle() * x / (width - 1);
            double vertical_angle = camera_.GetVerticalAngle(height, width) / 2
                                    - camera_.GetVerticalAngle(height, width) * y / (height - 1);
            Vector rotated_vector = camera_.GetVector().HorizontalRotation(horizontal_angle);
            rotated_vector = rotated_vector.VerticalRotation(vertical_angle);
            rays_of_same_horizontal.push_back(Ray(camera_.GetPoint(), rotated_vector));
        }
        rays.push_back(rays_of_same_horizontal);
    }
    return rays;
}

Color World::CalculateColor_(const Ray &ray) {
    auto best_primitive = FindClosestPrimitive(ray);
    return CalculateLight_(best_primitive, ray);
}

std::shared_ptr<Primitive> World::FindClosestPrimitive(const Ray &ray) {
    PointWithNumber closest_object;
    closest_object.distance = std::numeric_limits<double>::max();
    closest_object.object = std::shared_ptr<Primitive>(nullptr);
    for (auto primitive: objects_) {
        if (primitive->TryToIntersect(ray)) {
            Point point_of_intersection = primitive->Intersect(ray);
            double distance = (point_of_intersection - ray.get_point()).Length();
            if (distance < closest_object.distance) {
                closest_object.distance = distance;
                closest_object.object = primitive;
                closest_object.point = point_of_intersection;
            }
        }
    }
    return closest_object.object;
}

Color World::CalculateLight_(const std::shared_ptr<Primitive> &object, const Ray &ray) {
    if (object == nullptr) {
        return Color();
    }
    Point point_of_intersection = object->Intersect(ray);
    HslColor basic_color = object->GetColor(point_of_intersection, ray.get_vector()).ToHsl();
    basic_color.RemoveLight();
    Vector normal = object->GetNormal(point_of_intersection);
    if (ray.get_vector().DotProduct(normal) < 0) {
        normal = normal * -1;
    }
    for (LightSource &light: lights_) {
        Vector vector_from_light_to_object = point_of_intersection - light.get_source();
        double power_of_light = light.get_intensity();
        double angle = normal.DotProduct(vector_from_light_to_object);
        if (angle < 0) {
            continue;
        }
        power_of_light *= angle;
        power_of_light /= vector_from_light_to_object.Length();
        basic_color.AddLight(power_of_light);
    }
    return basic_color.ToRgb();
}