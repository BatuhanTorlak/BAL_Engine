#include <malloc.h>
#include "component.h"

void* ComponentCreate(int componentSize, ColorMap* background, Point2D position, Point2D size)
{
    Component* _comp = malloc(componentSize);
    _comp->background = background;
    _comp->position = position;
    _comp->size = size;
    _comp->events = malloc(sizeof(ComponentEvents));

    memset(_comp->events, 0, sizeof(ComponentEvents));
    
    return _comp;
}

void ComponentDestroy(void* component)
{
    free(((Component*)component)->events);
    free((Component*)component);
}