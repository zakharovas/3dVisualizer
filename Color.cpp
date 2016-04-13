//
// Created by User on 12.04.2016.
//

#include <cmath>
#include "Color.h"

unsigned int Color::GetR() const {
    return static_cast<unsigned int>(round(r_));
}

unsigned int Color::GetG() const {
    return static_cast<unsigned int>(round(g_));
}

unsigned int Color::GetB() const {
    return static_cast<unsigned int>(round(b_));
}





