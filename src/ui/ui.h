#ifndef UI_H
#define UI_H

#ifdef OP_WIN
#include "windows/wui.h"
#elif OP_LIN
#include "ui/linux/lui.h"
#endif // OS Check

#include "../math/point.h"
#include "../colormap/color.h"
#include "../colormap/colormap.h"
#include "events.h"
#include "key_id.h"

/// @brief Generic window manager
typedef struct Window_t
{
    void* windowPointer;
}Window, *PWindow;

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

/// @brief Draw color map on window
/// @param window Generic window manager
/// @param start Start point on window
/// @param colorMap color map to be drawn
void WindowDrawColorMap(Window* window, Point2D start, ColorMap* colorMap);

/// @brief Set new event handler to window
/// @param window Generic window manager
/// @param newEvents New event handler of window
void WindowSetEvents(Window* window, WindowEvents* newEvents);

/// @brief Fills area of the same colored neighboor area of fillPoint's color
/// @param window Generic window manager
/// @param fillPoint Start point of filling
/// @param color Fill color
void WindowFill(Window* window, Point2D fillPoint, Color color);

#endif // UI_H