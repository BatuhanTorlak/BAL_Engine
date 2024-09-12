#ifndef UI_EVENTS
#define UI_EVENTS

typedef void(*WindowEventUpdate)(const void* win);
typedef void(*WindowEventOnClick)(const void* win, int xPos, int yPos);
typedef void(*WindowEventOnKeyDown)(const void* win, int keyCode);
typedef void(*WindowEventOnSizeChanged)(const void* win, int xSize, int ySize);
typedef void(*WindowEventOnMoving)(const void* win, int xPos, int yPos);
typedef void(*WindowEventOnMoved)(const void* win, int xPos, int yPos);

typedef struct WindowEvents_t
{
    WindowEventUpdate Update;
    WindowEventOnClick OnClick;
    WindowEventOnKeyDown OnKeyDown;
    WindowEventOnSizeChanged OnSizeChanged;
    WindowEventOnMoving OnMoving;
    WindowEventOnMoved OnMoved;
    //void(*playerUpdate)(float deltaTime);
} *WindowEvents, **PWindowEvents;

typedef void(*ComponentEventOnClick)(const void* win, const void* component, int xPos, int yPos);
typedef void(*ComponentEventOnKeyDown)(const void* win, const void* component, int key);
typedef void(*ComponentEventOnSizeChanged)(const void* win, const void* component, int xSize, int ySize);

typedef struct ComponentEvents_t
{
    ComponentEventOnClick OnClick;
    ComponentEventOnKeyDown OnKeyDown;
    ComponentEventOnSizeChanged OnSizeChanged;
} ComponentEvents;

WindowEvents WindowEventsCreate();
void WindowEventsDestroy(WindowEvents events);

#endif