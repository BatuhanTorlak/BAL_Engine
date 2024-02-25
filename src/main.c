#define OP_WIN
#include "ui/ui.h"
#include <stdio.h>
#include <math.h>
#include "math/math.h"

unsigned short _oX = 300;
unsigned short _oY = 130;
unsigned short x = 0;
unsigned short y = 0;

void TestUpdate(const WinWindow* win)
{
    x += (unsigned short)rand();
    y += (unsigned short)rand();
    ColorMapClear(win->colorMap);
    WinWindowDrawLine(win, Point2DCreate(_oX % win->width, _oY % win->height), Point2DCreate(x % 200, y % 200), ColorCreate(x % 255, y % 255, (x + y) % 255));
    printf("x1: %i, y1: %i, x2: %i, y2: %i\n", _oX % win->width, _oY % win->height, x % 200, y % 200);
    Sleep(1000);
    _oX = x;
    _oY = y;
}

int main()
{
    printf("sqrt(2) = %f\n", Sqrtf(2));

    WinWindow* win = WinWindowCreate(L"test", L"test", 623, 280);

    PWinWindow window = &win;
    
    WindowEvents* events = WindowEventsCreate();
    events->update = TestUpdate;
    WinWindowSetEvents(win, events);
    
    WinWindowDrawLine(win, Point2DCreate(10, 10), Point2DCreate(100, 100), ColorCreate(255, 0, 255));

    while (WinWindowUpdate(win, 120))
    {
    }

    WinWindowDestroy(window);
    WindowEventsDestroy(events);

    return EXIT_SUCCESS;
}