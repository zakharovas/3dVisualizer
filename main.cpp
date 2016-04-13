
#include "PixelToaster.h"
#include "Drawer.h"

using namespace PixelToaster;

int main()
{
    const int width = 1400;
    const int height = 600;
    Display display( "TrueColor Example", width, height, Output::Default, Mode::TrueColor );

    vector<TrueColorPixel> pixels( width * height );

    while ( display.open() )
    {
        unsigned int index = 0;

        for ( int y = 0; y < height; ++y )
        {
            for ( int x = 0; x < width; ++x )
            {
                pixels[index].r = x < 2550 ? x * .10 / 10 : 255;
                pixels[index].g = y < 2550 ? y/10 : 255;
                pixels[index].b = x+y < 2550 ? (x+y)/10 : 255;

                ++index;
            }
        }

        display.update( pixels );

    }
}
