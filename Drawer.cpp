//
// Created by User on 13.04.2016.
//

#include <PixelToaster.h>
#include "Drawer.h"

void Drawer::Draw() const {
    unsigned int width = image_.get_width();
    unsigned int height = image_.get_height();
    PixelToaster::Display display("Ray Tracing", width, height, PixelToaster::Output::Default,
                                  PixelToaster::Mode::TrueColor);
    std::vector<PixelToaster::TrueColorPixel> pixels(width * height);
    while (display.open()) {
        unsigned int index = 0;
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                pixels[index].r = image_.GetPixel(x, y).GetR();
                pixels[index].g = image_.GetPixel(x, y).GetG();
                pixels[index].b = image_.GetPixel(x, y).GetB();
                ++index;
            }
        }
        display.update(pixels);
    }
}

