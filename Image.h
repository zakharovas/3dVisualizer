//
// Created by User on 13.04.2016.
//

#ifndef INC_3DVISUALIZER_IMAGE_H
#define INC_3DVISUALIZER_IMAGE_H


#include <vector>
#include "Color.h"

class Image {
public:

    Image(unsigned int height, unsigned int width);

    unsigned int get_height() const { return image_.size(); }

    unsigned int get_width() const { return image_[0].size(); }

    Color GetPixel(unsigned int x, unsigned int y) const;

    void SetPixel(unsigned int x, unsigned int y, Color color_);


private:
    std::vector<std::vector<Color>> image_;
};


#endif //INC_3DVISUALIZER_IMAGE_H
