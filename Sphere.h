//
// Created by User on 09.05.2016.
//

#ifndef INC_3DVISUALIZER_SPHERE_H
#define INC_3DVISUALIZER_SPHERE_H


#include "Primitive.h"

class Sphere : public Primitive {

public:
    Sphere(Point center, double radius);

    virtual double GetMinCoordinate(size_t number_of_coordinate) const override;

    virtual double GetMaxCoordinate(size_t number_of_coordinate) const override;


    virtual bool TryToIntersect(const Ray &ray) const override;

    virtual Point Intersect(const Ray &ray) const override;

    virtual Color GetColor(const Point &point, const Vector &direction) const override;

    virtual Vector GetNormal(const Point &point) const override;

    virtual Point GetImportantPoint() const override;

    virtual void Move(const Vector &vector) override;

    virtual void SetOutsideColor(const Color &color) override;

    virtual void SetMaterial(const Material &material) override {

    }

    virtual Material GetMaterial() const override {
        return Material("", Color(), 0, 0);
    }

private:
    Point center_;
    double radius_;
    Color outside_color_;
    Color inside_color_;

};


#endif //INC_3DVISUALIZER_SPHERE_H
