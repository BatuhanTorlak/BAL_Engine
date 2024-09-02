#include "colormap/colormap.h"
#include <malloc.h>
#include <math.h>
#include "math/point.h"
#include <stdio.h>

int RoundInt(float number)
{
    return (int)(number + .5f);
}

ColorMap* ColorMapCreate(const int width, const int height)
{
    ColorMap* _map = malloc(sizeof(ColorMap));
    _map->width = width;
    _map->height = height;
    _map->linearSize = width * height;
    Color* _clr = malloc(_map->linearSize * sizeof(Color));
    for (int x = 0; x < _map->linearSize; x++)
    {
        _clr[x] = ColorCreate(255, 255, 255);
    }
    _map->colors = _clr;
    _map->ratio = (float)width / (float)height;
    return _map;
}

void ColorMapDestroy(const ColorMap* colorMap)
{
    free(colorMap->colors);
    free(colorMap);
}

void ColorMapResize(ColorMap* colorMap, const int newWidth, const int newHeight)
{
    int _nW;
    int _nH;
    if (newWidth < colorMap->width)
        _nW = newWidth;
    else
        _nW = colorMap->width;
    if (newHeight < colorMap->height)
        _nH = newHeight;
    else
        _nH = colorMap->height;
    int newLinearSize = newWidth * newHeight;
    Color* _clr = malloc(newLinearSize * sizeof(Color));
    Color __color = {0};
    Color* __colorPtr = &__color;
    for (int y = 0; y < _nH; y++)
    {
        for (int x = 0; x < _nW; x++)
        {
            if (ColorMapGetPixelA(colorMap, x, y, __colorPtr))
            {
                _clr[y * newWidth + x] = __color;
            }
        }
    }
    Color* __ = colorMap->colors;
    colorMap->width = newWidth;
    colorMap->height = newHeight;
    colorMap->colors = _clr;
    colorMap->ratio = (float)newWidth / (float)newHeight;
    colorMap->linearSize = newLinearSize;
    free(__);
}

int ColorMapSetPixel(const register ColorMap* colorMap, const register Point2D position, const register Color color)
{
    if (colorMap == 0)
        return 0;
    if ((position.x | position.y) >= 0 && position.y < colorMap->height && position.x < colorMap->width)
    {
        ColorMapPixel(colorMap, position.x, position.y) = color;
        return 1;
    }
    return 0;
}

int ColorMapSetPixelA(const register ColorMap* colorMap, const register int x, const register int y, const register Color color)
{
    if (colorMap == 0)
        return 0;
    if ((x | y) >= 0 && y < colorMap->height && x < colorMap->width)
    {
        ColorMapPixel(colorMap, x, y) = color;
        return 1;
    }
    return 0;
}

int ColorMapGetPixel(const ColorMap* colorMap, const Point2D position, Color* colorOut)
{
    if (colorMap == 0)
        return 0;
    if ((position.x | position.y) >= 0 && position.y * colorMap->width + position.x < colorMap->linearSize)
    { 
        *colorOut = ColorMapPixel(colorMap, position.x, position.y);
        return 1;
    }
    return 0;
}

int ColorMapGetPixelA(const ColorMap* colorMap, const int x, const int y, Color* colorOut)
{
    if (colorMap == 0)
        return 0;
    if ((x | y) >= 0 && y * colorMap->width + x < colorMap->linearSize)
    { 
        *colorOut = ColorMapPixel(colorMap, x, y);
        return 1;
    }
    return 0;
}

void ColorMapDrawLine(const ColorMap* colorMap, Point2D start, Point2D end, const Color color)
{
    ColorMapDrawLineA(colorMap, start.x, start.y, end.x, end.y, color);
}

void ColorMapDrawLineA(const ColorMap* colorMap, int startX, int startY, int endX, int endY, const Color color)
{
    int _a = endX - startX;
    int _b = endY - startY;
    if ((_a | _b) == 0)
        return;
    int _diff;
    if (abs(_a) > abs(_b))
    {
        if (_a > 0)
        {
            if (_b > 0)
            {
                _diff = _a / _b;
                for (int x = 0; x < _b; x++)
                {
                    int y = 0;
                    for (; y < _diff; y++)
                    {
                        ColorMapSetPixelA(colorMap, startX + y, startY + x, color);
                    }
                    startX += y;
                }
                return;
            }
            return;
        }
        return;
    }
}

void ColorMapClear(const ColorMap* colorMap)
{
    for (int x = 0; x < colorMap->width; x++)
    {
        for (int y = 0; y < colorMap->height; y++)
        {
            ColorMapSetPixelA(colorMap, x, y, ColorCreate(255, 255, 255));
        }
    }
}