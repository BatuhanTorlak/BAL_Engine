#include "wui.h"
#include <stdio.h>
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
    case WM_SIZING:
        RECT _sizeRect = *((RECT*)c);
        currentWindow->width = (int)_sizeRect.top;
        currentWindow->height = (int)_sizeRect.left;
        break;
    case WM_EXITSIZEMOVE:
        // I don't even know what I have done here but it works
        if (b == SC_MOVE)
        {
            if (currentWindow->events && currentWindow->events->OnMoved)
                currentWindow->events->OnMoved(currentWindow, currentWindow->position.x, currentWindow->position.y);
            break;
        }
        PColorMap _clp = currentWindow->colorMap;
        if (_clp == 0)
            break;
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

            // Window event
            if (currentWindow->events && currentWindow->events->OnSizeChanged)
                currentWindow->events->OnSizeChanged(currentWindow, _width, _height);

            // Component event
            Component** _comps = currentWindow->componentManager.components;
            for (int x = 0; x < currentWindow->componentManager.componentsCount; x++)
            {
                Component* _comp = _comps[x];
                if (_comp->events && _comp->events->OnSizeChanged)
                    _comp->events->OnSizeChanged(currentWindow, _comp, _width, _height);
            }
        }
        break;
    case WM_LBUTTONDOWN:
        int _xPos = LOWORD(c);
        int _yPos = HIWORD(c);
        
        // Window event
        if (currentWindow->events && currentWindow->events->OnClick)
            currentWindow->events->OnClick(currentWindow, _xPos, _yPos);

        // Component event
        _yPos = currentWindow->height - _yPos;
        Component** _comps = currentWindow->componentManager.components;
        int _size = currentWindow->componentManager.componentsCount;
        for (int x = 0; x < _size; x++)
        {
            Component* _com = _comps[x];
            if (!_com->events || !_com->events->OnKeyDown)
                continue;
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
            _com->events->OnClick(currentWindow, _com, _xPos, _yPos);
        }
        break;
    case WM_KEYDOWN:
        // Focused component event
        Component* _comp = currentWindow->componentManager.focusedComponent;
        if (_comp && _comp->events && _comp->events->OnKeyDown)
            _comp->events->OnKeyDown(currentWindow, _comp, b);
        
        // Window event
        if (currentWindow->events && currentWindow->events->OnKeyDown)
            currentWindow->events->OnKeyDown(currentWindow, b);
        break;
    case WM_MOVING:
        RECT _nPos = *((RECT*)c);
        currentWindow->position.x = _nPos.left;
        currentWindow->position.y = _nPos.top;
        if (currentWindow->events && currentWindow->events->OnMoving)
            currentWindow->events->OnMoving(currentWindow, _nPos.left, _nPos.top);
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
    *colorMap = (ColorMap){0};
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
    *_ = (WinWindow){0};
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

void WinWindowSetPosition(const WinWindow* win, Point2D pos)
{
    MoveWindow(win->windowHandler, pos.x, pos.y, win->width, win->height, FALSE);
}

void WinWindowSetPositionA(const WinWindow* win, int xPos, int yPos)
{
    MoveWindow(win->windowHandler, xPos, yPos, win->width, win->height, FALSE);
}

void WinWindowSetSize(const WinWindow* win, Point2D size)
{    
    MoveWindow(win->windowHandler, win->position.x, win->position.y, size.x, size.y, FALSE);
}

void WinWindowSetSizeA(const WinWindow* win, int xSize, int ySize)
{
    MoveWindow(win->windowHandler, win->position.x, win->position.y, xSize, ySize, FALSE);
}

void WinWindowSetEvents(WinWindow* win, WindowEvents* newEvents)
{
    win->events = newEvents;
}

void WinWindowAddComponent(WinWindow* win, Component* component)
{
    WinWindow _win = *win;
    if (_win.componentManager.componentsCount == _win.componentManager.componentsCapacity)
    {
        win->componentManager.componentsCapacity = _win.componentManager.componentsCount + 6;
        Component** _comps = malloc(sizeof(Component*) * win->componentManager.componentsCapacity);
        memcpy(_comps, _win.componentManager.components, sizeof(Component*) * _win.componentManager.componentsCount);
        win->componentManager.components = _comps;
        free(_win.componentManager.components);
        _win.componentManager.components = win->componentManager.components;
    }
    _win.componentManager.components[_win.componentManager.componentsCount] = component;
    win->componentManager.componentsCount++;
}

void WinWindowRemoveComponent(WinWindow* win, Component* component)
{
    WinWindow _win = *win;
    if (_win.componentManager.componentsCount == _win.componentManager.componentsCapacity - 6)
    {
        win->componentManager.componentsCapacity = _win.componentManager.componentsCount - 6;
        Component** _comps = malloc(sizeof(Component*) * win->componentManager.componentsCapacity);
        int x;
        for (x = 0; x < _win.componentManager.componentsCount; x++)
        {
            if (_win.componentManager.components[x] == component)
                break;
        }
        memcpy(_comps, _win.componentManager.components, sizeof(Component*) * x);
        x++;
        memcpy(_comps + x * sizeof(Component*), _win.componentManager.components + x * sizeof(Component*), sizeof(Component*) * (_win.componentManager.componentsCount - x - 1));
        win->componentManager.components = _comps;
        free(_win.componentManager.components);
        _win.componentManager.components = win->componentManager.components;
        win->componentManager.componentsCount--;
        return;
    }
    int x;
    for (x = 0; x < _win.componentManager.componentsCount; x++)
    {
        if (_win.componentManager.components[x] == component)
            break;
    }
    memcpy(win->componentManager.components, _win.componentManager.components, sizeof(Component*) * x);
    x++;
    memcpy(win->componentManager.components + x * sizeof(Component*), _win.componentManager.components + x * sizeof(Component*), sizeof(Component*) * (_win.componentManager.componentsCount - x - 1));
 
    win->componentManager.componentsCount--;
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
