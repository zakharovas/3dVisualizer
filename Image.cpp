//
// Created by User on 13.04.2016.
//

#include <cassert>
#include <cmath>
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

Image Image::Smooth() const {
    assert(get_height() > kAntiAliasingSize && get_width() > kAntiAliasingSize);
    Image new_image(get_height() - kAntiAliasingSize, get_width() - kAntiAliasingSize);
    for (unsigned int y = 0; y < new_image.get_height(); ++y) {
        for (unsigned int x = 0; x < new_image.get_width(); ++x) {
            new_image.SetPixel(x, y, SmoothColor_(x, y));
        }
    }
    return new_image;
}

Color Image::SmoothColor_(unsigned int x, unsigned int y) const {
    double red = 0;
    double green = 0;
    double blue = 0;
    for (int dx = 0; dx < kAntiAliasingSize; ++dx) {
        for (int dy = 0; dy < kAntiAliasingSize; dy++) {
            red += image_[y + dy][x + dx].get_r();
            green += image_[y + dy][x + dx].get_g();
            blue += image_[y + dy][x + dx].get_b();
        }
    }
    red /= pow((kAntiAliasingSize + 1), 2);
    green /= pow((kAntiAliasingSize + 1), 2);
    blue /= pow((kAntiAliasingSize + 1), 2);
    return Color(red, green, blue);
}









