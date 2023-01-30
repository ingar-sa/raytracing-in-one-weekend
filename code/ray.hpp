#ifndef RAY_H
#define RAY_H

#include "vec3.hpp"

struct ray
{
    point3 Origin;
    point3 Direction;
};

point3 RayAtTime(ray *RayIn, double t);

#endif
