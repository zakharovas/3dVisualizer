//
// Created by User on 10.05.2016.
//

#ifndef INC_3DVISUALIZER_MATERIAL_H
#define INC_3DVISUALIZER_MATERIAL_H


#include <string>
#include "Color.h"

class Material {
public:
    Material(const Color &color, double reflect, double refract) : color(color), reflect(reflect), refract(refract) { }

    const Color get_color() const {
        return color;
    }

    double get_reflect() const {
        return reflect;
    }

    double get_refract() const {
        return refract;
    }


private:
    Color color;
    double reflect;
    double refract;
};


#endif //INC_3DVISUALIZER_MATERIAL_H
