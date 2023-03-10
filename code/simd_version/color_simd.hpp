#ifndef COLOR_H
#define COLOR_H

#include "vec3.hpp"

#include <stdio.h>

#define PIXEL_BOUND 255.999

inline void WriteColor(FILE *File, const color *Color)
{
    fprintf(File, "%d %d %d\n",
            (int)(Color->X * PIXEL_BOUND),
            (int)(Color->Y * PIXEL_BOUND),
            (int)(Color->Z * PIXEL_BOUND));
}

#endif // COLOR_H