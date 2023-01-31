#ifndef RAY_H
#define RAY_H

#include "vec3.hpp"

struct ray
{
    point3 Origin;
    point3 Direction;
};

point3 RayAtTime(ray *RayIn, double t)
{
    vec3 ScaledDirection = Vec3NewScaled(&RayIn->Direction, t);
    point3 PointInTime = {};
    Vec3Add(&RayIn->Origin, &ScaledDirection, &PointInTime);
    return PointInTime;
}

#endif
