//
// Created by User on 25.04.2016.
//

#include "StlReader.h"

StlReader::StlReader(const std::string &name_of_file) {
    input_stream_.open(name_of_file);
}

StlReader::~StlReader() {
    input_stream_.close();
}

World StlReader::CreateWorld() {
    World new_world;


    return new_world;
}









