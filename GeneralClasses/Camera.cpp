//
// Created by User on 13.04.2016.
//

#include <cmath>
#include <assert.h>
#include "Camera.h"
#include "../Primitives/Primitive.h"

const double kPi = 3.141592653589793238462643383279502884;

Camera::Camera(Point upper_left_corner_, Point upper_right_corner_, Point lower_left_corner_, Point lower_right_corner_,
               Point point_) : upper_left_corner_(upper_left_corner_),
                               upper_right_corner_(upper_right_corner_),
                               lower_left_corner_(lower_left_corner_),
                               lower_right_corner_(lower_right_corner_),
                               point_(point_) {
    Vector upper_horizontal = upper_right_corner_ - upper_left_corner_;
    Vector lower_horizontal = lower_right_corner_ - lower_left_corner_;
    Vector left_vertical = upper_left_corner_ - lower_left_corner_;
    Vector right_vertical = upper_right_corner_ - lower_right_corner_;
    assert(upper_horizontal.CrossProduct(lower_horizontal).Length() < Primitive::kAccuracy);
    assert(left_vertical.CrossProduct(right_vertical).Length() < Primitive::kAccuracy);
}

Camera::Camera() : Camera(Point(0, 1, 1), Point(1, 0, 1), Point(0, 1, 0), Point(1, 0, 0), Point(-1, -1, 0.5)) { }




