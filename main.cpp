#include <iostream>
#include <cstdlib>
#include <limits>
#include "PixelToaster.h"
#include "PrincipalClasses/Drawer.h"
#include "PrincipalClasses/World.h"
#include "Readers/StlReader.h"
#include "Readers/RtReader.h"


int main(int argc, char *argv[]) {
    std::string filename = argv[1];
    std::cout << filename << std::endl;
    RtReader reader(filename);
    World perfect_world = reader.CreateWorld();
    const int width = 800;
    const int height = 600;
    Image image = perfect_world.CreateImage(height, width);
    Drawer drawer(image);
    drawer.Draw();
    return 0;
}