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
#include "component.h"
#include "events.h"
#include "key_id.h"

/// @brief Generic window manager
typedef void* Window, *PWindow;

/// @brief Window constructer
/// @return Generic window manager
Window* WindowCreate();

/// @brief Window constructer
/// @param name Name of the window in 16 bits
/// @return Generic window manager
Window* WindowCreateA(const short* name);

/// @brief Window constructer
/// @param name Name of the window in 16 bits
/// @param xPos X coordinate of window's position
/// @param yPos Y coordinate of window's position
/// @param xSize X element of window's size
/// @param ySize Y element of window's size
/// @return Generic window manager
Window* WindowCreateB(const short* name, int xPos, int yPos, int xSize, int ySize);

/// @brief Window destructer
/// @param window Window to be destructed
void WindowDestroy(Window** window);

/// @brief Set pixel on window
/// @param window Generic window manager
/// @param point Point to drawn on window
/// @param color Color of line
void WindowSetPixel(Window* window, Point2D point, Color color);

/// @brief Set pixel on window
/// @param window Generic window manager
/// @param xPos X coordinate of point to drawn on window
/// @param yPos Y coordinate of point to drawn on window
/// @param color Color of line
void WindowSetPixelA(Window* window, int xPos, int yPos, Color color);

/// @brief Draws line on window
/// @param window Generic window manager
/// @param start Start point on window
/// @param end End point on window
/// @param color Color of line
void WindowDrawLine(Window* window, Point2D start, Point2D end, Color color);

/// @brief Draws line on window
/// @param window Generic window manager
/// @param startX X coordinate of start point on window
/// @param startY Y coordinate of start point on window
/// @param endX X coordinate of end point on window
/// @param endY Y coordinate of end point on window
/// @param color Color of line
void WindowDrawLineA(Window* window, int startX, int startY, int endX, int endY, Color color);

/// @brief Set new event handler to window
/// @param window Generic window manager
/// @param newEvents New event handler of window
void WindowSetEvents(Window* window, WindowEvents* newEvents);

/// @brief Add new component to window
/// @param window Generic window manager
/// @param newComponent New component for window
void WindowAddComponent(Window* window, Component* newComponent);

/// @brief Add new component to window
/// @param window Generic window manager
/// @param oldComponent Old component for window (As a joke)
void WindowRemoveComponent(Window* window, Component* oldComponent);

/// @brief Updates window's screen
/// @param window Generic window manager
void WindowRender(Window* window);

/// @brief Checks if program is still running or not, runs specific events, updates window's screen and makes program wait for (1second / FPS)
/// @param window Generic window manager
/// @param newComponent New component for window
char WindowUpdate(Window* window, int FPS);

/// @brief Fills area of the same colored neighboor area of fillPoint's color
/// @param window Generic window manager
/// @param fillPoint Start point of filling
/// @param color Fill color
void WindowFill(Window* window, Point2D fillPoint, Color color);

#endif // UI_H