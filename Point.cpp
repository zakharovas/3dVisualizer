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

//Point::Point(const Point &point_) : x(point_.x), y(point_.y), z(point_.z) { }



Point Point::HorizontalRotation(double angle) const {
    double x_new = cos(angle) * x + sin(angle) * y;
    double y_new = -sin(angle) * x + cos(angle) * y;
    double z_new = z;
    return Point(x_new, y_new, z_new);
}

Point Point::VerticalRotation(double angle) const {
    double horizontal_angle = atan2(y, x);
    Point new_point(HorizontalRotation(horizontal_angle));
    double new_x = cos(angle) * new_point.x - sin(angle) * new_point.z;
    double new_y = 0;
    double new_z = sin(angle) * new_point.x + cos(angle) * new_point.z;
    return Point(new_x, new_y, new_z).HorizontalRotation(-horizontal_angle);
}

Point::Point() : Point(0, 0, 0) { }

bool Point::operator==(Point another_point) const {
    return (fabs(x - another_point.x) < 1e-9) && (fabs(y - another_point.y) < 1e-9) &&
           (fabs(z - another_point.z) < 1e-9);
}







