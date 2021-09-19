#include <gmath.h>

/* Vector operations */

/* V4 + V4 */
void addV4(V4 a, V4 b, V4 *out)
{
    out->x = a.x + b.x;
    out->y = a.y + b.y;
    out->z = a.z + b.z;
    out->w = a.w + b.w;
}