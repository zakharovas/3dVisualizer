//
// Created by User on 12.04.2016.
//

#include <cassert>
#include <cmath>
#include "Point.h"

double Point::DotProduct(const Point &point) const {
    return point.x * x + point.y * y + point.z * z;
}

Point Point::CrossProduct(const Point &point) const {
    double new_x = y * point.z - point.y * z;
    double new_y = z * point.x - x * point.z;
    double new_z = x * point.y - y * point.x;
    return Point{new_x, new_y, new_z};
}

Point Point::operator+(const Point &point) const {
    return Point{x + point.x, y + point.y, z + point.z};
}

Point Point::operator-(const Point &point) const {
    return Point{x - point.x, y - point.y, z - point.z};
}

Point Point::operator*(double number) const {
    return Point{x * number, y * number, z * number};
}

Point Point::operator/(double number) const {
    assert(number != 0);
    return Point{x / number, y / number, z / number};
}

double Point::Length() const {
    return sqrt(x * x + y * y + z * z);
}

Point::Point(const Point &point_) : x(point_.x), y(point_.y), z(point_.z) {
}

Point Point::operator=(const Point &point_) {
    return Point(point_);
}























