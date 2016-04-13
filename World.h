//
// Created by User on 12.04.2016.
//

#ifndef INC_3DVISUALIZER_WORLD_H
#define INC_3DVISUALIZER_WORLD_H


#include "Primitive.h"
#include "Image.h"
#include "LightSource.h"
#include "Camera.h"

class World {
public:
    void AddPrimitive(Primitive &primitive);

    void AddLight(LightSource &light);

    void SetCamera(Camera &camera);

    Image CreateImage(unsigned int height, unsigned int width);

private:
    std::vector<Primitive> objects_;
    Camera camera_;
    std::vector<LightSource> lights_;

};


#endif //INC_3DVISUALIZER_WORLD_H
