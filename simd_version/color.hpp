#ifndef COLOR_H
#define COLOR_H

#include "vec3.hpp"

#include <iostream>

#define PIXEL_BOUND 255.999

inline void WriteColor(std::ostream &out, const color *Color)
{
    // Write the translated [0,255] value of each color component.
    out << static_cast<int>(Color->X * PIXEL_BOUND) << ' '
        << static_cast<int>(Color->Y * PIXEL_BOUND) << ' '
        << static_cast<int>(Color->Z * PIXEL_BOUND) << '\n';
}

#endif // COLOR_H