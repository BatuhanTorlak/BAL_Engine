#include "math/vector.h"
#include <math.h>

/// @brief Distance between start to end in two dimension
/// @param start Start point
/// @param end End point
/// @return Distance
float Vector2Distance(Vector2 start, Vector2 end)
{
    end = Vector2Sub(end, start);
    end = Vector2Mul(end, end);
    return sqrtf(end.x + end.y);
}

/// @brief Distance between start to end in three dimension
/// @param start Start point
/// @param end End point
/// @return Distance
float Vector3Distance(Vector3 start, Vector3 end)
{
    end = Vector3Sub(end, start);
    end = Vector3Mul(end, end);
    return sqrtf(end.x + end.y + end.z);
}