#include "vec3.hpp"
#include "color_simd.hpp"

#if BUILD_SIMD
#include "vec3_simd.cpp"
#else
#include "vec3_no_simd.cpp"
#endif

#include "ray_simd.cpp"

#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <cstdio>
#include <ctime>
#include <chrono>
#include <immintrin.h>

#define WINDOWS 0

#if WINDOWS
#include <windows.h>
#endif

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
    double A = Vec3LengthSquared(&Ray->Direction);
    // double a = Vec3Dot(&Ray->Direction, &Ray->Direction);
    double HalfB = Vec3Dot(&OriginToCenter, &Ray->Direction);
    double C = Vec3Dot(&OriginToCenter, &OriginToCenter) - Radius * Radius;
    double Discriminant = HalfB * HalfB - A * C;

    if (Discriminant < 0)
    {
        return -1.0;
    }
    else
    {
        return (-HalfB - sqrt(Discriminant)) / A;
    }
}

// double hit_sphere(const point3 &center, double radius, const ray &r)
// {
//     vec3 oc = r.origin() - center;
//     auto a = r.direction().length_squared();
//     auto half_b = dot(oc, r.direction());
//     auto c = oc.length_squared() - radius * radius;
//     auto discriminant = half_b * half_b - a * c;

//     if (discriminant < 0)
//     {
//         return -1.0;
//     }
//     else
//     {
//         return (-half_b - sqrt(discriminant)) / a;
//     }
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
        color PixelColor = {Normal.X + 1, Normal.Y + 1, Normal.Z + 1};

        return Vec3NewScaled(&PixelColor, 0.5);
    }

    vec3 DirectionUnitVec = Vec3NewUnitVector(&Ray->Direction);
    t = 0.5 * (DirectionUnitVec.Y + 1.0);

    color Lerp1Base = {1, 1, 1};
    color Lerp1 = Vec3NewScaled(&Lerp1Base, (1.0 - t));

    color Lerp2Base = {0.5, 0.7, 1.0};
    color Lerp2 = Vec3NewScaled(&Lerp2Base, t);

    color PixelColor = {};
    Vec3Add(&Lerp1, &Lerp2, &PixelColor);

    return PixelColor;
}

void RenderScene()
{
    const float AspectRatio = 16.0 / 9.0;
    const int32_t ImageWidth = 1280;
    const int32_t ImageHeight = (int)(ImageWidth / AspectRatio);

    float ViewportHeight = 2.0;
    float ViewportWidth = AspectRatio * ViewportHeight;
    float FocalLenght = 1.0;

    point3 Origin = {};
    vec3 Horizontal = {ViewportWidth, 0, 0};
    vec3 Vertical = {0, ViewportHeight, 0};
    vec3 Center = {0, 0, FocalLenght}; // I think this is the center

    // point3 LowerLeftCorner = {-ViewportWidth / 2, -ViewportHeight / 2, -FocalLenght};

    // /* Note(ingar): Replaced by the line above
    point3 LowerLeftCorner = {};
    vec3 HalfHorizontal = Vec3NewScaled(&Horizontal, 0.5);
    vec3 HalfVertical = Vec3NewScaled(&Vertical, 0.5);

    Vec3Sub(&Origin, &HalfHorizontal, &LowerLeftCorner);
    Vec3Sub(&LowerLeftCorner, &HalfVertical, &LowerLeftCorner);
    Vec3Sub(&LowerLeftCorner, &Center, &LowerLeftCorner);
    // */

    __m256d mmLowerLeftCorner = _mm256_loadu_pd((const double *)&LowerLeftCorner);
    __m256d mmOrigin = _mm256_loadu_pd((const double *)&Origin);
    __m256d mmHorizontal = _mm256_loadu_pd((const double *)&Horizontal);
    __m256d mmVertical = _mm256_loadu_pd((const double *)&Vertical);

#if BUILD_SIMD
    const char ppm_filename[] = "image_simd.ppm";
#else
    const char ppm_filename[] = "image_no_simd.ppm";
#endif

    FILE *ImageFile = fopen(ppm_filename, "w");
    fprintf(ImageFile, "P3\n%d %d\n255\n", ImageWidth, ImageHeight);

#if WINDOWS
    LARGE_INTEGER StartCounter;
    QueryPerformanceCounter(&StartCounter);
#else
    auto wcts = std::chrono::system_clock::now();
#endif

    for (int Y = ImageHeight - 1; Y >= 0; --Y)
    {
        // printf("\rScanlines remaining: %d\n", Y);

        for (int X = 0; X < ImageWidth; ++X)
        {
            double U = (double)X / (ImageWidth - 1);
            double V = (double)Y / (ImageHeight - 1);
            double UArray[] = {U, U, U};
            double VArray[] = {V, V, V};
            
            __m256d mmU = _mm256_loadu_pd((const double *)&UArray);
            __m256d mmV = _mm256_loadu_pd((const double *)&VArray);
            __m256d mmUHorizontal = _mm256_mul_pd(mmHorizontal, mmU);
            __m256d mmVVertical = _mm256_mul_pd(mmVertical, mmV);

            __m256d mmRayDirection = _mm256_add_pd(mmLowerLeftCorner, mmUHorizontal);
            mmRayDirection = _mm256_add_pd(mmRayDirection, mmVVertical);
            mmRayDirection = _mm256_sub_pd(mmRayDirection, mmOrigin);

            double *Result = (double *)&mmRayDirection;
            vec3 RayDirection = {Result[0], Result[1], Result[2]};

            ray Ray = {Origin, RayDirection};
            color PixelColor = RayColor(&Ray);
            WriteColor(ImageFile, &PixelColor);
        }
    }

    fclose(ImageFile);

#if WINDOWS
    LARGE_INTEGER EndCounter;
    QueryPerformanceCounter(&EndCounter);
    int64_t CounterElapsed = EndCounter.QuadPart - StartCounter.QuadPart;

    LARGE_INTEGER PerfCounterFrequencyResult;
    QueryPerformanceFrequency(&PerfCounterFrequencyResult);
    int64_t PerfCounterFrequency = PerfCounterFrequencyResult.QuadPart;
    int64_t MSElapsed = ((1000 * CounterElapsed) / PerfCounterFrequency);
    printf("Time elapse: %I64d", MSElapsed);
#else
    std::chrono::duration<double> wctduration = (std::chrono::system_clock::now() - wcts);
    printf("Finished in %f seconds\n", wctduration.count());
#endif
}

int main()
{
#if BUILD_SIMD // The macro is in vec3.hpp
    printf("Compiled for SIMD.\n");
#else
    printf("Not compiled for SIMD.\n");
#endif

    RenderScene();

    return 0;
}
