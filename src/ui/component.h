#ifndef COMPONENT_H
#define COMPONENT_H
#include "../colormap/colormap.h"

typedef struct ComponentEvents_t
{
} ComponentEvents;

/*
Kendime not. Kullanım:
typedef struct ComponentAdi_t
{
    Component comp;
    // Buraya da kendi özelliklerini ekle
} ComponentAdi;

ComponentAdi* component = ComponentCreate(sizeof(ComponentAdi), image, Point2DCreate(10, 10), Point2DCreate(200, 140));
ComponentDestroy(component);

*/

typedef struct Component_t
{
    ComponentEvents* events;
    ColorMap* image;
    Point2D position;
    Point2D size;
} Component;

void* ComponentCreate(int componentSize, ColorMap* image, Point2D position, Point2D size);
void ComponentDestroy(void* destroyComponent);

#endif