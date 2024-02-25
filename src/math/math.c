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

float Sqrtf(float number)
{
    number--;
    float num = 1;
    float sqrtNum = 1;
    float _a = 1;
    float _b = 1;
    float _c = 1;
    for (int x = 1; x <= 16; x++)
    {
        register float __ = 2 * x;
        _a *= __ * (__ - 1);
        _b *= x;
        _c *= 4;
        num *= number;
        register float _ = (num * _a) / (float)((1 - 2 * x) * _b * _b * _c);        
        printf("(%f * %f) / ((1 - 2 * %i) * %f * %f) = %f\n", num, _a, x, _b * _b, _c, _);
        if (x & 0b1)
        {
            sqrtNum -= _;
            continue;
        }
        sqrtNum += _;
    }
    return sqrtNum;
}