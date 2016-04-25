//
// Created by User on 13.04.2016.
//

#include "Camera.h"

const double kPi = 3.141592653589793238462643383279502884;

Camera::Camera() : point_(Point(0, 0, 0)), vector_(Vector(1, 1, 0)), horizontal_angle_(2 * kPi / 3)/*,
                   vertical_angle_(2 * kPi / 3)*/ { }

double Camera::GetVerticalAngle(unsigned int height, unsigned int width) {
    return horizontal_angle_ / height * width;
}

Camera::Camera(Point point, Vector vector, double horizontal_angle) : point_(point), vector_(vector / vector.Length()),
                                                                      horizontal_angle_(horizontal_angle) { }





