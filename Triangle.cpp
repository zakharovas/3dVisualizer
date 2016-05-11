//
// Created by User on 19.04.2016.
//

#include <cmath>
#include <assert.h>
#include <vector>
#include <algorithm>
#include "Triangle.h"

Triangle::Triangle(Point point1, Point point2, Point point3, Vector normal) : point1_(point1), point2_(point2),
                                                                              point3_(point3),
                                                                              normal_(normal / normal.Length()),
                                                                              inside_color_(Color::kDefaultInsideColor),
                                                                              outside_color_(
                                                                                      Color::kDefaultOutsideColor) { }

//Moeller-Trumbore intersection algorithm
bool Triangle::TryToIntersect(const Ray &ray) const {
    if (fabs(ray.get_vector().DotProduct(normal_)) < Primitive::kAccuracy) {
        return false;
    }
    Vector edge1 = point2_ - point1_;
    Vector edge2 = point3_ - point1_;
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
    if (second_coordinate + first_coordinate > 1 + Primitive::kAccuracy) {
        return false;
    }
    double ray_coordinate = edge2.DotProduct(q) * determinant;
    if (ray_coordinate < Primitive::kAccuracy) {
        return false;
    }
    return true;
}

Point Triangle::Intersect(const Ray &ray) const {
    assert(fabs(ray.get_vector().DotProduct(normal_)) > Primitive::kAccuracy);
    Vector edge1 = point2_ - point1_;
    Vector edge2 = point3_ - point1_;
    Vector p = ray.get_vector().CrossProduct(edge2);
    double determinant = 1 / edge1.DotProduct(p);
    Vector from_origin_to_vertex = ray.get_point() - point1_;
    double first_coordinate = from_origin_to_vertex.DotProduct(p) * determinant;
    assert (first_coordinate > -Primitive::kAccuracy && first_coordinate < 1 + Primitive::kAccuracy);
    Vector q = from_origin_to_vertex.CrossProduct(edge1);
    double second_coordinate = ray.get_vector().DotProduct(q) * determinant;
    assert(second_coordinate > -Primitive::kAccuracy && second_coordinate < 1 + Primitive::kAccuracy);
    assert(first_coordinate + second_coordinate < 1 + Primitive::kAccuracy);
    double ray_coordinate = edge2.DotProduct(q) * determinant;
    assert(ray_coordinate > Primitive::kAccuracy);
    return ray.get_point() + ray.get_vector() * ray_coordinate;
}

Color Triangle::GetColor(const Point &point, const Vector &direction) const {
    if (direction.DotProduct(normal_) < Primitive::kAccuracy) {
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

void Triangle::Move(const Vector &vector) {
    point1_ = point1_ + vector;
    point2_ = point2_ + vector;
    point3_ = point3_ + vector;
}

double Triangle::GetMinCoordinate(size_t number_of_coordinate) const {
    assert(number_of_coordinate < 3);
    std::vector<double> numbers;
    if (number_of_coordinate == 0) {
        numbers.push_back(point1_.x);
        numbers.push_back(point2_.x);
        numbers.push_back(point3_.x);
    }
    if (number_of_coordinate == 1) {
        numbers.push_back(point1_.y);
        numbers.push_back(point2_.y);
        numbers.push_back(point3_.y);
    }
    if (number_of_coordinate == 2) {
        numbers.push_back(point1_.z);
        numbers.push_back(point2_.z);
        numbers.push_back(point3_.z);
    }
    return *std::min_element(numbers.begin(), numbers.end());
}

double Triangle::GetMaxCoordinate(size_t number_of_coordinate) const {
    assert(number_of_coordinate < 3);
    std::vector<double> numbers;
    if (number_of_coordinate == 0) {
        numbers.push_back(point1_.x);
        numbers.push_back(point2_.x);
        numbers.push_back(point3_.x);
    }
    if (number_of_coordinate == 1) {
        numbers.push_back(point1_.y);
        numbers.push_back(point2_.y);
        numbers.push_back(point3_.y);
    }
    if (number_of_coordinate == 2) {
        numbers.push_back(point1_.z);
        numbers.push_back(point2_.z);
        numbers.push_back(point3_.z);
    }
    return *std::max_element(numbers.begin(), numbers.end());
}





















