#include "colormap/colormap.h"
#include <windows.h>
#include <math.h>
#include "math/point.h"
#include "win_defs.h"
#define START_MUT(map) WaitForSingleObject(map->reserved, INFINITE)
#define END_MUT(map) ReleaseMutex(map->reserved)

void ColorMapSetPixelPrivate(ColorMap* colorMap, int x, int y, Color color);
int ColorMapGetPixelPrivate(ColorMap* colorMap, int x, int y, Color* colorOut);

ColorMap* ColorMapCreate(const int width, const int height)
{
    ColorMap* _map = BAL_MALLOC(sizeof(ColorMap));
    _map->width = width;
    _map->height = height;
    _map->linearSize = width * height;
    Color* _clr = BAL_MALLOC_0(_map->linearSize * sizeof(Color));
    _map->colors = _clr;
    _map->ratio = (float)width / (float)height;
    _map->reserved = CreateMutex(0, 0, 0);
    return _map;
}

void ColorMapDestroy(const ColorMap* colorMap)
{
    CloseHandle(colorMap->reserved);
    BAL_FREE(colorMap->colors);
    BAL_FREE(colorMap);
}

void ColorMapResize(ColorMap* colorMap, const int newWidth, const int newHeight)
{
    START_MUT(colorMap);
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
    Color* _clr = BAL_MALLOC(newLinearSize * sizeof(Color));
    Color __color = {0};
    Color* __colorPtr = &__color;
    for (int y = 0; y < _nH; y++)
    {
        for (int x = 0; x < _nW; x++)
        {
            if (ColorMapGetPixelPrivate(colorMap, x, y, __colorPtr))
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
    BAL_FREE(__);
    END_MUT(colorMap);
}

int ColorMapSetPixel(const register ColorMap* colorMap, const register Point2D position, const register Color color)
{
    START_MUT(colorMap);
    if (colorMap == 0)
        return 0;
    if ((position.x | position.y) >= 0 && position.y < colorMap->height && position.x < colorMap->width)
    {
        ColorMapPixel(colorMap, position.x, position.y) = color;
        END_MUT(colorMap);
        return 1;
    }
    END_MUT(colorMap);
    return 0;
}

int ColorMapSetPixelA(const register ColorMap* colorMap, const register int x, const register int y, const register Color color)
{
    START_MUT(colorMap);
    if (colorMap == 0)
        return 0;
    if ((x | y) >= 0 && y < colorMap->height && x < colorMap->width)
    {
        ColorMapPixel(colorMap, x, y) = color;
        END_MUT(colorMap);
        return 1;
    }
    END_MUT(colorMap);
    return 0;
}

void ColorMapSetPixelPrivate(ColorMap* colorMap, int x, int y, Color color)
{
    if ((x | y) >= 0 && y < colorMap->height && x < colorMap->width)
        ColorMapPixel(colorMap, x, y) = color;
}

int ColorMapGetPixel(const ColorMap* colorMap, const Point2D position, Color* colorOut)
{
    START_MUT(colorMap);
    if (colorMap == 0)
        return 0;
    if ((position.x | position.y) >= 0 && position.y * colorMap->width + position.x < colorMap->linearSize)
    { 
        *colorOut = ColorMapPixel(colorMap, position.x, position.y);
        END_MUT(colorMap);
        return 1;
    }
    END_MUT(colorMap);
    return 0;
}

int ColorMapGetPixelA(const ColorMap* colorMap, const int x, const int y, Color* colorOut)
{
    START_MUT(colorMap);
    if (colorMap == 0)
        return 0;
    if ((x | y) >= 0 && y * colorMap->width + x < colorMap->linearSize)
    { 
        *colorOut = ColorMapPixel(colorMap, x, y);
        END_MUT(colorMap);
        return 1;
    }
    END_MUT(colorMap);
    return 0;
}

int ColorMapGetPixelPrivate(ColorMap* colorMap, int x, int y, Color* colorOut)
{
    if ((x | y) >= 0 && y < colorMap->height && x < colorMap->width)
    {
        *colorOut = ColorMapPixel(colorMap, x, y);
        return 1;
    }
    return 0;
}

void ColorMapDrawLine(const ColorMap* colorMap, Point2D start, Point2D end, const Color color)
{
    START_MUT(colorMap);
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
            ColorMapSetPixelPrivate(colorMap, (int)(sX * n + m + .5f), sX, color);
        }
        END_MUT(colorMap);
        return;
    }
    for (; sX < eX; sX++)
    {
        ColorMapSetPixelPrivate(colorMap, sX, (int)(sX * n + m + .5f), color);
    }
    END_MUT(colorMap);
}

void ColorMapDrawLineA(const ColorMap* colorMap, int startX, int startY, int endX, int endY, const Color color)
{
    ColorMapDrawLine(colorMap, Point2DCreate(startX, startY), Point2DCreate(endX, endY), color);
}

void ColorMapClear(const ColorMap* colorMap)
{
    START_MUT(colorMap);
    BAL_MEMSET(colorMap->colors, 255, colorMap->linearSize);
    END_MUT(colorMap);
}