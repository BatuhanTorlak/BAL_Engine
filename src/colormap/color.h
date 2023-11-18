#ifndef COLOR_H
#define COLOR_H
#define ColorCreate(red, green, blue) (Color){.r = (unsigned char)red, .g = (unsigned char)green, .b = (unsigned char)blue}

/// @brief RGBA 24 bit color
typedef struct Color_t
{
    unsigned char b;
    unsigned char g;
    unsigned char r;
}Color;

#endif // COLOR_H