#ifndef UI_H
#define UI_H

#ifdef OP_WIN
#include "windows/wui.h"
#elif OP_LIN
#include "linux/lui.h"
#endif // OP Check

#include "math/point.h"
#include "colormap/color.h"

/// @brief Generic window manager
typedef struct Window_t
{
    void* windowPointer;
}Window;

/// @brief Window constructer
/// @return Generic window manager
Window* WindowCreate();

/// @brief Window destructer
/// @param window Window to be destructed
void WindowDestroy(Window* window);

/// @brief Draws line on window
/// @param window Generic window manager
/// @param start Start point on window
/// @param end End point on window
/// @param color Color of line
void WindowDrawLine(Window* window, Point2D start, Point2D end, Color color);

/// @brief Fills area of the same colored neighboor area of fillPoint's color
/// @param window Generic window manager
/// @param fillPoint Start point of filling
/// @param color Fill color
void WindowFill(Window* window, Point2D fillPoint, Color color);

#endif // UI_H