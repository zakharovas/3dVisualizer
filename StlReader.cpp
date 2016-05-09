//
// Created by User on 25.04.2016.
//

#include <cassert>
#include <iostream>
#include "StlReader.h"
#include "Triangle.h"

StlReader::StlReader(const std::string &name_of_file) {
    input_stream_.open(name_of_file.c_str(), std::ifstream::in);
    assert(input_stream_);
}

StlReader::~StlReader() {
    input_stream_.close();
}

World StlReader::CreateWorld() {
    World new_world;
    std::string s;
    std::getline(input_stream_, s);
    while (!input_stream_.eof()) {
        std::string name;
        input_stream_ >> name;
        if (name == "facet") {
            std::shared_ptr<Primitive> triangle = ReadTriangle_();
            new_world.AddPrimitive(triangle);
        }
        //TODO reader
//        if (name == "light") {
//            LightSource light = ReadLight_();
//            new_world.AddLight(light);
//        }
//        if (name == "sphere") {
//            std::shared_ptr<Primitive> sphere = ReadSphere_();
//            new_world.AddPrimitive(sphere);
//        }
//        if (name == "parallelogram") {
//            std::shared_ptr<Primitive> parallelogram = ReadParallelogram_();
//            new_world.AddPrimitive(parallelogram);
//        }
//        if (name == "camera") {
//            Camera camera = ReadCamera_();
//            new_world.SetCamera(camera);
//        }
        if (name == "endsolid") {
            break;
        }
    }
    std::cout << "READING COMPLETED";
    return new_world;
}

std::shared_ptr<Primitive> StlReader::ReadTriangle_() {
    std::string s;
    input_stream_ >> s;
    double nx;
    double ny;
    double nz;
    input_stream_ >> nx >> ny >> nz;
    std::vector<Point> points;
    Vector normal;
    Color outside_color = Color::kDefaultOutsideColor;
    Color inside_color = Color::kDefaultInsideColor;

    while (true) {
        std::string name;
        input_stream_ >> name;
        if (name == "endfacet") { break; }
        if (name == "outer") {
            points = ReadVertexes_();
            assert(points.size() == 3);
            normal = (points[1] - points[0]).CrossProduct(points[2] - points[0]);
            normal = normal / normal.Length();
        }
        if (name == "insidecolor") {
            int r, g, b;
            input_stream_ >> r >> g >> b;
            inside_color = Color(r, g, b);
        }

        if (name == "outsidecolor") {
            int r, g, b;
            input_stream_ >> r >> g >> b;
            outside_color = Color(r, g, b);
        }
    }
    std::shared_ptr<Primitive> object(new Triangle(points[0], points[1], points[2], normal));
    object->SetInsideColor(inside_color);
    object->SetOutsideColor(outside_color);
    return object;
}

std::vector<Point> StlReader::ReadVertexes_() {
    std::string s;
    input_stream_ >> s;
    std::vector<Point> points;
    while (true) {
        std::string name;
        input_stream_ >> name;
        assert(name == "endloop" || name == "vertex");
        if (name == "endloop") {
            return points;
        }
        if (name == "vertex") {
            double x, y, z;
            input_stream_ >> x >> y >> z;
            points.push_back(Point(x, y, z));
        }
    }
}













