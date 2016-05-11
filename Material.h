//
// Created by User on 10.05.2016.
//

#ifndef INC_3DVISUALIZER_MATERIAL_H
#define INC_3DVISUALIZER_MATERIAL_H


#include <string>
#include "Color.h"

class Material {

private:
public:
    Material(const std::string &name, const Color &color, double reflect, double refract) : name(name), color(color),
                                                                                            reflect(reflect),
                                                                                            refract(refract) { }

    std::string get_name() const {
        return name;
    }

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
    std::string name;
    Color color;
    double reflect;
    double refract;
};


#endif //INC_3DVISUALIZER_MATERIAL_H
