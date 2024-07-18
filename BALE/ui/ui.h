#ifndef WUI_H
#define WUI_H
#include <Windows.h>
#include "../math/point.h"
#include "../colormap/color.h"
#include "../colormap/colormap.h"
#include "../ui/events.h"
#include "../ui/component.h"

/// @brief Generic window manager
typedef void *Window, **PWindow;

/// @brief Window constructer
/// @return Generic window manager
Window WindowCreate();

/// @brief Window constructer
/// @param name [IN] Name of the window in 16 bits
/// @return Generic window manager
Window WindowCreateA(const short* name);

/// @brief Window constructer
/// @param name [IN] Name of the window in 16 bits
/// @param xPos [IN] X coordinate of window's position
/// @param yPos [IN] Y coordinate of window's position
/// @param width [IN] Width of the window
/// @param height [IN] Height of the window
/// @return Generic window manager
Window WindowCreateB(const short* name, int xPos, int yPos, int width, int height);

/// @brief Window destructer
/// @param window [IN, OUT] Window to be destructed
void WindowDestroy(const Window* window);

/// @brief Set pixel on window
/// @param window [IN] Generic window manager
/// @param point [IN] Point to drawn on window
/// @param color [IN] Color of line
void WindowSetPixel(const Window window, Point2D point, Color color);

/// @brief Set pixel on window
/// @param window [IN] Generic window manager
/// @param xPos [IN] X coordinate of point to drawn on window
/// @param yPos [IN] Y coordinate of point to drawn on window
/// @param color [IN] Color of line
void WindowSetPixelA(const Window window, int xPos, int yPos, Color color);

/// @brief Draws line on window
/// @param window [IN] Generic window manager
/// @param start [IN] Start point on window
/// @param end [IN] End point on window
/// @param color [IN] Color of line
void WindowDrawLine(const Window window, Point2D start, Point2D end, Color color);

/// @brief Draws line on window
/// @param window [IN] Generic window manager
/// @param startX [IN] X coordinate of start point on window
/// @param startY [IN] Y coordinate of start point on window
/// @param endX [IN] X coordinate of end point on window
/// @param endY [IN] Y coordinate of end point on window
/// @param color [IN] Color of line
void WindowDrawLineA(const Window window, int startX, int startY, int endX, int endY, Color color);

/// @brief Sets all the pixel on the window to black(0, 0, 0)
/// @param window [IN] Generic window manager
void WindowClear(const Window window);

/// @brief Set all the pixel on the window to the chosen color
/// @param window [IN] Generic window manager
/// @param color [IN] Chosen color
void WindowClearA(const Window window, Color color);

void WindowSetPosition(const Window window, Point2D position);
void WindowSetPositionA(const Window window, int xPos, int yPos);

void WindowSetSize(const Window window, Point2D size);
void WindowSetSizeA(const Window window, int xSize, int ySize);

/// @brief Set new event handler to window
/// @param window [IN] Generic window manager
/// @param newEvents [IN] New event handler of window
void WindowSetEvents(Window window, WindowEvents newEvents);

/// @brief Add new component to window
/// @param window [IN] Generic window manager
/// @param newComponent [IN] New component for window
void WindowAddComponent(Window window, Component* newComponent);

/// @brief Add new component to window
/// @param window [IN] Generic window manager
/// @param oldComponent [IN] Old component for window (As a joke)
void WindowRemoveComponent(Window window, Component* oldComponent);

/// @brief Updates window's screen
/// @param window [IN] Generic window manager
void WindowRender(const Window window);

/// @brief Checks if program is still running or not, runs specific events, updates window's screen and makes program wait for (1second / FPS)
/// @param window [IN] Generic window manager
/// @param FPS [IN] Frames that should rendered in one second
char WindowUpdate(const Window window, int FPS);

/// @brief Fills area of the same colored neighboor area of fillPoint's color
/// @param window [IN] Generic window manager
/// @param fillPoint [IN] Start point of filling
/// @param color [IN] Fill color
void WindowFill(const Window window, Point2D fillPoint, Color color);

#endif // WUI_H