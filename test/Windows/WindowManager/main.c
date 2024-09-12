#include <stdio.h>

#include <bale.h>

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
    printf("%i\n", charac);
    if (charac == BAL_KEY_LSHIFT)
    {
        WindowSetSize(win, _posGoin);
        printf("LSHIFT\n");
        _posGoin.x += 10;
        _posGoin.y += 10;
        return;
    }
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

    events->Update = (WindowEventUpdate)TestUpdate;
    events->OnClick = (WindowEventOnClick)TestOnClick;
    events->OnKeyDown = (WindowEventOnKeyDown)TestOnKeyDown;
    events->OnSizeChanged = (WindowEventOnSizeChanged)TestOnSizeChanged;
    WindowSetEvents(win, events);

    while (WindowUpdate(win, 120))
    {
    }

    WindowDestroy(pWin);
    WindowEventsDestroy(events);

    return 0;
}
