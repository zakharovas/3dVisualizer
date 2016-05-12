//
// Created by User on 10.05.2016.
//

#ifndef INC_3DVISUALIZER_RTREADER_H
#define INC_3DVISUALIZER_RTREADER_H


#include <string>
#include <fstream>
#include <map>
#include "World.h"
#include "Material.h"

class RtReader {


    RtReader(const std::string &name_of_file);

    World CreateWorld();

    ~RtReader();

private:
    std::ifstream input_stream_;
    std::map<std::string, Material> materials;

    std::shared_ptr<Primitive> ReadTriangle_();

    std::vector<Point> ReadVertexes_();

    Camera ReadCamera_();
};


#endif //INC_3DVISUALIZER_RTREADER_H
