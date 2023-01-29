#include <stdio.h>

#include "vec3.hpp"
#include "color.hpp"

// #define IMAGE_WIDTH 256;
// #define IMAGE_HEIGHT 256

void GeneratePPMFile()
{
    const int IMAGE_WIDTH = 256;
    const int IMAGE_HEIGHT = 256;
    char FileLine[15] = {};

    const char ppm_filename[] = "../build/image.ppm";
    FILE *ImageFile = fopen(ppm_filename, "w");

    fprintf(ImageFile, "P3\n%d %d\n255\n", IMAGE_WIDTH, IMAGE_HEIGHT);
    for (int Y = IMAGE_HEIGHT; Y > 0; --Y)
    {
        // printf("Scanlines remaining: %d\n", Y);

        for (int X = 0; X < IMAGE_WIDTH; ++X)
        {
            color PixelColor = {(double)X / (IMAGE_WIDTH - 1), (double)Y / (IMAGE_HEIGHT - 1), 0.25};
            WriteColor(FileLine, &PixelColor);
            fprintf(ImageFile, FileLine);
        }
    }

    fclose(ImageFile);
}

int main()
{
    GeneratePPMFile();
    // TestVec3();

    return 0;
}