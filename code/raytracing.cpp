#include <stdio.h>
#include <stdint.h>

#include "vec3.hpp"
#include "color.hpp"
#include "ray.hpp"
#define IMAGE_WIDTH 256
#define IMAGE_HEIGHT 256

void GenerateExamplePPMFile()
{
    // const int IMAGE_WIDTH = 256;
    // const int IMAGE_HEIGHT = 256;
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
            WriteColor(ImageFile, &PixelColor);
        }
    }

    fclose(ImageFile);
}

color RayColor(ray *Ray)
{
    vec3 DirectionUnitVector = Vec3NewUnitVector(&Ray->Direction);
    double Scalar = 0.5 * (DirectionUnitVector.Y + 1.0);

    color Color1 = {1.0, 1.0, 1.0};
    color Color2 = {0.5, 0.7, 1.0};
    Vec3Scale(&Color1, (1.0 - Scalar));
    Vec3Scale(&Color2, Scalar);

    color RayColor = {};
    Vec3Add(&Color1, &Color2, &RayColor);
    return RayColor;
}

void RenderBlueGradient()
{
    const float AspectRatio = 16.0 / 9.0;
    const int32_t ImageWidth = 1080;
    const int32_t ImageHeight = (int)(ImageWidth / AspectRatio);

    float ViewportHeight = 2.0;
    float ViewportWidth = AspectRatio * ViewportHeight;
    float FocalLenght = 1.0;

    point3 Origin = {};
    vec3 Horizontal = {ViewportWidth, 0, 0};
    vec3 Vertical = {0, ViewportHeight, 0};
    vec3 Center = {0, 0, FocalLenght}; // I think this is the center

    point3 LowerLeftCorner = {};
    vec3 HalfHorizontal = Vec3NewScaled(&Horizontal, 0.5);
    vec3 HalfVertical = Vec3NewScaled(&Vertical, 0.5);

    Vec3Sub(&Origin, &HalfHorizontal, &LowerLeftCorner);
    Vec3Sub(&LowerLeftCorner, &HalfVertical, &LowerLeftCorner);
    Vec3Sub(&LowerLeftCorner, &Center, &LowerLeftCorner);

    const char ppm_filename[] = "../build/image.ppm";
    FILE *ImageFile = fopen(ppm_filename, "w");
    fprintf(ImageFile, "P3\n%d %d\n255\n", ImageWidth, ImageHeight);

    for (int Y = ImageHeight - 1; Y >= 0; --Y)
    {
        printf("\rScanlines remaining: %d\n", Y);

        for (int X = 0; X < ImageWidth; ++X)
        {
            double U = (double)X / (ImageWidth - 1);
            double V = (double)Y / (ImageHeight - 1);
            vec3 UHorizontal = Vec3NewScaled(&Horizontal,U);
            vec3 VVertical = Vec3NewScaled(&Vertical, V);

            vec3 RayDirection = {};
            Vec3Add(&LowerLeftCorner, &UHorizontal, &RayDirection);
            Vec3Add(&RayDirection, &VVertical, &RayDirection);
            Vec3Sub(&RayDirection, &Origin, &RayDirection);

            ray Ray = {Origin, RayDirection};
            color PixelColor = RayColor(&Ray);
            WriteColor(ImageFile, &PixelColor);
        }
    }
}

int main()
{
    // GeneratePPMFile();
    // TestVec3();

    RenderBlueGradient();

    return 0;
}