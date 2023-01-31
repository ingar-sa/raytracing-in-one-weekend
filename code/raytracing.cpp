#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <iostream>
#include <cstdio>
#include <ctime>
#include <chrono>

#include "vec3.hpp"
#include "color.hpp"
#include "ray.hpp"

#define WINDOWS 0

#if WINDOWS
#include <windows.h>
#endif

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

color BackgroundColor(ray *Ray)
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

double HitSphere(point3 *Center, double Radius, ray *Ray)
{
    // We will be using the abc-formula: (-b +/- sqrt(b^2-4ac))/2
    vec3 OriginToCenter = {};
    Vec3Sub(&Ray->Origin, Center, &OriginToCenter);

    double a = Vec3Dot(&Ray->Direction, &Ray->Direction);
    double b = 2.0 * Vec3Dot(&OriginToCenter, &Ray->Direction);
    double c = Vec3Dot(&OriginToCenter, &OriginToCenter) - Radius * Radius;
    double Discriminant = b * b - 4 * a * c;

    if (Discriminant < 0)
    {
        return -1.0;
    }
    else
    {
        return (-b - sqrt(Discriminant)) / (2.0 * a);
    }
    
}


// color ray_color(const ray& r) {
//     auto t = hit_sphere(point3(0,0,-1), 0.5, r);
//     if (t > 0.0) {
//         vec3 N = unit_vector(r.at(t) - vec3(0,0,-1));
//         return 0.5*color(N.x()+1, N.y()+1, N.z()+1);
//     }
//     vec3 unit_direction = unit_vector(r.direction());
//     t = 0.5*(unit_direction.y() + 1.0);
//     return (1.0-t)*color(1.0, 1.0, 1.0) + t*color(0.5, 0.7, 1.0);
// }

color RayColor(ray *Ray)
{    
    point3 SphereCenter = {0, 0, -1};
    double t = HitSphere(&SphereCenter, 0.5, Ray);

    if (t > 0.0)
    {
        point3 RayAtT = RayAtTime(Ray, t);
        Vec3Sub(&RayAtT, &SphereCenter, &RayAtT);
        vec3 Normal = Vec3NewUnitVector(&RayAtT);
        color PixelColor = { Normal.X + 1, Normal.Y + 1, Normal.Z + 1 };

        return Vec3NewScaled(&PixelColor, 0.5);
    }

    vec3 DirectionUnitVec = Vec3NewUnitVector(&Ray->Direction);
    t = 0.5 * (DirectionUnitVec.Y + 1.0);

    color Lerp1Base = { 1, 1, 1 };
    color Lerp1 = Vec3NewScaled(&Lerp1Base, (1.0 - t));

    color Lerp2Base = { 0.5, 0.7, 1.0 };
    color Lerp2 = Vec3NewScaled(&Lerp2Base, t);

    color PixelColor = {};
    Vec3Add(&Lerp1, &Lerp2, &PixelColor);

    return PixelColor;
}

void RenderScene()
{
    const float AspectRatio = 16.0 / 9.0;
    const int32_t ImageWidth = 2480;
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

    #if WINDOWS
    LARGE_INTEGER StartCounter;
    QueryPerformanceCounter(&StartCounter);
    #endif 

    auto wcts = std::chrono::system_clock::now();

    for (int Y = ImageHeight - 1; Y >= 0; --Y)
    {
        printf("\rScanlines remaining: %d\n", Y);

        for (int X = 0; X < ImageWidth; ++X)
        {
            double U = (double)X / (ImageWidth - 1);
            double V = (double)Y / (ImageHeight - 1);
            vec3 UHorizontal = Vec3NewScaled(&Horizontal, U);
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

    #if WINDOWS
    LARGE_INTEGER EndCounter;
    QueryPerformanceCounter(&EndCounter);
    int64_t CounterElapsed = EndCounter.QuadPart - StartCounter.QuadPart;

    LARGE_INTEGER PerfCounterFrequencyResult;
    QueryPerformanceFrequency(&PerfCounterFrequencyResult);
    int64_t PerfCounterFrequency = PerfCounterFrequencyResult.QuadPart;
    int64_t MSElapsed = ((1000 * CounterElapsed) / PerfCounterFrequency);
    #endif
    
    std::chrono::duration<double> wctduration = (std::chrono::system_clock::now() - wcts);
    std::cout << "Finished in " << wctduration.count() << " seconds [Wall Clock]" << std::endl;
}

int main()
{
    // GeneratePPMFile();
    // TestVec3();

    RenderScene();

    return 0;
}