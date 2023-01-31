
#include "ray.hpp"
#include "vec3.hpp"

point3 RayAtTime(ray *RayIn, double t)
{
    vec3 ScaledDirection = Vec3NewScaled(&RayIn->Direction, t);
    point3 PointInTime = {};
    Vec3Add(&RayIn->Origin, &ScaledDirection, &PointInTime);
    return PointInTime;
}