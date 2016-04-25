//
// Created by User on 19.04.2016.
//

#include <cmath>
#include "Triangle.h"

Triangle::Triangle(Point point1, Point point2, Point point3, Vector normal) : point1_(point1), point2_(point2),
                                                                              point3_(point3),
                                                                              normal_(normal / normal.Length()),
                                                                              inside_color_(Color::kDefaultInsideColor),
                                                                              outside_color_(
                                                                                      Color::kDefaultOutsideColor) { }

bool Triangle::TryToIntersect(const Ray &ray) const {
    if (fabs(ray.get_vector().DotProduct(normal_)) < Primitive::kAccuracy) {
        return false;
    }
}

Point Triangle::Intersect(const Ray &ray) const {
    return Point();
}

Color Triangle::GetColor(const Point &point, const Vector &direction) const {
    if (direction.DotProduct(normal_) > -Primitive::kAccuracy) {
        return outside_color_;
    } else {
        return inside_color_;
    }
}

Vector Triangle::GetNormal(const Point &point) const {
    return normal_;
}

Point Triangle::GetImportantPoint() const {
    return (point1_ + point2_ + point3_) / 3;
}

void Triangle::SetInsideColor(const Color &color) {
    inside_color_ = color;
}

void Triangle::SetOutsideColor(const Color &color) {
    outside_color_ = color;
}















