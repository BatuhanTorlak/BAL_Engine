#ifndef WUI_H
#define WUI_H
#include <Windows.h>

typedef struct WinWindow_t
{
    WNDCLASS windowClass;
    HWND windowHandler;
}WinWindow;

WinWindow* WinWindowCreate(const char* windowName, int width, int height);
void WinWindowDestroy(WinWindow*);

#endif // WUI_H