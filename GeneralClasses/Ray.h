//
// Created by User on 13.04.2016.
//

#ifndef INC_3DVISUALIZER_RAY_H
#define INC_3DVISUALIZER_RAY_H


#include <initializer_list>
#include "Point.h"

class Ray {
public:
    Ray(Point point, Vector vector) : point_(point), vector_(vector) { };

    Point get_point() const { return point_; }

    Vector get_vector() const { return vector_; }

private:
    Point point_;
    Vector vector_;
};


#endif //INC_3DVISUALIZER_RAY_H
