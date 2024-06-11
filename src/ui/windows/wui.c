#include "wui.h"
#include <malloc.h>
#include <math.h>
#include "../../math/point.h"

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

//WindowList* allWindows = 0;

LRESULT WinWindowProc(HWND hWnd, UINT msg, WPARAM b, LPARAM c)
{
    if (msg == WM_CREATE)
    {
        CREATESTRUCTW* _crstruct = c;
        __paramsPtr _params = _crstruct->lpCreateParams;
        PWinWindow _win = _params->win;
        SetWindowLongPtrA(hWnd, GWLP_USERDATA, _win);
    }

    WinWindow* currentWindow = (WinWindow*)GetWindowLongPtrA(hWnd, GWLP_USERDATA);
    if (currentWindow == 0)
        return DefWindowProcW(hWnd, msg, b, c);

    switch (msg)
    {
    case WM_DESTROY:
    case WM_CLOSE:
        currentWindow->isAlive = 0;
        break;
    case WM_EXITSIZEMOVE:
        PColorMap _clp = currentWindow->colorMap;
        int _width = currentWindow->width;
        int _height = currentWindow->height;
        if (_width != _clp->width || _height != _clp->height)
        {
            int _nW;
            int _nH;
            if (_width < _clp->width)
                _nW = _width;
            else
                _nW = _clp->width;
            if (_height < _clp->height)
                _nH = _height;
            else
                _nH = _clp->height;
            
            int newLinearSize = _width * _height;

            BITMAPINFO _info = (BITMAPINFO){
                .bmiHeader = {
                    .biPlanes = 1,
                    .biBitCount = sizeof(Color) * 8,
                    .biWidth = _width,
                    .biHeight = _height,
                    .biCompression = BI_RGB,
                    .biSize = sizeof(BITMAPINFOHEADER)
                }
            };
            
            HBITMAP _oldBitmap = currentWindow->drawing.bitmap;
            HDC _oldDC = currentWindow->drawing.bitmapDrawingContentHandler;

            Color* _clrs = 0;
            HBITMAP _newBitmap = CreateDIBSection(0, &_info, DIB_RGB_COLORS, &(_clrs), 0, 0);
            HDC _dc = CreateCompatibleDC(0);

            SelectObject(_dc, _newBitmap);

            BitBlt(_dc, 0, 0, _nW, _nH, _oldDC, 0, 0, SRCCOPY);

            _clp->width = _width;
            _clp->height = _height;
            _clp->colors = _clrs;
            _clp->ratio = (float)_width / (float)_height;
            _clp->linearSize = newLinearSize;

            currentWindow->drawing.bitmapColors = _clrs;
            currentWindow->drawing.bitmap = _newBitmap;
            currentWindow->drawing.bitmapDrawingContentHandler = _dc;

            DeleteObject(_oldBitmap);
            DeleteDC(_oldDC);
        }
        break;
    case WM_LBUTTONDOWN:
        int _xPos = LOWORD(c);
        int _yPos = HIWORD(c);
        _yPos = currentWindow->height - _yPos;
        Component** _comps = currentWindow->componentManager.components;
        int _size = currentWindow->componentManager.componentsCount;
        for (int x = 0; x < _size; x++)
        {
            Component* _com = _comps[x];
            Point2D _pos = _com->position;
            Point2D _siz = _com->size;
            register int _posCache = _pos.x;
            if (_posCache > _xPos)
                continue;
            _posCache += _siz.x;
            if (_posCache < _xPos)
                continue;
            _posCache = _pos.y;
            if (_com->position.y > _yPos)
                continue;
            _posCache += _siz.y;
            if (_com->position.y + _com->size.y < _yPos)
                continue;
            _com->events->OnClick(_xPos, _yPos);
        }
        if (currentWindow->events->OnClick)
            currentWindow->events->OnClick(currentWindow, _xPos, _yPos);
        break;
    case WM_KEYDOWN:

    case WM_SIZE:
        UINT width = LOWORD(c);
        UINT height = HIWORD(c);
        currentWindow->width = (int)width;
        currentWindow->height = (int)height;
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

    HWND windowHandler = CreateWindowExW(0, _ch, param->windowName, WS_OVERLAPPEDWINDOW | WS_VISIBLE, 10, 10, param->width, param->height, 0, 0, 0, param);

    WinWindow* win = param->win;

    win->windowHandler = windowHandler;
    win->windowClass = _class;
    win->drawingContentHandler = GetDC(windowHandler);

    BITMAPINFO _info = {
        .bmiHeader = {
            .biPlanes = 1,
            .biBitCount = sizeof(Color) * 8,
            .biWidth = param->width,
            .biHeight = param->height,
            .biCompression = BI_RGB,
            .biSize = sizeof(BITMAPINFOHEADER),
        }
    };

    win->drawing.bitmapDrawingContentHandler = CreateCompatibleDC(0);
    win->drawing.bitmap = CreateDIBSection(0, &_info, DIB_RGB_COLORS, &(win->drawing.bitmapColors), 0, 0);
    SelectObject(win->drawing.bitmapDrawingContentHandler, win->drawing.bitmap);

    PColorMap colorMap = malloc(sizeof(ColorMap));
    colorMap->width = param->width;
    colorMap->height = param->height;
    colorMap->linearSize = param->width * param->height;
    colorMap->colors = win->drawing.bitmapColors;
    
    win->colorMap = colorMap;

    win->width = param->width;
    win->height = param->height;
    win->events = 0;
    win->isAlive = 1;

    param->started = 0;

    ShowWindow(windowHandler, SW_SHOW);

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
    _->componentManager.componentsCapacity = 6;
    _->componentManager.componentsCount = 0;
    _->componentManager.components = malloc(sizeof(Component**) * 6);
    _->threadHandler = CreateThread(0, 3000, (DWORD(*)(LPVOID))WinWindowManagement, _param, 0, 0);
    while (_param->started)
    {
        Sleep(1);
    }
    free(_param);

    return _;
}

void WinWindowSetPixel(const WinWindow* win, const Point2D position, const Color color)
{
    ColorMapSetPixel(win->colorMap, position, color);
}

void WinWindowSetPixelA(const WinWindow* win, const int xPos, const int yPos, const Color color)
{
    ColorMapSetPixelA(win->colorMap, xPos, yPos, color);
}

void WinWindowDrawLine(const WinWindow* win, Point2D start, Point2D end, const Color color)
{
    ColorMapDrawLine(win->colorMap, start, end, color);
}

void WinWindowDrawLineA(const WinWindow* win, int x1, int y1, int x2, int y2, const Color color)
{
    ColorMapDrawLineA(win->colorMap, x1, y1, x2, y2, color);
}

void WinWindowSetEvents(WinWindow* win, WindowEvents* newEvents)
{
    win->events = newEvents;
}

void WinWindowAddEvents(WinWindow* win, Component* component)
{
}

void WinWindowRender(const WinWindow* win)
{
    const restrict HDC _hdc = win->drawingContentHandler;
    const restrict PColorMap colorMap = win->colorMap;
    const restrict HDC bitmapDrawingContentHandler = win->drawing.bitmapDrawingContentHandler;

    BOOL _x = BitBlt(_hdc, 0, 0, colorMap->width, colorMap->height, bitmapDrawingContentHandler, 0, 0, SRCCOPY);
}

char WinWindowUpdate(const WinWindow* win, const int FPS)
{
    if (((int long long)win | (int long long)win->isAlive) == 0)
        return 0;
    WindowEvents* events = win->events;
    if (events)
    {
        if (events->Update)
            events->Update(win);
    }
    WinWindowRender(win);
    Sleep(1000 / FPS);
    return win->isAlive;
}

void WinWindowDestroy(const WinWindow** win)
{
    if (*win == 0)
        return;
    WinWindow* _win = *win;
    *win = 0;

    TerminateThread(_win->threadHandler, 0);

    ReleaseDC(_win->windowHandler, _win->drawingContentHandler);
    DeleteDC(_win->drawing.bitmapDrawingContentHandler);

    DeleteObject(_win->drawing.bitmap);

    DestroyWindow(_win->windowHandler);

    UnregisterClassW(_win->windowClass.lpszClassName, 0);

    free(_win->colorMap);
    free(_win);
}
