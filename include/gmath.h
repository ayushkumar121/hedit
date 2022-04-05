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

typedef union Vec2
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
} Vec2;

typedef union Vec3
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
} Vec3;

typedef union Vec4
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
} Vec4;

/* Vector operations */
Vec2 AddV2(Vec2 a, Vec2 b);
Vec4 AddV4(Vec4 a, Vec4 b);

#endif /* GMATH_H */