#ifndef UI_EVENTS
#define UI_EVENTS

typedef struct WindowEvents_t
{
    void(*update)(const void* win);
    //void(*playerUpdate)(float deltaTime);
}WindowEvents, PWindowEvents;

WindowEvents* WindowEventsCreate();
void WindowEventsDestroy(WindowEvents* events);

#endif