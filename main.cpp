#include <iostream>
#include <cstdlib>
#include "PixelToaster.h"
#include "Drawer.h"
#include "World.h"
#include "Triangle.h"

using namespace PixelToaster;

int main() {
    srand(10);
    const int width = 1400;
    const int height = 600;
    Point point = Point(1, 1, 0);
    point = Point(0, 0, 0);
    Point p = Point(0, 1, 1).HorizontalRotation(-3.141592 / 4);
    World perfect_world;
    Point a(1, 0, 0);
    Point b(0, 0, 0.4);
    Point c(0, 1, 0);
    Point d(0.5, 0.5, 1);
    std::shared_ptr<Primitive> triangle(
            new Triangle(a, b, d, Vector(1, 1, 0)));
    std::shared_ptr<Primitive> triangle2(
            new Triangle(a, b, c, (c - a).CrossProduct(b - a)));
    std::shared_ptr<Primitive> triangle3(
            new Triangle(a, c, d, (c - a).CrossProduct((d - a))));
    std::shared_ptr<Primitive> triangle4(
            new Triangle(b, c, d, (c - b).CrossProduct(d - b)));
    triangle->Move(Point(3, 3, 0));
    triangle2->Move(Point(3, 3, 0));
    triangle3->Move(Point(3, 3, 0));
    triangle4->Move(Point(3, 3, 0));
    perfect_world.AddPrimitive(triangle);
    perfect_world.AddPrimitive(triangle2);
    perfect_world.AddPrimitive(triangle3);
    perfect_world.AddPrimitive(triangle4);
    LightSource light(Point(0, 0, 0), 5.5);

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