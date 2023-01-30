
#include "ray.hpp"
#include "vec3.hpp"

point3 RayAtTime(ray RayIn, double t)
{
    vec3 ScaledDirection = Vec3NewScaled(&RayIn.Direction, t);
    point3 PointInTime = {};
    Vec3Add(&RayIn.Origin, &ScaledDirection, &PointInTime);
    return PointInTime;
}

// class ray {
//     public:
//         ray() {}
//         ray(const point3& origin, const vec3& direction)
//             : orig(origin), dir(direction)
//         {}

//         point3 origin() const  { return orig; }
//         vec3 direction() const { return dir; }

//         point3 at(double t) const {
//             return orig + t*dir;
//         }

//     public:
//         point3 orig;
//         vec3 dir;
// };
