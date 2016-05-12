//
// Created by User on 12.04.2016.
//

#ifndef INC_3DVISUALIZER_WORLD_H
#define INC_3DVISUALIZER_WORLD_H


#include <memory>
#include "Primitive.h"
#include "Image.h"
#include "LightSource.h"
#include "Camera.h"
#include "KdTree.h"

class World {
public:
    World() : camera_(Camera()) { };

    void AddPrimitive(std::shared_ptr<Primitive> primitive);

    void AddLight(const LightSource &light);

    void SetCamera(const Camera &camera);

    Image CreateImage(unsigned int height, unsigned int width);

private:
    std::vector<std::shared_ptr<Primitive>> objects_;
    Camera camera_;
    std::vector<LightSource> lights_;
    std::shared_ptr<KdTree> tree;

    std::vector<std::vector<Ray>> CalculateRays_(unsigned int height, unsigned int width);

    Color CalculateColor_(const Ray &ray, size_t depth);

    struct PointWithNumber {
        Point point;
        std::shared_ptr<Primitive> object;
        double distance;
    };

    Color CalculateLight_(const std::shared_ptr<Primitive> &object, const Ray &ray, const Color &basic_color_rgb);

    Color Reflect_(std::shared_ptr<Primitive> object, const Ray &ray, size_t depth, double reflect);
};


#endif //INC_3DVISUALIZER_WORLD_H
