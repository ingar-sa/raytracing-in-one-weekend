#ifndef VEC3_H
#define VEC3_H

struct vec3
{
    double X = 0.0;
    double Y = 0.0;
    double Z = 0.0;
};

void Vec3Add(vec3 *Vec1, vec3 *Vec2, vec3 *VecOut);
void Vec3Sub(vec3 *Vec1, vec3 *Vec2, vec3 *VecOut);
void Vec3Mul(vec3 *Vec1, vec3 *Vec2, vec3 *VecOut);
double Vec3Dot(vec3 *Vec1, vec3 *Vec2);
void Vec3Cross(vec3 *Vec1, vec3 *Vec2, vec3 *VecOut);
void Vec3Scale(vec3 *Vec, double Scalar);
double Vec3LengthSquared(vec3 *Vec);
double Vec3Length(vec3 *Vec);
void UnitVector(vec3 *Vec);

void TestVec3();
// Type aliases for vec3

using point3 = vec3; // 3D point
using color = vec3; // RGB color

#endif //VEC3_H