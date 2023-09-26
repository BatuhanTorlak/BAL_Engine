#ifndef COLORMAP_H
#define COLORMAP_H

#include "colormap/color.h"
#include "math/point.h"

#define ColorMapPixel(map, x, y) map->colors[y * map->width + x]
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
ColorMap;

/// @brief Color map contructer, must destructed after usage
/// @param width Width of color map
/// @param height Height of color map
/// @return Color map created in heap
ColorMap* ColorMapCreate(int width, int height);

/// @brief Color map destructer
/// @param colorMap The pointer of color map created (must be non 0)
void ColorMapDestroy(ColorMap* colorMap);

/// @brief Draws direct line on color map from start point to end point with selected color
/// @param colorMap The pointer of color map
/// @param start Start point of line
/// @param end End point of line
/// @param color Color of line
void ColorMapDrawLine(ColorMap* colorMap, Point2D start, Point2D end, Color color);

/// @brief Saves as a bitmap(.bmp) file on selected location (name of file contains name, example: "./test.bmp")
/// @param colorMap The pointer of color map
/// @param location Location of file
void ColorMapSave(ColorMap* colorMap, const char* location);

#endif