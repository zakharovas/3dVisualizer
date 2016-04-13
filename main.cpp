#include <iostream>
#include <cstdlib>
#include "PixelToaster.h"
#include "Drawer.h"

using namespace PixelToaster;

int main() {
    srand(10);
    const int width = 1400;
    const int height = 600;
    Image image(600, 1400);
    unsigned int index = 0;
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            Color cur(rand() % 256, rand() % 256, rand() % 256);

            image.SetPixel(x, y, cur);
            ++index;
        }
    }
    image = image.Smooth();
    Drawer drawer(image);
    drawer.Draw();
    return 0;
}