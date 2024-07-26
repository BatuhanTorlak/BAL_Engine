#include "ui/events.h"
#include <malloc.h>
#include <memory.h>

WindowEvents WindowEventsCreate()
{
    WindowEvents _events = malloc(sizeof(_events));
    *_events = (struct WindowEvents_t){ 0 };

    memset(_events, 0, sizeof(struct WindowEvents_t));
    
    return _events;
}

void WindowEventsDestroy(WindowEvents events)
{
    free(events);
}