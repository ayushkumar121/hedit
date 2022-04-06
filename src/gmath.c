#include <gmath.h>

Vec2 V2(float x, float y)
{
    return (Vec2) {x, y};
}

/* Vector operations */

Vec2 AddV2(Vec2 a, Vec2 b)
{
    return (Vec2){
        a.x + b.x,
        a.y + b.y,
    };
}

Vec4 AddV4(Vec4 a, Vec4 b)
{
    return (Vec4){
        a.x + b.x,
        a.y + b.y,
        a.z + b.z,
        a.w + b.w,
    };
}