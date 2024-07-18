#ifndef COLOR_H
#define COLOR_H
#define ColorCreate(red, green, blue) (Color){.r = (unsigned char)red, .g = (unsigned char)green, .b = (unsigned char)blue, .reserved = (unsigned char)0}

/// @brief RGB 32 bit color
typedef struct Color_t
{
    unsigned char b;
    unsigned char g;
    unsigned char r;
    unsigned char reserved;
}Color;

#endif // COLOR_H