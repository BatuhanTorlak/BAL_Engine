#define BAL_DEFAULT_WIDTH 500
#define BAL_DEFAULT_HEIGHT 200
#define BAL_DEFAULT_STARTPOS_X 50
#define BAL_DEFAULT_STARTPOS_Y 50
#define BAL_DEFAULT_NAME L"Test"
#include "win_defs.h"
#include "./wui.h"
#include <math.h>
#include <stdio.h>

#define BAL_DEFAULT_CLEAR_COLOR ColorCreate(0, 0, 0)

typedef struct __params_t
{
    const wchar_t* className;
    const wchar_t* windowName;
    int width;
    int height;
    int startX;
    int startY;
    WinWindow* win;
    HANDLE mainThread;
    HANDLE myHeap;
    int started;
} __params, *__paramsPtr;

typedef struct ColorCOLORREF_u
{
    Color color;
    COLORREF winColor;
} ColorCOLORREF;

WinWindow* WinWindowCreate(const short* name, const short* className, int xPos, int yPos, int width, int height);
LRESULT WINAPI WinWindowProc(HWND hWnd, UINT msg, WPARAM b, LPARAM c);
DWORD WINAPI WinWindowManagement(__paramsPtr param);

LRESULT WINAPI WinWindowProc(HWND hWnd, UINT msg, WPARAM b, LPARAM c)
{
    if (msg == WM_CREATE)
    {
        CREATESTRUCTW* _crstruct = (CREATESTRUCTW*)c;
        __paramsPtr _params = _crstruct->lpCreateParams;
        WinWindow* _win = _params->win;
        SetWindowLongPtrA(hWnd, GWLP_USERDATA, (LONG_PTR)_win);
    }

    WinWindow* currentWindow = (WinWindow*)GetWindowLongPtrA(hWnd, GWLP_USERDATA);
    if (currentWindow == 0 || currentWindow->isAlive == 0)
        return DefWindowProcW(hWnd, msg, b, c);
    switch (msg)
    {
    case WM_DESTROY:
    case WM_CLOSE:
        currentWindow->isAlive = 0;
        break;
    /*case WM_SIZING:
        RECT _sizeRect = *((RECT*)c);
        currentWindow->width = (int)_sizeRect.top;
        currentWindow->height = (int)_sizeRect.left;
        break;*/
    case WM_SIZE:
        currentWindow->width = LOWORD(c);
        currentWindow->height = HIWORD(c);
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
            WaitForSingleObject(_clp->reserved, INFINITE);
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
            HBITMAP _newBitmap = CreateDIBSection(0, &_info, DIB_RGB_COLORS, (void**)&(_clrs), 0, 0);
            HDC _dc = CreateCompatibleDC(0);

            SelectObject(_dc, _newBitmap);

            int _o = BitBlt(_dc, 0, 0, _nW, _nH, _oldDC, 0, 0, SRCCOPY);

            if (_o == 0)
            {
                DeleteDC(_dc);
                DeleteObject(_newBitmap);
                break;
            }

            _clp->width = _width;
            _clp->height = _height;
            _clp->colors = _clrs;
            _clp->ratio = (float)_width / (float)_height;
            _clp->linearSize = newLinearSize;

            currentWindow->drawing.bitmapColors = _clrs;
            currentWindow->drawing.bitmap = _newBitmap;
            currentWindow->drawing.bitmapDrawingContentHandler = _dc;

            DeleteDC(_oldDC);
            DeleteObject(_oldBitmap);

            // Window event
            if (currentWindow->events && currentWindow->events->OnSizeChanged)
                currentWindow->events->OnSizeChanged(currentWindow, _width, _height);

            // Component event
            Component** _comps = currentWindow->componentManager.components;
            for (int x = 0; x < currentWindow->componentManager.componentsCount; x++)
            {
                Component* _comp = _comps[x];
                if (_comp->enabled && _comp->events && _comp->events->OnSizeChanged)
                    _comp->events->OnSizeChanged(currentWindow, _comp, _width, _height);
            }

	        currentWindow->drawing.renderOrder = 1;
            ReleaseMutex(_clp->reserved);
        }
        break;
    case WM_LBUTTONDOWN: ;
        int _xPos = LOWORD(c);
        int _yPos = HIWORD(c);

        _yPos = currentWindow->height - _yPos;

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
            if (_com->enabled == 0 || !_com->events || !_com->events->OnKeyDown)
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
            if (_posCache > _yPos)
                continue;
            _posCache += _siz.y;
            if (_posCache < _yPos)
                continue;
            _com->events->OnClick(currentWindow, _com, _xPos, _yPos);
        }
        break;
    case WM_KEYDOWN: ;
        // Focused component event
        Component* _comp = currentWindow->componentManager.focusedComponent;
        if (_comp && _comp->enabled && _comp->events && _comp->events->OnKeyDown)
            _comp->events->OnKeyDown(currentWindow, _comp, b);
        
        // Window event
        if (currentWindow->events && currentWindow->events->OnKeyDown)
            currentWindow->events->OnKeyDown(currentWindow, b);
        break;
    case WM_MOVING: ;
        RECT _nPos = *((RECT*)c);
        currentWindow->position.x = _nPos.left;
        currentWindow->position.y = _nPos.top;
        if (currentWindow->events && currentWindow->events->OnMoving)
            currentWindow->events->OnMoving(currentWindow, _nPos.left, _nPos.top);
        break;
    }
    return DefWindowProcW(hWnd, msg, b, c);
}

DWORD WINAPI WinWindowManagement(__paramsPtr param)
{
    WNDCLASSW _class = { 0 };
    _class.hCursor = LoadCursor(0, IDC_ARROW);
    _class.lpszClassName = param->className;
    _class.lpfnWndProc = WinWindowProc;
    _class.hbrBackground = (HBRUSH)COLOR_WINDOW;
    _class.hCursor = LoadCursorA(0, IDC_ARROW);

    ATOM _ch = RegisterClassW(&_class);

    HWND windowHandler = CreateWindowExW(0, (LPCWSTR)_ch, param->windowName, WS_OVERLAPPEDWINDOW | WS_VISIBLE, param->startX, param->startY, param->width, param->height, 0, 0, 0, param);

    WinWindow* win = param->win;

    win->windowHandler = windowHandler;
    win->windowClass = _class;
    win->drawingContentHandler = GetDC(windowHandler);
    win->heapHandler = param->myHeap;

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
    win->drawing.bitmap = CreateDIBSection(0, &_info, DIB_RGB_COLORS, (void**)&(win->drawing.bitmapColors), 0, 0);
    SelectObject(win->drawing.bitmapDrawingContentHandler, win->drawing.bitmap);

    PColorMap colorMap = BAL_WIN_MALLOC_0(win, sizeof(ColorMap));
    colorMap->width = param->width;
    colorMap->height = param->height;
    colorMap->linearSize = param->width * param->height;
    colorMap->colors = win->drawing.bitmapColors;
    
    win->colorMap = colorMap;

    win->width = param->width;
    win->height = param->height;
    //win->events = 0;
    win->drawing.renderOrder = 1;
    win->isAlive = 1;

    ResumeThread(param->mainThread);
    
    //param->started = 0;
    BAL_WIN_FREE(win, param);

    ShowWindow(windowHandler, SW_SHOW);

    MSG _msg;
    PMSG _pMsg = &_msg;
    int _err;
    while (_err = GetMessageW(_pMsg, windowHandler, 0, 0))
    {
        if (_err == -1)
        {
            break;
        }
        TranslateMessage(_pMsg);
        DispatchMessage(_pMsg);
    }
}

WinWindow* WinWindowCreate(const short* name, const short* className, int xPos, int yPos, int width, int height)
{
    HANDLE _threadHandle;
    HANDLE _processHandle = GetCurrentProcess();
    DuplicateHandle(_processHandle, GetCurrentThread(), _processHandle, &_threadHandle, DUPLICATE_SAME_ACCESS, 1, DUPLICATE_SAME_ACCESS);

    HANDLE _heapHandler = HeapCreate(0, 0, 0);

    //WinWindow* _ = BAL_MALLOC(sizeof(WinWindow));
    WinWindow* _ = HeapAlloc(_heapHandler, HEAP_ZERO_MEMORY, sizeof(WinWindow));

    //__paramsPtr _param = BAL_MALLOC(sizeof(__params));
    __paramsPtr _param = HeapAlloc(_heapHandler, 0, sizeof(__params));
    *_param = (__params){
        .className = name,
        .windowName = name,
        .width = width,
        .height = height,
        .startX = xPos,
        .startY = yPos,
        .win = _,
        .mainThread = _threadHandle,
        .myHeap = _heapHandler,
        .started = 1
    };

    _->componentManager.componentsCapacity = 6;
    _->componentManager.componentsCount = 0;
    _->componentManager.components = HeapAlloc(_heapHandler, HEAP_ZERO_MEMORY, sizeof(Component*) * 6);
    _->threadHandler = CreateThread(0, 0, (DWORD(*)(LPVOID))WinWindowManagement, _param, 0, 0);

    SuspendThread(_threadHandle);

    return _;
}

WinWindow* WindowCreate()
{
    return WindowCreateA(BAL_DEFAULT_NAME);
}

WinWindow* WindowCreateA(const short* name)
{
    return WinWindowCreate(name, name, BAL_DEFAULT_STARTPOS_X, BAL_DEFAULT_STARTPOS_Y, BAL_DEFAULT_WIDTH, BAL_DEFAULT_WIDTH);
}

WinWindow* WindowCreateB(const short* name, int xPos, int yPos, int width, int height)
{
    return WinWindowCreate(name, name, xPos, yPos, width, height);
}

void WindowSetPixel(WinWindow* win, Point2D position, Color color)
{
    if (win == 0 && win->isAlive)
    {
        ColorMapSetPixel(win->colorMap, position, color);
    }
}

void WindowSetPixelA(WinWindow* win, int xPos, int yPos, Color color)
{
    if (win && win->isAlive)
    {
        ColorMapSetPixelA(win->colorMap, xPos, yPos, color);
    }
}

void WindowDrawLine(WinWindow* win, Point2D start, Point2D end, Color color)
{
    if (win && win->isAlive)
    {
        ColorMapDrawLine(win->colorMap, start, end, color);
    }
}

void WindowDrawLineA(WinWindow* win, int x1, int y1, int x2, int y2, Color color)
{
    if (win && win->isAlive)
    {
        ColorMapDrawLineA(win->colorMap, x1, y1, x2, y2, color);
    }
}

void WindowClear(WinWindow* window)
{
    if (window == 0 || window->isAlive == 0)
        return;
    for (int x = 0; x < window->width; x++)
    {
        for (int y = 0; y < window->height; y++)
        {
            WindowSetPixelA(window, x, y, BAL_DEFAULT_CLEAR_COLOR);
        }
    }
}

void WindowClearA(WinWindow* window, Color color)
{
    if (window == 0 || window->isAlive == 0)
        return;
    for (int x = 0; x < window->width; x++)
    {
        for (int y = 0; y < window->height; y++)
        {
            WindowSetPixelA(window, x, y, color);
        }
    }
}

void WindowSetPosition(const WinWindow* win, Point2D pos)
{
    if (win && win->isAlive)
        MoveWindow(win->windowHandler, pos.x, pos.y, win->width, win->height, FALSE);
}

void WindowSetPositionA(const WinWindow* win, int xPos, int yPos)
{
    if (win && win->isAlive)
        MoveWindow(win->windowHandler, xPos, yPos, win->width, win->height, FALSE);
}

void WindowSetSize(WinWindow* win, Point2D size)
{
    if (win && win->isAlive)
    {
        MoveWindow(win->windowHandler, win->position.x, win->position.y, size.x, size.y, FALSE);
    }
}

void WindowSetSizeA(WinWindow* win, int xSize, int ySize)
{
    if (win && win->isAlive)
    {
        MoveWindow(win->windowHandler, win->position.x, win->position.y, xSize, ySize, FALSE);
    }
}

void WindowFocusComponent(WinWindow* win, Component* component)
{
    if (win && win->isAlive && component && component->sOC)
	    win->componentManager.focusedComponent = component;
}

void WindowSetEvents(WinWindow* win, WindowEvents newEvents)
{
    if (win && win->isAlive && newEvents)
        win->events = newEvents;
}

void WindowAddComponent(WinWindow* win, Component* component)
{
    if (win == 0 || win->isAlive == 0 || component == 0 || component->sOC == 0)
        return;
    struct ComponentManager_t _win = win->componentManager;
    if (_win.componentsCount == _win.componentsCapacity)
    {
        win->componentManager.componentsCapacity = _win.componentsCount + 6;
        Component** _comps = _win.components;
        _comps = BAL_WIN_REALLOC(win, _comps, sizeof(Component*) * win->componentManager.componentsCapacity);
        win->componentManager.components = _comps;
        _win.components = _comps;
    }
    _win.components[_win.componentsCount] = component;
    win->componentManager.componentsCount++;
}

void WindowRemoveComponent(WinWindow* win, Component* component)
{
    if (win == 0 || win->isAlive == 0 || component == 0 || component->sOC == 0)
        return;
    struct ComponentManager_t _win = win->componentManager;
    Component** _comps = _win.components;
    int x = 0;
    for (; x < _win.componentsCount; x++)
    {
        if (_comps[x] == component)
            goto _c;
    }
    return;
    _c: ;
    Component** __ = _comps + x * sizeof(Component*);
    BAL_MEMMOV(__ + 1, __, _win.componentsCount - x);
    if (_win.componentsCount == _win.componentsCapacity - 8)
    {
        win->componentManager.componentsCapacity = _win.componentsCount -= 6;
        _comps = BAL_WIN_REALLOC(win, _comps, sizeof(Component*) * _win.componentsCapacity);
        win->componentManager.components = _comps;
    }
    win->componentManager.componentsCount--;
}

void WindowRender(WinWindow* win)
{
    if (win == 0 || win->isAlive == 0)
        return;

    WaitForSingleObject(win->colorMap, INFINITE);

    const restrict HDC _hdc = win->drawingContentHandler;
    const restrict PColorMap colorMap = win->colorMap;
    const restrict HDC bitmapDrawingContentHandler = win->drawing.bitmapDrawingContentHandler;

    BOOL _x = BitBlt(_hdc, 0, 0, colorMap->width, colorMap->height, bitmapDrawingContentHandler, 0, 0, SRCCOPY);

    win->drawing.renderOrder = 0;
    ReleaseMutex(colorMap->reserved);
}

void WindowRenderRequest(WinWindow* win)
{
    if (win == 0 || win->isAlive == 0)
	    return;
    win->drawing.renderOrder = 1;
}

char WindowUpdate(const WinWindow* win, const int FPS)
{
    if ((int long long)win == 0 || (int long long)win->isAlive == 0)
        return 0;
    WindowEvents events = win->events;
    if (events && events->Update)
        events->Update(win);
    if (win->drawing.renderOrder != 0)
	    WindowRender(win);
    Sleep(1000 / FPS);
    return 1;
}

void WindowDestroy(const WinWindow** win)
{
    if (win == 0 || *win == 0)
        return;
    WinWindow* _win = *win;
    *win = 0;

    TerminateThread(_win->threadHandler, 0);
    CloseHandle(_win->threadHandler);

    DestroyWindow(_win->windowHandler);

    ReleaseDC(_win->windowHandler, _win->drawingContentHandler);
    DeleteDC(_win->drawing.bitmapDrawingContentHandler);
    //CloseHandle(_win->drawingContentHandler);
    //CloseHandle(_win->drawing.bitmapDrawingContentHandler);

    DeleteObject(_win->drawing.bitmap);
    //CloseHandle(_win->drawing.bitmapColors);

    UnregisterClassW(_win->windowClass.lpszClassName, 0);

    BAL_WIN_FREE(_win, _win->colorMap);

    HeapDestroy(_win->heapHandler);
}
