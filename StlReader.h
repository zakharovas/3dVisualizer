//
// Created by User on 25.04.2016.
//

#ifndef INC_3DVISUALIZER_STLREADER_H
#define INC_3DVISUALIZER_STLREADER_H


#include <string>
#include <fstream>
#include "World.h"

class StlReader {
public:


    StlReader(const std::string &name_of_file);

    World CreateWorld();

    ~StlReader();

private:
    std::ifstream input_stream_;

    std::shared_ptr<Primitive> ReadTriangle_();

    std::vector<Point> ReadVertexes_();
};


#endif //INC_3DVISUALIZER_STLREADER_H
