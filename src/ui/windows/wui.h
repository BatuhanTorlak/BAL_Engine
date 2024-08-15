#ifndef WUI_H
#define WUI_H
#include <Windows.h>
#include "math/point.h"
#include "colormap/color.h"
#include "colormap/colormap.h"
#include "ui/events.h"
#include "ui/component.h"

typedef struct WinWindow_t
{
    WNDCLASSW windowClass;
    HWND windowHandler;
    HANDLE threadHandler;
    HDC drawingContentHandler;
    PColorMap colorMap;
    WindowEvents events;
    struct Drawing_t
    {
	int renderOrder;
        Color* bitmapColors;
        HBITMAP bitmap;
        HDC bitmapDrawingContentHandler;
    } drawing;
    Point2D position;
    int width;
    int height;
    char isAlive;
    struct ComponentManager_t
    {
        Component** components;
        int componentsCount;
        int componentsCapacity;
        Component* focusedComponent;
    } componentManager;
}WinWindow;

/// @brief Window constructer
/// @return Generic window manager
WinWindow* WindowCreate();

/// @brief Window constructer
/// @param name Name of the window in 16 bits
/// @return Generic window manager
WinWindow* WindowCreateA(const short* name);

/// @brief Window constructer
/// @param name Name of the window in 16 bits
/// @param xPos X coordinate of window's position
/// @param yPos Y coordinate of window's position
/// @param xSize X element of window's size
/// @param ySize Y element of window's size
/// @return Generic window manager
WinWindow* WindowCreateB(const short* name, int xPos, int yPos, int xSize, int ySize);

/// @brief Window destructer
/// @param window Window to be destructed
void WindowDestroy(const WinWindow** window);

/// @brief Set pixel on window
/// @param window Generic window manager
/// @param point Point to drawn on window
/// @param color Color of line
void WindowSetPixel(const WinWindow* window, Point2D point, Color color);

/// @brief Set pixel on window
/// @param window Generic window manager
/// @param xPos X coordinate of point to drawn on window
/// @param yPos Y coordinate of point to drawn on window
/// @param color Color of line
void WindowSetPixelA(const WinWindow* window, int xPos, int yPos, Color color);

/// @brief Draws line on window
/// @param window Generic window manager
/// @param start Start point on window
/// @param end End point on window
/// @param color Color of line
void WindowDrawLine(const WinWindow* window, Point2D start, Point2D end, Color color);

/// @brief Draws line on window
/// @param window Generic window manager
/// @param startX X coordinate of start point on window
/// @param startY Y coordinate of start point on window
/// @param endX X coordinate of end point on window
/// @param endY Y coordinate of end point on window
/// @param color Color of line
void WindowDrawLineA(const WinWindow* window, int startX, int startY, int endX, int endY, Color color);

void WindowClear(const WinWindow* window);
void WindowClearA(const WinWindow* window, Color color);

void WindowSetPosition(const WinWindow* window, Point2D position);
void WindowSetPositionA(const WinWindow* window, int xPos, int yPos);

void WindowSetSize(const WinWindow* window, Point2D size);
void WindowSetSizeA(const WinWindow* window, int xSize, int ySize);

/// @brief Set new event handler to window
/// @param window Generic window manager
/// @param newEvents New event handler of window
void WindowSetEvents(WinWindow* window, WindowEvents newEvents);

/// @brief Add new component to window
/// @param window Generic window manager
/// @param newComponent New component for window
void WindowAddComponent(WinWindow* window, Component* newComponent);

/// @brief Add new component to window
/// @param window Generic window manager
/// @param oldComponent Old component for window (As a joke)
void WindowRemoveComponent(WinWindow* window, Component* oldComponent);

/// @brief Updates window's screen
/// @param window Generic window manager
void WindowRender(const WinWindow* window);

/// @brief Checks if program is still running or not, runs specific events, updates window's screen and makes program wait for (1second / FPS)
/// @param window Generic window manager
/// @param FPS Frames that should rendered in one second
char WindowUpdate(const WinWindow* window, int FPS);

/// @brief Fills area of the same colored neighboor area of fillPoint's color
/// @param window Generic window manager
/// @param fillPoint Start point of filling
/// @param color Fill color
void WindowFill(const WinWindow* window, Point2D fillPoint, Color color);

#endif // WUI_H
