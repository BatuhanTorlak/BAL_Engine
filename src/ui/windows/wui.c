#include "ui/windows/wui.h"
#include <malloc.h>

WinWindow* WinWindowCreate(const char* windowName, int width, int height)
{
    WNDCLASS _class = {
        .hCursor = LoadCursor(0, IDC_ARROW),
        .hbrBackground = COLOR_WINDOW,
        .hIcon = NULL,
        0
    };
    RegisterClass(&_class);
    HWND windowHandler = CreateWindowA(&_class, windowName, WS_BORDER, 10, 10, width, height, 0, 0, 0, 0);

    WinWindow* _ = malloc(sizeof(WinWindow));
    *_ = (WinWindow){
        .windowHandler = windowHandler,
        .windowClass = _class
    };
    return _
}