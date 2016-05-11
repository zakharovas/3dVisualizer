//
// Created by User on 12.04.2016.
//

#ifndef INC_3DVISUALIZER_PRIMITIVE_H
#define INC_3DVISUALIZER_PRIMITIVE_H


#include <cstdlib>
#include "Color.h"
#include "Ray.h"
#include "Point.h"

class Primitive {
public:
    virtual bool TryToIntersect(const Ray &ray) const = 0;

    virtual Point Intersect(const Ray &ray) const = 0;

    virtual Color GetColor(const Point &point, const Vector &direction) const = 0;

    virtual Vector GetNormal(const Point &point) const = 0;

    virtual Point GetImportantPoint() const = 0;

    virtual double GetMinCoordinate(size_t number_of_coordinate) const = 0;

    virtual double GetMaxCoordinate(size_t number_of_coordinate) const = 0;

    virtual void Move(const Vector &vector) = 0;

    virtual void SetOutsideColor(const Color &color) = 0;

    virtual void SetInsideColor(const Color &color) = 0;


    static const double kAccuracy;
};


#endif //INC_3DVISUALIZER_PRIMITIVE_H
