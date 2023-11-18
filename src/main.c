#define OP_WIN
#include "ui/ui.h"
#include <stdio.h>
#include <math.h>

unsigned short _oX = 300;
unsigned short _oY = 130;
unsigned short x = 0;
unsigned short y = 0;

void TestUpdate(const WinWindow* win)
{
    x += (unsigned short)rand();
    y += (unsigned short)rand();
    WinWindowDrawLine(win, Point2DCreate(_oX % win->width, _oY % win->height), Point2DCreate(x % 200, y % 200), ColorCreate(255, 0, 0));
    _oX = x;
    _oY = y;
}

int main()
{
    WinWindow* win = WinWindowCreate(L"test", L"test", 600, 260);

    WindowEvents* events = WindowEventsCreate();
    events->update = TestUpdate;
    WinWindowSetEvents(win, events);

    while (WinWindowUpdate(&win, 120))
    {
    }

    WinWindowDestroy(&win);
    WindowEventsDestroy(events);

    return EXIT_SUCCESS;
}