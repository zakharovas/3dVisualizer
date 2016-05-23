//
// Created by User on 10.05.2016.
//

#ifndef INC_3DVISUALIZER_RTREADER_H
#define INC_3DVISUALIZER_RTREADER_H


#include <string>
#include <fstream>
#include <map>
#include "../PrincipalClasses/World.h"
#include "../GeneralClasses/Material.h"

class RtReader {

public:

    RtReader(const std::string &name_of_file);

    World CreateWorld();

    ~RtReader();

private:
    std::ifstream input_stream_;
    std::map<std::string, Material> materials_;

    std::shared_ptr<Primitive> ReadTriangle_();

    Camera ReadCamera_();

    std::vector<LightSource> ReadLights_();

    std::vector<std::shared_ptr<Primitive>> ReadPrimitives_();

    void ReadMaterials_();

    std::shared_ptr<Primitive> ReadSphere_();

    std::vector<std::shared_ptr<Primitive>> ReadQuadrangle_();
};


#endif //INC_3DVISUALIZER_RTREADER_H
