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

    double DotProduct(const Point &point) const;

    Point CrossProduct(const Point &point) const;

    Point operator+(const Point &point) const;

    Point operator-(const Point &point) const;

    Point operator*(double number) const;

    Point operator/(double number) const;

    double Length() const;

};

typedef Point Vector;
#endif //INC_3DVISUALIZER_POINT_H
