//
// Created by User on 13.04.2016.
//

#ifndef INC_3DVISUALIZER_CAMERA_H
#define INC_3DVISUALIZER_CAMERA_H


#include "Point.h"

class Camera {

private:
    double horizontal_angle_;
    double vertical_angle_;
    Point point_;
    Vector vector_;

};


#endif //INC_3DVISUALIZER_CAMERA_H
