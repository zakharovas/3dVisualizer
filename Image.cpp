//
// Created by User on 13.04.2016.
//

#include <cassert>
#include "Image.h"

Image::Image(unsigned int height, unsigned int width) {
    std::vector<Color> empty(width);
    image_.assign(height, empty);
}

Color Image::GetPixel(unsigned int x, unsigned int y) const {
    assert(x < image_[0].size() && y < image_.size());
    return image_[y][x];
}

void Image::SetPixel(unsigned int x, unsigned int y, Color color_) {
    assert(x < image_[0].size() && y < image_.size());
    image_[y][x] = color_;
}





