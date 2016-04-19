//
// Created by User on 19.04.2016.
//

#include "Triangle.h"

Triangle::Triangle(Point point1, Point point2, Point point3, Vector normal) : point1_(point1), point2_(point2),
                                                                              point3_(point3), normal_(normal) { }

bool Triangle::TryToIntersect(const Ray &ray) const {
    return false;
}

Point Triangle::Intersect(const Ray &ray) const {
    return Point();
}

Color Triangle::GetColor(const Point &point, const Vector &direction) const {
    return Color();
}

Vector Triangle::GetNormal(const Point &point) const {
    return normal_;
}

Point Triangle::GetImportantPoint() const {
    return (point1_ + point2_ + point3_) / 3;
}











