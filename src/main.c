#define OP_WIN
#include "ui/ui.h"
#include "math/vector2.h"
#include <stdio.h>
#include <math.h>

void TestUpdate(const WinWindow* win)
{
}

int _x = -1;
int _y = -1;

void TestOnClick(const WinWindow* win, int xPos, int yPos)
{
    if (_x & _y == -1)
    {
        _x = xPos;
        _y = yPos;
        return;
    }
    WinWindowDrawLineA(win, _x, _y, xPos, yPos, ColorCreate(0, 0, 255));
    _x = xPos;
    _y = yPos;
}

int main()
{
    WinWindow* win = WinWindowCreate(L"test", L"test", 623, 280);

    PWinWindow* window = &win;
    
    WindowEvents* events = WindowEventsCreate();
    events->Update = TestUpdate;
    events->OnClick = TestOnClick;
    WinWindowSetEvents(win, events);
    
    WinWindowDrawLine(win, Point2DCreate(10, 10), Point2DCreate(100, 100), ColorCreate(255, 0, 255));

    while (WinWindowUpdate(win, 120))
    {
    }

    WinWindowDestroy(window);
    WindowEventsDestroy(events);

    return EXIT_SUCCESS;
}