#include "math.h"

float Roundf(float number)
{
    return (float)(int)(number + .5f);
}

float Absf(float number)
{
    int _i = *((int*)(&number));
    _i = _i & ~(1 << 31);
    return *(float*)(&_i);
}

float Clampf(const float number, const float min, const float max) {
    if (number > max)
        return max;
    if (number < min)
        return min;
    return number;
}

int AbsInt(int number)
{
    if (number >= 0)
        return number;
    return -number;
}

int RoundInt(float number)
{
    return (int)(number + .5f);
}