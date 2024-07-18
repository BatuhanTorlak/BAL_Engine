#ifndef COMPONENT_H
#define COMPONENT_H
#include "../colormap/colormap.h"
#include "events.h"

/*
Kendime not. Kullanım:
typedef struct ComponentAdi_t
{
    Component comp; // <--- En başta bu ifade olmalı(değişken adı önemli değil)
    // Buraya da kendi özelliklerini ekle
} ComponentAdi;

ComponentAdi* component = ComponentCreate(sizeof(ComponentAdi), image, Point2DCreate(10, 10), Point2DCreate(200, 140));
ComponentDestroy(component);
*/

/// @brief Custom component header
typedef struct Component_t
{
    int sOC;
    void* parent;
    ComponentEvents* events;
    ColorMap* background;
    Point2D position;
    Point2D size;
} Component;

/// @brief Creates an custom component for window
/// @param componentSize [IN] Size of the custom component
/// @param background [IN, OPTIONAL] Background ColorMap of component
/// @param position [IN] Position of the component
/// @param size [IN] Size of the component
/// @return Custom component
void* ComponentCreate(int componentSize, ColorMap* background, Point2D position, Point2D size);

/// @brief Destroys the custom component
/// @param destroyComponent [IN] Component to be destroyed
void ComponentDestroy(Component** destroyComponent);

#endif