#ifndef COLOR_H
#define COLOR_H

#include <stdio.h>

#include "vec3.hpp"

#define PIXEL_BOUND 255.999

inline void WriteColor(char ColorBuffer[15], const color *Color)
{
    sprintf(ColorBuffer, "%d %d %d\n",
            (int)(Color->X * PIXEL_BOUND),
            (int)(Color->Y * PIXEL_BOUND),
            (int)(Color->Z * PIXEL_BOUND));
}

#endif // COLOR_H