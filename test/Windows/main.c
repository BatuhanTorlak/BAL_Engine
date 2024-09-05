#include <stdio.h>

#include "bale.h"

void TestUpdate(const Window win)
{
}

int _x = -1;
int _y = -1;

Point2D _posGoin = {100, 100};

void TestOnClick(const Window win, int xPos, int yPos)
{
    if (_x & _y == -1)
    {
        _x = xPos;
        _y = yPos;
        return;
    }
    WindowDrawLineA(win, _x, _y, xPos, yPos, ColorCreate(0, 0, 255));
    _x = xPos;
    _y = yPos;
    WindowRenderRequest(win);
}

void TestOnKeyDown(const Window win, int charac)
{
    if (charac == BAL_KEY_LSHIFT)
    {
        WindowSetSize(win, _posGoin);
        _posGoin.x += 10;
        _posGoin.y += 10;
        return;
    }
    printf("%c\n", charac);
}

void TestOnSizeChanged(const Window win, int xSize, int ySize)
{
    printf("size: %i, %i\n", xSize, ySize);
}

int main()
{
    Window win = WindowCreateB(L"Pengu", 400, 400, 600, 300);

    PWindow pWin = &win;
    
    WindowEvents events = WindowEventsCreate();

    events->Update = (void(*)(const void*))TestUpdate;
    events->OnClick = (void(*)(const void*, int, int))TestOnClick;
    events->OnKeyDown = (void(*)(const void*, int))TestOnKeyDown;
    events->OnSizeChanged = (void(*)(const void*, int, int))TestOnSizeChanged;
    WindowSetEvents(win, events);

    WindowDrawLine(win, Point2DCreate(10, 10), Point2DCreate(100, 100), ColorCreate(255, 0, 255));

    while (WindowUpdate(win, 120))
    {
    }

    WindowDestroy(pWin);
    WindowEventsDestroy(events);

    return 0;
}
