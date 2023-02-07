#include "vec3.hpp"

/*
When there are multiple calls to the simd vec3 functions,
the vectors are loaded into the ymm registers and then the
<<<<<<< HEAD
=======

>>>>>>> 29a5a1100e40e58ce41d5e74aed296a3794c1488
result is retrieved from them in every call. This is probably a huge waste
of processor time, and the reason for why it's the slowest version of the tracers.

This means that we really have to write the simd intrinsics 
inline, which will make the code less legible. But hey, anything
in the name of going fast, right?
*/

#include <immintrin.h>
#include <stdio.h>
#include <math.h>

void Vec3AddSimd(vec3 *Vec1, vec3 *Vec2, vec3 *VecOut)
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

void Vec3SubSimd(vec3 *Vec1, vec3 *Vec2, vec3 *VecOut)
{
    __m256d mVec1 = _mm256_loadu_pd((const double *)Vec1);
    __m256d mVec2 = _mm256_loadu_pd((const double *)Vec2);
    __m256d mResult = _mm256_sub_pd(mVec1, mVec2);

    double *Result = (double *)&mResult;
    VecOut->X = Result[0];
    VecOut->Y = Result[1];
    VecOut->Z = Result[2];
}

void Vec3MulSimd(vec3 *Vec1, vec3 *Vec2, vec3 *VecOut)
{
    __m256d mVec1 = _mm256_loadu_pd((const double *)Vec1);
    __m256d mVec2 = _mm256_loadu_pd((const double *)Vec2);
    __m256d mResult = _mm256_mul_pd(mVec1, mVec2);

    double *Result = (double *)&mResult;
    VecOut->X = Result[0];
    VecOut->Y = Result[1];
    VecOut->Z = Result[2];
}

double Vec3DotSimd(vec3 *Vec1, vec3 *Vec2)
{
    __m256d mVec1 = _mm256_loadu_pd((const double *)Vec1);
    __m256d mVec2 = _mm256_loadu_pd((const double *)Vec2);
    __m256d mResult = _mm256_mul_pd(mVec1, mVec2);

    double *Result = (double *)&mResult;
    double DotProduct = Result[0] + Result[1] + Result[2];

    return DotProduct;
}

void Vec3CrossSimd(vec3 *Vec1, vec3 *Vec2, vec3 *VecOut)
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
void Vec3ScaleSimd(vec3 *Vec, double Scalar)
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

vec3 Vec3NewScaledSimd(vec3 *Vec, double Scalar)
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

void Vec3UnitVectorSimd(vec3 *Vec)
{
    Vec3ScaleSimd(Vec, 1.0 / Vec3LengthSimd(Vec));
}

vec3 Vec3NewUnitVectorSimd(vec3 *Vec)
{
    return Vec3NewScaledSimd(Vec, 1.0 / Vec3LengthSimd(Vec));
}