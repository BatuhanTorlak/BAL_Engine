#include "math/vector2.h"
#include "math/math.h"

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