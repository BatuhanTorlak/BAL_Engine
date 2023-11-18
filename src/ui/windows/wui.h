#ifndef WUI_H
#define WUI_H
#include <Windows.h>
#include "../../math/point.h"
#include "../../colormap/color.h"
#include "../../colormap/colormap.h"
#include "../events.h"

typedef struct WinWindow_t
{
    WNDCLASSW windowClass;
    HWND windowHandler;
    HANDLE threadHandler;
    HDC drawingContentHandler;
    PColorMap colorMap;
    WindowEvents* events;
    int width;
    int height;
    char isAlive;
}WinWindow, *PWinWindow;

WinWindow* WinWindowCreate(const wchar_t* className, const wchar_t* windowName, int width, int height);
void WinWindowSetPixel(const WinWindow* win, const Point2D position, const Color color);
void WinWindowSetPixelA(const WinWindow* win, const int xPos, const int yPos, const Color color);
void WinWindowDrawLine(const WinWindow* win, Point2D start, Point2D end, const Color color);
void WinWindowDrawLineA(const WinWindow* win, int x1, int y1, int x2, int y2, const Color color);
void WinWindowSetEvents(WinWindow* win, WindowEvents* newEvents);
void WinWindowRender(const WinWindow* win);
char WinWindowUpdate(const WinWindow* win, const int FPS);
void WinWindowDestroy(const WinWindow* win);

#endif // WUI_H