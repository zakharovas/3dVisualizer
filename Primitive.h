//
// Created by User on 12.04.2016.
//

#ifndef INC_3DVISUALIZER_PRIMITIVE_H
#define INC_3DVISUALIZER_PRIMITIVE_H


#include "Color.h"
#include "Ray.h"
#include "Point.h"

class Primitive {
public:
    virtual bool TryToIntersect(const Ray &ray) const = 0;

    virtual Point Intersect(const Ray &ray) const = 0;

    virtual Color getColor(const Point &point) const = 0;

};


#endif //INC_3DVISUALIZER_PRIMITIVE_H
