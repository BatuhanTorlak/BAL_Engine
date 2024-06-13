#define OP_WIN
#include "ui/ui.h"
#include "math/vector2.h"
#include "ui/key_id.h"
#include <stdio.h>
#include <math.h>

void TestUpdate(const WinWindow* win)
{
}

int _x = -1;
int _y = -1;

Point2D _posGoin = {0};

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

void TestOnKeyDown(const WinWindow* win, int charac)
{
    if (charac == KEY_LSHIFT)
    {
        printf("Left shift!\n");
        WinWindowSetPosition(win, _posGoin);
        _posGoin.x += 10;
        _posGoin.y += 10;
        if (_posGoin.x == 400)
        {
            _posGoin.x = 10;
        }
        
        return;
    }
    printf("%c\n", charac);
}

int main()
{
    Window* win = WindowCreateA(L"test");

    PWindow* window = &win;
    
    WindowEvents* events = WindowEventsCreate();
    events->Update = TestUpdate;
    events->OnClick = TestOnClick;
    events->OnKeyDown = TestOnKeyDown;
    WindowSetEvents(win, events);
    
    WindowDrawLine(win, Point2DCreate(10, 10), Point2DCreate(100, 100), ColorCreate(255, 0, 255));

    while (WindowUpdate(win, 120))
    {
    }

    WindowDestroy(window);
    WindowEventsDestroy(events);

    return EXIT_SUCCESS;
}