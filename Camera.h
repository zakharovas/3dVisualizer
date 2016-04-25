//
// Created by User on 13.04.2016.
//

#ifndef INC_3DVISUALIZER_CAMERA_H
#define INC_3DVISUALIZER_CAMERA_H


#include "Point.h"

class Camera {
public:
    Camera();

    Camera(Point upper_left_corner_, Point upper_right_corner_, Point lower_left_corner_,
           Point lower_right_corner_, Point point_);

    Point get_upper_left_corner_() const {
        return upper_left_corner_;
    }

    Point get_upper_right_corner_() const {
        return upper_right_corner_;
    }

    Point get_lower_left_corner_() const {
        return lower_left_corner_;
    }

    Point get_lower_right_corner_() const {
        return lower_right_corner_;
    }

    Point get_point() const {
        return point_;
    }

private:
    Point upper_left_corner_;
    Point upper_right_corner_;
    Point lower_left_corner_;
    Point lower_right_corner_;
    Point point_;
};


#endif //INC_3DVISUALIZER_CAMERA_H
