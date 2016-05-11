//
// Created by User on 25.04.2016.
//

#ifndef INC_3DVISUALIZER_PARALLELOGRAM_H
#define INC_3DVISUALIZER_PARALLELOGRAM_H


#include "Primitive.h"

class Parallelogram : public Primitive {

public:
    Parallelogram(Point point1, Point point2, Point point3, Point point4, Vector normal);

    virtual bool TryToIntersect(const Ray &ray) const override;

    virtual Point Intersect(const Ray &ray) const override;

    virtual Color GetColor(const Point &point, const Vector &direction) const override;

    virtual Vector GetNormal(const Point &point) const override;

    virtual Point GetImportantPoint() const override;

    virtual void Move(const Vector &vector) override;

    virtual void SetInsideColor(const Color &color) override;

    virtual void SetOutsideColor(const Color &color) override;

    virtual double GetMinCoordinate(size_t number_of_coordinate) const override;

    virtual double GetMaxCoordinate(size_t number_of_coordinate) const override;


private:
    Point point1_;
    Point point2_;
    Point point3_;
    Point point4_;
    Vector normal_;
    Color inside_color_;
    Color outside_color_;
};


#endif //INC_3DVISUALIZER_PARALLELOGRAM_H
