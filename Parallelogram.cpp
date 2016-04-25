//
// Created by User on 25.04.2016.
//

#include <assert.h>
#include <cmath>
#include "Parallelogram.h"

Parallelogram::Parallelogram(Point point1, Point point2,
                             Point point3, Point point4, Vector normal) : point1_(point1),
                                                                          point2_(point2),
                                                                          point3_(point3),
                                                                          point4_(point4),
                                                                          normal_(normal / normal.Length()) {
    Vector horizontal_vector1 = point2_ - point1_;;
    Vector horizontal_vector2 = point3_ - point4_;
    Vector vertical_vector1 = point3_ - point2_;
    Vector vectical_vector2 = point4_ - point1_;
    assert((horizontal_vector2 - horizontal_vector1).Length() < Primitive::kAccuracy);
    assert((vectical_vector2 - vertical_vector1).Length() < Primitive::kAccuracy);
}

bool Parallelogram::TryToIntersect(const Ray &ray) const {
    if (fabs(ray.get_vector().DotProduct(normal_)) < Primitive::kAccuracy) {
        return false;
    }
    Vector edge1 = point2_ - point1_;
    Vector edge2 = point4_ - point1_;
    Vector p = ray.get_vector().CrossProduct(edge2);
    double determinant = 1 / edge1.DotProduct(p);
    Vector from_origin_to_vertex = ray.get_point() - point1_;
    double first_coordinate = from_origin_to_vertex.DotProduct(p) * determinant;
    if (first_coordinate < -Primitive::kAccuracy || first_coordinate > 1 + Primitive::kAccuracy) {
        return false;
    }
    Vector q = from_origin_to_vertex.CrossProduct(edge1);
    double second_coordinate = ray.get_vector().DotProduct(q) * determinant;
    if (second_coordinate < -Primitive::kAccuracy || second_coordinate > 1 + Primitive::kAccuracy) {
        return false;
    }
    double ray_coordinate = edge2.DotProduct(q) * determinant;
    if (ray_coordinate < Primitive::kAccuracy) {
        return false;
    }
    return true;
}

Point Parallelogram::Intersect(const Ray &ray) const {
    assert(fabs(ray.get_vector().DotProduct(normal_)) > Primitive::kAccuracy);
    Vector edge1 = point2_ - point1_;
    Vector edge2 = point4_ - point1_;
    Vector p = ray.get_vector().CrossProduct(edge2);
    double determinant = 1 / edge1.DotProduct(p);
    Vector from_origin_to_vertex = ray.get_point() - point1_;
    double first_coordinate = from_origin_to_vertex.DotProduct(p) * determinant;
    assert (first_coordinate > -Primitive::kAccuracy && first_coordinate < 1 + Primitive::kAccuracy);
    Vector q = from_origin_to_vertex.CrossProduct(edge1);
    double second_coordinate = ray.get_vector().DotProduct(q) * determinant;
    assert(second_coordinate > -Primitive::kAccuracy && second_coordinate < 1 + Primitive::kAccuracy);
    double ray_coordinate = edge2.DotProduct(q) * determinant;
    assert(ray_coordinate > Primitive::kAccuracy);
    return ray.get_point() + ray.get_vector() * ray_coordinate;
}

Color Parallelogram::GetColor(const Point &point, const Vector &direction) const {
    if (direction.DotProduct(normal_) > -Primitive::kAccuracy) {
        return outside_color_;
    } else {
        return inside_color_;
    }
}

Vector Parallelogram::GetNormal(const Point &point) const {
    return normal_;
}

Point Parallelogram::GetImportantPoint() const {
    return (point1_ + point2_ + point3_ + point4_) / 4;
}

void Parallelogram::Move(const Vector &vector) {
    point1_ = point1_ + vector;
    point2_ = point2_ + vector;
    point3_ = point3_ + vector;
    point4_ = point4_ + vector;
}

void Parallelogram::SetInsideColor(const Color &color) {
    inside_color_ = color;
}

void Parallelogram::SetOutsideColor(const Color &color) {
    outside_color_ = color;
}

















