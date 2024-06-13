#include "events.h"
#include <malloc.h>
#include <memory.h>

WindowEvents* WindowEventsCreate()
{
    WindowEvents* _events = malloc(sizeof(_events));
    *_events = (WindowEvents){ 0 };
    return _events;
}

void WindowEventsDestroy(WindowEvents* events)
{
    free(events);
}