#include "colormap.h"
#include <malloc.h>
#include <math.h>
#include "../math/point.h"
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

void ColorMapSetPixel(const register ColorMap* colorMap, const register Point2D position, const register Color color)
{
    if (colorMap == 0)
        return;
    if ((position.x | position.y) >= 0 && position.y * colorMap->width + position.x < colorMap->linearSize)
        ColorMapPixel(colorMap, position.x, position.y) = color;
}

void ColorMapSetPixelA(const register ColorMap* colorMap, const register int x, const register int y, const register Color color)
{
    if (colorMap == 0)
        return;
    if ((x | y) >= 0 && y * colorMap->width + x < colorMap->linearSize)
        ColorMapPixel(colorMap, x, y) = color;
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
    int _a = end.x - start.x;
    int _b = end.y - start.y;
    const int _w = colorMap->width;
    const int _h = colorMap->height;
    int sX;
    int eX;
    const char _check = abs(_b) > abs(_a);
    if (_check)
    {
        register int _s = start.x;
        start.x = start.y;
        start.y = _s;
        _s = end.x;
        end.x = end.y;
        end.y = _s;
        _s = _a;
        _a = _b;
        _b = _s;
    }
    if (start.x < end.x)
    {
        sX = start.x;
        eX = end.x;
    }
    else
    {
        sX = end.x;
        eX = start.x;
    }
    const register float n = (float)_b / (float)_a;
    const register float m = start.y - start.x * n;
    if (_check)
    {
        for (; sX < eX; sX++)
        {
            ColorMapSetPixelA(colorMap, RoundInt(sX * n + m), sX, color);
        }
        return;
    }
    for (; sX < eX; sX++)
    {
        ColorMapSetPixelA(colorMap, sX, RoundInt(sX * n + m), color);
    }
}

void ColorMapClear(const ColorMap* colorMap)
{
    for (int x = 0; x < colorMap->width; x++)
    {
        for (int y = 0; y < colorMap->height; y++)
        {
            ColorMapPixel(colorMap, x, y) = ColorCreate(255, 255, 255);
        }
    }
}