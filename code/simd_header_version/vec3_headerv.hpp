#ifndef VEC3_H
#define VEC3_H

#include <immintrin.h>
#include <stdio.h>
#include <math.h>

struct vec3
{
    double X = 0.0;
    double Y = 0.0;
    double Z = 0.0;
};

using point3 = vec3; // 3D point
using color = vec3; // RGB color

void Vec3Add(vec3 *Vec1, vec3 *Vec2, vec3 *VecOut)
{
    __m256d mVec1 = _mm256_loadu_pd((const double *)Vec1);
    __m256d mVec2 = _mm256_loadu_pd((const double *)Vec2);
    __m256d mResult = _mm256_add_pd(mVec1, mVec2); // Fun fact: The m prefix comes from the MMX instruction set name

    // VecOut = (vec3 *)&mVecOut; // Won't work since the registers are zeroed at the end of the function
    double *Result = (double *)&mResult;
    VecOut->X = Result[0];
    VecOut->Y = Result[1];
    VecOut->Z = Result[2];
}

void Vec3Sub(vec3 *Vec1, vec3 *Vec2, vec3 *VecOut)
{
    __m256d mVec1 = _mm256_loadu_pd((const double *)Vec1);
    __m256d mVec2 = _mm256_loadu_pd((const double *)Vec2);
    __m256d mResult = _mm256_sub_pd(mVec1, mVec2);

    double *Result = (double *)&mResult;
    VecOut->X = Result[0];
    VecOut->Y = Result[1];
    VecOut->Z = Result[2];
}

void Vec3Mul(vec3 *Vec1, vec3 *Vec2, vec3 *VecOut)
{
    __m256d mVec1 = _mm256_loadu_pd((const double *)Vec1);
    __m256d mVec2 = _mm256_loadu_pd((const double *)Vec2);
    __m256d mResult = _mm256_mul_pd(mVec1, mVec2);

    double *Result = (double *)&mResult;
    VecOut->X = Result[0];
    VecOut->Y = Result[1];
    VecOut->Z = Result[2];
}

double Vec3Dot(vec3 *Vec1, vec3 *Vec2)
{
    __m256d mVec1 = _mm256_loadu_pd((const double *)Vec1);
    __m256d mVec2 = _mm256_loadu_pd((const double *)Vec2);
    __m256d mResult = _mm256_mul_pd(mVec1, mVec2);

    double *Result = (double *)&mResult;
    double DotProduct = Result[0] + Result[1] + Result[2];

    return DotProduct;
}

void Vec3Cross(vec3 *Vec1, vec3 *Vec2, vec3 *VecOut)
{
    // Note(ingar): It might not be worth SIMDing this because of the need to shuffle the values around.
    // That might cause the code to be less effective than simply doing the math the old fashoned way
    // Note(ingar): Might be worth checking into whether the compiler optimizes away the variable declarations
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

// NOTE(ingar): It might be prudent to make a version that returns a new vec3
void Vec3Scale(vec3 *Vec, double Scalar)
{
    double aScalar[4] = {Scalar, Scalar, Scalar, Scalar};

    __m256d mVec = _mm256_loadu_pd((const double *)Vec);
    __m256d mScalar = _mm256_loadu_pd((double *)aScalar);
    __m256d mResult = _mm256_mul_pd(mVec, mScalar);

    double *Result = (double *)&mResult;

    Vec->X = Result[0];
    Vec->Y = Result[1];
    Vec->Z = Result[2];
}

vec3 Vec3NewScaled(vec3 *Vec, double Scalar)
{
    double aScalar[4] = {Scalar, Scalar, Scalar, Scalar};

    __m256d mVec = _mm256_loadu_pd((const double *)Vec);
    __m256d mScalar = _mm256_loadu_pd((double *)aScalar);
    __m256d mResult = _mm256_mul_pd(mVec, mScalar);

    double *Result = (double *)&mResult;

    vec3 ScaledVec = {Result[0], Result[1], Result[2]};
    return ScaledVec;
}

double Vec3LengthSquaredSimd(vec3 *Vec)
{
    __m256d mVec = _mm256_loadu_pd((const double *)Vec);
    __m256d mVec1 = _mm256_loadu_pd((const double *)Vec);
    __m256d mResult = _mm256_mul_pd(mVec, mVec1);

    double *Result = (double *)&mResult;
    double LengthSquared = Result[0] + Result[1] + Result[2];

    return LengthSquared;
}

double Vec3LengthSimd(vec3 *Vec)
{
    return sqrt(Vec3LengthSquaredSimd(Vec));
}

// NOTE(ingar): It might be prudent to make a version that returns a new vec3
void Vec3UnitVectorSimd(vec3 *Vec)
{
    Vec3Scale(Vec, 1.0 / Vec3LengthSimd(Vec));
}

vec3 Vec3NewUnitVectorSimd(vec3 *Vec)
{
    return Vec3NewScaled(Vec, 1.0 / Vec3LengthSimd(Vec));
}

void TestVec3Simd()
{
    vec3 Vec1 = {10.0, -20.0, 30.0};
    vec3 Vec2 = {1.0, 2.0, 2.0};
    vec3 VecOut = {};

    Vec3Add(&Vec1, &Vec2, &VecOut);
    printf("Add result: X = %f Y = %f Z = %f\n\n", VecOut.X, VecOut.Y, VecOut.Z);

    Vec3Sub(&VecOut, &Vec2, &Vec1);
    printf("Sub result: X = %f Y = %f Z = %f\n\n", Vec1.X, Vec1.Y, Vec1.Z);

    Vec3Mul(&Vec1, &Vec2, &VecOut);
    printf("Multiply result: X = %f Y = %f Z = %f\n\n", VecOut.X, VecOut.Y, VecOut.Z);

    Vec3Scale(&Vec2, 3);
    printf("Scale result: X = %f Y = %f Z = %f\n\n", Vec2.X, Vec2.Y, Vec2.Z);

    printf("Vec2 Length squared: %f\n\n", Vec3LengthSquaredSimd(&Vec2));
    printf("Vec2 length: %f\n\n", Vec3LengthSimd(&Vec2));
    printf("Vec2 dot product: %f\n\n", Vec3Dot(&Vec1, &Vec2));

    Vec3Cross(&Vec1, &Vec2, &VecOut);
    printf("Vec1 & Vec2 cross product result: X = %f Y = %f Z = %f\n\n", VecOut.X, VecOut.Y, VecOut.Z);

    Vec3UnitVectorSimd(&Vec1);
    printf("Unit vector: X = %f Y = %f Z = %f\n\n", Vec1.X, Vec1.Y, Vec1.Z);
    printf("Vec1 length: %f\n\n", Vec3LengthSimd(&Vec1));

    Vec3UnitVectorSimd(&Vec2);
    printf("Unit vector: X = %f Y = %f Z = %f\n\n", Vec2.X, Vec2.Y, Vec2.Z);
    printf("Vec2 length: %f\n\n", Vec3LengthSimd(&Vec2));

    vec3 NewUnitVector = Vec3NewUnitVectorSimd(&VecOut);
    printf("Unit vector: X = %f Y = %f Z = %f\n\n", NewUnitVector.X, NewUnitVector.Y, NewUnitVector.Z);
    printf("NewUnitVector length: %f\n\n", Vec3LengthSimd(&NewUnitVector));

    Vec3UnitVectorSimd(&VecOut);
    printf("Unit vector: X = %f Y = %f Z = %f\n\n", VecOut.X, VecOut.Y, VecOut.Z);
    printf("VecOut length: %f\n\n", Vec3LengthSimd(&VecOut));
}

#endif