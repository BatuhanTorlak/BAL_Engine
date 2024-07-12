#ifndef UI_EVENTS
#define UI_EVENTS

typedef struct WindowEvents_t
{
    void(*Update)(const void* win);
    void(*OnClick)(const void* win, int xPos, int yPos);
    void(*OnKeyDown)(const void* win, int keyCode);
    void(*OnSizeChanged)(const void* win, int xSize, int ySize);
    void(*OnMoving)(const void* win, int xPos, int yPos);
    void(*OnMoved)(const void* win, int xPos, int yPos);
    //void(*playerUpdate)(float deltaTime);
} *WindowEvents, **PWindowEvents;

typedef struct ComponentEvents_t
{
    void(*OnClick)(const void* win, const void* component, int xPos, int yPos);
    void(*OnKeyDown)(const void* win, const void* component, int key);
    void(*OnSizeChanged)(const void* win, const void* component, int xSize, int ySize);
} ComponentEvents;

WindowEvents WindowEventsCreate();
void WindowEventsDestroy(WindowEvents events);

#endif