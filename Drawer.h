//
// Created by User on 13.04.2016.
//

#ifndef INC_3DVISUALIZER_DRAWER_H
#define INC_3DVISUALIZER_DRAWER_H


#include "Image.h"

class Drawer {
public:
    Drawer(const Image &image) : image_(image) { };

    void Draw() const;

private:
    const Image image_;
};


#endif //INC_3DVISUALIZER_DRAWER_H
