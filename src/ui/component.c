#include <malloc.h>
#include <string.h>
#include "ui/component.h"

void* ComponentCreate(int componentSize, int isRenderable, ColorMap* background, Point2D position, Point2D size)
{
    Component* _comp = malloc(componentSize);
    _comp->background = background;
    _comp->position = position;
    _comp->size = size;
    _comp->sOC = componentSize;
    _comp->render = isRenderable;
    _comp->enabled = 1;
    _comp->events = malloc(sizeof(ComponentEvents));

    memset(_comp->events, 0, sizeof(ComponentEvents));
    
    return _comp;
}

void ComponentDestroy(Component** component)
{
    if (component == 0 || *component == 0)
        return;
    Component* _comp = *component;
    *component = 0;
    _comp->sOC = 0;
    free(_comp->events);
    free((void*)_comp);
}
