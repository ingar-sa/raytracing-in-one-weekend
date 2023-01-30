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
            WriteColor(FileLine, &PixelColor);
            // fwrite(FileLine, 1, sizeof(FileLine), ImageFile);
            fprintf(ImageFile, FileLine);
        }
    }

    fclose(ImageFile);
}

color RayColor(ray *Ray)
{
    vec3 DirectionUnitVector = Vec3NewUnitVector(&Ray->Direction);
    double Scalar = 0.5 * (DirectionUnitVector.Y + 1.0);
    
    color Color1 = { 1.0, 1.0, 1.0 };
    color Color2 = { 0.5, 0.7, 1.0 };
    Vec3Scale(&Color1, (1.0-Scalar));
    Vec3Scale(&Color2, Scalar);

    color RayColor = {};
    Vec3Add(&Color1, &Color2, &RayColor);
    return RayColor;
}

void RenderBlueGradient()
{
    //Image 
    const float AspectRatio = 16.0 / 9.0;
    const int32_t ImageWidth = 400;
    const int32_t ImageHeight = (int)(ImageWidth / AspectRatio);

    float ViewportHeight = 2.0;
    float ViewportWidth = AspectRatio * ViewportHeight;
    float FocalLenght = 1.0;

    point3 Origin = {}; 
    vec3 Horizontal = { ViewportWidth, 0, 0 };
    vec3 Vertical = { 0, ViewportHeight, 0 };
    vec3 Center = { 0, 0, FocalLenght }; // I think this is the center

    point3 LowerLeftCorner = {};
    Vec3Sub(&Origin, &Vec3NewScaled(&Horizontal, 0.5), &LowerLeftCorner);
    Vec3Sub(&LowerLeftCorner, &Vec3NewScaled(&Vertical, 0.5), &LowerLeftCorner);
    Vec3Sub(&LowerLeftCorner, &Center, &LowerLeftCorner);

    for(int Y = ImageHeight -1; Y >= 0; ++Y)
    {
        printf("\rScanlines remaining: %d", Y);
        for(int X = 0; X < ImageWidth; ++X)
        {
            double U = (double)X / (ImageWidth - 1);
            double V = (double)Y / (ImageHeight - 1);
            vec3 RayDirection = {};
            
            ray Ray = {}
        }
    }
}


int main()
{
    // GeneratePPMFile();
    TestVec3();

    return 0;
}


// color ray_color(const ray& r) {
//     vec3 unit_direction = unit_vector(r.direction());
//     auto t = 0.5*(unit_direction.y() + 1.0);
//     return (1.0-t)*color(1.0, 1.0, 1.0) + t*color(0.5, 0.7, 1.0);
// }

int main() {

    // Image
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio);

    // Camera

    auto viewport_height = 2.0;
    auto viewport_width = aspect_ratio * viewport_height;
    auto focal_length = 1.0;

    auto origin = point3(0, 0, 0);
    auto horizontal = vec3(viewport_width, 0, 0);
    auto vertical = vec3(0, viewport_height, 0);
    auto lower_left_corner = origin - horizontal/2 - vertical/2 - vec3(0, 0, focal_length);

    // Render

    std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";

    for (int j = image_height-1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            auto u = double(i) / (image_width-1);
            auto v = double(j) / (image_height-1);
            ray r(origin, lower_left_corner + u*horizontal + v*vertical - origin);
            color pixel_color = ray_color(r);
            write_color(std::cout, pixel_color);
        }
    }

    std::cerr << "\nDone.\n";
}