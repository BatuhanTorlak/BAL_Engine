#define OP_WIN
#include "ui/ui.h"
#include "math/vector2.h"
#include <stdio.h>
#include <math.h>

typedef struct Body_t
{
    float mass;
} Body;

void TestUpdate(const WinWindow* win)
{
    for (int x = 0; x < 100; x++)
        WinWindowSetPixelA(win, rand() % 100, rand() % 100, ColorCreate(255, 0, 255));
}

int main()
{
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