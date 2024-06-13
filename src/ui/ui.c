#define OP_WIN
#define BAL_DEFAULT_NAME L"BAL Engine"
#define BAL_DEFAULT_X_POS 10
#define BAL_DEFAULT_Y_POS 10
#define BAL_DEFAULT_X_SIZE 600
#define BAL_DEFAULT_Y_SIZE 280
#include "ui.h"

#ifdef OP_WIN
#define BAL_DEFAULT_NAME_WIN L"BALENGINE"
Window* WindowCreate()
{
    WinWindow* window = WinWindowCreate(BAL_DEFAULT_NAME, BAL_DEFAULT_NAME_WIN, BAL_DEFAULT_X_SIZE, BAL_DEFAULT_Y_SIZE);
    return window;
}

Window* WindowCreateA(const short* name)
{
    WinWindow* window = WinWindowCreate(name, name, BAL_DEFAULT_X_SIZE, BAL_DEFAULT_Y_SIZE);
    return window;
}

Window* WindowCreateB(const short* name, int xPos, int yPos, int xSize, int ySize)
{
    WinWindow* window = WinWindowCreate(name, name, xSize, ySize);
    WinWindowSetPositionA(window, xPos, yPos);
    return window;
}

void WindowSetPixel(Window* window, Point2D point, Color color)
{
    WinWindowSetPixel(window, point, color);
}

void WindowSetPixelA(Window* window, int xPos, int yPos, Color color)
{
    WinWindowSetPixelA(window, xPos, yPos, color);
}

void WindowDrawLine(Window* window, Point2D start, Point2D end, Color color)
{
    WinWindowDrawLine(window, start, end, color);
}

void WindowDrawLineA(Window* window, int startX, int startY, int endX, int endY, Color color)
{
    WinWindowDrawLineA(window, startX, startY, endX, endY, color);
}

void WindowSetEvents(Window* window, WindowEvents* newEvents)
{
    WinWindowSetEvents(window, newEvents);
}

void WindowAddComponent(Window* window, Component* newComponent)
{
    WinWindowAddComponent(window, newComponent);
}

void WindowRemoveComponent(Window* window, Component* oldComponent)
{
    WinWindowRemoveComponent(window, oldComponent);
}

void WindowRender(Window* window)
{
    WinWindowRender(window);
}

char WindowUpdate(Window* window, int FPS)
{
    return WinWindowUpdate(window, FPS);
}

void WindowDestroy(Window** window)
{
    WinWindowDestroy(window);
}
#else

#endif