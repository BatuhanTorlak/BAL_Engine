#ifndef COLORMAP_H
#define COLORMAP_H

#include "color.h"
#include "../math/point.h"

#define ColorMapPixel(map, x, y) ((map)->colors[(y) * (map)->width + (x)])
#define ColorMapPixelA(map, x) map->colors[x]

/// @brief Two dimensional color storage and graphical structer
typedef struct ColorMap_t
{
    Color* colors;
    int width;
    int height;
    int linearSize;
    float ratio;
} 
/// @brief Two dimensional color storage and graphical structer
ColorMap, *PColorMap;

/// @brief Color map contructer, must destructed after usage
/// @param width Width of color map
/// @param height Height of color map
/// @return Color map created in heap
ColorMap* ColorMapCreate(const int width, const int height);

/// @brief Color map destructer
/// @param colorMap The pointer of color map created (must be non 0)
void ColorMapDestroy(const ColorMap* colorMap);

void ColorMapResize(ColorMap* colorMap, const int newWidth, const int newHeight);

int ColorMapSetPixel(const ColorMap* colorMap, const Point2D position, const register Color color);

int ColorMapSetPixelA(const register ColorMap* colorMap, const register int x, const register int y, const register Color color);

int ColorMapGetPixel(const ColorMap* colorMap, const Point2D position, Color* colorOut);

int ColorMapGetPixelA(const ColorMap* colorMap, const int x, const int y, Color* colorOut);

/// @brief Draws direct line on color map from start point to end point with selected color
/// @param colorMap The pointer of color map
/// @param start Start point of line
/// @param end End point of line
/// @param color Color of line
void ColorMapDrawLine(const ColorMap* colorMap, Point2D start, Point2D end, const Color color);

void ColorMapDrawLineA(const ColorMap* colorMap, int startX, int startY, int endX, int endY, const Color color);

void ColorMapFill(const ColorMap* colorMap, const Point2D position, const Color color);

void ColorMapClear(const ColorMap* colorMap);

void ColorMapClearA(const ColorMap* colorMap, const Color color);

#endif