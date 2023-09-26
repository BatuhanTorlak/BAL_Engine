#include "colormap/colormap.h"
#include <malloc.h>
#include <stdio.h>

typedef struct Slope_t
{
    float m;
    float n;
}Slope;

ColorMap* ColorMapCreate(int width, int height)
{
    ColorMap* _map = malloc(sizeof(ColorMap));
    _map->width = width;
    _map->height = height;
    _map->linearSize = width * height;
    _map->colors = malloc(_map->linearSize * sizeof(Color));
    _map->ratio = (float)width / (float)height;
    return _map;
}

void ColorMapDestroy(ColorMap* colorMap)
{
    free(colorMap->colors);
    free(colorMap);
}

void ColorMapDrawLine(ColorMap* colorMap, Point2D start, Point2D end, Color Color)
{
    Point2D _p = {end.x - start.x, end.y - start.y};
    Slope s;
}

void ColorMapSave(ColorMap* colorMap, const char* location)
{
    FILE* _f;
    
    int _size = colorMap->linearSize * 13 + 20;
    int _nSize = 0;
    int _s = 0;
    
    char* _txt = malloc(_size);
    char* _txtA = _txt;
    char _nTxt[100];
    
    Color color;
    
    sprintf(_nTxt, "P3 %i %i 255\n\0", colorMap->width, colorMap->height);
        
    for (; _nTxt[_s]; _s++)
    {
    }
        
    sprintf(_txtA, "%s", _nTxt);
    _txtA += _s;
    _nSize += _s;

    for (int x = 0; x < colorMap->linearSize; x++)
    {
        
        color = ColorMapPixelA(colorMap, x);
        
        sprintf(_nTxt, "%i %i %i\n\0", color.r, color.g, color.b);
        
        for (_s = 0; _nTxt[_s]; _s++)
        {
        }
        
        sprintf(_txtA, "%s", _nTxt);
        _txtA += _s;
        _nSize += _s;
    }
    _txtA[1] = '\0';

    _f = fopen(location, "w");
    
    fwrite(_txt, 1, _nSize, _f);
    
    fclose(_f);
    
    free(_txt);
}