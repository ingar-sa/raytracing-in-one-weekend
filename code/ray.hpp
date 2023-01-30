#ifndef RAY_H
#define RAY_H

#include "vec3.hpp"

struct ray
{
    point3 Origin;
    point3 Direction;
};

void RayAtTime(ray RayIn, double t,  point3 *PointIntime);

#endif
