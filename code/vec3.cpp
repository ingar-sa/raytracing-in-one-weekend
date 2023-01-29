#include <immintrin.h>
#include <math.h>

#include <stdio.h>

#include "vec3.h"

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

double Vec3Dot(vec3 *Vec1, vec3 *Vec2, vec3 *VecOut)
{
    __m256d mVec1 = _mm256_loadu_pd((const double *)Vec1);
    __m256d mVec2 = _mm256_loadu_pd((const double *)Vec2);
    __m256d mResult = _mm256_mul_pd(mVec1, mVec2);

    double *Result = (double *)&mResult;
    double DotProduct = Result[0] + Result[1] + Result[2];
    
    return DotProduct;
}

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

double Vec3LengthSquared(vec3 *Vec)
{
    __m256d mVec = _mm256_loadu_pd((const double *)Vec);
    __m256d mVec1 = _mm256_loadu_pd((const double *)Vec);
    __m256d mResult = _mm256_mul_pd(mVec, mVec1);

    double *Result = (double *)&mResult;
    double LengthSquared = Result[0] + Result[1] + Result[2];

    return LengthSquared;
}

double Vec3Length(vec3 *Vec)
{
    return sqrt(Vec3LengthSquared(Vec));
}
