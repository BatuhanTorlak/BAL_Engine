#include "wui.h"
#include <malloc.h>
#include <stdio.h>
#include "../../math/math.h"

typedef struct __params_t
{
    const wchar_t* className;
    const wchar_t* windowName;
    int width;
    int height;
    char started;
    WinWindow* win;
} __params, *__paramsPtr;

typedef struct ColorCOLORREF_u
{
    Color color;
    COLORREF winColor;
} ColorCOLORREF;

typedef struct WindowList_t
{
    struct WindowList_t* previous;
    WinWindow* win;
} WindowList;

LRESULT WinWindowProc(HWND hWnd, UINT msg, WPARAM b, LPARAM c);
DWORD WinWindowManagement(__paramsPtr param);

WindowList* allWindows = 0;

LRESULT WinWindowProc(HWND hWnd, UINT msg, WPARAM b, LPARAM c)
{
    WindowList* __wl = allWindows;
    WinWindow* currentWindow = __wl->win;
    while (currentWindow->windowHandler != hWnd)
    {
        __wl = __wl->previous;
        if (__wl == 0)
        {
            return DefWindowProcW(hWnd, msg, b, c);
        }
        currentWindow = __wl->win;
    }
    switch (msg)
    {
    case WM_DESTROY:
    case WM_CLOSE:
        WindowList* _wl = allWindows;
        WindowList* _nWl = 0;
        do
        {
            if (_wl->win->windowHandler == hWnd)
            {
                _wl->win->isAlive = 0;
                _nWl->previous = _wl->previous;
                free(_wl);
            }
            _nWl = _wl;
            _wl = _wl->previous;
        }while (_wl->previous);
        break;
    case WM_EXITSIZEMOVE:
        PColorMap _clp = currentWindow->colorMap;
        if (currentWindow->width != _clp->width || currentWindow->height != _clp->height)
        {
            ColorMapResize(currentWindow->colorMap, currentWindow->width, currentWindow->height);
        }
        break;
    case WM_SIZE:
        unsigned int _w = LOWORD(c);
        unsigned int _h = HIWORD(c);
        currentWindow->width = (int)_w;
        currentWindow->height = (int)_h;
        break;
    }
    return DefWindowProcW(hWnd, msg, b, c);
}

DWORD WinWindowManagement(__paramsPtr param)
{
    WNDCLASSW _class = { 0 };
    _class.hCursor = LoadCursor(0, IDC_ARROW);
    _class.lpszClassName = param->className;
    _class.lpfnWndProc = WinWindowProc;
    _class.hbrBackground = (HBRUSH)COLOR_WINDOW;
    _class.hCursor = LoadCursorA(0, IDC_ARROW);

    ATOM _ch = RegisterClassW(&_class);

    HWND windowHandler = CreateWindowExW(0, _ch, param->windowName, WS_OVERLAPPEDWINDOW | WS_VISIBLE, 10, 10, param->width, param->height, 0, 0, 0, 0);
    ShowWindow(windowHandler, SW_SHOW);

    WinWindow* win = param->win;
    win->windowHandler = windowHandler;
    win->windowClass = _class;
    win->drawingContentHandler = GetDC(windowHandler);
    win->colorMap = ColorMapCreate(param->width, param->height);
    win->width = param->width;
    win->height = param->height;
    win->events = 0;
    win->isAlive = 1;

    param->started = 0;

    MSG _msg;
    PMSG _pMsg = &_msg;
    while (GetMessageW(_pMsg, windowHandler, 0, 0))
    {
        TranslateMessage(_pMsg);
        DispatchMessage(_pMsg);
    }
}

WinWindow* WinWindowCreate(const wchar_t* className, const wchar_t* windowName, int width, int height)
{
    WinWindow* _ = malloc(sizeof(WinWindow));
    __paramsPtr _param = malloc(sizeof(__params));
    *_param = (__params){
        .className = className,
        .windowName = windowName,
        .width = width,
        .height = height,
        .win = _,
        .started = 1
    };
    WindowList* winList = malloc(sizeof(WindowList));
    winList->previous = allWindows;
    if (allWindows == 0)
    {
        allWindows = winList;
    }
    winList->win = _;
    _->threadHandler = CreateThread(0, 300, (DWORD(*)(LPVOID))WinWindowManagement, _param, 0, 0);
    while (_param->started)
    {
        Sleep(1);
    }
    free(_param);

    return _;
}

void WinWindowSetPixel(const WinWindow* win, const Point2D position, const Color color)
{
    ColorMapPixel(win->colorMap, position.x, position.y) = color;
}

void WinWindowSetPixelA(const WinWindow* win, const int xPos, const int yPos, const Color color)
{
    ColorMapPixel(win->colorMap, xPos, yPos) = color;
}

void WinWindowDrawLine(const WinWindow* win, Point2D start, Point2D end, const Color color)
{
    ColorMapDrawLine(win->colorMap, start, end, color);
}

void WinWindowSetEvents(WinWindow* win, WindowEvents* newEvents)
{
    win->events = newEvents;
}

void WinWindowRender(const WinWindow* win)
{
    const restrict HDC _hdc = win->drawingContentHandler;
    const restrict PColorMap colorMap = win->colorMap;
    register union
    {
        DWORD colorref;
        Color color;
    } CDWORD;
    for (int x = 0; x < colorMap->width; x++)
    {
        for (int y = 0; y < colorMap->height; y++)
        {
            CDWORD.color = ColorMapGetPixelA(colorMap, x, y);
            SetPixel(_hdc, x, y, CDWORD.colorref);
        }
    }
}

char WinWindowUpdate(const WinWindow* win, const int FPS)
{
    if (((int long long)win | (int long long)win->isAlive) == 0)
        return 0;
    //const int _ = 1000 / FPS;
    //int _t = GetTickCount();
    // printf("tickCount: %i\n", _t);
    WindowEvents* events = win->events;
    if (events)
    {
        if (events->update)
            events->update(win);
    }
    WinWindowRender(win);
    //int _delta = _ - (GetTickCount() - _t);
    //if (_delta > 0)
    //    Sleep(_ - (GetTickCount() - _t));
    return win->isAlive;
}

void WinWindowDestroy(const WinWindow** win)
{
    if (*win == 0)
        return;
    WinWindow* _win = *win;
    *win = 0;
    WindowList* _wl = allWindows;
    WindowList* _nWl = 0;
    do
    {
        if (_wl->win == win)
        {
            _wl->win->isAlive = 0;
            _nWl->previous = _wl->previous;
            free(_wl);
        }
        _nWl = _wl;
        _wl = _wl->previous;
    }while (_wl->previous);
    TerminateThread(_win->threadHandler, 0);
    ReleaseDC(_win->windowHandler, _win->drawingContentHandler);
    DestroyWindow(_win->windowHandler);
    UnregisterClassW(_win->windowClass.lpszClassName, 0);
    ColorMapDestroy(_win->colorMap);
    free(_win);
}