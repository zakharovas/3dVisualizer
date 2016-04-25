//
// Created by User on 12.04.2016.
//

#ifndef INC_3DVISUALIZER_POINT_H
#define INC_3DVISUALIZER_POINT_H


struct Point {
    double x;
    double y;
    double z;

    Point(double x_, double y_, double z_) : x(x_), y(y_), z(z_) { };

    Point(const Point &point_);

    Point();

    double DotProduct(const Point &point) const;

    Point CrossProduct(const Point &point) const;

    Point operator+(const Point &point) const;

    Point operator-(const Point &point) const;

    Point operator*(double number) const;

    Point operator/(double number) const;

    double Length() const;

    Point HorizontalRotation(double angle) const;

    Point VerticalRotation(double angle) const;



};

typedef Point Vector;
#endif //INC_3DVISUALIZER_POINT_H
