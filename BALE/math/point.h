#ifndef POINT_H
#define POINT_H
#define Point2DCreate(x, y) (Point2D){(int)x, (int)y}
#define Point3DCreate(x, y, z) (Point3D){(int)x, (int)y, (int)z}

typedef struct Point2D_t
{
    int x;
    int y;
} Point2D;

typedef struct Point3D_t
{
    int x;
    int y;
    int z;
} Point3D;

#endif