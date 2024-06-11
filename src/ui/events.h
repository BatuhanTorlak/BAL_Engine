#ifndef UI_EVENTS
#define UI_EVENTS

typedef struct WindowEvents_t
{
    void(*Update)(const void* win);
    void(*OnClick)(const void* win, int xPos, int yPos);
    void(*OnKeyDown)(const void* win, int keyCode);
    //void(*playerUpdate)(float deltaTime);
}WindowEvents, PWindowEvents;

typedef struct ComponentEvents_t
{
    void(*OnClick)(int xPos, int yPos);
} ComponentEvents;

WindowEvents* WindowEventsCreate();
void WindowEventsDestroy(WindowEvents* events);

#endif