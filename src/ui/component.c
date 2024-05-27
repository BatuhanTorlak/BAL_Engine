#include <malloc.h>
#include "component.h"

void* ComponentCreate(int componentSize, ColorMap* image, Point2D position, Point2D size)
{
    Component* _comp = malloc(componentSize);
    _comp->image = image;
    _comp->position = position;
    _comp->size = size;
    _comp->events = malloc(sizeof(ComponentEvents));
    return _comp;
}

void ComponentDestroy(void* component)
{
    free(((Component*)component)->events);
    free((Component*)component);
}