//
// Created by User on 10.05.2016.
//

#include <assert.h>
#include <ios>
#include <iostream>
#include <algorithm>
#include "RtReader.h"
#include "Triangle.h"
#include "Sphere.h"

RtReader::RtReader(const std::string &name_of_file) {
    input_stream_.open(name_of_file.c_str(), std::ifstream::in);
    assert(input_stream_);
}

RtReader::~RtReader() {
    input_stream_.close();
}

World RtReader::CreateWorld() {
    World new_world;
    std::string s;
    while (!input_stream_.eof()) {
        std::string name;
        input_stream_ >> name;
//        std::cout << name;
        if (name == "geometry") {
            for (std::shared_ptr<Primitive> object: ReadPrimitives_()) {
                new_world.AddPrimitive(object);
            }
        }
        if (name == "lights") {
            for (LightSource &light: ReadLights_()) {
                new_world.AddLight(light);
            }
        }
        if (name == "materials") {
            ReadMaterials_();
        }
        if (name == "viewport") {
            Camera camera = ReadCamera_();
            new_world.SetCamera(camera);
        }
        if (name == "endsolid") {
            break;
        }
    }
    std::cout << "READING COMPLETED" << std::endl;
    return new_world;
}


Camera RtReader::ReadCamera_() {
    std::string s;
    input_stream_ >> s;
    double x, y, z;
    input_stream_ >> x >> y >> z;
    Point origin(x, y, z);
    input_stream_ >> s;
    input_stream_ >> x >> y >> z;
    Point top_left(x, y, z);
    input_stream_ >> s;
    input_stream_ >> x >> y >> z;
    Point bottom_left(x, y, z);
    input_stream_ >> s;
    input_stream_ >> x >> y >> z;
    Point top_right(x, y, z);
    Point bottom_right = bottom_left + (top_right - top_left);
    input_stream_ >> s;
    return Camera(top_left, top_right, bottom_left, bottom_right, origin);
}

std::vector<LightSource> RtReader::ReadLights_() {
    std::string s;
    input_stream_ >> s;
    input_stream_ >> s;
    double power;
    input_stream_ >> power;
    input_stream_ >> s;
    double distance;
    input_stream_ >> distance;
    double correction = 0.5 * distance * distance / power;
    std::string name;
    input_stream_ >> name;
    input_stream_ >> name;
    std::vector<LightSource> lights;
    while (name == "point") {
        input_stream_ >> s;
        double x;
        double y;
        double z;
        input_stream_ >> x >> y >> z;
        Point point(x, y, z);
        input_stream_ >> s;
        double power;
        input_stream_ >> power;
        lights.push_back(LightSource(point, correction * power));
        input_stream_ >> name;
        input_stream_ >> name;
    }
    return lights;
}

std::vector<std::shared_ptr<Primitive>> RtReader::ReadPrimitives_() {
    std::string name;
    input_stream_ >> name;
    std::vector<std::shared_ptr<Primitive>> objects;
    while (!(name == "endgeometry")) {
        if (name == "sphere") {
            objects.push_back(ReadSphere_());
        }
        if (name == "triangle") {
            objects.push_back(ReadTriangle_());
        }
        if (name == "quadrangle") {
            std::vector<std::shared_ptr<Primitive>> read_objects = ReadQuadrangle_();
            for (auto object: read_objects) {
                objects.push_back(object);
            }
        }
        input_stream_ >> name;
    }
    return objects;
}

void RtReader::ReadMaterials_() {
    std::string name;
    input_stream_ >> name;
    while (name == "entry") {
        std::string s;
        input_stream_ >> s;
        std::string material_name;
        input_stream_ >> material_name;
        input_stream_ >> s;
        double b;
        double r;
        double g;
        input_stream_ >> r >> g >> b;
        Color color(r, g, b);
        double reflect;
        double refract;
        input_stream_ >> s >> reflect;
        input_stream_ >> s >> refract;
        Material material(color, reflect, refract);
        materials_.insert(std::make_pair(material_name, material));
        input_stream_ >> name;
        input_stream_ >> name;
    }
}

std::shared_ptr<Primitive> RtReader::ReadSphere_() {
    std::string s;
    input_stream_ >> s;
    double x;
    double y;
    double z;
    input_stream_ >> x >> y >> z;
    Point center(x, y, z);
    input_stream_ >> s;
    double r;
    input_stream_ >> r;
    input_stream_ >> s;
    std::string name;
    input_stream_ >> name;
    Material material = materials_.find(name)->second;
    std::shared_ptr<Primitive> sphere(new Sphere(center, r));
    sphere->SetMaterial(material);
    input_stream_ >> s;
    return sphere;
}

std::vector<std::shared_ptr<Primitive>> RtReader::ReadQuadrangle_() {
    std::string s;
    std::vector<Point> points;
    for (size_t i = 0; i < 4; ++i) {
        double x;
        double y;
        double z;
        input_stream_ >> s;
        input_stream_ >> x >> y >> z;
        points.push_back(Point(x, y, z));
    }
    for (size_t i = 0; i < 4; ++i) {
        Vector from_2_to_0 = points[2] - points[0];
        Vector form_1_to_0 = points[1] - points[0];
        Vector form_3_to_0 = points[3] - points[0];
        Vector direction_0 = form_1_to_0.CrossProduct(from_2_to_0);
        Vector direction_1 = form_3_to_0.CrossProduct(from_2_to_0);
        if (direction_0.DotProduct(direction_1) < -Primitive::kAccuracy) {
            break;
        }
        std::rotate(points.begin(), points.begin() + 1, points.end());
    }
    Vector normal = (points[1] - points[0]).CrossProduct(points[2] - points[0]);
    normal = normal / normal.Length();
    std::vector<std::shared_ptr<Primitive>> halfs;

    input_stream_ >> s;
    std::string name;
    input_stream_ >> name;
    Material material = materials_.find(name)->second;
    std::shared_ptr<Primitive> first_half(new Triangle(points[0], points[1], points[2], normal));
    first_half->SetMaterial(material);
    halfs.push_back(first_half);
    std::shared_ptr<Primitive> second_half(new Triangle(points[0], points[2], points[3], normal));
    second_half->SetMaterial(material);
    halfs.push_back(second_half);
    input_stream_ >> s;
    return halfs;
}

std::shared_ptr<Primitive> RtReader::ReadTriangle_() {
    std::string s;
    input_stream_ >> s;
    double x;
    double y;
    double z;
    input_stream_ >> x >> y >> z;
    std::vector<Point> points;
    points.push_back(Point(x, y, z));
    input_stream_ >> s;
    input_stream_ >> x >> y >> z;
    points.push_back(Point(x, y, z));
    input_stream_ >> s;
    input_stream_ >> x >> y >> z;
    points.push_back(Point(x, y, z));
    input_stream_ >> s;
    Vector normal = (points[1] - points[0]).CrossProduct(points[2] - points[0]);
    normal = normal / normal.Length();
    std::string name;
    input_stream_ >> name;
    Material material = materials_.find(name)->second;
    std::shared_ptr<Primitive> triangle(new Triangle(points[0], points[1], points[2], normal));
    triangle->SetMaterial(material);
    input_stream_ >> s;
    return triangle;
}




















