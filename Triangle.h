//
// Created by User on 19.04.2016.
//

#ifndef INC_3DVISUALIZER_TRIANGLE_H
#define INC_3DVISUALIZER_TRIANGLE_H


#include "Primitive.h"

class Triangle : public Primitive {
public:
    Triangle(Point point1, Point point2, Point point3, Vector normal);

    virtual bool TryToIntersect(const Ray &ray) const override;

    virtual Point Intersect(const Ray &ray) const override;

    virtual Color GetColor(const Point &point, const Vector &direction) const override;

    virtual Vector GetNormal(const Point &point) const override;

    virtual Point GetImportantPoint() const override;

private:
    Point point1_;
    Point point2_;
    Point point3_;
    Vector normal_;
};


#endif //INC_3DVISUALIZER_TRIANGLE_H
