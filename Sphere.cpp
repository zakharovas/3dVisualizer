//
// Created by User on 09.05.2016.
//

#include <cassert>
#include <cmath>
#include <limits>
#include <algorithm>
#include <iostream>
#include "Sphere.h"


bool Sphere::TryToIntersect(const Ray &ray) const {
    Vector between_points = ray.get_point() - center_;
    double length_between_points = between_points.Length();
    double product = between_points.DotProduct(ray.get_vector());
    double direction_length = ray.get_vector().Length();
    double discriminant =
            pow(product, 2) - pow(direction_length, 2) * (pow(length_between_points, 2) - pow(radius_, 2));
    if (discriminant < Primitive::kAccuracy) {
        return false;
    }
    discriminant = sqrt(discriminant);
    double t1 = (-product - discriminant) / (pow(direction_length, 2));
    double t2 = (-product + discriminant) / (pow(direction_length, 2));
    if (t1 > Primitive::kAccuracy || t2 > Primitive::kAccuracy) {
        return true;
    }
    return false;
}

Point Sphere::Intersect(const Ray &ray) const {
    Vector between_points = ray.get_point() - center_;
    double length_between_points = between_points.Length();
    double product = between_points.DotProduct(ray.get_vector());
    double direction_length = ray.get_vector().Length();
    double discriminant =
            pow(product, 2) - pow(direction_length, 2) * (pow(length_between_points, 2) - pow(radius_, 2));
    assert (discriminant > Primitive::kAccuracy);
    discriminant = sqrt(discriminant);
    double t1 = (-product - discriminant) / (pow(direction_length, 2));
    double t2 = (-product + discriminant) / (pow(direction_length, 2));
    assert (t1 > Primitive::kAccuracy || t2 > Primitive::kAccuracy);
    if (t1 < Primitive::kAccuracy) {
        t1 = std::numeric_limits<double>::max();
    }
    if (t2 < Primitive::kAccuracy) {
        t2 = std::numeric_limits<double>::max();
    }
    double t = std::min(t1, t2);
    return ray.get_point() + ray.get_vector() * t;
}

Color Sphere::GetColor(const Point &point, const Vector &direction) const {
    Vector normal = point - center_;
    if (normal.DotProduct(direction) < 0) {
        return outside_color_;
    } else {
        return inside_color_;
    }
}

Vector Sphere::GetNormal(const Point &point) const {
    return (point - center_) / radius_;
}

Point Sphere::GetImportantPoint() const {
    return center_;
}

void Sphere::Move(const Vector &vector) {
    center_ = center_ + vector;
}

void Sphere::SetOutsideColor(const Color &color) {
    outside_color_ = color;
}

Sphere::Sphere(Point center, double radius) : center_(center), radius_(radius),
                                              inside_color_(Color::kDefaultInsideColor),
                                              outside_color_(Color::kDefaultOutsideColor) {
    assert(radius > Primitive::kAccuracy);

}

double Sphere::GetMinCoordinate(size_t number_of_coordinate) const {
    assert(number_of_coordinate < 3);
    switch (number_of_coordinate) {
        case 0:
            return center_.x - radius_;
            break;
        case 1:
            return center_.y - radius_;
            break;
        case 2:
            return center_.z - radius_;
            break;
        default:
            return -1;
            break;
    }
}

double Sphere::GetMaxCoordinate(size_t number_of_coordinate) const {
    assert(number_of_coordinate < 3);
    switch (number_of_coordinate) {
        case 0:
            return center_.x + radius_;
            break;
        case 1:
            return center_.y + radius_;
            break;
        case 2:
            return center_.z + radius_;
            break;
        default:
            return -1;
            break;
    }
}





















