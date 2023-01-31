#ifndef VEC3_H
#define VEC3_H

#include <iostream>

struct vec3
{
    double X = 0.0;
    double Y = 0.0;
    double Z = 0.0;
};


// TODO(ingar): Maybe it would be prudent to have variable arg versions of these functions
// to make repeated operations (such as Vec1 * Vec2 * Vec3 * ...) easier.
// TODO(ingar): Maybe do some C++ operator overloading?
void Vec3Add(vec3 *Vec1, vec3 *Vec2, vec3 *VecOut);
void Vec3Sub(vec3 *Vec1, vec3 *Vec2, vec3 *VecOut);
void Vec3Mul(vec3 *Vec1, vec3 *Vec2, vec3 *VecOut);
double Vec3Dot(vec3 *Vec1, vec3 *Vec2);
void Vec3Cross(vec3 *Vec1, vec3 *Vec2, vec3 *VecOut);
void Vec3Scale(vec3 *Vec, double Scalar);
vec3 Vec3NewScaled(vec3 *Vec, double Scalar);
double Vec3LengthSquared(vec3 *Vec);
double Vec3Length(vec3 *Vec);
void UnitVector(vec3 *Vec);
vec3 Vec3NewUnitVector(vec3 *Vec);

void TestVec3();
// Type aliases for vec3

using point3 = vec3; // 3D point
using color = vec3; // RGB color

// typedef vec3 point3;
// typedef vec3 color;

inline std::ostream& operator<<(std::ostream &out, const vec3 &Vec) {
    return out << Vec.X << ' ' << Vec.Y << ' ' << Vec.Z;
}

#endif //VEC3_H