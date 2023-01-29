#include <stdio.h>

#include "vec3.h"

// #define IMAGE_WIDTH 256;
// #define IMAGE_HEIGHT 256

const char ppm_filename[] = "image.ppm";
const int IMAGE_WIDTH = 256;
const int IMAGE_HEIGHT = 256;

void TestVec3()
{
    vec3 Vec1 = { 10.0, -20.0, 30.0 };
    vec3 Vec2 = { 1.0, 2.0, 2.0 };

    vec3 VecOut = {};
    Vec3Add(&Vec1, &Vec2, &VecOut);
    printf("Add result: X = %f Y = %f Z = %f\n\n", VecOut.X, VecOut.Y, VecOut.Z);

    Vec3Sub(&VecOut, &Vec2, &Vec1);
    printf("Sub result: X = %f Y = %f Z = %f\n\n", Vec1.X, Vec1.Y, Vec1.Z);

    Vec3Scale(&Vec2, 3);
    printf("Scale result: X = %f Y = %f Z = %f\n\n", Vec2.X, Vec2.Y, Vec2.Z);

    double Vec2LengthSquared = Vec3LengthSquared(&Vec2);
    printf("Vec2 Length squared: %f\n\n", Vec2LengthSquared);

    printf("Vec2 length: %f\n\n", Vec3Length(&Vec2));

}

int main()
{
    char FileLine[15] = {};
    FILE *ImageFile = fopen(ppm_filename, "w");
    fprintf(ImageFile, "P3\n%d %d\n255\n", IMAGE_WIDTH, IMAGE_HEIGHT);

    for (int Y = IMAGE_HEIGHT; Y > 0; --Y)
    {
        // printf("Scanlines remaining: %d\n", Y);

        for (int X = 0; X < IMAGE_WIDTH; ++X)
        {
            double R = (double)X / (IMAGE_WIDTH - 1);
            double G = (double)Y / (IMAGE_HEIGHT - 1);
            double B = 0.25;

            int IR = (int)(255.999 * R);
            int IG = (int)(255.999 * G);
            int IB = (int)(255.999 * B);

            fprintf(ImageFile, "%d %d %d\n", IR, IG, IB);
            
        }
    }

    fclose(ImageFile);

    TestVec3();

    return 0;
}

