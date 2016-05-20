//
// Created by User on 12.04.2016.
//

#include <limits>
#include <iostream>
#include <chrono>
#include <future>
#include <functional>
#include "World.h"
#include "KdTree.h"
#include "ThreadPool.h"

void World::SetCamera(const Camera &camera) {
    camera_ = camera;
}

void World::AddLight(const LightSource &light) {
    lights_.push_back(light);
}


Image World::CreateImage(unsigned int height, unsigned int width) {
    std::cout << "Building tree started" << std::endl;
    std::chrono::time_point<std::chrono::system_clock> start_of_building = std::chrono::system_clock::now();
    tree = std::shared_ptr<KdTree>(new KdTree(objects_));
    tree->BuildTree();
    std::chrono::time_point<std::chrono::system_clock> end_of_building = std::chrono::system_clock::now();
    long long int elapsed_seconds = std::chrono::duration_cast<std::chrono::milliseconds>
            (end_of_building - start_of_building).count();
    std::cout << "Kd Tree has been built in " << elapsed_seconds << " ms" << std::endl;
    std::cout << "Building image started" << std::endl;
    start_of_building = std::chrono::system_clock::now();
    Image image_without_anti_aliasing(height, width);
    std::vector<std::vector<Ray>> rays = CalculateRays_(height, width);
    ThreadPool thread_pool;
    std::vector<std::vector<std::future<Color>>> future_image(height);
    for (unsigned int y = 0; y < height; ++y) {
        for (unsigned int x = 0; x < width; ++x) {
            std::function<Color()> current_function = std::bind(&World::CalculateColor_, this, std::cref(rays[y][x]),
                                                                0);
            future_image[y].push_back(thread_pool.Submit(current_function));
        }
    }
    for (unsigned int y = 0; y < height; ++y) {
        for (unsigned int x = 0; x < width; ++x) {
            image_without_anti_aliasing.SetPixel(x, y, future_image[y][x].get());
        }
    }
    end_of_building = std::chrono::system_clock::now();
    elapsed_seconds = std::chrono::duration_cast<std::chrono::milliseconds>
            (end_of_building - start_of_building).count();
    std::cout << "Image has been built in " << elapsed_seconds << " ms" << std::endl;
    return image_without_anti_aliasing;
}

void World::AddPrimitive(std::shared_ptr<Primitive> primitive) {
    objects_.push_back(primitive);
}

std::vector<std::vector<Ray>> World::CalculateRays_(unsigned int height, unsigned int width) {
    std::vector<std::vector<Ray>> rays;
    Vector camera_horizontal_vector = camera_.get_lower_right_corner_() - camera_.get_lower_left_corner_();
    Vector camera_vertical_vector = camera_.get_lower_left_corner_() - camera_.get_upper_left_corner_();
    for (unsigned int y = 0; y < height; ++y) {
        std::vector<Ray> rays_of_same_horizontal;
        for (unsigned int x = 0; x < width; ++x) {
            Point end_of_direction_vector =
                    camera_.get_upper_left_corner_() + camera_horizontal_vector * x / (width - 1) +
                    camera_vertical_vector * y / (height - 1);
            rays_of_same_horizontal.push_back(Ray(camera_.get_point(), end_of_direction_vector - camera_.get_point()));
        }
        rays.push_back(rays_of_same_horizontal);
    }
    return rays;
}

Color World::CalculateColor_(const Ray &ray, size_t depth) {
    auto best_primitive = tree->FindIntersection(ray);
    if (best_primitive == nullptr) {
        return Color();
    }
    Point point_of_intersection = best_primitive->Intersect(ray);
    Color basic_color = best_primitive->GetColor(point_of_intersection, ray.get_vector());
    basic_color = CalculateLight_(best_primitive, ray, basic_color);
    if (depth < 10) {
        if (best_primitive->GetMaterial().get_reflect() > Primitive::kAccuracy) {
            Color reflex_color = Reflect_(best_primitive, ray, depth + 1, best_primitive->GetMaterial().get_reflect());
            basic_color = basic_color.Mix(reflex_color, best_primitive->GetMaterial().get_reflect());
        }
    }
    return basic_color;
}

Color World::CalculateLight_(const std::shared_ptr<Primitive> &object, const Ray &ray, const Color &basic_color_rgb) {
    Point point_of_intersection = object->Intersect(ray);

    HslColor basic_color = basic_color_rgb.ToHsl();
    Vector normal = object->GetNormal(point_of_intersection);
    if (ray.get_vector().DotProduct(normal) < 0) {
        normal = normal * -1;
    }
    double point_light = 0;
    basic_color.RemoveLight();
    basic_color.AddLight(1e-3);
    for (LightSource &light: lights_) {
        Vector vector_from_light_to_object = point_of_intersection - light.get_source();
        Ray light_ray(light.get_source(), vector_from_light_to_object);
        if (tree->FindIntersection(light_ray) == object) {
            Point real_intersection = object->Intersect(light_ray);
            if (!(real_intersection == point_of_intersection)) {
                continue;
            }
            double power_of_light = light.get_intensity();
            double angle =
                    normal.DotProduct(vector_from_light_to_object) / normal.Length() /
                    vector_from_light_to_object.Length();
            if (angle < 0) {
                continue;
            }
            power_of_light *= angle;
            power_of_light /= vector_from_light_to_object.Length() * vector_from_light_to_object.Length();
            point_light += power_of_light;
        }
    }
    basic_color.AddLight(point_light);
    return basic_color.ToRgb();
}

Color World::Reflect_(std::shared_ptr<Primitive> object, const Ray &ray, size_t depth, double reflect) {
    Point point = object->Intersect(ray);
    Vector normal = object->GetNormal(point);
    double product = -normal.DotProduct(ray.get_vector()) / normal.Length();
    normal = normal * product;
    Vector delta = ray.get_vector() + normal;
    Vector reflected = (ray.get_vector() * -1) + (delta * 2);
    Ray reflected_ray(point + (reflected * Primitive::kAccuracy), reflected);
    return CalculateColor_(reflected_ray, depth);
}

Image World::CreateImageWithAntialiasing(unsigned int height, unsigned int width) {
    Image image_without_antialiasing = CreateImage(2 * height, 2 * width);
    return image_without_antialiasing.Smooth();
}



