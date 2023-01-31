#ifndef VEC3_STRUCT_H
#define VEC3_STRUCT_H

struct vec3
{
    double X = 0.0;
    double Y = 0.0;
    double Z = 0.0;
};

// Type aliases for vec3
using point3 = vec3; // 3D point
using color = vec3; // RGB color

#endif // VEC3_STRUCT_H