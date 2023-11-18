#include "math/vector3.h"
#include "math/math.h"

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