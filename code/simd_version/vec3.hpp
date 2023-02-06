#ifndef VEC3_H
#define VEC3_H

#include "vec3_struct.hpp"

#define BUILD_SIMD 0

#if BUILD_SIMD

#define Vec3Add Vec3AddSimd
#define Vec3Sub Vec3SubSimd
#define Vec3Mul Vec3MulSimd
#define Vec3Dot Vec3DotSimd
#define Vec3Cross Vec3CrossSimd
#define Vec3Scale Vec3ScaleSimd
#define Vec3NewScaled Vec3NewScaledSimd
#define Vec3LengthSquared Vec3LengthSquaredSimd
#define Vec3Length Vec3LengthSimd
#define UnitVector UnitVectorSimd
#define Vec3NewUnitVector Vec3NewUnitVectorSimd

#endif

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

#endif // VEC3_H