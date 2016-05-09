//
// Created by User on 09.05.2016.
//

#ifndef INC_3DVISUALIZER_SPHERE_H
#define INC_3DVISUALIZER_SPHERE_H


#include "Primitive.h"

class Sphere : public Primitive {

public:
    Sphere(Point center, double radius);

    virtual bool TryToIntersect(const Ray &ray) const override;

    virtual Point Intersect(const Ray &ray) const override;

    virtual Color GetColor(const Point &point, const Vector &direction) const override;

    virtual Vector GetNormal(const Point &point) const override;

    virtual Point GetImportantPoint() const override;

    virtual void Move(const Vector &vector) override;

    virtual void SetOutsideColor(const Color &color) override;

    virtual void SetInsideColor(const Color &color) override;


private:
    Point center_;
    double radius_;
    Color outside_color_;
    Color inside_color_;

};


#endif //INC_3DVISUALIZER_SPHERE_H
