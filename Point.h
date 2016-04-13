//
// Created by User on 12.04.2016.
//

#ifndef INC_3DVISUALIZER_POINT_H
#define INC_3DVISUALIZER_POINT_H


class Point {
public:

    const double x;
    const double y;
    const double z;

    Point(const double x_, const double y_, const double z_) : x(x_), y(y_), z(z_) { };

    Point(const Point &point_);

    double DotProduct(const Point &point) const;

    Point CrossProduct(const Point &point) const;

    Point operator+(const Point &point) const;

    Point operator-(const Point &point) const;

    Point operator*(double number) const;

    Point operator/(double number) const;

    double Length() const;

    Point operator=(const Point &point_);

};

typedef Point Vector;
#endif //INC_3DVISUALIZER_POINT_H
