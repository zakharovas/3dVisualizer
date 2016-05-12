#include <iostream>
#include <cstdlib>
#include <limits>
#include "PixelToaster.h"
#include "Drawer.h"
#include "World.h"
#include "Triangle.h"
#include "Parallelogram.h"
#include "Sphere.h"
#include "StlReader.h"


int main(int argc, char *argv[]) {
    std::string filename = argv[1];
    StlReader reader(filename);
    Camera camera(Point(-100, 0, 200), Point(100, 0, 200), Point(-100, 0, 0), Point(100, 0, 0), Point(0, -300, 100));
    World perfect_world = reader.CreateWorld();
    perfect_world.SetCamera(camera);
//    Point point = Point(1, 1, 0);
//    Sphere sph1(point, 2);
//    Ray ray(Point(-1,-1,0), Vector(1,1,0));
//    sph1.TryToIntersect(ray);
//    srand(10);
    const int width = 800;
    const int height = 600;
//
//    Point p = Point(0, 1, 1).HorizontalRotation(-3.141592 / 4);
//    World perfect_world;
//    Point a(1, 0, 0);
//    Point b(0, 0, 0.4);
//    Point c(0, 1, 0);
//    Point d(0.5, 0.5, 1);
//    Vector v(0, 0, 2);
//    std::shared_ptr<Primitive> triangle(
//            new Triangle(a, b, d, Vector(-1, -1, 0)));
//    std::shared_ptr<Primitive> sph(
//            new Sphere(a, 0.5));
//    std::shared_ptr<Primitive> triangle2(
//            new Triangle(a, b, c, (c - a).CrossProduct(b - a) * -1));
//    std::shared_ptr<Primitive> triangle3(
//            new Triangle(a, c, d, (c - a).CrossProduct((d - a) * -1)));
//    std::shared_ptr<Primitive> triangle4(
//            new Triangle(b, c, d, (c - b).CrossProduct(d - b) * -1));
//    std::shared_ptr<Primitive> par1(new Parallelogram(a, c, c + v, a + v, Vector(-1, -1, 0)));
//    Parallelogram *aa = dynamic_cast<Parallelogram *>(par1.get());
//    aa->SetOutsideColor(Color(50, 0, 0));
//    sph->Move(Point(3, 3, 0));
//    triangle->Move(Point(3, 3, 0));
//    triangle2->Move(Point(3, 3, 0));
//    triangle3->Move(Point(3, 3, 0));
//    triangle4->Move(Point(3, 3, 0));
//    par1->Move(Point(3, 3, 0));
//    perfect_world.AddPrimitive(sph);
//    perfect_world.AddPrimitive(par1);
//    perfect_world.AddPrimitive(triangle);
//    perfect_world.AddPrimitive(triangle2);
//    perfect_world.AddPrimitive(triangle3);
//    perfect_world.AddPrimitive(triangle4);
    LightSource light(Point(0, -300, 100), 50000.5);
//
    LightSource light1(Point(7, 7, -10), 3000.5);
    perfect_world.AddLight(light1);
//    std::shared_ptr<Primitive> sphere(new Sphere(Point(3, 3, 0), 0.5));
//    perfect_world.AddPrimitive(sphere);
    perfect_world.AddLight(light);
    Image image = perfect_world.CreateImage(height, width);
    Drawer drawer(image);
    drawer.Draw();
    return 0;
}