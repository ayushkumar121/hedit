/*
 * Goal of this file is to provide a consitent set of types
 * and math operations so i won't have to use types from multiple 
 * places instead i could use types from just one place
 */

#ifndef GMATH_H
#define GMATH_H

#include <stdint.h>

typedef int Int;
typedef char Char;
typedef float Float;

typedef unsigned int Uint;
typedef unsigned char Uchar;

typedef union V2
{
    struct
    {
        float x, y;
    };
    struct
    {
        float u, v;
    };
    struct
    {
        float width, height;
    };
    float data[2];
} V2;

typedef union V3
{
    struct
    {
        float x, y, z;
    };
    struct
    {
        float r, g, b;
    };
    float data[3];
} V3;

typedef union V4
{
    struct
    {
        float x, y, z, w;
    };
    struct
    {
        float r, g, b, a;
    };
    float data[4];
} V4;

/* Vector operations */
void addV4(V4 a, V4 b, V4 *out);

#endif /* GMATH_H */