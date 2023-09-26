#ifndef COLOR_H
#define COLOR_H
#define ColorCreate(r, g, b) (Color){(unsigned char)r, (unsigned char)g, (unsigned char)b, (unsigned char)255}
#define ColorCreateA(r, g, b, a) (Color){(unsigned char)r, (unsigned char)g, (unsigned char)b, (unsigned char)a}

/// @brief RGBA 32 bit color
typedef struct Color_t
{
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
}Color;

#endif // COLOR_H