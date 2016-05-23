//
// Created by User on 13.04.2016.
//

#ifndef INC_3DVISUALIZER_LIGHTSOURCE_H
#define INC_3DVISUALIZER_LIGHTSOURCE_H


#include "Point.h"

class LightSource {
public:
    LightSource(Point point, double intensity) : point_(point), intensity_(intensity) { };

    Point get_source() const { return point_; }

    double get_intensity() const { return intensity_; }

private:
    Point point_;
    double intensity_;
};


#endif //INC_3DVISUALIZER_LIGHTSOURCE_H
