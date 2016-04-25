#include <iostream>
#include <cstdlib>
#include "PixelToaster.h"
#include "Drawer.h"
#include "World.h"
#include "Triangle.h"

using namespace PixelToaster;

int main() {
    srand(10);
    const int width = 10;
    const int height = 10;
    Point point = Point(1, 1, 0);
    point = Point(0, 0, 0);

    World perfect_world;
    std::shared_ptr<Primitive> triangle(
            new Triangle(Point(1, 0, 0), Point(0.2, 0.8, 0), Point(0.5, 0.5, 0.1), Vector(1, 1, 0)));
    perfect_world.AddPrimitive(triangle);
    LightSource light(Point(0, 0, 0), 0.5);
    perfect_world.AddLight(light);
    Image image = perfect_world.CreateImage(height, width);
    unsigned int index = 0;
//    for (int y = 0; y < height; ++y) {
//        for (int x = 0; x < width; ++x) {
//            Color cur(rand() % 256, rand() % 256, rand() % 256);
//
//            image.SetPixel(x, y, Color::kDefaultOutsideColor);
//            ++index;
//        }
//    }
//    image = image.Smooth();
    Drawer drawer(image);
    drawer.Draw();
    return 0;
}