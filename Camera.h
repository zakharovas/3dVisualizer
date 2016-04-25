//
// Created by User on 13.04.2016.
//

#ifndef INC_3DVISUALIZER_CAMERA_H
#define INC_3DVISUALIZER_CAMERA_H


#include "Point.h"

class Camera {
public:
    Camera();

    Camera(Point point, Vector vector, double horizontal_angle);


    double get_horizontal_angle() const {
        return horizontal_angle_;
    }

    double GetVerticalAngle(unsigned int height, unsigned int width);

//    double get_vertical_angle() const {
//        return vertical_angle_;
//    }

    Point GetPoint() const {
        return point_;
    }

    Vector GetVector() const {
        return vector_;
    }

private:
    double horizontal_angle_;
//    double vertical_angle_;
    Point point_;
    Vector vector_;

public:

};


#endif //INC_3DVISUALIZER_CAMERA_H
