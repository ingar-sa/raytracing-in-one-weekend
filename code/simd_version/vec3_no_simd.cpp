#include "vec3_struct.hpp"

#include <math.h>

void Vec3Add(vec3 *Vec1, vec3 *Vec2, vec3 *VecOut)
{
    VecOut->X = Vec1->X + Vec2->X;
    VecOut->Y = Vec1->Y + Vec2->Y;
    VecOut->Z = Vec1->Z + Vec2->Z;
}

void Vec3Sub(vec3 *Vec1, vec3 *Vec2, vec3 *VecOut)
{
    VecOut->X = Vec1->X - Vec2->X;
    VecOut->Y = Vec1->Y - Vec2->Y;
    VecOut->Z = Vec1->Z - Vec2->Z;
}

void Vec3Mul(vec3 *Vec1, vec3 *Vec2, vec3 *VecOut)
{
    VecOut->X = Vec1->X * Vec2->X;
    VecOut->Y = Vec1->Y * Vec2->Y;
    VecOut->Z = Vec1->Z * Vec2->Z;
}

double Vec3Dot(vec3 *Vec1, vec3 *Vec2)
{
    return (Vec1->X * Vec2->X) + (Vec1->Y * Vec2->Y) + (Vec1->Z * Vec2->Z);
}

void Vec3Cross(vec3 *Vec1, vec3 *Vec2, vec3 *VecOut)
{
    double U0 = Vec1->X;
    double U1 = Vec1->Y;
    double U2 = Vec1->Z;

    double V0 = Vec2->X;
    double V1 = Vec2->Y;
    double V2 = Vec2->Z;

    VecOut->X = U1 * V2 - U2 * V1;
    VecOut->Y = U2 * V0 - U0 * V2;
    VecOut->Z = U0 * V1 - U1 * V0;
}

void Vec3Scale(vec3 *Vec, double Scalar)
{
    Vec->X *= Scalar;
    Vec->Y *= Scalar;
    Vec->Z *= Scalar;
}

vec3 Vec3NewScaled(vec3 *Vec, double Scalar)
{
    vec3 NewVec = {Vec->X * Scalar,
                   Vec->Y * Scalar,
                   Vec->Z * Scalar};

    return NewVec;
}

double Vec3LengthSquared(vec3 *Vec)
{
    return Vec3Dot(Vec, Vec);
}

double Vec3Length(vec3 *Vec)
{
    return sqrt(Vec3LengthSquared(Vec));
}

void UnitVector(vec3 *Vec)
{
    Vec3Scale(Vec, (1.0 / Vec3Length(Vec)));
}

vec3 Vec3NewUnitVector(vec3 *Vec)
{
    double Scalar = Vec3Length(Vec);
    return Vec3NewScaled(Vec, 1 / Scalar);
}