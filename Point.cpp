//
// Created by User on 12.04.2016.
//

#include <cassert>
#include <cmath>
#include "Point.h"

double Point::DotProduct(const Point &point_) const {
    return point_.x * x + point_.y * y + point_.z * z;
}

Point Point::CrossProduct(const Point &point_) const {
    double new_x = y * point_.z - point_.y * z;
    double new_y = z * point_.x - x * point_.z;
    double new_z = x * point_.y - y * point_.x;
    return Point{new_x, new_y, new_z};
}

Point Point::operator+(const Point &point_) const {
    return Point{x + point_.x, y + point_.y, z + point_.z};
}

Point Point::operator-(const Point &point_) const {
    return Point{x - point_.x, y - point_.y, z - point_.z};
}

Point Point::operator*(double number_) const {
    return Point{x * number_, y * number_, z * number_};
}

Point Point::operator/(double number_) const {
    assert(number_ != 0);
    return Point{x / number_, y / number_, z / number_};
}

double Point::Length() {
    return sqrt(x * x + y * y + z * z);
}
















